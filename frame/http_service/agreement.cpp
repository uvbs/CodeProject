#include "agreement.h"

void Agreement::_loadArgv(HashArgv *hash, char* data)
{
	while(data != NULL && data[0] != 0)
	{
		char *temp, *arg;
		if ((temp = strchr(data, '&')) != NULL) {
			*temp++ = 0;
		}
		if ((arg = strchr(data, '=')) != NULL) {
			*arg++ = 0;
			hash->Insert(data, arg);
		}
		data = temp;
	}
}

void	Agreement::parser(char* uri)
{
	_loadArgv(&_argv, uri);
	char* data = NULL;
	if (_argv.Lookup("type", data) && data) {
		_type = str_util::strToInt(data);
	}
	if (_argv.Lookup("flag", data) && data) {
		_flag = str_util::strToInt(data);
	}

}
