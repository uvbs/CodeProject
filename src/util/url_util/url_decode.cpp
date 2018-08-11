#include "url_decode.h"

#define ushort unsigned short

#define MAX_BUFSIZE 1024
//static ushort _g_wcs[MAX_BUFSIZE];
//static qwww_code_t _g_code = qwww_code_utf8;

static inline int convert(char c)
{
    if (c >'9') return tolower(c) - 'a' + 10;
    else return c - '0';
}

int url_decode(char *buf, int len, char *res, int rlen)
{
	ushort _g_wcs[MAX_BUFSIZE];
	qwww_code_t _g_code = qwww_code_utf8;
	int ucs = 0;
	rlen =0;
    char *p = buf, *end = buf + len;
    char *q = res, *rend = res + MAX_BUFSIZE - 1;
    ushort *wcs = _g_wcs, *w = _g_wcs;
    while (p < end && q < rend) {
        if (*p == '%') {
            if (p[1] == 'u' || p[1] == 'U') {
                char c1 = (convert(p[2]) << 4) + convert(p[3]);
                char c2 = (convert(p[4]) << 4) + convert(p[5]);
                *q++ = c1; *q++ = c2;
                ushort v = ((ushort)c1 << 8) | (unsigned char)c2;
                if (v >= 0xff || isprint(v)) *w++ = v;
                p += 6;
                ucs = 1;
            }
            else {
                char v = (convert(p[1]) << 4) + convert(p[2]);
                if (v < 0 || isprint(v)) *q++ = *w++ = v;
                p += 3;
            }
        }
        else if (*p == '+' ) {
            *w++ = *q++ = ' ';
            p++;
        }
        else {
            *w++ = *q++ = *p++;
        }
    }
    *q = 0;
    int n = q - res;
    if (ucs != 0) {
        n = ConvFrUnicode(wcs, w - wcs, res, MAX_BUFSIZE, _g_code);
    }
    else {
        qwww_code_t from = qwww_code_gbk;
        int r = is_utf8_encoding(res, q);
        if (r > 0) from = qwww_code_utf8;
        if (r >= 0 && from != _g_code) {
            n = ConvToUnicode(res, n, wcs, MAX_BUFSIZE, from);
            n = ConvFrUnicode(wcs, n, res, MAX_BUFSIZE - 1, _g_code);
        }
    }
    res[n] = 0;
    return n;

}


bool is_utf8(const char* str,long length)
{
    int i;
    int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr;
    bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
    for(i=0;i<length;i++)
    {
        chr= *(str+i);
        if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
            bAllAscii= false;
        if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if(chr>=0x80)
            {
                if(chr>=0xFC&&chr<=0xFD)
                    nBytes=6;
                else if(chr>=0xF8)
                    nBytes=5;
                else if(chr>=0xF0)
                    nBytes=4;
                else if(chr>=0xE0)
                    nBytes=3;
                else if(chr>=0xC0)
                    nBytes=2;
                else
                {
                    return false;
                }
                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if( (chr&0xC0) != 0x80 )
            {
                return false;
            }
            nBytes--;
        }
    }

    if( nBytes > 0 ) //违返规则
    {
        return false;
    }

    if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
    {
        return false;
    }
    return true;
}


int url_gbk2utf8(char *buf, int len)
{
    ushort _g_wcs[MAX_BUFSIZE];
    //qwww_code_t _g_code = qwww_code_utf8;

    ushort *wcs = _g_wcs;
    int n = len;
    int r = is_utf8_encoding(buf,buf+len);
    int ansic_num = 0;
    char* p = buf;
    for (int i = 0; i < len; ++i){
        if(isascii(*(p + i))){
            ansic_num++;
        }
    }

    bool _ret_utf8 = is_utf8((const char*)buf,len);
    //if(r > 0 || (r< 0 && len <= 6) || (r<0 && len - ansic_num == -r*2)) {
    if(_ret_utf8 || r > 0 || (r<0 && len - ansic_num == -r*2)){
        return n;
    } else {
        qwww_code_t from = qwww_code_gbk;
        qwww_code_t to   = qwww_code_utf8;
        n = ConvToUnicode(buf, n, wcs, MAX_BUFSIZE, from);
        n = ConvFrUnicode(wcs, n, buf, MAX_BUFSIZE - 1, to);
        buf[n] = '\0';
        return strlen(buf);
        //return n;
    }
    return n;
}

char* trim(char * ptr)
{
	int start,end,i;
	if (ptr)
	{
		for (start=0; isspace(ptr[start]) && start < (int)strlen(ptr); start++)
			;
		for (end=strlen(ptr)-1; isspace(ptr[end]) && ptr[end] != '\n'; end--)
			;
		for (i=start; i<=end; i++)
			ptr[i-start]=ptr[i];
		ptr[end-start+1]='\0';
		return (ptr);
	}
	else
		return NULL;
}


int url_tradmap(char *buf, int len, char *res, int rlen)
{
    ushort _g_wcs[MAX_BUFSIZE];
	//qwww_code_t _g_code = qwww_code_utf8;
	
	int n = 0;
	rlen = 0;
    n = ConvToUnicode(buf, len, _g_wcs, MAX_BUFSIZE, qwww_code_utf8);
    n = tradmap(_g_wcs, n);
    n = ConvFrUnicode(_g_wcs, n, res, MAX_BUFSIZE - 1, qwww_code_utf8);
    res[n] = '\0';
	trim(res);
    return strlen(res);
}

static inline char *escape(char *p, char *q)
{ 
    int ch = (unsigned char)(*p);
    int hi = ch / 16;
    int lo = ch % 16;
    *q++ = '%';
    *q++ = hi + (hi > 9 ? ('A' - 10) : '0');
    *q++ = lo + (lo > 9 ? ('A' - 10) : '0');
    return q;
}

static inline int is_reserved_char(char c)
{
    switch (c) {
        case ';':
        case '/':
        case '?':
        case ':':
        case '@':
        case '=':
        case '&':
        case '#':
            return 1;
        default:
            return 0;
    }
}

static inline int is_safe_char(char c)
{
    if (isalnum(c)) return 1;
    switch(c) {
        case '$':
        case '-':
        case '_':
        case '.':
        case '!':
        case '*':
        case '(':
        case ')':
        case ',':
            //case '+':
            return 1;
        default:
            return 0;
    }
}

int url_escape(char *buf, int len, char *res, int rlen, int gbk /*= 0*/)
{
    char *p = buf, *end = buf + len;
    char *q = res, *rend = res + rlen - 5;
    while (p < end && q < rend && q - res < rlen - 5) {
        if (*p < 0) {
            if (gbk) {
                while (p < end && *p < 0) {
                    q = escape(p++, q);
                    q = escape(p++, q);
                }
            }
            else {
                while (p < end && *p < 0) q = escape(p++, q);
            }
        }
        else if (is_safe_char(*p)) *q++ = *p++;
        else q = escape(p++, q);
    }
    *q = 0;
    return q - res;
}

#ifdef __TEST_DECODE
void usage(char *name)
{
    static char *us = "[-f field] [-g]";
    fprintf(stderr, "usage: %s %s file\n", name, us);
    exit(0);
}

#include <unistd.h>
static char _g_buf[MAX_BUFSIZE];
static char _g_res[MAX_BUFSIZE];

int main(int argc, char **argv)
{
    int ch = 0, field = 1, esc = 0;
    while ((ch = getopt(argc, argv, "f:ge")) > 0) {
        switch(ch) {
            case 'f':
                field = atoi(optarg);
                break;
            case 'g':
                _g_code = qwww_code_gbk;
                break;
            case 'e':
                esc = 1;
                break;
            default:
                usage(argv[0]);
                break;
        }
    }

    FILE *f = stdin;
    if (argc > optind) f = fopen(argv[optind], "rb");
    if (f == NULL) return 0;

    while(fgets(_g_buf, MAX_BUFSIZE, f)) {
        char *p = _g_buf;
        int len = strlen(p);
        while (len > 0 && isspace(p[len-1])) len--;
        if (len <= 0) continue;
        p[len] = 0;
        char *q = p, *end = p + len;
        if (field > 0) {
            int n = field;
            while (q < end) {
                if (*q++ == '\t') {
                    if (--n == 0) break;
                    p = q;
                }
            }
            len = q - p;
            if (q < end) len--;
            if (n > 1) len = -1;
        }
        _g_res[0] = 0;
        if (len <= 0) continue;
        if (esc) url_escape(p, len, _g_res, sizeof(_g_res));
        else url_decode(p, len, _g_res, sizeof(_g_res));
        //printf("%s\t%s\n", _g_res, _g_buf);
        if (p > _g_buf) printf("%.*s", (int)(p - _g_buf), _g_buf);
        printf("%s", _g_res);
        if (q < end) printf("\t%s", q);
        printf("\n");
    }
    return 0;
}

#endif

