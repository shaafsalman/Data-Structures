#include "dns.h"

using namespace std;

// Constructor initializes the domainTree with a root node "ROOT"
DNS::DNS() {
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

// This function generates the output of a list of webpages into a text file
void DNS::generateOutput(vector<shared_ptr<Webpage>> pages) {
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;
    string line;
    int length = pages.size();

    for (int i = 0; i <= length; i++) {
        if (!i)
            file << length << endl;
        else {
            ptr = pages.at(i - 1);
            line = ptr->domain + "." + ptr->TLD;
            if (ptr->subdomains.size()) {
                for (auto &element : ptr->subdomains)
                    line += "/" + element;
            }
            file << line << endl;
        }
    }

    file.close();
}

// Add a webpage to the DNS tree
void DNS::addWebpage(string url) {
    shared_ptr<Webpage> page = make_shared<Webpage>(url);
    string domainKey = page->domain + "." + page->TLD;

    // Find or create domain node in the tree
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domainKey);
    if (domainNode == nullptr) {
        domainNode = make_shared<node<string, string>>(domainKey, domainKey);
        domainTree->insertChild(domainNode, "ROOT");
    }

    // Add subdomains
    string parentKey = domainKey;
    for (const auto& subdomain : page->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (subdomainNode == nullptr) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, parentKey);
        }
        parentKey = subdomain;
    }
}

// Return the number of registered top-level domains (TLDs) in the DNS tree
int DNS::numRegisteredTLDs() {
    return domainTree->getAllChildren("ROOT").size();
}

// Retrieve all webpages with the specified top-level domain (TLD)
vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD) {
    vector<shared_ptr<Webpage>> result;
    vector<shared_ptr<node<string, string>>> domains = domainTree->getAllChildren("ROOT");

    for (auto& domainNode : domains) {
        if (domainNode->key.find("." + TLD) != string::npos) {
            vector<shared_ptr<string>> pages;
            getDomainPagesHelper(domainNode, domainNode->key, make_shared<vector<string>>());
            for (auto& page : pages) {
                result.push_back(make_shared<Webpage>(page));
            }
        }
    }
    return result;
}

// Retrieve all pages under the specified domain
vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain) {
    vector<shared_ptr<Webpage>> result;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);

    if (domainNode != nullptr) {
        shared_ptr<vector<string>> pages = make_shared<vector<string>>();
        getDomainPagesHelper(domainNode, domain, pages);
        for (auto& page : *pages) {
            result.push_back(make_shared<Webpage>(page));
        }
    }
    return result;
}

// Helper function to get all pages under a domain recursively
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) {
    if (currDomain->children.empty()) {
        results->push_back(currentPath);
    } else {
        for (auto& child : currDomain->children) {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results);
        }
    }
}

// Return the domain tree
shared_ptr<Tree<string, string>> DNS::getDomainTree() {
    return domainTree;
}

// Find the top-level domain (TLD) from a domain name
string DNS::findTLD(string domain) {
    size_t pos = domain.find_last_of('.');
    if (pos != string::npos) {
        return domain.substr(pos + 1);
    }
    return "";
}
