#include "socketdef.h"
#include <signal.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include "worker.h"
#include "http_service.h"
#include "assert.h"
#include "log.h"
#include "string_util.h"
#include "agreement.h"

extern volatile sig_atomic_t g_bMainLoop;

bool	Worker::init(EVHttpServer* pServer)
{
	_server = pServer;
	Assert(_server);
	_base = event_base_new();
	Assert(_base);
	_http = evhttp_new(_base);
	Assert(_http);
	return true;
}

void	Worker::wait()
{
	_thread.wait();
	evhttp_free(_http);
	event_base_free(_base);
}

void* Worker::run(void* arg)
{
	Worker* p = static_cast<Worker*>(arg);
	if (p) {
		p->do_service();
	}
	return NULL;
}

bool	Worker::do_service()
{
__ENTER_FUNCTION
	int ret = evhttp_accept_socket(_http, _server->getFd());
	if (ret != 0) {
		LogSystem::getSinglePtr()->saveLog(LOG_FILE_FATAL, "Could not accept socket.");
		return false;
	}
	evhttp_set_timeout(_http, _server->getTimeout() / 1000);
	evhttp_set_cb(_http, "/route", do_logic, this);
#if defined(_WIN32)
	while (g_bMainLoop) {
		ret = event_base_loop(_base, EVLOOP_ONCE);
	}
#else
	ret = event_base_dispatch(_base);
	if (ret != 0) {
		LogSystem::getSinglePtr()->saveLog(LOG_FILE_FATAL, "Could not dispatch base of worker.");
		return NULL;
	}
#endif
	//evhttp_free(http);
	//event_base_free(base);
	return true;
__LEAVE_FUNCTION
	return false;
}

void Worker::do_logic(struct evhttp_request* req, void* arg)
{
	Worker* pWorker = static_cast<Worker*>(arg);
    if (pWorker) { 
        pWorker->processRequest(req); // parse request
    }
}

void  Worker::processRequest(struct evhttp_request* req)
{
	// send data back
	int http_status = HTTP_OK;
	struct evbuffer* evb = evbuffer_new();
	Assert(evb);
	do 
	{
		evhttp_cmd_type cmd_type = evhttp_request_get_command(req);
		char* uri = (char*) evhttp_request_get_uri(req);
		if (uri) {
			printf ("%s\n", uri);
		}
		if (cmd_type == EVHTTP_REQ_POST) {
			printf ("%s\n", "http post");
		}
		else if (cmd_type == EVHTTP_REQ_GET) {
			printf ("%s\n", "http get");
		}
	
		Agreement r;
		uri = evhttp_uridecode(uri, 0, NULL);
		if ((uri = strchr(uri, '?')) != NULL) {
			r.parser(++uri);
		}
		
		char* buffer_data = _recv_buffer;
		size_t buffer_data_len = evbuffer_get_length(req->input_buffer);
		if (buffer_data_len < 0) {
			evhttp_send_error(req, HTTP_BADREQUEST, 0);
			LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR, "The POST request is too short.");
			http_status = HTTP_BADREQUEST;
			break;
		}
		if (buffer_data_len >= kMaxReceiveBufferSize) {
			buffer_data = new char[buffer_data_len + 1];
			if (!buffer_data) { break; }
		}
		evbuffer_copyout(req->input_buffer, buffer_data, buffer_data_len);
		buffer_data[buffer_data_len] = 0;
		// set up headers
		struct evkeyvalq* out_headers = req->output_headers;
		evhttp_add_header(out_headers, "Content-Type", "application/octet-stream");
		evhttp_add_header(out_headers, "Connection", "close");
		char temp[32] = {0};
		const char* response = "Hello world";
		int data_size = str_util::strlen(response);
		str_util::snprintf(temp, sizeof(temp), "%d", data_size);
		evhttp_add_header(out_headers, "Content-Length", temp);
		// set up sequence id attribute
		struct evkeyvalq* headers = req->input_headers;
		for (evkeyval* header = headers->tqh_first; header; header = header->next.tqe_next) {
			if (str_util::strcmp(header->key, "Client-SequenceId") == 0) {
				evhttp_add_header(out_headers, "Client-SequenceId", header->value);
				break;
			}
		}
		evbuffer_add(evb, response, data_size);
	} while (0);
	
	evhttp_send_reply(req, http_status, "OK", evb);
	evbuffer_free(evb);
}

