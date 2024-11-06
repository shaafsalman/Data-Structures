#ifndef __DNS_H
#define __DNS_H

#include "../Part 1/tree.cpp"
#include <string>
#include <fstream>

struct Webpage
{
    string TLD;
    string domain;
    vector<string> subdomains;

    // Improved constructor to parse URL properly
    Webpage(string url)
    {
        size_t pos = url.find('.');
        if (pos == string::npos) return; // Invalid URL format

        // Extract domain and TLD
        domain = url.substr(0, pos);
        size_t tld_start = pos + 1;
        size_t tld_end = url.find('/', tld_start);

        if (tld_end == string::npos) {
            TLD = url.substr(tld_start); // No subdomains
        } else {
            TLD = url.substr(tld_start, tld_end - tld_start);
            
            // Extract subdomains
            string subdomain_part = url.substr(tld_end + 1);
            size_t sub_start = 0, sub_end;
            while ((sub_end = subdomain_part.find('/', sub_start)) != string::npos) {
                subdomains.push_back(subdomain_part.substr(sub_start, sub_end - sub_start));
                sub_start = sub_end + 1;
            }
            if (sub_start < subdomain_part.size()) {
                subdomains.push_back(subdomain_part.substr(sub_start));
            }
        }
    }
};


class DNS
{
    shared_ptr<Tree<string, string>> domainTree;
    void getDomainPagesHelper(shared_ptr<node<string,string>> currDomain, string currentPath, shared_ptr<vector<string>> results);

public:
    DNS();
    void addWebpage(string url);
    int numRegisteredTLDs();
    vector<shared_ptr<Webpage>> getDomainPages(string domain);
    vector<shared_ptr<Webpage>> getAllWebpages(string TLD);
    shared_ptr<Tree<string, string>> getDomainTree();
    string findTLD(string domain);
    void generateOutput(vector<shared_ptr<Webpage>> pages);
};

#endif
