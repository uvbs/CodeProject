#include "http_client.h"
#include "string_util.h"
#include "macrodef.h"
#include <string.h>

static size_t
write_response(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct memory_t *mem = (struct memory_t*)data;
    if (mem->len + realsize >= mem->size)
    {
        //mem->memory[0] = 0;
        //mem->len = 0;
        //实际这块有隐患
        return realsize;
    }
    if (mem->memory != NULL) 
    {
        memcpy(mem->memory + mem->len, ptr, realsize);
        mem->len += realsize;
        mem->memory[mem->len] = 0;
        //write to file
        //if (fp)
        //{
        //    int written = fwrite(ptr, size, nmemb, fp);
        //    printf("download file size=%d, write file size=%d\n", size*nmemb, written);
        //}
    }
    return realsize;
}

static int 
write_response_progress(double dltotal, double dlnow, double ultotal, double ulnow)
{
    if (dltotal > 0 && dlnow > 0 && dlnow >= dltotal)
    {
        printf("download file over!\n");
    }
    return 0;
}

static size_t
write_header_response(void *ptr, size_t size, size_t nmemb, void *data)
{
    uint *status_code = (uint*)data; 
    size_t realsize = size * nmemb;

    if ( realsize >= HttpClient::DEFAULT_HEADER_SIZE ) 
    {
        return realsize;
    }

    int http_version1 = 1;
    int http_version2 = 1;
    int code = 0;
    if (3 == sscanf((char*)ptr,"HTTP/%d.%d %d]", &http_version1, &http_version2, &code)) 
    {
        *status_code = code;
    }
    else if (1 ==  sscanf((char*)ptr,"HTTP %d]",&code)) 
    {
        *status_code = code;
    }

    return realsize;
}

static int 
write_header_response_progress(double dltotal, double dlnow, double ultotal,double ulnow)
{
    if (dltotal > 0 && dlnow > 0 && dlnow >= dltotal)
    {
        //parser_header();
        return 1;
    }
    return 0;
}

//construct
HttpClient::HttpClient() : _easy_handle(NULL), _status_code(0)
{
    _mem_buf.memory = NULL;
    _mem_buf.size = 0;
    _mem_buf.len = 0;
}

//destruct
HttpClient::~HttpClient()
{
    SAFE_DELETE_ARRAY(_mem_buf.memory);
    _mem_buf.size = 0;
    _mem_buf.len = 0;
}

int HttpClient::init(size_t respBufSize, unsigned long connTimeout, unsigned long readTimeout)
{
    if (respBufSize <= 0)
    {
        return -1;
    }    
    if (_mem_buf.memory == NULL)
    {
        _mem_buf.memory = new char[respBufSize];
        _mem_buf.size = respBufSize;
        if (_mem_buf.memory == NULL)
            return -1;
    }
    _mem_buf.len = 0;
    _easy_handle = curl_easy_init();
    if (_easy_handle == NULL)
    {
        return -1;
    }
    //NOSIGNAL for 多线程
    //毫秒超时一定要设置这个
    curl_easy_setopt(_easy_handle, CURLOPT_NOSIGNAL, 1L);
    //curl_easy_setopt(_easy_handle, CURLOPT_HEADER, false); 
    //curl_easy_setopt(_easy_handle, CURLOPT_NOBODY, false);  
    curl_easy_setopt(_easy_handle, CURLOPT_WRITEDATA, &_mem_buf);
    curl_easy_setopt(_easy_handle, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(_easy_handle, CURLOPT_WRITEHEADER, &_status_code);
    curl_easy_setopt(_easy_handle, CURLOPT_HEADERFUNCTION, write_header_response);
    //关闭 tcp reuse
    curl_easy_setopt(_easy_handle, CURLOPT_FORBID_REUSE, 1L);
    //timeout
    curl_easy_setopt(_easy_handle, CURLOPT_CONNECTTIMEOUT, connTimeout);
    curl_easy_setopt(_easy_handle, CURLOPT_TIMEOUT, readTimeout);
    //curl_easy_setopt(_easy_handle, CURLOPT_VERBOSE, 1L);
    //progress
    curl_easy_setopt(_easy_handle, CURLOPT_NOPROGRESS,false);
    curl_easy_setopt(_easy_handle, CURLOPT_PROGRESSDATA,this);
    curl_easy_setopt(_easy_handle, CURLOPT_PROGRESSFUNCTION, write_response_progress);
    //curl_easy_setopt(_easy_handle, CURLOPT_NOBODY, true);
    //curl_easy_setopt(_easy_handle, CURLOPT_NOPROGRESS,false);
    //curl_easy_setopt(_easy_handle, CURLOPT_PROGRESSDATA,this);
    //curl_easy_setopt(_easy_handle, CURLOPT_PROGRESSFUNCTION, write_header_response_progress);
    //curl_easy_setopt(_easy_handle,CURLOPT_BUFFERSIZE,2048);
    //curl_easy_setopt(_easy_handle,CURLOPT_TCP_NODELAY,1);
    //curl_easy_setopt(_easy_handle,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(_easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    return 0;
}

int HttpClient::request(const char *url, void* binaryptr, int size)
{
    _mem_buf.len = 0;
    if (url == NULL)
    {
        return -1;
    }
    int ret = curl_easy_setopt(_easy_handle, CURLOPT_URL, url);
    if (binaryptr)
    {
        ret = curl_easy_setopt(_easy_handle, CURLOPT_POST, 1L);
        ret = curl_easy_setopt(_easy_handle, CURLOPT_POSTFIELDS, binaryptr);
        ret = curl_easy_setopt(_easy_handle, CURLOPT_POSTFIELDSIZE,(long)size);
    }
    if (ret != 0)
    {
        return ret;
    }
    ret = curl_easy_perform(_easy_handle);
    curl_easy_cleanup(_easy_handle);

    return ret;
}

char* HttpClient::getResponse(uint *status_code, size_t* response_len)
{
    if (status_code != NULL)
    {
        *status_code = _status_code;
    }
    if (response_len != NULL) 
    {
        *response_len = _mem_buf.len;
    }
    return _mem_buf.memory;
}

//long  HttpClient::getContentLength(const char* url)
//{
//	long downloadFileLenth = 0;
//	curl_easy_setopt(_easy_handle, CURLOPT_URL, url);
//	curl_easy_setopt(_easy_handle, CURLOPT_HEADER, true);    //只需要header头
//	curl_easy_setopt(_easy_handle, CURLOPT_NOBODY, true);    //不需要body
//	CURLcode code = curl_easy_perform(_easy_handle);
//	if (CURLE_OK == code)
//	{
//		curl_easy_getinfo(_easy_handle, CURLINFO_RESPONSE_CODE , &downloadFileLenth); 
//	}
//	else
//	{
//		downloadFileLenth = -1;
//	}
//	return downloadFileLenth;
//}
