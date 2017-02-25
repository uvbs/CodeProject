////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_httpclient.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 23:42
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_httpclient.h"
#include "json.h"
#include "test_tabreaderV2.h"
#include "string_util.h"
#include <string.h>
int test_httpost()
{
	const int buf_len = 1024*512;
	HttpClient obj;
	if (obj.init(buf_len,5,5) < 0)
	{
		return -1;
	}
	//const char* url = "http://v.baidu.com/v?word=exo&ct=301989888&rn=20&pn=0&db=0&s=0&fbl=800&ie=utf-8&otype=json_out&ifrom=video";
	char url[1024] = {0};
	char resp_buf[buf_len] = {0};
	//char postdata[10] = {'0','1','2'};
	char postdata[16] = "test=123";
	//访问url，获取结果
	str_util::snprintf(url,1024,"http://10.48.57.53:8692/v?ct=301989888&rn=20&pn=0&db=0&s=25&ie=utf-8&mcpacktest=1&word=exo");
	int ret = obj.request(url,postdata,sizeof(postdata));
	if(ret == 0) 
	{
		size_t retLen = 0;
		uint code = 0;
		char *res = obj.getResponse(&code, &retLen);
		if (code != 200) 
		{
			printf("url request error \n");
		}
		memcpy(resp_buf,res,retLen);
		if(retLen <= buf_len)
		{
			resp_buf[retLen] = '\0';
		}
		else 
		{
			resp_buf[buf_len-1] = '\0';
		}
	}
	return 0;
}

//audit query
int get_query_result(Table<TableRowAudit>& table, vector<string>& result)
{
	const int buf_len = 1024*512;
	HttpClient obj;
	if(obj.init(buf_len,5,5) < 0)
		return -1;

	//const char* url = "http://v.baidu.com/v?word=exo&ct=301989888&rn=20&pn=0&db=0&s=0&fbl=800&ie=utf-8&otype=json_out&ifrom=video";
	char url[1024] = {0};
	char resp_buf[buf_len] = {0};
	int count = table.rowCount();
	for (int i=0; i<count; ++i) 
	{
		const TableRowAudit* pRow = &table.getRowByIndex(i);
		if(!pRow) {
			continue;
		}
		//cout<<pRow->m_query<<endl;
		//访问url，获取结果
		memset(url,0,sizeof(url));
		str_util::snprintf(url,1024,"http://v.baidu.com/v?word=%s&ct=301989888&rn=20&pn=0&db=0&s=0&fbl=800&ie=utf-8&otype=json_out&ifrom=video",
			pRow->m_query);
		int ret = obj.request(url,NULL,0);
		if(ret == 0) {
			size_t retLen = 0;
			uint code = 0;
			char *res = obj.getResponse(&code, &retLen);
			if (code != 200) {
				printf("url request error, query=%s\n", pRow->m_query);
				continue;
			}
			memcpy(resp_buf,res,retLen);
			if(retLen <= buf_len){
				resp_buf[retLen] = '\0';
			}
			else {
				resp_buf[buf_len-1] = '\0';
			}

			string field = pRow->m_query;
			json_object* root_obj = json_tokener_parse(resp_buf);
			if(is_error(root_obj) || root_obj == NULL){
				printf("json_tokener_parse error, query=%s\n", pRow->m_query);
				field.append("\n");
				result.push_back(field);
				continue;
			}
			if(!json_object_is_type(root_obj, json_type_array)){
				json_object_put(root_obj);
				field.append("\n");
				result.push_back(field);
				continue;
			}
			int length = json_object_array_length(root_obj);
			if(length <= 0) {
				field.append("\n");
				result.push_back(field);
				continue;
			}

			for (int j=0; j<length; ++j)
			{
				field = pRow->m_query;
				json_object* sub_obj = json_object_array_get_idx(root_obj, j);
				if(is_error(sub_obj) || sub_obj == NULL){
					break;
				}
				struct json_object *sub_obj_id =NULL, *sub_obj_ty=NULL;
				sub_obj_id  = json_object_object_get(sub_obj, "id");
				sub_obj_ty  = json_object_object_get(sub_obj, "ty");
				//validate
				if(NULL == sub_obj_id || is_error(sub_obj_id)){
					json_object_put(sub_obj);
					break;
				}
				if(!json_object_is_type(sub_obj_id, json_type_string)){
					json_object_put(sub_obj);
					break;
				}
				if(NULL == sub_obj_ty || is_error(sub_obj_ty)){
					json_object_put(sub_obj);
					break;
				}
				if(!json_object_is_type(sub_obj_ty, json_type_int)){
					json_object_put(sub_obj);
					break;
				}
				//get data from obj
				int id = 0, ty = 0;
				id = json_object_get_int(sub_obj_id);
				ty = json_object_get_int(sub_obj_ty);
				//printf ("id=%d,ty=%d\n", id, ty);

				char info[64] = {0};
				str_util::snprintf(info,64,"\t\tid=%d\t\tty=%d\n", id,ty);
				field.append(info);
				result.push_back(field);
			}
		}
	}

	return 0;
}

int test_httpclient_jsonc()
{
	TableDBSystem::getSinglePtr()->load();
	vector<string> result;
	//input 
	if(get_query_result(TableDBSystem::getSinglePtr()->m_TableRowAudit1Obj, result)<0)
	{
		return -1;
	}
	//output
	FILE* fp = ::fopen("./data/audit1-result.txt", "w+");
	if (!fp)
	{
		return -1;
	}
	for (vector<string>::iterator iter=result.begin(); iter!=result.end(); ++iter)
	{
		fwrite(iter->c_str(), iter->length(), 1, fp);
	}
	fclose(fp);
	result.clear();
	////////////////////////////////////////////////////////////////////////////////
	//input
	if(get_query_result(TableDBSystem::getSinglePtr()->m_TableRowAudit2Obj, result)<0)
	{
		return -1;
	}
	//output
	fp = ::fopen("./data/audit2-result.txt", "w+");
	if (!fp)
	{
		return -1;
	}
	for (vector<string>::iterator iter=result.begin(); iter!=result.end(); ++iter)
	{
		fwrite(iter->c_str(), iter->length(), 1, fp);
	}
	fclose(fp);
	result.clear();
	return 0;

}

