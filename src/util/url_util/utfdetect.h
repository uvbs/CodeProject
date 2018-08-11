#ifndef __UTF8_DETECT_H
#define __UTF8_DETECT_H

#define is_utf8_head1(p) ((p[0]&0xe0) == 0xc0)
#define is_utf8_head2(p) ((p[0]&0xf0) == 0xe0)
#define is_utf8_tail1(p) ((p[1]&0xc0) == 0x80)
#define is_utf8_tail2(p) ((p[1]&0xc0) == 0x80 && (p[2]&0xc0) == 0x80)

#define MAX_UTF_DETECT_NUM 256

inline int get_utf8_encoding_range(char *s)
{
    int r = 0;
    unsigned char *p = (unsigned char *)s;
    switch (p[0]) {
        case 0xc2:
        case 0xc3: r = 1; break;
        case 0xc4:
        case 0xc5: r = 2; break;
        case 0xc6:
        case 0xc7:
        case 0xc8: r = 3; break;
        case 0xc9: if (p[1] < 0x90) r = 3; else r = 4; break;
        case 0xca: if (p[1] < 0xb0) r = 4; else r = 5; break;
        case 0xcb: r = 5; break;
        case 0xcc: r = 6; break;
        case 0xcd: if (p[1] < 0xb0) r = 6; else r = 7; break;
        case 0xce:
        case 0xcf: r = 7; break;
        case 0xd0:
        case 0xd1:
        case 0xd2:
        case 0xd3: r = 8; break;
        case 0xd4: if (p[1] < 0xb0) r = 8; else r = 9; break;
        case 0xd5: r = 9; break;
        case 0xd6: if (p[1] < 0x90) r = 9; else r = 10; break;
        case 0xd7: r = 10; break;
        case 0xd8:
        case 0xd9:
        case 0xda:
        case 0xdb: r = 11; break;
        case 0xdc: r = 12; break;
        case 0xdd: if (p[1] < 0x90) r = 12; else r = 11; break;
        case 0xde: r = 13; break;
        case 0xdf: r = 14; break;
        default: break;
    }
    return r;
}

/* r>0: utf8, r=0: not utf8, r<0: unknown */
inline int is_utf8_encoding_strict(char *p, char *end)
{
    int utf8 = 0, utf2 = 0, detect = 0;
#ifdef __CHECK_RANGE
    int range = 0;
#endif
    while (p < end) {
        if (p[0] < 0) {
            if (is_utf8_head2(p)) {
                if (!is_utf8_tail2(p)) {
                    if (utf8 > 3 && p >= end - 2) {
                        if (is_utf8_tail1(p)) break;
                    }
                    return 0;
                }
                utf8++;
                p += 3;
            }
            else if (is_utf8_head1(p)) {
                if (!is_utf8_tail1(p)) return 0;
#ifdef __CHECK_RANGE
                if (utf8 == 0) {
                    int r = get_utf8_encoding_range(p);
                    if (range && r != range) return 0;
                    range = r;
                }
#endif
                utf2++;
                p += 2;
            }
            else return 0;
            if (++detect > MAX_UTF_DETECT_NUM) break;
        }
        else p++;
    }
#ifdef __DEBUG
    printf("detect:%d, utf8:%d utf2:%d\n", detect, utf8, utf2);
#endif
    if (utf8 > 0) return utf8;
    return -utf2;
}

/* r>0: utf8, r=0: not utf8, r<0: unknown */
inline int is_utf8_encoding_ignore(char *p, char *end, int skip = 10)
{
    int utf8 = 0, utf2 = 0, detect = 0, error = 0, comment = 0;
    while (p < end) {
        if (comment == 0 && p[0] < 0) {
            if (is_utf8_head2(p)) {
                if (!is_utf8_tail2(p)) error++;
                else {
                    utf8++;
                    p += 2;
                }
            }
            else if (is_utf8_head1(p)) {
                if (!is_utf8_tail1(p)) error++;
                else {
                    utf2++;
                    p++;
                }
            }
            else error++;
            if (++detect > MAX_UTF_DETECT_NUM) break;
        }
#ifdef __SKIP_HTML_COMMENT
        else if (p[0] == '<' && p[1] == '!' && p[2] == '-') {
            comment = 1;
            p += 2;
        }
        else if (p[0] == '-' && p[1] == '-' && p[2] == '>') {
            comment = 0;
            p += 2;
        }
#endif
        p++;
    }
#ifdef __DEBUG
    printf("detect:%d, error:%d, utf8:%d utf2:%d\n",
            detect, error, utf8, utf2);
#endif
    if (!error && utf8 == 0) return (utf2 > 10) ? utf2 : -utf2;
    if (!error || utf8 / error >= skip) return utf8;
    if (utf8 >= detect / 5) return -utf8;
    return 0;
}

inline int is_utf8_encoding(char *p, char *end = NULL)
{
    return is_utf8_encoding_strict(p, end);
}

#endif

