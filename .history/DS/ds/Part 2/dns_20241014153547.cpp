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
                for (auto &element : ptr->subdomains)
                    line += "/" + element;
            }
            file << line << endl;
        }
    }

    file.close();
}

// .....................................................................................
// To Implement

void DNS::addWebpage(string url) {
    Webpage webpage(url);
    
    // Start from the ROOT node
    shared_ptr<node<string, string>> currNode = domainTree->getRoot();

    // 1. Add TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage.TLD);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage.TLD, webpage.TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // 2. Add Domain node
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage.domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage.domain, webpage.domain);
        domainTree->insertChild(domainNode, webpage.TLD);
    }

    // 3. Add Subdomains, if any
    for (const string& subdomain : webpage.subdomains) {
        shared_ptr<node<string, string>> subNode = domainTree->findKey(subdomain);
        if (!subNode) {
            subNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subNode, webpage.domain);
        }
    }
}

int DNS::numRegisteredTLDs() {
    return domainTree->getAllChildren("ROOT").size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD) {
    vector<shared_ptr<Webpage>> pages;
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(TLD);

    if (!tldNode) return pages; // TLD not found

    // Collect all domains and subdomains under this TLD
    shared_ptr<vector<string>> resultPaths = make_shared<vector<string>>();
    getDomainPagesHelper(tldNode, TLD, resultPaths);

    for (const string& path : *resultPaths) {
        pages.push_back(make_shared<Webpage>(path));
    }

    return pages;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain) {
    vector<shared_ptr<Webpage>> pages;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);

    if (!domainNode) return pages; // Domain not found

    // Collect all subdomains under this domain
    shared_ptr<vector<string>> resultPaths = make_shared<vector<string>>();
    getDomainPagesHelper(domainNode, domain, resultPaths);

    for (const string& path : *resultPaths) {
        pages.push_back(make_shared<Webpage>(path));
    }

    return pages;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) {
    // Base case: add current path to results
    results->push_back(currentPath);

    // Recursive case: Traverse each child (subdomain)
    for (const auto& child : currDomain->children) {
        getDomainPagesHelper(child, currentPath + "/" + child->key, results);
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree() {
    return domainTree;
}

string DNS::findTLD(string domain) {
    // Traverse all TLDs and find the one that contains the domain
    for (const auto& tldNode : domainTree->getAllChildren("ROOT")) {
        if (domainTree->findKey(domain)) {
            return tldNode->key;
        }
    }
    return ""; // TLD not found
}
