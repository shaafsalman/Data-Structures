#include "dns.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

DNS::DNS()
{
    // Initialize the domain tree with a root node
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

void DNS::generateOutput(vector<shared_ptr<Webpage>> pages)
{
    ofstream file("output.txt", ios::out); // Open output file
    shared_ptr<Webpage> ptr;

    int length = pages.size();
    file << length << endl; // Write the number of pages

    for (int i = 0; i < length; i++)
    {
        ptr = pages.at(i);
        // Generate the full URL with domain and TLD
        string line = ptr->domain + "." + ptr->TLD;

        // Append subdomains if they exist
        if (!ptr->subdomains.empty())
        {
            for (const auto &element : ptr->subdomains)
                line += "/" + element; // Correctly append subdomains
        }

        file << line << endl; // Write the generated line to the file
        cout << "Generated URL: " << line << endl; // Output for debugging
    }

    file.close(); // Close output file
}

void DNS::addWebpage(string url) 
{
    // Create a new Webpage from the given URL
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Find or create the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT"); // Insert under ROOT
    }

    // Find or create the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key); // Insert under TLD
    }

    // Add the subdomains, if any
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto &subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key); // Insert under current node
        }
        currentNode = subdomainNode; // Move to the current subdomain node
    }
}

int DNS::numRegisteredTLDs()
{
    // Return the count of registered TLDs under ROOT
    return domainTree->getAllChildren("ROOT").size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> results; // Initialize result vector
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(TLD);
    
    if (tldNode == nullptr) {
        return results; // Return empty vector if TLD not found
    }
    
    // Traverse all domains and collect their pages
    for (const auto& domainNode : tldNode->children) {
        shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
        // Call the helper function to get all pages under this domain
        getDomainPagesHelper(domainNode, domainNode->key + "." + TLD, subdomainResults);
        
        // Create a Webpage object for each result string
        for (const auto& result : *subdomainResults) {
            results.push_back(make_shared<Webpage>(result));
        }
    }
    
    return results; // Return collected webpages
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> results; // Initialize result vector
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    
    if (domainNode == nullptr) {
        return results; // Return empty vector if domain not found
    }
    
    // Use helper to retrieve all pages for the given domain
    shared_ptr<vector<string>> subdomainResults = make_shared<vector<string>>();
    getDomainPagesHelper(domainNode, domainNode->key + "." + domainNode->TLD, subdomainResults);
    
    // Create a Webpage object for each result string
    for (const auto& result : *subdomainResults) {
        results.push_back(make_shared<Webpage>(result));
    }
    
    return results; // Return collected webpages
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    // If the current node has no children, this is a leaf; add to results
    if (currDomain->children.empty()) {
        results->push_back(currentPath); // Add the complete URL
    }
    for (const auto& child : currDomain->children) {
        // Recurse to gather all pages in the subtree
        getDomainPagesHelper(child, currentPath + "/" + child->key, results);
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree; // Return the root of the domain tree
}

string DNS::findTLD(string domain)
{
    // Get all TLD nodes under ROOT
    vector<shared_ptr<node<string, string>>> tlds = domainTree->getAllChildren("ROOT");
    
    // Check each TLD node for the specified domain
    for (const auto& tldNode : tlds) {
        for (const auto& domainNode : tldNode->children) {
            if (domainNode->key == domain) {
                return tldNode->key; // Return TLD if domain found
            }
        }
    }
    
    return ""; // Return empty string if not found
}
