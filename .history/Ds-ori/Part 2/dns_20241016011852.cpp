#include "dns.h"

using namespace std;

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




void DNS::addWebpage(string url) {
    Webpage webpage(url);
    auto tldNode = domainTree->findKey(webpage.TLD);

    // Create TLD node if it doesn't exist
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage.TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // Find or create the domain node
    auto domainNode = tldNode->findKey(webpage.domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage.domain);
        domainTree->insertChild(domainNode, webpage.TLD);
    }

    // Add subdomains
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto& subdomain : webpage.subdomains) {
        auto subdomainNode = currentNode->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode;
    }

    // Add the webpage as a child of the last subdomain node
    auto webpageNode = make_shared<node<string, string>>(url);
    domainTree->insertChild(webpageNode, currentNode->key);
}

int DNS::numRegisteredTLDs() {
    return domainTree->getAllChildren("ROOT").size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD) {
    vector<shared_ptr<Webpage>> results;
    auto tldNode = domainTree->findKey(TLD);
    
    if (tldNode) {
        for (auto& domainNode : tldNode->children) {
            getDomainPagesHelper(domainNode, "", results);
        }
    }
    return results;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain) {
    vector<shared_ptr<Webpage>> results;
    string tld = findTLD(domain);
    
    if (!tld.empty()) {
        auto domainNode = domainTree->findKey(domain);
        if (domainNode) {
            getDomainPagesHelper(domainNode, "", results);
        }
    }
    return results;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, vector<shared_ptr<Webpage>>& results) {
    for (auto& child : currDomain->children) {
        // Construct the full path
        string fullPath = currentPath.empty() ? child->key : currentPath + "/" + child->key;

        // Check if child is a webpage
        if (child->children.empty()) {
            results.push_back(make_shared<Webpage>(fullPath));
        } else {
            getDomainPagesHelper(child, fullPath, results);
        }
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree() {
    return domainTree;
}

string DNS::findTLD(string domain) {
    auto tldNode = domainTree->findKey(domain);
    if (tldNode) {
        return tldNode->key;
    }
    return "";
}

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages) {
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;

    string line;
    int length = pages.size();

    for (int i = 0; i <= length; i++) {
        if (i == 0) {
            file << length << endl;
        } else {
            ptr = pages.at(i - 1);
            line = ptr->domain + "." + ptr->TLD;  // Correctly reference TLD

            if (!ptr->subdomains.empty()) {
                for (const auto& element : ptr->subdomains) {
                    line += "/" + element;
                }
            }

            file << line << endl;
        }
    }

    file.close();
}