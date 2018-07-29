/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <Archive.h>

#include <memory.h>

namespace Serialization
{

	Archive::Archive()
	{
		_buf = 0;
		_cursor = 0;
	}
	
	OutArchive::OutArchive(int size)
		: Archive(), _size(size)
	{
		_buf = new char[size];
		memset(_buf,0,size);
		_cursor = _buf;
	}

	void OutArchive::write(const void* data, int size)
	{
		memcpy(_cursor,data,size);
		_cursor += size;
	}
	
	void* OutArchive::get()
	{
		return _buf;
	}
	
	int OutArchive::size()
	{
		return _size;
	}

	OutArchive::~OutArchive()
	{
		delete [] _buf;
	}
	
	InArchive::InArchive(void* buf)
	{
		_buf = (char*) buf;
		_cursor = _buf;
	}
	
	void* InArchive::read(int size)
	{
		char* ret = _cursor;
		_cursor += size;
		return ret;
	}

} // namespace Serialization

