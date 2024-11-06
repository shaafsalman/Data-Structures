#include "dns.h"

using namespace std;

// You aren't allowed to edit these method declarations or declare global variables
// .....................................................................................

DNS::DNS()
{
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages)
{
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;

    string line;
    int length = pages.size();

    for (int i = 0; i <= length; i++)
    {
        if (!i)
            file << length << endl;
        else
        {
            ptr = pages.at(i - 1);
            line = ptr->domain + "." + ptr->TLD;
            if (ptr->subdomains.size())
            {
                for (int j = 0; j < ptr->subdomains.size(); j++)
                {
                    line += "/" + ptr->subdomains[j];
                }
            }
            file << line << endl;
        }
    }

    file.close();
}
// .....................................................................................
// To Implement

//------------------------
// Webpage Addition
//------------------------

void DNS::addWebpage(string url) 
{
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    
    if (!tldNode) 
    {
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    
    if (!domainNode) 
    {
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key);
    }

    shared_ptr<node<string, string>> currentNode = domainNode;
    
    for (int i = 0; i < webpage->subdomains.size(); i++) 
    {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(webpage->subdomains[i]);
        
        if (!subdomainNode) 
        {
            subdomainNode = make_shared<node<string, string>>(webpage->subdomains[i], webpage->subdomains[i]);
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
    
    if (tldNode == nullptr) 
    {
        return results;
    }
    
    for (int i = 0; i < tldNode->children.size(); i++) 
    {
        shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
        getDomainPagesHelper(tldNode->children[i], tldNode->children[i]->key + "." + TLD, subdomainResults);
        
        for (int j = 0; j < subdomainResults->size(); j++) 
        {
            results.push_back(make_shared<Webpage>(subdomainResults->at(j)));
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
    
    if (domainNode == nullptr || TLD.empty()) 
    {
        return results;
    }
    
    shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
    getDomainPagesHelper(domainNode, domain + "." + TLD, subdomainResults);
    
    for (int i = 0; i < subdomainResults->size(); i++) 
    {
        results.push_back(make_shared<Webpage>(subdomainResults->at(i)));
    }
    
    return results;
}

//------------------------
// Helper for Page Collection
//------------------------

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) 
{
    if (currDomain->children.empty()) 
    {
        results->push_back(currentPath);
    }
    
    for (int i = 0; i < currDomain->children.size(); i++) 
    {
        getDomainPagesHelper(currDomain->children[i], currentPath + "/" + currDomain->children[i]->key, results);
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
    vector<shared_ptr<node<string, string>>> allTldNodes = domainTree->getAllChildren("ROOT");
    
    for (int i = 0; i < allTldNodes.size(); i++) 
    {
        for (int j = 0; j < allTldNodes[i]->children.size(); j++) 
        {
            if (allTldNodes[i]->children[j]->key == domain) 
            {
                return allTldNodes[i]->key;
            }
        }
    }
    
    return "";
}
