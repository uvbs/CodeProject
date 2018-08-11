#ifndef __DTREE_H
#define __DTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef DTREE_C_API
#ifndef DTREE_CLASS
#ifdef  __cplusplus
#define DTREE_CLASS
#endif
#endif
#endif

#ifdef  DTREE_WCHAR
typedef unsigned short dtree_elem_t;
#else
typedef unsigned char  dtree_elem_t;
#endif
typedef long dtree_void_t;

#ifdef  DTREE_ICASE
#define TOLOWER(e) tolower(e)
#else
#define TOLOWER(e) (e)
#endif

#ifdef  DTREE_CLASS
template <class T = char>
class DTreeClass{
    private:
#endif

#define DTREE_SIZE_VOID   sizeof(dtree_void_t)
#define DTREE_SIZE_ELEM   sizeof(dtree_elem_t)
#define DTREE_ELEM_NUM   (DTREE_SIZE_VOID / DTREE_SIZE_ELEM - 1)
#define DTREE_ELEM_MAX   (1 << (8 * DTREE_SIZE_ELEM))

        typedef struct {
            unsigned char type:4;
            unsigned char n:4;
#ifdef  DTREE_WCHAR
            unsigned char none;
#endif
            dtree_elem_t v[DTREE_ELEM_NUM];
            dtree_void_t p;
        } dtree_leaf_t;

#define DTREE_SIZE_LEAF  sizeof(dtree_leaf_t)

#define DTREE_TYPE_NONE        0
#define DTREE_TYPE_NODE        1
#define DTREE_TYPE_DATA        2
#define DTREE_TYPE_LEAF        3
#define DTREE_TYPE_LEAF_NODE   4
#define DTREE_TYPE_LEAF_DATA   5
#define DTREE_TYPE_LIST        6
#define DTREE_TYPE_LIST_NODE   7
#define DTREE_TYPE_LIST_DATA   8

#define DTREE_IS_NODE(t)  ((t) % 3 == 1) 
#define DTREE_IS_DATA(t)  ((t) % 3 == 2) 
#define DTREE_IS_LEAF(t)  ((t) / 3 == 1) 
#define DTREE_IS_LIST(t)  ((t) >= 6) 
#define DTREE_IS_NEXT(t)  ((t) >= 3)
#define DTREE_IS_LINK(t)  ((t) == 5 || (t) == 8)

        typedef struct {
            dtree_void_t data;
            dtree_void_t p;
        } dtree_link_t;

#define DTREE_SIZE_LINK     (sizeof(dtree_link_t))

#define DTREE_LINK_DATA(t) ((dtree_link_t *)(long)(t))->data
#define DTREE_LINK_LIST(t)  (dtree_list_t *)(long)(((dtree_link_t *)(long)(t))->p)
#define DTREE_LINK_LEAF(t)  (dtree_leaf_t *)(long)(((dtree_link_t *)(long)(t))->p)

        typedef struct {
            dtree_elem_t max;
            dtree_elem_t n;
            dtree_leaf_t leaf[1];
        } dtree_list_t;

#define DTREE_SIZE_LIST(n) (sizeof(dtree_list_t)+((n)-1)*DTREE_SIZE_LEAF)

#ifdef  DTREE_DEBUG
        typedef struct {
            int mem_malloc;
            int mem_free;
            int num_list;
            int num_leaf;
            int num_link;
            int num_char;
            int num_full_list;
            int avg_list_len;
        } dtree_stat_t;
#endif

        typedef struct {
            int magic;
            int count;
            int me:8;
            int len:24;
            dtree_void_t root;
        } dtree_t;

#define DTREE_MAGIC1      0x1234
#define DTREE_MAGIC2      0xabcd
#define DTREE_SIZE_TREE  (sizeof(dtree_t))
#define DTREE_ME_COPY     0x63
#define DTREE_ME_DUMP     0x65

#define DTREE_VOID_T(v)  (dtree_void_t)(long)(v)
#define DTREE_LIST_T(v)  (dtree_list_t *)(long)(v)
#define DTREE_LEAF_T(v)  (dtree_leaf_t *)(long)(v)
#define DTREE_LINK_T(v)  (dtree_link_t *)(long)(v)

#define DTREE_GET_ADDR(p,t)  ((char *)(long)(p)+(((t)->me==DTREE_ME_DUMP)?(long)(t):0))
#define DTREE_GET_DATA(p,t)   (void *)(((t)->len)?DTREE_GET_ADDR(p,t):(void *)(long)p)
#define DTREE_GET_LIST(p,t)   (dtree_list_t *)DTREE_GET_ADDR(p,t)
#define DTREE_GET_LEAF(p,t)   (dtree_leaf_t *)DTREE_GET_ADDR(p,t)
#define DTREE_GET_LINK(p,t)   (dtree_link_t *)DTREE_GET_ADDR(p,t)

#define DTREE_GET_LINK_DATA(p,t) DTREE_GET_DATA(DTREE_LINK_DATA(DTREE_GET_LINK(p,t)),t)
#define DTREE_GET_LINK_LEAF(p,t) DTREE_GET_LEAF(DTREE_LINK_LEAF(DTREE_GET_LINK(p,t)),t)
#define DTREE_GET_LINK_LIST(p,t) DTREE_GET_LIST(DTREE_LINK_LIST(DTREE_GET_LINK(p,t)),t)

#define DTREE_FREE(p)     do{if(p)free((char *)(long)(p));p=0;}while(0)
#define DTREE_TRAN(c,i)   do{c^=(char)(i&0x55);}while(0)

#ifndef DTREE_CLASS
#ifdef  DTREE_DEBUG
        static void print_leaf(dtree_t * t, dtree_leaf_t *leaf, int lv, int lvmax);
        static void print_list(dtree_t *t, dtree_list_t *ls, int lv, int lvmax);
        void dtree_print(dtree_t *t, int lvmax);
#endif
        static dtree_void_t new_leaf();
        static dtree_void_t new_link(dtree_void_t data, dtree_void_t p);
        static dtree_void_t new_list(int n);
        static void delete_leaf(dtree_leaf_t *leaf, int copy);
        static void delete_list(dtree_list_t *ls, int copy);
        static void save_leaf(dtree_leaf_t *leaf, int copy, int len, FILE *f);
        static void save_list(dtree_list_t *ls, int copy, int len, FILE *f);
        static dtree_leaf_t *get_leaf2(dtree_list_t *ls, dtree_elem_t e);
        static dtree_leaf_t *get_leaf(dtree_list_t *ls, dtree_elem_t e);
        static int append(dtree_leaf_t *leaf, dtree_elem_t *p, dtree_elem_t *end, void *ptr);
        static dtree_list_t *resize_list(dtree_list_t *ls);
        static dtree_leaf_t *move_list(dtree_list_t *ls, dtree_elem_t e);
        dtree_t *dtree_create(int copy, int len);
        void dtree_destroy(dtree_t *t);
        dtree_t *dtree_load(const char *path);
        dtree_t *dtree_load_buffer(void *buf, int len);
        int dtree_save(dtree_t *t, const char *path);
        int dtree_match(dtree_t *t, void *s, int len, void **ptr);
        int dtree_match_internel(dtree_t *t, void *s, void *last, void **ptr);
        int dtree_match_all(dtree_t *t, void *s, int len, void **rp, int *rs, int rn);
        int dtree_erase(dtree_t *t, void *s, int len);
        int dtree_insert(dtree_t *t, void *s, int len, void *ptr);
        int dtree_transform(const char *path);
        int dtree_count(dtree_t *t);
#endif

#ifdef  DTREE_DEBUG

        static void print_leaf(dtree_t * t, dtree_leaf_t *leaf, int lv, int lvmax)
        {
            int i = 0, type = 0;
            if (!leaf || (dtree_t *)leaf == t || leaf->n == 0) return;
            if (lv > lvmax) return;
            type = leaf->type;
            for (i = 0; i < lv; i++) printf("  ");
            printf("leaf.%d: t=%d n=%d p=%p e=", lv, type, leaf->n, leaf->p);
            for (i = 0; i < leaf->n; i++) {
                if (isprint(leaf->v[i])) printf("%c,", leaf->v[i]);
                else printf("%x,", leaf->v[i]);
            }
            printf("\n");
            if (DTREE_IS_LINK(type)) leaf = DTREE_GET_LEAF(leaf->p, t);
            if (DTREE_IS_LIST(type)) {
                print_list(t, DTREE_GET_LIST(leaf->p, t), lv + 1, lvmax);
            }
            if (DTREE_IS_LEAF(type)) {
                print_leaf(t, DTREE_GET_LEAF(leaf->p, t), lv + 1, lvmax);
            }
        }

        static void print_list(dtree_t *t, dtree_list_t *ls, int lv, int lvmax)
        {
            int i = 0;
            int n = 0;
            if (!ls || (dtree_t *)ls == t || lv > lvmax) return;
            for (i = 0; i < lv; i++) printf("  ");
            n = ls->max ? ls->n : DTREE_ELEM_MAX;
            printf("list.%d: n=%d max=%d\n", lv, ls->n, ls->max);
            for (i = 0; i < n; i++) {
                if (ls->leaf[i].n) print_leaf(t, ls->leaf + i, lv, lvmax);
            }
        }

        /* 
         * print tree, lvmax: max nest level
         */
        void dtree_print(dtree_t *t, int lvmax)
        {
            assert(t);
            printf("dtree: me=%c len=%d\n", t->me ? t->me : '0', t->len);
            print_list(t, DTREE_GET_LIST(t->root, t), 0, lvmax);
        }

#endif

        static inline dtree_void_t new_leaf()
        {
            dtree_leaf_t *leaf = (dtree_leaf_t *)malloc(DTREE_SIZE_LEAF);
            if (!leaf) return (dtree_void_t)0;
            memset(leaf, 0, DTREE_SIZE_LEAF);
            return DTREE_VOID_T(leaf);
        }

        static inline dtree_void_t new_link(dtree_void_t data, dtree_void_t p)
        {
            dtree_link_t *link = (dtree_link_t *)malloc(DTREE_SIZE_LINK);
            if (!link) return (dtree_void_t)0;
            link->data = data;
            link->p = p;
            return DTREE_VOID_T(link);
        }

        static inline dtree_void_t new_list(int n)
        {
            dtree_list_t *ls = (dtree_list_t *)malloc(DTREE_SIZE_LIST(n));
            if (!ls) return (dtree_void_t)0;
            memset(ls, 0, DTREE_SIZE_LIST(n));
            ls->max = n;
            return DTREE_VOID_T(ls);
        }

        /* 
         * create tree, return NULL if failed
         * copy: if copy data, len: data len to save 
         */
        dtree_t *dtree_create(int copy, int len)
        {
            dtree_t *t = (dtree_t *)malloc(DTREE_SIZE_TREE);
            if (!t) return NULL;
            memset(t, 0, DTREE_SIZE_TREE);
            t->magic = DTREE_MAGIC1;
            t->root = new_list(DTREE_ELEM_MAX);
            if (!t->root) {
                DTREE_FREE(t);
                return NULL;
            }
            if (copy && len > 0) t->me = DTREE_ME_COPY;
            if (len > 0) t->len = len;
            return t;
        }

        static void delete_leaf(dtree_leaf_t *leaf, int copy)
        {
            int type = 0;
            if (!leaf || leaf->n == 0) return;
            type = leaf->type;
            if (DTREE_IS_LINK(type)) leaf = DTREE_LEAF_T(leaf->p);
            assert(leaf);
            if (DTREE_IS_LIST(type)) delete_list(DTREE_LIST_T(leaf->p), copy);
            if (DTREE_IS_LEAF(type)) delete_leaf(DTREE_LEAF_T(leaf->p), copy);
            if (leaf->p) {
                if (type == DTREE_TYPE_DATA) {
                    if (copy) DTREE_FREE(leaf->p);
                }
                else if (DTREE_IS_NEXT(type)) {
                    DTREE_FREE(leaf->p);
                }
            }
            if (DTREE_IS_LINK(type)) {
                if (copy) {
                    DTREE_FREE(DTREE_LINK_DATA(leaf));
                }
                DTREE_FREE(leaf);
            }
        }

        static void delete_list(dtree_list_t *ls, int copy)
        {
            if (!ls) return;
            int i = 0;
            int n = ls->max ? ls->n : DTREE_ELEM_MAX;
            for (i = 0; i < n; i++) {
                if (ls->leaf[i].n) delete_leaf(ls->leaf + i, copy);
            }
        }

        /*
         * destroy tree, free memory
         */
        void dtree_destroy(dtree_t *t)
        {
            assert(t);
            if (t->me != DTREE_ME_DUMP) {
                delete_list(DTREE_LIST_T(t->root), t->me == DTREE_ME_COPY);
                DTREE_FREE(t->root);
            }
            DTREE_FREE(t);
        }

        static void save_leaf(dtree_leaf_t *leaf, int copy, int len, FILE *f)
        {
            int type = 0, size = 0;
            dtree_void_t pos = (dtree_void_t)0;
            dtree_leaf_t *leaf_saved = leaf;
            if (!leaf || leaf->n == 0) return;
            type = leaf->type;
            if (DTREE_IS_LINK(type)) leaf = DTREE_LEAF_T(leaf->p);
            assert(leaf);
            if (DTREE_IS_LIST(type)) save_list(DTREE_LIST_T(leaf->p), copy, len, f);
            if (DTREE_IS_LEAF(type)) save_leaf(DTREE_LEAF_T(leaf->p), copy, len, f);
            if (leaf->p) {
                pos = (dtree_void_t)ftell(f);
                if (type == DTREE_TYPE_DATA) {
                    if (len) fwrite((char *)(long)leaf->p, len, 1, f);
                    else pos = leaf->p;
                    if (copy) DTREE_FREE(leaf->p);
                }
                else if (DTREE_IS_LIST(type)) {
                    size = (DTREE_LIST_T(leaf->p))->max;
                    size = size ? (DTREE_LIST_T(leaf->p))->n : DTREE_ELEM_MAX;
                    fwrite((char *)(long)leaf->p, DTREE_SIZE_LIST(size), 1, f);
                    DTREE_FREE(leaf->p);
                }
                else if (DTREE_IS_LEAF(type)) {
                    fwrite((char *)(long)leaf->p, DTREE_SIZE_LEAF, 1, f);
                    DTREE_FREE(leaf->p);
                }
                else pos = (dtree_void_t)0;
                leaf->p = pos;
            }
            if (DTREE_IS_LINK(type)) {
                pos = (dtree_void_t)ftell(f);
                if (len) fwrite((char *)(long)(DTREE_LINK_DATA(leaf)), len, 1, f);
                if (copy) DTREE_FREE(DTREE_LINK_DATA(leaf));
                if (len) DTREE_LINK_DATA(leaf) = pos;
                pos = (dtree_void_t)ftell(f);
                fwrite(leaf, DTREE_SIZE_LINK, 1, f);
                leaf_saved->p = pos;
                DTREE_FREE(leaf);
            }
        }

        static void save_list(dtree_list_t *ls, int copy, int len, FILE *f)
        {
            if (!ls) return;
            int i = 0;
            int n = ls->max ? ls->n : DTREE_ELEM_MAX;
            for (i = 0; i < n; i++) {
                if (ls->leaf[i].n) save_leaf(ls->leaf + i, copy, len, f);
            }
        }

        /*
         * save tree, return: 0=ok, -1=fail, path: file path
         */
        int dtree_save(dtree_t *t, const char *path)
        {
            dtree_list_t *root = NULL;
            FILE *f = NULL;
            assert(t && t->me != DTREE_ME_DUMP);
            f = fopen(path, "wb");
            if (!f) return -1;
            fwrite(t, sizeof(dtree_t), 1, f);
            save_list(DTREE_LIST_T(t->root), t->me == DTREE_ME_COPY , t->len, f);
            root = DTREE_LIST_T(ftell(f));
            fwrite((char *)(long)t->root, DTREE_SIZE_LIST(DTREE_ELEM_MAX), 1, f);
            DTREE_FREE(t->root);
            t->root = DTREE_VOID_T(root);
            t->me = DTREE_ME_DUMP;
            rewind(f);
            fwrite(t, sizeof(dtree_t), 1, f);
            DTREE_FREE(t);
            fclose(f);
            return 0;
        }

        /*
         * load tree, return NULL if failed, path: file path
         */
        dtree_t *dtree_load(const char *path)
        {
            FILE *f = NULL;
            unsigned int i = 0, s1 = 0, s2 = 0;
            struct stat fa;
            dtree_t *t = NULL;
            f = fopen(path, "rb");
            if (f == NULL) return NULL;
            stat(path, &fa);
            s1 = fa.st_size;
            t = (dtree_t *)malloc(s1);
            if (t == NULL) return NULL;
            fread(t, s1, 1, f);
            fclose(f);
            if (t->magic == DTREE_MAGIC2) {
                char *p = (char *)t;
                for (i = 0; i < s1; i++) DTREE_TRAN(p[i], i);
                t->magic = DTREE_MAGIC1;
            }
            if (t->magic != DTREE_MAGIC1) return NULL;
            s2 = DTREE_SIZE_LIST(DTREE_ELEM_MAX)+(int)t->root;
            if (s1 != s2 || t->me != DTREE_ME_DUMP) {
                return NULL;
            }
            return t;
        }

        dtree_t *dtree_load_buffer(void *buf, int len)
        {
            dtree_t *t = NULL;
            int i = 0, size = 0;
            t = (dtree_t *)malloc(len);
            if (t == NULL) return NULL;
            memcpy(t, buf, len);
            if (t->magic == DTREE_MAGIC2) {
                char *p = (char *)t;
                for (i = 0; i < len; i++) DTREE_TRAN(p[i], i);
                t->magic = DTREE_MAGIC1;
            }
            if (t->magic != DTREE_MAGIC1) return NULL;
            size = DTREE_SIZE_LIST(DTREE_ELEM_MAX)+(int)t->root;
            if (len != size || t->me != DTREE_ME_DUMP) {
                return NULL;
            }
            return t;
        }

        dtree_t *dtree_attach_buffer(void *buf, int len)
        {
            dtree_t *t = NULL;
            int size = 0;
            t = (dtree_t *)buf;
            if (t == NULL) return NULL;
            if (t->magic != DTREE_MAGIC1) return NULL;
            size = DTREE_SIZE_LIST(DTREE_ELEM_MAX)+(int)t->root;
            if (len != size || t->me != DTREE_ME_DUMP) {
                return NULL;
            }
            return t;
        }

        static inline dtree_leaf_t *get_leaf2(dtree_list_t *ls, dtree_elem_t e)
        {
            int pos = -1;
            int n = ls->n;
            int step = ls->max >> 1;
            while (step) {
                if (ls->leaf[pos + step].v[0] < e && pos + step < n) {
                    pos += step;
                }
                step >>= 1;
            }
            pos++;
            if (pos >= n || ls->leaf[pos].v[0] != e) {
                return NULL;
            }
            return ls->leaf + pos;
        }

        static inline dtree_leaf_t *get_leaf(dtree_list_t *ls, dtree_elem_t e)
        {
            int i = 0;
            if (ls->max == 0) {
                if (ls->leaf[e].n) return ls->leaf + e;
            }
            else if ((ls->max) < 32) {
                for (i = 0; i < ls->n; i++) {
                    if (ls->leaf[i].v[0] == e) {
                        return ls->leaf + i;
                    }
                }
            }
            else return get_leaf2(ls, e);
            return NULL;
        }

        static inline int append(dtree_leaf_t *leaf, dtree_elem_t *p, dtree_elem_t *end, void *ptr)
        {
            unsigned int i = leaf->n;
            while (p < end) {
                if (i >= DTREE_ELEM_NUM) {
                    leaf->p = new_leaf();
                    if (!leaf->p) return -1;
                    leaf->type = DTREE_TYPE_LEAF;
                    leaf->n = DTREE_ELEM_NUM;
                    leaf = DTREE_LEAF_T(leaf->p);
                    i = 0;
                }
                leaf->v[i++] = TOLOWER(*p++);
            }
            leaf->n = i;
            if (ptr) {
                leaf->type = DTREE_TYPE_DATA;
                leaf->p = DTREE_VOID_T(ptr);
            }
            else leaf->type = DTREE_TYPE_NODE;
            return 0;
        }

        static inline dtree_list_t *resize_list(dtree_list_t *ls)
        {
            int max_old = ls->max;
            if (max_old > 0) {
                ls->max <<= 1;
                ls = (dtree_list_t *)realloc(ls, DTREE_SIZE_LIST(max_old * 2));
                if (!ls) return NULL;
                memset(ls->leaf + max_old, 0, max_old * DTREE_SIZE_LEAF);
            }
            return ls;
        }

        static inline dtree_leaf_t *move_list(dtree_list_t *ls, dtree_elem_t e)
        {
            int i = 0;
            dtree_elem_t c = 0;
            if (ls->max == 0) {
                for (i = ls->n - 1; i >= 0; i--) {
                    c = ls->leaf[i].v[0];
                    if (c == i) continue;
                    memcpy(ls->leaf + c, ls->leaf + i, DTREE_SIZE_LEAF);
                    ls->leaf[i].n = 0;
                }
                return ls->leaf + e;
            }
            for (i = ls->n - 1; i >= 0; i--) {
                c = ls->leaf[i].v[0];
                if (c <= e) {
                    ls->leaf[i+1].n = 0;
                    return ls->leaf + i + 1;
                }
                memcpy(ls->leaf + i + 1, ls->leaf + i, DTREE_SIZE_LEAF);
            }
            ls->leaf[0].n = 0;
            return ls->leaf + 0;
        }

        inline int dtree_match(dtree_t *t, void *s, int len, void **ptr)
        {
            return dtree_match_internal(t, s, (dtree_elem_t *)s + len, ptr);
        }

        /* 
         * match, return: match length
         */
        inline int dtree_match_internal(dtree_t *t, void *s, void *last, void **ptr)
        {
            int i = 0, r = 0;
            dtree_elem_t *p = (dtree_elem_t *)s;
            dtree_elem_t *begin = p;
            dtree_elem_t *end = (dtree_elem_t *)last; 
            dtree_elem_t em = TOLOWER(*p);
            dtree_leaf_t *leaf = NULL;
            assert(t && s && end >= begin);
            leaf = get_leaf(DTREE_GET_LIST(t->root, t), em);
            while (leaf) {
                p++;
                for (i = 1; i < leaf->n; i++) {
                    if (p >= end || leaf->v[i] != TOLOWER(*p++)) {
                        return r;
                    }
                }
                em = TOLOWER(*p);
                switch (leaf->type) {
                    case DTREE_TYPE_DATA:
                        if (ptr) *ptr = DTREE_GET_DATA(leaf->p, t);
                        return (int)(p - begin);
                        break;
                    case DTREE_TYPE_NODE:
                        if (ptr) *ptr = 0;
                        return (int)(p - begin);
                        break;
                    case DTREE_TYPE_LEAF_NODE:
                        if (ptr) *ptr = 0;
                        r = (int)(p - begin);
                        /* no break */
                    case DTREE_TYPE_LEAF:
                        if (p >= end) return r;
                        leaf = DTREE_GET_LEAF(leaf->p, t);
                        //assert(leaf && (dtree_t *)leaf != t);
                        if (leaf->v[0] != em) return r;
                        break;
                    case DTREE_TYPE_LIST_NODE:
                        if (ptr) *ptr = 0;
                        r = (int)(p - begin);
                        /* no break */
                    case DTREE_TYPE_LIST:
                        if (p >= end) return r;
                        leaf = get_leaf(DTREE_GET_LIST(leaf->p, t), em);
                        break;
                    case DTREE_TYPE_LEAF_DATA:
                        r = (int)(p - begin);
                        if (ptr) *ptr = DTREE_GET_LINK_DATA(leaf->p, t);
                        if (p >= end) return r;
                        leaf = DTREE_GET_LINK_LEAF(leaf->p, t);
                        //assert(leaf && (dtree_t *)leaf != t);
                        if (leaf->v[0] != em) return r;
                        break;
                    case DTREE_TYPE_LIST_DATA:
                        r = (int)(p - begin);
                        if (ptr) *ptr = DTREE_GET_LINK_DATA(leaf->p, t);
                        if (p >= end) return r;
                        leaf = get_leaf(DTREE_GET_LINK_LIST(leaf->p, t), em);
                        break;
                    default:
                        return r;
                }
            }
            return r;
        }

        /*
         * get all matches, return: match number
         * rp: data pointers, rs: lens of matches, rn: max match
         */
        inline int dtree_match_all(dtree_t *t, void *s, int len, void **rp, int *rs, int rn)
        {
            int i = 0, r = 0;
            dtree_elem_t *p = (dtree_elem_t *)s;
            dtree_elem_t *begin = p;
            dtree_elem_t *end = p + len;
            dtree_elem_t em = TOLOWER(*p);
            dtree_leaf_t *leaf = NULL;
            assert(t && s && len >= 0);
            leaf = get_leaf(DTREE_GET_LIST(t->root, t), em);
            while (leaf) {
                p++;
                for (i = 1; i < leaf->n; i++) {
                    if (p >= end || leaf->v[i] != TOLOWER(*p++)) {
                        return r;
                    }
                }
                em=TOLOWER(*p);
                switch (leaf->type) {
                    case DTREE_TYPE_DATA:
                        if (rp) rp[r] = DTREE_GET_DATA(leaf->p, t);
                        if (rs) rs[r] = (int)(p - begin);
                        return r + 1;
                        break;
                    case DTREE_TYPE_NODE:
                        if (rp) rp[r] = 0;
                        if (rs) rs[r] = (int)(p - begin);
                        return r + 1;
                        break;
                    case DTREE_TYPE_LEAF_NODE:
                        if (rp) rp[r] = 0;
                        rs[r] = (int)(p - begin);
                        if (++r >= rn) return r;
                        /* no break */
                    case DTREE_TYPE_LEAF:
                        if (p >= end) return r;
                        leaf = DTREE_GET_LEAF(leaf->p, t);
                        //assert(leaf && (dtree_t *)leaf != t);
                        if (leaf->v[0] != em) return r;
                        break;
                    case DTREE_TYPE_LIST_NODE:
                        if (rp) rp[r] = 0;
                        if (rs) rs[r] = (int)(p - begin);
                        if (++r >= rn) return r;
                        /* no break */
                    case DTREE_TYPE_LIST:
                        if (p >= end) return r;
                        leaf = get_leaf(DTREE_GET_LIST(leaf->p, t), em);
                        break;
                    case DTREE_TYPE_LEAF_DATA:
                        if (rp) rp[r] = DTREE_GET_LINK_DATA(leaf->p, t);
                        if (rs) rs[r] = (int)(p - begin);
                        if (++r >= rn) return r;
                        if (p >= end) return r;
                        leaf = DTREE_GET_LINK_LEAF(leaf->p, t);
                        //assert(leaf && (dtree_t *)leaf != t);
                        if (leaf->v[0] != em) return r;
                        break;
                    case DTREE_TYPE_LIST_DATA:
                        if (rp) rp[r] = DTREE_GET_LINK_DATA(leaf->p, t);
                        if (rs) rs[r] = (int)(p - begin);
                        if (++r >= rn) return r;
                        if (p >= end) return r;
                        leaf = get_leaf(DTREE_GET_LINK_LIST(leaf->p, t), em);
                        break;
                    default:
                        return r;
                }
            }
            return r;
        }

        /*
         * set deleted flag, return: ok = 0, fail = -1
         */
        int dtree_erase(dtree_t *t, void *s, int len)
        {
            int i = 0;
            dtree_elem_t *p = (dtree_elem_t *)s;
            dtree_elem_t *end = p + len;
            dtree_leaf_t *leaf = NULL, *next = NULL;
            assert(t && s && len >= 0 && t->me != DTREE_ME_DUMP);
            t->count--;
            leaf = get_leaf(DTREE_LIST_T(t->root), *p);
            while (leaf) {
                p++;
                for (i = 1; i < leaf->n; i++) {
                    if (p >= end || leaf->v[i] != *p++) {
                        return -1;
                    }
                }
                if (p >= end) {
                    if (DTREE_IS_DATA(leaf->type)) {
                        if (DTREE_IS_LINK(leaf->type)) {
                            if (t->me == DTREE_ME_COPY) {
                                DTREE_FREE(DTREE_LINK_DATA(leaf->p));
                            }
                            next = DTREE_LINK_LEAF(leaf->p);
                            DTREE_FREE(leaf->p);
                        }
                        else {
                            if (t->me == DTREE_ME_COPY) {
                                DTREE_FREE(leaf->p);
                            }
                            next = NULL;
                        }
                        leaf->p = DTREE_VOID_T(next);
                        leaf->type -= 2;
                        return 0;
                    }
                    else if (DTREE_IS_NODE(leaf->type)) leaf->type--;
                    return -1;
                }
                switch (leaf->type) {
                    case DTREE_TYPE_LEAF_NODE:
                    case DTREE_TYPE_LEAF:
                        leaf = DTREE_LEAF_T(leaf->p);
                        //assert(leaf);
                        if (leaf->v[0] != *p) return -1;
                        break;
                    case DTREE_TYPE_LIST_NODE:
                    case DTREE_TYPE_LIST:
                        leaf = get_leaf(DTREE_LIST_T(leaf->p), *p);
                        break;
                    case DTREE_TYPE_LEAF_DATA:
                        leaf = DTREE_LINK_LEAF(leaf->p);
                        //assert(leaf);
                        if (leaf->v[0] != *p) return -1;
                        break;
                    case DTREE_TYPE_LIST_DATA:
                        leaf = get_leaf(DTREE_LINK_LIST(leaf->p), *p);
                        break;
                    case DTREE_TYPE_DATA:
                    case DTREE_TYPE_NODE:
                    default:
                        return -1;
                }
            }
            return -1;
        }

        /*
         * insert, return: ok = 0, fail = -1
         */
        int dtree_insert(dtree_t *t, void *s, int len, void *ptr)
        {
            int i = 0, j = 0;
            dtree_elem_t *p = (dtree_elem_t *)s;
            dtree_elem_t *end = p + len;
            dtree_elem_t em = TOLOWER(*p);
            dtree_leaf_t *leaf = NULL, *prev = NULL, *leaf_old = NULL, *leaf_new = NULL;
            dtree_list_t *ls = NULL;
            void *data = NULL;
            assert(t && s && len >= 0 && t->me != DTREE_ME_DUMP);
            t->count++;
            leaf = (DTREE_LIST_T(t->root))->leaf + em;
            if (t->me == DTREE_ME_COPY && ptr) {
                data = malloc(t->len);
                if (!data) return -1;
                memcpy(data, ptr, t->len);
                ptr = data;
            }
            while (leaf && p < end) {
                if (leaf->n == 0) {
                    if (!prev) (DTREE_LIST_T(t->root))->n++;
                    return append(leaf, p, end, ptr);
                }
                for (i = 0; i < leaf->n && p < end; i++, p++) {
                    em = TOLOWER(*p);
                    if (leaf->v[i] != em) { //split
                        ls = DTREE_LIST_T(new_list(2));
                        if (!ls) return -1;
                        leaf_old = ls->leaf + ((leaf->v[i] < (em)) ? 0 : 1);
                        leaf_new = ls->leaf + ((leaf->v[i] < (em)) ? 1 : 0);
                        leaf_old->type = leaf->type;
                        leaf_old->p = leaf->p;
                        leaf_old->n = leaf->n - i;
                        for (j = i; j < leaf->n; j++) {
                            leaf_old->v[j-i] = leaf->v[j];
                        }
                        //todo: merge leaves
                        if (append(leaf_new, p, end, ptr) < 0) return -1;
                        ls->n = 2;
                        if (i == 0) {
                            switch(prev->type) {
                                case DTREE_TYPE_LEAF_DATA:
                                    prev->type = DTREE_TYPE_LIST_DATA;
                                    (DTREE_LINK_T(prev->p))->p = DTREE_VOID_T(ls);
                                    break;
                                case DTREE_TYPE_LEAF_NODE:
                                    prev->type = DTREE_TYPE_LIST_NODE;
                                    prev->p = DTREE_VOID_T(ls);
                                    break;
                                case DTREE_TYPE_LEAF:
                                    prev->type = DTREE_TYPE_LIST;
                                    prev->p = DTREE_VOID_T(ls);
                                    break;
                                default:
                                    break;
                            }
                            DTREE_FREE(leaf);
                        }
                        else {
                            leaf->p = DTREE_VOID_T(ls);
                            leaf->n = i;
                            leaf->type = DTREE_TYPE_LIST;
                        }
                        return 0;
                    }
                }
                if (p >= end) {
                    if (i < leaf->n) { //cut
                        leaf_new = DTREE_LEAF_T(new_leaf());
                        if (!leaf_new) return -1;
                        for (j = i; j < leaf->n; j++) {
                            leaf_new->v[j-i] = leaf->v[j];
                        }
                        leaf_new->type = leaf->type;
                        leaf_new->p = leaf->p;
                        leaf_new->n = leaf->n - i;
                        leaf->p = DTREE_VOID_T(leaf_new);
                        leaf->n = i;
                        if (ptr) {
                            leaf->p = new_link(DTREE_VOID_T(ptr), leaf->p);
                            if (!leaf->p) return -1;
                            leaf->type = DTREE_TYPE_LEAF_DATA;
                        }
                        else leaf->type = DTREE_TYPE_LEAF_NODE;
                    }
                    else { //set type
                        switch(leaf->type) {
                            case DTREE_TYPE_NODE:
                            case DTREE_TYPE_DATA:
                                leaf->type = DTREE_TYPE_DATA;
                                leaf->p = DTREE_VOID_T(ptr);
                                break;
                            case DTREE_TYPE_LEAF:
                            case DTREE_TYPE_LEAF_NODE:
                                if (ptr) {
                                    leaf->type = DTREE_TYPE_LEAF_DATA;
                                    leaf->p = new_link(DTREE_VOID_T(ptr), leaf->p);
                                    if (!leaf->p) return -1;
                                }
                                else leaf->type = DTREE_TYPE_LEAF_NODE;
                                break;
                            case DTREE_TYPE_LIST:
                            case DTREE_TYPE_LIST_NODE:
                                if (ptr) {
                                    leaf->type = DTREE_TYPE_LIST_DATA;
                                    leaf->p = new_link(DTREE_VOID_T(ptr), leaf->p);
                                    if (!leaf->p) return -1;
                                }
                                else leaf->type = DTREE_TYPE_LIST_NODE;
                                break;
                            case DTREE_TYPE_LEAF_DATA:
                                if (ptr) (DTREE_LINK_T(leaf->p))->data = DTREE_VOID_T(ptr);
                                break;
                            case DTREE_TYPE_LIST_DATA:
                                if (ptr) (DTREE_LINK_T(leaf->p))->data = DTREE_VOID_T(ptr);
                                break;
                            default:
                                break;
                        }
                    }
                    return 0;
                }
                prev = leaf; em = TOLOWER(*p);
                switch (leaf->type) {
                    case DTREE_TYPE_DATA:
                        leaf->type = DTREE_TYPE_LEAF_DATA;
                        leaf->p = new_link(leaf->p, new_leaf());
                        if (!leaf->p) return -1;
                        /* no break */
                    case DTREE_TYPE_LEAF_DATA:
                        leaf = DTREE_LINK_LEAF(leaf->p);
                        break;
                    case DTREE_TYPE_NODE:
                        leaf->type = DTREE_TYPE_LEAF_NODE;
                        leaf->p = new_leaf();
                        if (!leaf->p) return -1;
                        /* no break */
                    case DTREE_TYPE_LEAF_NODE:
                    case DTREE_TYPE_LEAF:
                        leaf = DTREE_LEAF_T(leaf->p);
                        break;
                    case DTREE_TYPE_LIST_DATA:
                        leaf = DTREE_LEAF_T(leaf->p);
                        /* no break */
                    case DTREE_TYPE_LIST_NODE:
                    case DTREE_TYPE_LIST:
                        ls = DTREE_LIST_T(leaf->p);
                        leaf_old = leaf;
                        leaf = get_leaf(ls, em);
                        if (!leaf) {
                            if (ls->max > 0) {
                                if (ls->n >= ls->max) {
                                    ls = resize_list(ls);
                                    if (!ls) return -1;
                                    leaf_old->p = DTREE_VOID_T(ls);
                                }
                                leaf = move_list(ls, em);
                            }
                            else leaf = ls->leaf + em;
                            ls->n++;
                        }
                        break;
                    default:
                        return -1;
                }
            }
            return 0;
        }

        int dtree_transform(const char *path)
        {
            FILE *f = NULL;
            unsigned int i = 0, s1 = 0, s2 = 0;
            struct stat fa;
            dtree_t *t = NULL;
            f = fopen(path, "rb");
            if (f == NULL) return -1;
            stat(path, &fa);
            s1 = fa.st_size;
            t = (dtree_t *)malloc(s1);
            if (t == NULL) return -1;
            fread(t, s1, 1, f);
            fclose(f);
            s2 = DTREE_SIZE_LIST(DTREE_ELEM_MAX)+(int)(t->root);
            if (s1 != s2 || t->me != DTREE_ME_DUMP) {
                return -1;
            }
            if (t->magic == DTREE_MAGIC1) {
                char *p = (char *)t; 
                for (i = 0; i < s1; i++) DTREE_TRAN(p[i], i);
                t->magic = DTREE_MAGIC2;
                f = fopen(path, "wb");
                if (f == NULL) return -1;
                fwrite(t, s1, 1, f);
                fclose(f);
            }
            DTREE_FREE(t);
            return 0;
        }

        inline int dtree_count(dtree_t *t) {
            return t ? t->count : 0;
        }

#ifdef  DTREE_CLASS
    private:
        dtree_t *t;
        char copy;

    public:
        DTreeClass()
        {
            int len = sizeof(T);
            copy = 1;
            if (sizeof(T) <= DTREE_SIZE_VOID) {
                copy = 0;
                len = 0;
            }
            t = dtree_create(copy, len);
        }

        DTreeClass(const char *path)
        {
            t = NULL;
            t = dtree_load(path);
        }

        DTreeClass(void *buf, int len)
        {
            t = NULL;
            t = dtree_load_buffer(buf, len);
        }

        ~DTreeClass()
        {
            if (t) dtree_destroy(t);
            t = NULL;
        }

        int available() {return t ? 1 : 0;}
        int count() {return t ? t->count : 0;}
        int buffer(void **buf)
        {
            *buf = (void *)t;
            return DTREE_SIZE_LIST(DTREE_ELEM_MAX)+(int)(t->root);
        }

#ifdef DTREE_DEBUG
        void print(int lvmax = 0) {dtree_print(t, lvmax);}
#endif

        int save(const char *path)
        {
            int r = dtree_save(t, path);
            t = NULL;
            return r;
        }

        int load(const char *path) 
        {
            if (t) dtree_destroy(t);
            t = dtree_load(path); 
            return t ? 0 : -1;
        }

        int load(void *buf, int len) 
        {
            if (t) dtree_destroy(t);
            t = dtree_load_buffer(buf, len); 
            return t ? 0 : -1;
        }

        int attach(void *buf, int len)
        {
            t = dtree_attach_buffer(buf, len); 
            return t ? 0 : -1;
        }

        int erase(void *s, int len)
        {
            return dtree_erase(t, len);
        }

        int insert(void *s, int len, const T &v)
        {
            return dtree_insert(t, s, len, copy ? (void *)&v : *((void **)&v));
        }

        int match(void *s, int len, T &v)
        {
            void *ptr = NULL;
            int r = dtree_match(t, s, len, &ptr);
            if (r > 0) memcpy(&v, copy ? ptr : &ptr, sizeof(T));
            return r;
        }

        int match(void *s, void *last, T &v)
        {
            void *ptr = NULL;
            int r = dtree_match_internal(t, s, last, &ptr);
            if (r > 0) memcpy(&v, copy ? ptr : &ptr, sizeof(T));
            return r;
        }

        int match(void *s, int len, void **ptr)
        {
            return dtree_match(t, s, len, ptr);
        }

        int match_all(void *s, int len, void **rp, int *rs, int rn)
        {
            return dtree_match_all(t, s, len, rp, rs, rn);
        }

        int transform(const char *path)
        {
            return dtree_transform(path);
        }
};

#endif
#endif //__DTREE_H

#ifdef DTREE_STANDALONE
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

void print_usage()
{
    struct rusage u;
    double t = 0;
    if (getrusage(RUSAGE_SELF, &u) < 0) return;
    t = u.ru_utime.tv_sec + u.ru_utime.tv_usec/1000000.0;
    fprintf(stderr, "time=%f rss=%ld\n", t, u.ru_maxrss);
}

int main(int argc, char **argv)
{
    int i = 0, r = 0, len = 0, v = 0;
    char s[1024], *text;
    void *rp[1024]; int rs[1024]; int rn = 1024;
    unsigned short ws[1024]; 
    FILE *f = NULL;
#ifdef  DTREE_CLASS
    DTreeClass<int> t;
#else
    dtree_t *t = dtree_create(0, 0);
    if (t == NULL) return 1;
#endif
    if (argc < 2) return 0;
    f = fopen(argv[1], "r");
    if (f == NULL) return 2;
    while (fgets(s, sizeof(s), f)) {
        len = strlen(s);
        while (isspace(s[len-1])) s[--len] = 0;
        if (len <= 0) continue;
#ifdef  DTREE_CLASS
        if (t.insert(s, len, len) < 0) return 3;
#else
        if (dtree_insert(t, s, len, (void *)(long)len) < 0) return 3;
#endif
    }
    fclose(f);
#ifdef DTREE_CLASS
    t.save("1.dump");
    t.load("1.dump");
#else
    dtree_save(t, "1.dump");
    t = dtree_load("1.dump");
#endif
    f = fopen(argv[argc > 2 ? 2 : 1], "r");
    if (f == NULL) return 4;
    while (fgets(s, sizeof(s), f)) {
        len = strlen(s);
        while (isspace(s[len-1])) s[--len] = 0;
        if (len <= 0) continue;
#ifdef  DTREE_CLASS
        r = t.match(s, len, len);
#else
        r = dtree_match(t, s, len, NULL);
#endif
        if (r) printf("r=%d s=[%.*s]\n", r, r, s);
    }
    print_usage();
    fclose(f);
    return 0;
}
#endif //DTREE_STANDALONE
