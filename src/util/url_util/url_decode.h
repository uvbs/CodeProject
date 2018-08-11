#pragma once
#ifndef __URL_DECODE_H
#define __URL_DECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define __CHECK_RANGE
#include "utfdetect.h"
#include "transcode.h"
#include "tradmap.h"

int url_escape(char *buf, int len, char *res, int rlen, int gbk = 0);

int url_decode(char *buf, int len, char *res, int rlen);

int url_gbk2utf8(char *buf, int len);

char* trim(char * ptr);

int url_tradmap(char *buf, int len, char *res, int rlen);


#endif

