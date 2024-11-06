#include "dns.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <sstream>

using namespace std;

DNS::DNS()
{
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages)
{
    ofstream file("output.txt", ios::out);
    shared_ptr<Webpage> ptr;

    int length = pages.size();
    file << length << endl;

    for (int i = 0; i < length; i++)
    {
        ptr = pages.at(i);
        string line = ptr->domain + "." + ptr->TLD; // Construct base URL with domain and TLD

        if (!ptr->subdomains.empty())
        {
            for (const auto &element : ptr->subdomains)
                line += "/" + element; // Append subdomain paths
        }

        file << line << endl;
        cout << "Generated URL: " << line << endl; // Debug output
    }

    file.close();
}

void DNS::addWebpage(string url) 
{
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // Step 2: Add or find the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key);
    }

    // Step 3: Add the subdomains, if any, under the domain node
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto &subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode; // Move to the current subdomain node
    }
}

// Return the number of registered TLDs (second-level nodes under the ROOT)
int DNS::numRegisteredTLDs()
{
    return domainTree->getAllChildren("ROOT").size();
}

// Retrieve all webpages under a specific TLD
vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(TLD);
    
    if (tldNode == nullptr) {
        return results; // Return empty vector if TLD not found
    }
    
    // Traverse all domains and subdomains under the TLD and collect the pages
    for (const auto& domainNode : tldNode->children) {
        shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
        getDomainPagesHelper(domainNode, domainNode->key + "." + TLD, subdomainResults);
        
        // Convert string results to Webpage objects and add them to results
        for (const auto& result : *subdomainResults) {
            results.push_back(make_shared<Webpage>(result)); // Create a Webpage for each string
        }
    }
    
    return results;
}

// Retrieve all webpages under a specific domain
vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    
    if (domainNode == nullptr) {
        return results; // Return empty vector if domain not found
    }
    
    // Use the helper function to retrieve all pages under the domain
    shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
    getDomainPagesHelper(domainNode, domainNode->key, subdomainResults);
    
    // Convert string results to Webpage objects and add them to results
    for (const auto& result : *subdomainResults) {
        results.push_back(make_shared<Webpage>(result)); // Create a Webpage for each string
    }
    
    return results;
}

// Helper function to recursively collect domain pages
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    // If this is a leaf node, add the current path (webpage) to the results
    if (currDomain->children.empty()) {
        results->push_back(currentPath); // Add the path as a string
    }
    for (const auto& child : currDomain->children) {
        getDomainPagesHelper(child, currentPath + "/" + child->key, results);
    }
}

// Return the pointer to the DNS tree
shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

// Find the TLD for a given domain
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
