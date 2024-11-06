#include "dns.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

DNS::DNS()
{
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

//------------------------
// Output Generation
//------------------------

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages)
{
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;
    int length = pages.size();
    file << length << endl;

    for (int i = 0; i < length; i++)
    {
        ptr = pages.at(i);
        string line = ptr->domain + "." + ptr->TLD;
        if (!ptr->subdomains.empty())
        {
            for (const auto &element : ptr->subdomains)
                line += "/" + element;
        }
        file << line << endl;
    }

    file.close();
}

//------------------------
// Webpage Addition
//------------------------

void DNS::addWebpage(string url) 
{
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key);
    }

    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto &subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode;
    }
}

//------------------------
// TLD Counting
//------------------------

int DNS::numRegisteredTLDs()
{
    return domainTree->getAllChildren("ROOT").size();
}

//------------------------
// Webpage Retrieval by TLD
//------------------------

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(TLD);
    
    if (tldNode == nullptr) {
        return results;
    }
    
    for (const auto& domainNode : tldNode->children) {
        shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
        getDomainPagesHelper(domainNode, domainNode->key + "." + TLD, subdomainResults);
        
        for (const auto& result : *subdomainResults) {
            results.push_back(make_shared<Webpage>(result));
        }
    }
    
    return results;
}

//------------------------
// Webpage Retrieval by Domain
//------------------------

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    
    string TLD = findTLD(domain);
    
    if (domainNode == nullptr || TLD.empty()) {
        return results;
    }
    
    shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
    getDomainPagesHelper(domainNode, domain + "." + TLD, subdomainResults);
    
    for (const auto& result : *subdomainResults) {
        results.push_back(make_shared<Webpage>(result));
    }
    
    return results;
}

//------------------------
// Helper for Page Collection
//------------------------

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (currDomain->children.empty()) {
        results->push_back(currentPath);
    }
    for (const auto& child : currDomain->children) {
        getDomainPagesHelper(child, currentPath + "/" + child->key, results);
    }
}

//------------------------
// Domain Tree Retrieval
//------------------------

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

//------------------------
// TLD Search for Domain
//------------------------

string DNS::findTLD(string domain)
{
    vector<shared_ptr<node<string, string>>> tlds = domainTree->getAllChildren("ROOT");
    
    for (const auto& tldNode : tlds) {
        for (const auto& domainNode : tldNode->children) {
            if (domainNode->key == domain) {
                return tldNode->key;
            }
        }
    }
    
    return "";
}
