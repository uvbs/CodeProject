#include <ctype.h>
#include "gbklast.h"
#include "tradmap.h"
#include <stdio.h>

int tradmap(unsigned short *buf, int len, int del_tab)
{
    if (!buf || len <= 0) return 0;
    unsigned short *p = buf, *q = buf, *end = buf + len;
    int flag = 0;
	if(del_tab == 1){
		while (p < end) {
			switch (*p) {
				//case '\t':
				case 0x0009:
				case 0x3000:
				case 0xA1A1:
				case 0x000B:
				case 0x000C:
				case 0x0000:
				case ' ':
					flag |= 1;
					break;
				case '\n':
				case '\r':
					flag |= 2;
					break;
				default:
					switch (flag) {
						case 1:
							*q++ = ' ';
							flag = 0;
							break;
						case 2:
						case 3:
							*q++ = '\n';
							flag = 0;
							break;
						default:
							break;
					}
					*q++ = _g_gbk_map[*p];
					break;
			}
			p++;
		}

	} else {
		while (p < end) {
			switch (*p) {
				case 0x3000:
				case 0xA1A1:
				case 0x000B:
				case 0x000C:
				case 0x0000:
				case ' ':
					flag |= 1;
					break;
				case '\n':
				case '\r':
					flag |= 2;
					break;
				default:
					switch (flag) {
						case 1:
							*q++ = ' ';
							flag = 0;
							break;
						case 2:
						case 3:
							*q++ = '\n';
							flag = 0;
							break;
						default:
							break;
					}
					*q++ = _g_gbk_map[*p];
					break;
			}
			p++;
		}

	}
    if (flag >= 2) *q++ = '\n'; 
    return q - buf;
}

#ifdef __TEST_TRADMAP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "transcode.h"
#include "utfdetect.h"

#define MAX_BUFSIZE (512 * 1024)
static char _g_buf[MAX_BUFSIZE];
static unsigned short _g_wcs[MAX_BUFSIZE];

void usage(const char *name)
{
    fprintf(stderr, "usage: %s [-f|-t code] file\n", name);
    exit(0);
}

char * trim(char * ptr)
{
	int start,end,i;
	if (ptr)
	{
		for(start=0; isspace(ptr[start]); start++)
			;
		for(end=strlen(ptr)-1; isspace(ptr[end]) && ptr[end] != '\n'; end--)
			;
		for(i=start; i<=end; i++)
			ptr[i-start]=ptr[i];
		ptr[end-start+1]='\0';
		return (ptr);
	}
	else
		return NULL;
}

int main(int argc, char **argv)
{
    qwww_code_t from = qwww_code_utf8;
    qwww_code_t code = qwww_code_utf8;
    char ch = 0;
    while ((ch = getopt(argc, argv, "f:t:")) > 0) {
        switch(ch) {
            case 'f':
                if (strcasecmp(optarg, "gbk") == 0) {
                    from = qwww_code_gbk;
                }
                break;
            case 't':
                if (strcasecmp(optarg, "gbk") == 0) {
                    code = qwww_code_gbk;
                }
                break;
            default:
                usage(argv[0]);
                break;
        }
    }

    FILE *f = stdin;
    if (argc > optind) f = fopen(argv[optind], "rb");
    if (f == NULL) usage(argv[0]);

    while (fgets(_g_buf, MAX_BUFSIZE, f)) {
        int len = strlen(_g_buf);
        if (len <= 0) continue;
        len = ConvToUnicode(_g_buf, len, _g_wcs, MAX_BUFSIZE, from);
        len = tradmap(_g_wcs, len);
        len = ConvFrUnicode(_g_wcs, len, _g_buf, MAX_BUFSIZE, code);
        _g_buf[len] = '\0';
		trim(_g_buf);

        fputs(_g_buf, stdout);
    }
    return 0;
}

#endif

