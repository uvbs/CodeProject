#ifndef __URL_PARSE_H
#define __URL_PARSE_H

#include <string>

typedef struct {
    char *p;
    int len;
} url_string_t;

typedef struct {
    url_string_t scheme;
    url_string_t auth;
    url_string_t host;
    url_string_t port;
    url_string_t path;
    url_string_t query; 
    url_string_t frag; 
} url_struct_t;

int url_print(url_struct_t *res);

int url_parse(char *url, int len, url_struct_t *res);

int url_rewrite(char *res, int len, url_struct_t *url, bool keep_www = false);

std::string& url_rewrite(std::string& url_rewritten, const url_struct_t* url, bool keep_www = false);

#endif

