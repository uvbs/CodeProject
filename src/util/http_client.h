////////////////////////////////////////////////////////////////////////////////////
// 
// @file: http_client.h
// @author: by Mr.Chen
// @date: 2015/4/6	16:43
// @brief: http-request, implement with lib-curl
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __HTTPCLIENT_H_
#define	__HTTPCLIENT_H_

#include <string>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>
#include "type.h"

using namespace std;

struct memory_t {
    char *memory;
    size_t len;
    size_t size;
};

class HttpClient
{
public:
    static const int DEFAULT_HEADER_SIZE = 1024 * 1024;

	HttpClient();
	~HttpClient();

     /**
     * @brief 初始化
     *
     * @param [in] res_buf_size:  response buf的大小
     * @param [in] cnn_timeout:  connect timeout，单位:s
     * @param [in] read_timeout:  read timeout，单位:s
     * 
     * @return 
     *	0 success
     *	-1 fail
     * 
     * @note 
     *	这个函数必须在主线程中先调用一次，否则多线程可能core
     *	buf尽量往打了开，1M以上尤佳
    **/
	int init(size_t respBufSize, unsigned long connTimeout, unsigned long readTimeout);

    /** 
	 * @brief 执行http请求
	 * 
	 * @param url 访问的url
	 * 
	 * @return 
	 *	0 success
	 *	other fail
	 */
	int request(const char *url, void* binaryPtr, int size);

    
	/** 
	 * @brief 获取response
	 * 
	 * @param[out] status_code http返回码
	 * @param[out] response_len response数据大小
	 * 
	 * @return response buf的首地址
	 * @note 2个out参数可以为NULL，返回值可能为NULL
	 *
	 */
	char* getResponse(uint *statusCode, size_t* respLen);

    //解析httphead
    bool parseHeader();
    //获取目标长度
    //long  getContentLength(const char* url);

private:
    CURL* _easy_handle;
    struct memory_t _mem_buf; 
    uint _status_code;
};

#endif  //http_client.h

