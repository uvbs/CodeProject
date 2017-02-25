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
     * @brief ��ʼ��
     *
     * @param [in] res_buf_size:  response buf�Ĵ�С
     * @param [in] cnn_timeout:  connect timeout����λ:s
     * @param [in] read_timeout:  read timeout����λ:s
     * 
     * @return 
     *	0 success
     *	-1 fail
     * 
     * @note 
     *	����������������߳����ȵ���һ�Σ�������߳̿���core
     *	buf���������˿���1M�����ȼ�
    **/
	int init(size_t respBufSize, unsigned long connTimeout, unsigned long readTimeout);

    /** 
	 * @brief ִ��http����
	 * 
	 * @param url ���ʵ�url
	 * 
	 * @return 
	 *	0 success
	 *	other fail
	 */
	int request(const char *url, void* binaryPtr, int size);

    
	/** 
	 * @brief ��ȡresponse
	 * 
	 * @param[out] status_code http������
	 * @param[out] response_len response���ݴ�С
	 * 
	 * @return response buf���׵�ַ
	 * @note 2��out��������ΪNULL������ֵ����ΪNULL
	 *
	 */
	char* getResponse(uint *statusCode, size_t* respLen);

    //����httphead
    bool parseHeader();
    //��ȡĿ�곤��
    //long  getContentLength(const char* url);

private:
    CURL* _easy_handle;
    struct memory_t _mem_buf; 
    uint _status_code;
};

#endif  //http_client.h

