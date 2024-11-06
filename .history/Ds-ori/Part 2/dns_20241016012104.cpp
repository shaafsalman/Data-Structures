#include "dns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

DNS::DNS() {
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

void DNS::addWebpage(string url) {
    Webpage webpage(url);
    // Find or create the TLD node
    auto tldNode = domainTree->findKey(webpage.TLD);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage.TLD, webpage.TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // Find or create the domain node
    auto domainNode = tldNode->findKey(webpage.domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage.domain, webpage.domain);
        domainTree->insertChild(domainNode, tldNode->key);
    }

    // Add subdomains
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto& subdomain : webpage.subdomains) {
        auto subdomainNode = currentNode->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode;
    }

    // Add the webpage as a child of the last subdomain node
    auto webpageNode = make_shared<node<string, string>>(url, url);
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
            for (auto& childNode : domainNode->children) {
                // Traverse down the tree to collect all webpages
                string fullPath = domainNode->key + "." + tldNode->key;
                if (!childNode->children.empty()) {
                    for (auto& subchildNode : childNode->children) {
                        fullPath += "/" + subchildNode->key; // Append subdomain
                        results.push_back(make_shared<Webpage>(fullPath));
                    }
                } else {
                    results.push_back(make_shared<Webpage>(fullPath)); // Append domain directly
                }
            }
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
            for (auto& childNode : domainNode->children) {
                string fullPath = domain + "." + tld;
                results.push_back(make_shared<Webpage>(fullPath)); // Add domain directly
                // Iterate through subdomains
                for (auto& subchild : childNode->children) {
                    fullPath += "/" + subchild->key; // Append subdomain
                    results.push_back(make_shared<Webpage>(fullPath));
                }
            }
        }
    }
    return results;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) {
    for (auto& child : currDomain->children) {
        // Construct the full path
        string fullPath = currentPath.empty() ? child->key : currentPath + "/" + child->key;
        // Check if child is a webpage
        if (child->children.empty()) {
            results->push_back(make_shared<Webpage>(fullPath));
        } else {
            getDomainPagesHelper(child, fullPath, results); // Recursion for deeper levels
        }
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree() {
    return domainTree;
}

string DNS::findTLD(string domain) {
    auto domainNode = domainTree->findKey(domain);
    if (domainNode) {
        // Assuming we traverse the tree to find the associated TLD
        for (auto& tldNode : domainTree->getAllChildren("ROOT")) {
            if (tldNode->findKey(domain)) {
                return tldNode->key; // Return TLD when found
            }
        }
    }
    return "";
}

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages) {
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;

    int length = pages.size();
    file << length << endl;

    for (int i = 0; i < length; i++) {
        ptr = pages.at(i);
        string line = ptr->domain + "." + ptr->TLD;

        if (!ptr->subdomains.empty()) {
            for (const auto& element : ptr->subdomains) {
                line += "/" + element;
            }
        }

        file << line << endl;
    }

    file.close();
}
