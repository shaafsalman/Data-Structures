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

    int length = pages.size();

    file << length << endl; // Output the number of pages

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

// Add a new webpage to the DNS hierarchy
void DNS::addWebpage(string url)
{
    // Parse the URL into TLD, domain, and subdomains using the Webpage struct
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (tldNode == nullptr) {
        // If the TLD doesn't exist, create a new node for the TLD
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT"); // Insert under the ROOT
    }

    // Step 2: Add or find the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (domainNode == nullptr) {
        // If the domain doesn't exist, create a new node for the domain
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key); // Insert under the TLD
    }

    // Step 3: Add the subdomains, if any, under the domain node
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto& subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (subdomainNode == nullptr) {
            // Create a new node for each subdomain
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode; // Move to the next level
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
        getDomainPagesHelper(domainNode, domainNode->key + "." + TLD, results);
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
    getDomainPagesHelper(domainNode, domain + "." + domainNode->key, results);
    
    return results;
}

// Corrected helper function signature to match the declaration in dns.h
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (currDomain->children.empty()) {
        // If this is a leaf node, create a Webpage object and add it to the results
        size_t dotPos = currentPath.find_last_of('.');
        string domain = currentPath.substr(0, dotPos);
        string tld = currentPath.substr(dotPos + 1);
        
        // Create a new Webpage object with the current path and add to results
        results->push_back(domain + "." + tld); // Push back the string representation
    } else {
        for (const auto& child : currDomain->children) {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results);
        }
    }
}

// Return the pointer to the DNS tree
shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

// Find the TLD for the given domain
string DNS::findTLD(string domain)
{
    vector<shared_ptr<node<string, string>>> tlds = domainTree->getAllChildren("ROOT");
    
    // Traverse through each TLD and search for the domain
    for (const auto& tldNode : tlds) {
        for (const auto& domainNode : tldNode->children) {
            if (domainNode->key == domain) {
                return tldNode->key; // Return the TLD if the domain is found
            }
        }
    }
    
    return ""; // Return an empty string if the domain is not found
}
