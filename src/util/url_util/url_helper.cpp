#include "url_helper.h"
#include "url_parse.h"
//#include <boost/regex.hpp>
#include <regex>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

bool load_domains(const char* path, unordered_set<string>& domains)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return false;
    }
    char line[1024] = {0};
    while (fgets(line, sizeof(line), f)) 
    {
        if (line[0] == '#') 
            continue;
        int len = strlen(line);
        while (len > 0 && isspace(line[len-1])) 
            line[--len] = 0;
        if (len <= 0) 
            continue;
	    domains.insert(line);
    }
    fclose(f);
    return true;
}

bool is_ipv4(const string &ip) 
{
    vector<string> ips;
    boost::split(ips, ip, boost::is_any_of("."));
    size_t iplen = ips.size();
    if (iplen != 4) {
        return false;
    }
    for (unsigned int i = 0; i < iplen; i++) {
        if (ips[i].empty() || ips[i].length() > 3) {
            return false;
        }
        int ip = atoi(ips[i].c_str());
        if (ip < 0 || ip > 255) {
            return false;
        }
        if ((i == 0 || i == iplen - 1) && ip == 0) {
            return false;
        }
        char ipstr[4] = { 0 };
        snprintf(ipstr, 4, "%d", ip);
        if (ips[i] != ipstr) {
            return false;
        }
    }
    return true;
}

bool get_from_host(string& host, string& host_name, string& domain, unordered_set<string>* valid_domains, 
        unordered_set<string>* special_sites) 
{
    if (!valid_domains || !special_sites) {
        return false;
    }
    bool found_domain = false;
    bool valid_domain = false;
    string h = host;
    bool with_www = false;
    host_name = "";
    if (h.find("www.") == 0) {
        h = h.substr(4);
        with_www = true;
    }
    if (!with_www && is_ipv4(host)) {
        domain = host;
        return true;
    }
    
    if (special_sites->find(host) != special_sites->end()) {
        size_t dot_pos = host.find(".");
        if (dot_pos != string::npos && dot_pos != host.length() - 2) {
            domain = host.substr(dot_pos + 1);
        }
        return true;
    }

    if (h.length() <= 1) {
        return false;
    }
    while (h.c_str()[0] == '.' && h.length() > 0) {
        h = h.substr(1);
    }
    size_t dot_pos = h.length();
    string dm = "";
    int pcounter = 0;
    do {
        dot_pos = h.rfind(".", dot_pos - 1);
        if (dot_pos != string::npos) {
            dm = h.substr(dot_pos + 1);
        }
        else {
            dm = h;
        }
        if (valid_domains->find(dm) != valid_domains->end()) {
            valid_domain = true;
        }
        else {
            if (valid_domain) {
                domain = dm;
                found_domain = true;
            }
            if (pcounter != 0) {
                break;
            }
        }
        pcounter++;
    } while (!found_domain && dot_pos != string::npos);
    if (!found_domain) {
        if (valid_domain && dm != h) {
            domain = h;
        }
        else {
            return false;
        }
    }
    dot_pos = domain.find(".");
    if (dot_pos == string::npos) {
        domain = "";
        return false;
    }
    string trunk_ = domain.substr(0, dot_pos);
    if (trunk_.find("xn--") == 0) {
        trunk_ = trunk_.substr(4);
    }
    for (string::size_type i = 0; i < trunk_.length(); i++) {
        if (isalpha(trunk_[i]) || isalnum(trunk_[i])) {
            continue;
        }
        else if (trunk_[i] == '-') {
            if (i == 0 || i == trunk_.length() - 1) {
                domain = "";
                return false;
            }
            else if (i < trunk_.length() - 2) {
                if (trunk_[i + 1] != '-' || trunk_[i + 2] != '-') {
                    continue;
                }
            }
            else {
                continue;
            }
        }
        domain = "";
        return false;
    }
    if (domain != host) {
        host_name = host.substr(0, host.find(domain) - 1);
    }
    return true;
}

bool is_host_main_page(char* url, string& host) {
    bool is_main_page = false;
    if (strstr(url, "?") != NULL) {
        return false;
    }
    url_struct_t res;
    size_t len = strlen(url);
    url_parse(url, len, &res);
    string scheme = string(res.scheme.p, res.scheme.len);
    host = string(res.host.p, res.host.len);
    string path = string(res.path.p, res.path.len);
    string query = string(res.query.p, res.query.len);
    const char* main_page_regex = "(.*)/(index|thread|forum|default|portal|home)\\.(php|html|htm|jsp|asp|aspx|shtml|js|do|cgi|pl|phtml)";
    if (path == "/" || path == "") {
        is_main_page = true;
    }
    else {
        std::regex pattern(main_page_regex);
        if (std::regex_match(path.c_str(), pattern)) {
            is_main_page = true;
        }else {
            is_main_page = false;
        }
    }
    return is_main_page;
}

int get_domain_main_flag(char* url, unordered_set<string>* valid_domains, unordered_set<string>* special_sites,
        string& host_name, string& domain) {
    string host = "";
    host_name = "";
    domain = "";
    if (!is_host_main_page(url, host)) {
        return -1;
    }
    if (!get_from_host(host, host_name, domain, valid_domains, special_sites)) {
        return 0;
    }
    set<string> main_hostname;
    main_hostname.insert("");
    main_hostname.insert("www");
    main_hostname.insert("www1");
    main_hostname.insert("wap");
    main_hostname.insert("3g");
    main_hostname.insert("4g");
    main_hostname.insert("m");
    main_hostname.insert("mobile");
    main_hostname.insert("mobi");
    if (main_hostname.find(host_name) != main_hostname.end()) {
        return 1;
    }
    return -1;
}

