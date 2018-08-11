#ifndef __TRANSCODE_H
#define __TRANSCODE_H

enum qwww_code_t {
    qwww_code_gbk = 0,
    qwww_code_utf8,
    qwww_code_gb2312,
    qwww_code_iso88591,
    qwww_code_ucs2,
    qwww_code_max,
};

int ConvToUnicode(char *src, int nsrc, unsigned short *dst, int ndst, qwww_code_t code);
int ConvFrUnicode(unsigned short *src, int nsrc, char *dst, int ndst, qwww_code_t code);

#endif
