#include "share_memory_obj.h"
#include "share_memory_api.h"
#include "myassert.h"
#include <stdio.h>

using namespace ShareMemAPI;

ShareMemObj::ShareMemObj() : _size(0), _dataPtr(NULL), _header(NULL), 
_hold(INVALID_SM_HANDLE) {}

bool ShareMemObj::create(SM_Key key,  uint size)
{
	__ENTER_FUNCTION
	_hold = ShareMemAPI::CreateShareMem(key, size);
	if (_hold == INVALID_SM_HANDLE) { return false; }
	_header = ShareMemAPI::MapShareMem(_hold);
	if (_header)
	{
		_dataPtr = _header + sizeof(SMHead);
		((SMHead*)_header)->_size = size;
		((SMHead*)_header)->_key = key;
		_size = size;
		return true;
	}
	return false;
	__LEAVE_FUNCTION
	return false;
}

void ShareMemObj::destroy()
{
	__ENTER_FUNCTION
	if (_header)
	{
		ShareMemAPI::UnMapShareMem(_header);
		_header = 0;
	}
	if (_hold)
	{
		ShareMemAPI::CloseShareMem(_hold);
		_hold = 0;
	}
	_size = 0;
	_dataPtr = 0;
	__LEAVE_FUNCTION
}

bool ShareMemObj::attach(SM_Key key, uint size)
{
	__ENTER_FUNCTION
	_hold = ShareMemAPI::OpenShareMem(key, size);
	if (_hold == INVALID_SM_HANDLE)
	{
		printf("attach error: %d \r\n", key);
		return false;
	}
	_header = ShareMemAPI::MapShareMem(_hold);
	if (_header)
	{
		_dataPtr = _header + sizeof(SMHead);
		MyAssert(((SMHead*)_header)->_key	 ==	key);
		MyAssert(((SMHead*)_header)->_size  ==	size);
		_size = size;
		return true;
	}
	return false;
	__LEAVE_FUNCTION
	return false;
}
