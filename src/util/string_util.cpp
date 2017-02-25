#include "string_util.h"
#include <string.h>
#include <stdarg.h>
#include <algorithm>
#include "assert.h"
#include <stdio.h>
#include <sstream>
using std::stringstream;

#ifdef _WIN32
#include <Windows.h>
#elif _LINUX64
#endif

namespace str_util {

char* strcpy(char* pDestination, const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strcpy(pDestination, pSource);
	__LEAVE_FUNCTION
	return NULL;
}

char* strncpy(char* pDestination, const char* pSource, uint length)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strncpy(pDestination, pSource, length);
	__LEAVE_FUNCTION
	return NULL;
}

char* strcpy_s(char* pDestination, const char* pSource, const int length)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	if (str_util::strlen(pSource) >= length)
	{
		Assert(false);
	}

#ifdef _WIN32
	::strcpy_s(pDestination, length / sizeof(char), pSource);
#endif

#ifdef _LINUX64
	::strncpy(pDestination, pSource, length / sizeof(char));
#endif

	pDestination[length / sizeof(char) - 1] = 0;
	return pDestination;
	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strcat, Strncat
//	- 把pSource所指的由NULL结尾的字符串添加到pDestination所指的内存区域
//	- 覆盖原来的'/0'并添加'/0'
//	- pDestination与pSource所指的区域不可以重叠
//	- 返回指向pDestination的指针
/////////////////////////////////////////////////////////////////////////////
char* strcat(char* pDestination, const char* pSource)	
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strcat(pDestination, pSource);
	__LEAVE_FUNCTION
	return NULL;
}

char* strncat(char* pDestination, const char* pSource, uint length)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strncat(pDestination, pSource, length);
	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strcmp, Strncmp
//	- 比较字符串pSource1和字符串pSource2
//	- 如果 (pSource1 < pSource2) 返回值 <0
//	- 如果 (pSource2 = pSource2) 返回值 =0
//	- 如果 (pSource1 > pSource2) 返回值 >0
/////////////////////////////////////////////////////////////////////////////
int strcmp(const char* pSource1, const char* pSource2)	
{
	__ENTER_FUNCTION
	Assert(pSource1);
	Assert(pSource2);
	return ::strcmp(pSource1, pSource2);
	__LEAVE_FUNCTION
	return -1;
}

int strncmp(const char* pSource1, const char* pSource2, uint length)	
{
	__ENTER_FUNCTION
	Assert(pSource1);
	Assert(pSource2);
	return ::strncmp(pSource1, pSource2, length);
	__LEAVE_FUNCTION
	return -1;
}

int strlen(const char* pSource)			
{
	__ENTER_FUNCTION
	Assert(pSource);
	return (int)::strlen(pSource);
	__LEAVE_FUNCTION
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Utf8len
//	- 计算字符串pSource的长度
//	- 返回字符串的长度, 不包括结束符
/////////////////////////////////////////////////////////////////////////////
int	 strUtf8len(const char* s)
{
	__ENTER_FUNCTION
	Assert(s);
	uchar tcp;
	const uchar* buf = reinterpret_cast<const uchar*>(s);
	int count = 0;
	int len = str_util::strlen(s);
	while (len > 0)
	{
		--len;
		tcp = *buf++;
		++count;

		if (tcp < 0x80)
		{
		}
		else if (tcp < 0xE0)
		{
			--len;
			++buf;
		}
		else if (tcp < 0xF0)
		{
			len -= 2;
			buf += 2;
		}
		else
		{
			len -= 3;
			buf += 3;
		}
	}
	if (len < 0)
	{
		//字符出错了,默认长度为0
		count = 0;
	}
	return count;
	__LEAVE_FUNCTION
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strstr
//	- 从pHaystack字符串中寻找pNeedle字符串第一次出现的位置
//	- 不比较结束符
//	- 返回第一次出现pNeedle位置的指针, 如果没有找到返回NULL
/////////////////////////////////////////////////////////////////////////////
const char* strstr(const char* pHaystack, const char* pNeedle)
{
	__ENTER_FUNCTION
	Assert(pHaystack);
	Assert(pNeedle);
	return ::strstr(pHaystack, pNeedle);
	__LEAVE_FUNCTION
	return NULL;
}

int snprintf(char *pBuffer, int maxlen, const char *pFormat, ...)
{
	__ENTER_FUNCTION
	Assert(pBuffer);
	Assert(pFormat);

	int result;
	va_list argptr ;
	va_start(argptr, pFormat);

//#if defined (_LINUX64)
//	result = vsnprintf(pBuffer, maxlen, pFormat, argptr);
//#elif defined (_WIN32)
//	result = _vsnprintf(pBuffer, maxlen, pFormat, argptr);
//#endif	
	result = vsnprintf(pBuffer, maxlen, pFormat, argptr);
	va_end(argptr);
	return result;
	__LEAVE_FUNCTION
	return -1;
}

int vsnprintf(char* buf, int maxlen, const char* format, va_list varList)
{
	__ENTER_FUNCTION
	Assert(buf);
	Assert(format);

	int result = 0;
#if defined (_LINUX64)
	result = ::vsnprintf(buf, maxlen, format, varList);
#elif defined (_WIN32)
	result = _vsnprintf(buf, maxlen, format, varList);
#endif

	return result;
	__LEAVE_FUNCTION
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtok
//	- 分解字符串为一组标记串, pSource为分解的字符串, pDelimit为分隔符
//	- 首次调用时, pSource必须指向要分解的字符串, 
//	- 之后的调用要把pSource设置成NULL
/////////////////////////////////////////////////////////////////////////////
char* strtok(char* pSource, const char* pDelimit)
{
	__ENTER_FUNCTION
	Assert(pDelimit);
	return ::strtok(pSource, pDelimit);
	__LEAVE_FUNCTION
	return NULL;
}

char* strtok_s(char* pSource, const char* pDelimit, char** pContext)
{
	__ENTER_FUNCTION
		
	Assert(pDelimit);
#ifdef _WIN32
	return ::strtok_s(pSource, pDelimit, pContext);
#endif

#ifdef _LINUX64
	return ::strtok_r(pSource, pDelimit, pContext);
#endif

	__LEAVE_FUNCTION
	return NULL;
}

bool split(const string& str, char delim, vector<string>& r) 
{
	stringstream ss(str);
	string item;
	while (getline(ss, item, delim))
    {
        r.push_back(item);
    }
	return true;
}

int strToVector(const char* strSource, vector<string>& vRet, const char* strKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();

	//------------------------------------------------------------
	//合法性
	if (!strSource || strSource[0] == '\0')
	{
		return 0;
	}
	if (!strKey || strKey[0] == '\0')
	{
		return 0;
	}
	string strSrc = strSource;

	//------------------------------------------------------------
	//查找第一个分割点
	string::size_type left = 0;
	string::size_type right;
	if (bOneOfKey)
	{
		right = strSrc.find_first_of(strKey);
	}
	else
	{
		right = strSrc.find(strKey);
	}
	if (right == string::npos)
	{
		right = strSrc.length();
	}

	while(true)
	{
		string strItem = strSrc.substr(left, right-left);
		if (strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}
		if (right == strSrc.length())
		{
			break;
		}
		left = right + (bOneOfKey ? 1 : str_util::strlen(strKey));
		if (bOneOfKey)
		{
			string strTemp = strSrc.substr(left);
			right = strTemp.find_first_of(strKey);
			if (right != string::npos) right += left;
		}
		else
		{
			right = strSrc.find(strKey, left);
		}
		if (right == string::npos)
		{
			right = strSrc.length();
		}
	}

	return (int)vRet.size();
}

const char* getLineFromMemory(char* pStringBuf, int bufSize, const char* pMemStart, const char* pMemEnd)
{
	__ENTER_FUNCTION

	//寻找下一个换行符
	const char* pMem = pMemStart;
	if (pMem >= pMemEnd || *pMem==NULL)
	{
		return NULL;
	}
	while( pMem < pMemEnd && 
		pMem - pMemStart + 1 < bufSize &&
		*pMem != 0 && *pMem != '\r' && *pMem != '\n') 
	{
		*(pStringBuf++) = *(pMem++);
	}
	//添加结束符
	*pStringBuf = 0;

	//继续跳过后面紧跟的'\r'或'\n'，找到下一个开始的位置
	while(pMem < pMemEnd && *pMem != 0 && 
		(*pMem == '\r' || *pMem == '\n')) 
	{
		pMem++;
	}

	return pMem;

	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Atoi
//	- 将pSource指向的字符串转换为整型
//	- 跳过pSource前面的空格字符, 直到遇到数字或正负符号开始转换
//	- 遇到非数字或者字符串结束符结束转换
/////////////////////////////////////////////////////////////////////////////
int strToInt(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::atoi(pSource);
	__LEAVE_FUNCTION
	return -1;
}

uint strToUint(const char*  pSource, char**  pEnd, int nBase)
{
    __ENTER_FUNCTION
    Assert(pSource);
    return ::strtoul(pSource, pEnd, nBase);
    __LEAVE_FUNCTION
     return (uint)-1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtol
//	- 将pSource指向的字符串转换为长整型, nBase代表的进制方式
/////////////////////////////////////////////////////////////////////////////
long strToL(const char* pSource, char**  pEnd, int nBase)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtol(pSource, pEnd, nBase);
	__LEAVE_FUNCTION
	return -1;
}

ulong strToUL(const char*  pSource, char**  pEnd, int nBase)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtoul(pSource, pEnd, nBase);
	__LEAVE_FUNCTION
	return (ulong)-1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtoint64
//	- 将pSource指向的字符串转换为64位整型, nBase代表的进制方式
//////////////////////////////////////////////////////////////////////
int64 strToInt64(const char*  pSource, char**  pEnd, int nBase)
{
#ifdef _WIN32
	return ::_strtoi64(pSource, pEnd, nBase);
#else
	return ::strtoll(pSource, pEnd, nBase);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//	- StrToUInt64
//	- 将pSource指向的字符串转换为64位无符号整型, nBase代表的进制方式
//////////////////////////////////////////////////////////////////////
uint64 strToUint64(const char*  pSource, char**  pEnd, int nBase)
{
#ifdef _WIN32
	return ::_strtoui64(pSource, pEnd, nBase);
#else
	return ::strtoull(pSource, pEnd, nBase);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//	- Atof
//	- 将pSource指向的字符串转换为浮点型
/////////////////////////////////////////////////////////////////////////////
float strToFloat(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::atof(pSource);
	__LEAVE_FUNCTION
	return -1.0f;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtod
//	- 将pSource指向的字符串转换为浮点型
/////////////////////////////////////////////////////////////////////////////
double strToDouble(const char*  pSource, char**  pEnd)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtod(pSource, pEnd);
	__LEAVE_FUNCTION
	return -1.0f;
}

char* int32ToStr(int nValue, char* pString, int nRadix)
{
	__ENTER_FUNCTION
	Assert(pString);

#if defined (_WIN32)
	return ::itoa(nValue, pString, nRadix);

#elif defined (_LINUX64)
	char* pEnd = pString;
	char* pStart = pString;
	//待转换的为0
	if (0 == nValue)
	{
		pString[0] = '0';
		pString[1] = 0;
		return pString;
	}
	//待转换的为负数，进制10
	if ((nValue < 0)&&( 10 == nRadix))
	{
		//设置符号
		pString[0] = '-';
		pEnd++;
		pStart++;
		nValue = -nValue;
	}
	//根据nRadix进制转换数字，倒序字符串
	while (nValue != 0)
	{
		int nMode = nValue % nRadix;
		nValue = nValue / nRadix;
		*pEnd++ = (nMode < 10) ? '0' + nMode:'a' + nMode - 10;
	}
	*pEnd-- = 0;
	//反转字符串
	char cTemp;
	while (pEnd > pStart)
	{
		cTemp = *pEnd;
		*pEnd = *pStart;
		*pStart = cTemp;
		pEnd--;
		pStart++;
	}
	return pString;
#endif
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

bool uint64ToStr(uint64 i, char* pDest, int base)
{
__ENTER_FUNCTION
	if (base==16)
	{
		sprintf(pDest, "%llx", i);
	}
	else
	{
		sprintf(pDest, "%llu", i);
	}
//#ifdef __WINDOWS_
//	_ui64toa(i, pDest, base);
//#else
//	
//#endif
	return true;
__LEAVE_FUNCTION
	return false;
}

bool floatToStr(float i,char* pDest, int base/*=10*/)
{
	if (base==16)
	{
		sprintf(pDest, "%lx", i);
	}
	else
	{
		sprintf(pDest, "%lf", i);
	}
	return true;
}

int trimRight( char* s )
{
	int stringLength = ::strlen(s);
	int deleteEndCount = 0;
	//先清除末尾的空字符
	for (int i = stringLength - 1; i > 0; --i)
	{
		if (!::isspace(s[i]))
			break;
		s[i] = 0;
		deleteEndCount++;
	}
	return deleteEndCount;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrLeftBlank
//	- 删除字符串左侧的空格字符
/////////////////////////////////////////////////////////////////////////////
bool trimLeft(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);
	if (pSrc == NULL)
	{
		return false;
	}
	while (*pSrc==' ' || *pSrc=='\t')
	{
		memmove(pSrc, pSrc+1, str_util::strlen(pSrc));
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrRightBlank
//	- 删除字符串右侧的空格字符
/////////////////////////////////////////////////////////////////////////////
bool trimRightV2(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);

	if (pSrc == NULL)
	{
		return false;
	}
	char* pTail = pSrc+str_util::strlen(pSrc)-1;
	while (*pTail==' ' || *pTail=='\t')
	{
		*pTail = '\0';
		pTail = pSrc+str_util::strlen(pSrc)-1;
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrBlank
//	- 删除字符串中的空格字符
/////////////////////////////////////////////////////////////////////////////
bool trim(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);

	bool bErase1 = trimLeft(pSrc);
	bool bErase2 = trimRightV2(pSrc);
	return bErase1 && bErase2;

	__LEAVE_FUNCTION
	return false;
}

bool trim(string& str, char c)
{
    if (str.empty()) 
    {
        return true;
    }
    str.erase(0, str.find_first_not_of(c));
    str.erase(str.find_last_not_of(c) + 1);
    return true;
}

bool isInteger(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);

	if (pSource[0] == '-')
	{
		if (str_util::strlen(pSource) == 1)
		{
			return false;
		}
		++pSource;
	}
	while(*pSource != '\0')
	{
		if((*pSource < '0') || (*pSource > '9'))
		{
			return false;
		}
		++pSource;
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool isFloat(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);

	if (pSource[0] == '-')
	{
		if (str_util::strlen(pSource) == 1)
		{
			return false;
		}
		++pSource;
	}

	while(*pSource != '\0')
	{
		if(((*pSource >= '0') && (*pSource <= '9'))||(*pSource == '.'))
		{
			++pSource;
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

void toUpper(std::string &str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void toLower(std::string &str)
{
	transform(str.begin(),str.end(), str.begin(), ::tolower);
}

std::string bin2hex(const void *data, unsigned int len)
{
	uint8 *bytes = (uint8*)data;
	std::string ret;
	ret.reserve(len*2+1);

	char tmp[8];
	for(unsigned int i=0; i<len; ++i)
	{
		snprintf(tmp,sizeof(tmp), "%02X", bytes[i]);
		ret += tmp;
	}
	return ret;
}

bool hex2bin(std::string &hex_str, uint8*&data, unsigned int len)
{
	toUpper(hex_str);

	if(hex_str.size() < len * 2) 
	{
		return false;
	}

	char tmp[16]={0};
	unsigned int ch;
	unsigned int idx=0;
	for(unsigned int i = 0; i < len*2;) 
	{
		::memcpy(tmp, hex_str.substr(i, 2).c_str(),2);
		::sscanf(tmp, "%02X", &ch);
		data[idx++] = (uint8)ch;
		i+=2;
	}
	return true;
}

/* Convert byte array to hex string. */
string bytesToHexString(const unsigned char* input, size_t length) {

    const char HEX[16] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };
    string str;
    str.reserve(length << 1);
    for (size_t i = 0; i < length; ++i) {
        int t = input[i];
        int a = t / 16;
        int b = t % 16;
        str.append(1, HEX[a]);
        str.append(1, HEX[b]);
    }
    return str;
}

bool gbkToUtf8(const char *src, string& result)
{
#ifdef _WIN32
 
    //TODO: check 
    int len = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_UTF8,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    if (strTemp.find('?') != std::string::npos)
    {
        strTemp.assign(src);
    }

    result = strTemp;
    delete[]szGBK;
    delete[]wszGBK;

    return true;

#elif _LINUX64

    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码    
    {
        return false;
    }    
    int unicodeLen=mbstowcs(NULL,src,0);//计算转换后的长度
    if(unicodeLen<=0)    
    {
        return false;    
    }
    wchar_t *unicodestr = new wchar_t[unicodeLen+1];
    mbstowcs(unicodestr,src,str_util::strlen(src));//将gbk转换为unicode
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8    
    {
        return false;    
    }   
    int len = wcstombs(NULL,unicodestr,0);//计算转换后的长度    
    if(len <= 0)    
    {
        return false;    
    }
    char* dest = new char[len+1];
    wcstombs(dest,unicodestr,len);
    dest[len]=0;//添加结束符
    result = dest;

    delete[] unicodestr;
    delete[] dest;
    return true;
#endif
}

bool utf8ToGbk(const char *src, string& result)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    if (strTemp.find('?') != std::string::npos)
    {
        strTemp.assign(src);
    }

    result = strTemp;
    delete[]szGBK;
    delete[]wszGBK;
    
    return true;

#elif _LINUX64

    if(NULL== setlocale(LC_ALL,"zh_CN.utf8"))//设置转换为unicode前的码,当前为utf8编码    
	{
		return false;
	}    
	int unicodeLen = mbstowcs(NULL,src,0);//计算转换后的长度
	if(unicodeLen <= 0)    
	{
		return false;    
	}
    wchar_t *unicodestr = new wchar_t[unicodeLen+1];
    mbstowcs(unicodestr,src,str_util::strlen(src));//将utf8转换为unicode
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置unicode转换后的码,当前为gbk    
	{
		return false;    
	}   
	int len = wcstombs(NULL,unicodestr,0);//计算转换后的长度    
	if(len<=0)    
	{
		return false;    
	}
    char* dest = new char[len+1];
	wcstombs(dest,unicodestr,len);
	dest[len]=0;//添加结束符

    result = dest;
    delete[] unicodestr;
    delete[] dest;
    
    return true;

#endif
}

} // namespace str_util
