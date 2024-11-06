#include "dns.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Constructor to initialize the DNS tree with the ROOT node
DNS::DNS()
{
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

// Generate output to a file for a list of webpages
void DNS::generateOutput(const vector<shared_ptr<Webpage>>& pages)
{
    ofstream file("output.txt", ios::out);
    if (!file.is_open()) {
        cerr << "Error opening output.txt file" << endl;
        return;
    }

    file << pages.size() << endl;  // Write the number of pages

    for (const auto& ptr : pages)
    {
        string line = ptr->domain + "." + ptr->TLD;

        if (!ptr->subdomains.empty())
        {
            for (const auto& subdomain : ptr->subdomains)
            {
                line += "/" + subdomain;
            }
        }

        file << line << endl;
    }

    file.close();
}

// Add a new webpage to the DNS hierarchy
void DNS::addWebpage(const string& url)
{
    auto webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    auto tldNode = domainTree->findKey(webpage->TLD);
    if (!tldNode)
    {
        // Create a new node for the TLD if it doesn't exist
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT"); // Insert under ROOT
    }

    // Step 2: Add or find the domain node under the TLD
    auto domainNode = domainTree->findKey(webpage->domain);
    if (!domainNode)
    {
        // Create a new node for the domain if it doesn't exist
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key); // Insert under TLD
    }

    // Step 3: Add the subdomains, if any, under the domain node
    auto currentNode = domainNode;
    for (const auto& subdomain : webpage->subdomains)
    {
        auto subdomainNode = domainTree->findKey(subdomain);
        if (!subdomainNode)
        {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode; // Move to the next subdomain
    }

    // Debug log to verify correct addition
    cout << "Added webpage: " << webpage->domain << "." << webpage->TLD;
    if (!webpage->subdomains.empty()) {
        cout << " with subdomains: ";
        for (const auto& sub : webpage->subdomains) {
            cout << sub << " ";
        }
    }
    cout << endl; // Log the added webpage
}

// Retrieve all webpages under a specific TLD
vector<shared_ptr<Webpage>> DNS::getAllWebpages(const string& TLD)
{
    vector<shared_ptr<Webpage>> results;
    auto tldNode = domainTree->findKey(TLD);

    if (tldNode == nullptr)
    {
        cout << "TLD " << TLD << " not found." << endl; // Debug log
        return results; // TLD not found, return empty vector
    }

    for (const auto& domainNode : tldNode->children)
    {
        collectDomainPages(domainNode, domainNode->key + "." + TLD, results);
    }

    return results;
}

// Retrieve all webpages under a specific domain
vector<shared_ptr<Webpage>> DNS::getDomainPages(const string& domain)
{
    vector<shared_ptr<Webpage>> results;
    auto domainNode = domainTree->findKey(domain);

    if (domainNode == nullptr)
    {
        cout << "Domain " << domain << " not found." << endl; // Debug log
        return results; // Domain not found, return empty vector
    }

    collectDomainPages(domainNode, domainNode->key, results);
    return results;
}

// Helper function to recursively collect pages under a domain
void DNS::collectDomainPages(shared_ptr<node<string, string>> currNode, const string& currentPath, vector<shared_ptr<Webpage>>& results)
{
    if (currNode->children.empty())
    {
        // Extract domain and TLD from currentPath
        string domain, TLD;
        vector<string> subdomains;

        size_t pos = currentPath.find('.');
        if (pos != string::npos) {
            domain = currentPath.substr(0, pos);       // Extract domain
            TLD = currentPath.substr(pos + 1);         // Extract TLD
        }

        // Extract subdomains from the path
        size_t slashPos = currentPath.find('/', pos);
        if (slashPos != string::npos) {
            string subdomainPart = currentPath.substr(slashPos + 1); // Everything after the first '/'
            size_t subPos = 0, found;
            while ((found = subdomainPart.find('/', subPos)) != string::npos) {
                subdomains.push_back(subdomainPart.substr(subPos, found - subPos));
                subPos = found + 1;
            }
            if (subPos < subdomainPart.size()) {
                subdomains.push_back(subdomainPart.substr(subPos)); // Add the last subdomain
            }
        }

        // Create a webpage object and add it to the results
        auto webpage = make_shared<Webpage>(domain, subdomains, TLD);
        results.push_back(webpage);
        
        // Debug log to verify correct collection
        cout << "Collected webpage: " << domain << "." << TLD;
        if (!subdomains.empty()) {
            cout << " with subdomains: ";
            for (const auto& sub : subdomains) {
                cout << sub << " ";
            }
        }
        cout << endl; // Log the collected webpage
    }
    else
    {
        // Recursively call the function for each child node
        for (const auto& child : currNode->children)
        {
            collectDomainPages(child, currentPath + "/" + child->key, results);
        }
    }
}

// Return the pointer to the DNS tree
shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

// Find the TLD for a given domain
string DNS::findTLD(const string& domain)
{
    auto tlds = domainTree->getAllChildren("ROOT");

    for (const auto& tldNode : tlds)
    {
        for (const auto& domainNode : tldNode->children)
        {
            if (domainNode->key == domain)
            {
                return tldNode->key; // Return the TLD if domain found
            }
        }
    }

    return ""; // Domain not found, return empty string
}
