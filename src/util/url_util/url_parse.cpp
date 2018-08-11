#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include "url_parse.h"

using namespace std;

int url_print(url_struct_t *res)
{
    printf("scheme=[%.*s]\n", res->scheme.len, res->scheme.p);
    printf("auth=[%.*s]\n",   res->auth.len, res->auth.p);
    printf("host=[%.*s]\n",   res->host.len, res->host.p);
    printf("port=[%.*s]\n",   res->port.len, res->port.p);
    printf("path=[%.*s]\n",   res->path.len, res->path.p);
    printf("query=[%.*s]\n",  res->query.len, res->query.p);
    printf("frag=[%.*s]\n",   res->frag.len, res->frag.p);
    return 0;
}

#define STEP_START   0
#define STEP_SCHEME  1
#define STEP_AUTH    2
#define STEP_HOST    3
#define STEP_PORT    5
#define STEP_PATH    6
#define STEP_QUERY   7
#define STEP_FRAG    8

int url_parse(char *url, int len, url_struct_t *res)
{
    char *p = url, *start = url, *end = url + len;    
    memset(res, 0, sizeof(url_struct_t));
    int step = STEP_START;
    while (p < end) {
        switch (step) {
            case STEP_START:
                if (*p == ':') {
                    res->scheme.p   = start;
                    res->scheme.len = p - start;
                    start = p + 1;
                    step = STEP_AUTH;
                }
                else if (*p == '@') {
                    res->auth.p   = start;
                    res->auth.len = p - start;
                    start = p + 1;
                    step = STEP_HOST;
                }
                else if (*p == '.') {
                    step = STEP_HOST;
                }
                else if (*p == '/') {
                    start = p + 1;
                    step = STEP_AUTH;
                }
                break;
            case STEP_AUTH:
                if (*p == '/') {
                    start = p + 1;
                }
                else if (*p == '@') {
                    res->auth.p   = start;
                    res->auth.len = p - start;
                    start = p + 1;
                    step = STEP_HOST;
                }
                else if (*p == '.') {
                    step = STEP_HOST;
                }
                break;
            case STEP_HOST:
                if (*p == ':') step = STEP_PORT;
                else if (*p == '/') step = STEP_PATH;
                else if (*p == '?') step = STEP_QUERY;
                else if (*p == '#' &&  p + 1 < end &&  *(p+1) != '!')  step = STEP_FRAG;
                if (step != STEP_HOST) {
                    res->host.p   = start;
                    res->host.len = p - start; 
                    start = p + 1;
                }
                break;
            case STEP_PORT:
                if (*p == '/') step = STEP_PATH;
                else if (*p == '?') step = STEP_QUERY;
                else if (*p == '#' &&  p + 1 < end &&  *(p+1) != '!') step = STEP_FRAG;
                if (step != STEP_PORT) {
                    res->port.p   = start;
                    res->port.len = p - start; 
                    start = p + 1;
                }
                break;
            case STEP_PATH:
                if (*p == '?') step = STEP_QUERY;
                else if (*p == '#' &&  p + 1 < end &&  *(p+1) != '!') step = STEP_FRAG;
                if (step != STEP_PATH) {
                    res->path.p   = start;
                    res->path.len = p - start; 
                    start = p + 1;
                }
                break;
            case STEP_QUERY:
                if (*p == '#' &&  p + 1 < end &&  *(p+1) != '!') {
                    res->query.p   = start;
                    res->query.len = p - start; 
                    start = p + 1;
                    step = STEP_FRAG;
                }
                break;
            default:
                break;
        }
        p++;
    }
    switch (step) {
        case STEP_HOST:
            res->host.p   = start;
            res->host.len = p - start;
            break;
        case STEP_PORT:
            res->port.p   = start;
            res->port.len = p - start;
            break;
        case STEP_PATH:
            res->path.p   = start;
            res->path.len = p - start;
            break;
        case STEP_QUERY:
            res->query.p   = start;
            res->query.len = p - start;
            break;
        case STEP_FRAG:
            res->frag.p   = start;
            res->frag.len = p - start;
            break;
        default:
            break;
    }
    if (res->host.len <= 0) return -1;
    for (int i = 0; i < res->host.len; i++) {
        res->host.p[i] = tolower(res->host.p[i]);
    }
    return 0;
}

string& url_rewrite(string& url_rewritten, const url_struct_t* url, bool keep_www)
{
    if (url->host.len <= 0) 
        return url_rewritten;

    url_rewritten.reserve(url->scheme.len + url->host.len + url->path.len + url->query.len + 9);

    if (url->scheme.len <= 0) 
    {
        url_rewritten.append("http://");
    }
    else 
    {
        url_rewritten.append(url->scheme.p, url->scheme.len);
        url_rewritten.append("://");
    }

    if (strncasecmp(url->host.p, "www.", 4) == 0 && !keep_www) 
    {
        url_rewritten.append(url->host.p + 4, url->host.len - 4);
    }
    else 
    {
        url_rewritten.append(url->host.p, url->host.len);
    }

    if (url->path.len > 0) 
    {
        int len = url->path.len;
        url_rewritten.append("/");

        if (url->path.p[url->path.len - 1] == '/')
            len--;

        url_rewritten.append(url->path.p, len);
    }

    if (url->query.len > 0) 
    {
        url_rewritten.append("?");
        url_rewritten.append(url->query.p, url->query.len);
    }


    if(url->path.len == 0 && url->query.len == 0) {
	    if(url_rewritten[url_rewritten.size() - 1] != '/') url_rewritten.append("/");
    }

    transform(url_rewritten.begin(), url_rewritten.end(), url_rewritten.begin(), ::tolower);

    return url_rewritten; 
}

int url_rewrite(char *res, int len, url_struct_t *url, bool keep_www)
{
    int n = 0;
    res[0] = 0;
    if (url->host.len <= 0) return -1;
    if (len < url->scheme.len + url->host.len + url->path.len 
            + url->query.len + 9) return -1;
    if (url->scheme.len <= 0) {
        memcpy(res, "http://", 7);
        n += 7;
    }
    else {
        memcpy(res, url->scheme.p, url->scheme.len);
        n += url->scheme.len;
        memcpy(res + n, "://", 3);
        n += 3;
    }
    if (strncasecmp(url->host.p, "www.", 4) == 0 && !keep_www) {
        memcpy(res + n, url->host.p + 4, url->host.len - 4);
        n += url->host.len - 4;
    }
    else {
        memcpy(res + n, url->host.p, url->host.len);
        n += url->host.len;
    }
    if (url->path.len > 0) {
        res[n++] = '/';
        memcpy(res + n, url->path.p, url->path.len);
        n += url->path.len;
        if (res[n - 1] == '/') n--;
    }
    if (url->query.len > 0) {
        res[n++] = '?';
        memcpy(res + n, url->query.p, url->query.len);
        n += url->query.len;
    }

    if(url->path.len == 0 && url->query.len == 0) {
	if(res[n - 1] != '/') res[n++] = '/';
    }

    for (int i = 0; i < n; i++) res[i] = tolower(res[i]);
    res[n] = 0;
    return n;
}

#ifdef __URL_STANDALONE
#include <unistd.h>
#define  MAX_BUFSIZE 1024
static char _g_buf[MAX_BUFSIZE];
static char _g_res[MAX_BUFSIZE];

int main(int argc, char **argv)
{
    FILE *f = stdin;
    if (argc > 1) f = fopen(argv[1], "rb");
    if (f == NULL) return 0;

    while(fgets(_g_buf, MAX_BUFSIZE, f)) {
        char *p = _g_buf;
        int len = strlen(p);
        while (len > 0 && isspace(p[len-1])) len--;
        if (len <= 0) continue;
        p[len] = 0;
        url_struct_t res;
        url_parse(p, len, &res);
        url_print(&res);
        url_rewrite(_g_res, MAX_BUFSIZE, &res);
        printf("url=[%s]\n", _g_res);
        string str_url;
        printf("url=[%s]\n", url_rewrite(str_url, &res).c_str());
    }
    return 0;
}

#endif

