#ifndef _URL_HELPER_H
#define _URL_HELPER_H

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <boost/unordered_set.hpp>

using std::string;
using std::vector;
using std::set;
using boost::unordered_set;

bool is_ipv4(const string &ip);

bool load_domains(const char* path, unordered_set<string>& domains);

bool is_host_main_page(char* url, string& host);

bool get_from_host(string& host, string& host_name, string& domain, unordered_set<string>* valid_domains, 
        unordered_set<string>* special_sites);

int get_domain_main_flag(char* url, unordered_set<string>* valid_domains, unordered_set<string>* special_sites, 
        string& host_name, string& domain);

#endif //_URL_HELPER_H

