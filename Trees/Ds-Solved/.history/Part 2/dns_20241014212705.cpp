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

    file << pages.size() << endl; // First line: number of pages

    // Output each page
    for (const auto& ptr : pages)
    {
        string line = ptr->domain + "." + ptr->TLD; // Domain + TLD
        if (!ptr->subdomains.empty())
        {
            for (const auto &element : ptr->subdomains)
                line += "/" + element; // Append subdomains
        }
        file << line << endl; // Write to the output file
    }

    file.close();
}

// Add a new webpage to the DNS hierarchy
void DNS::addWebpage(string url)
{
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (tldNode == nullptr) {
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // Step 2: Add or find the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (domainNode == nullptr) {
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key);
    }

    // Step 3: Add the subdomains under the domain node
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto& subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (subdomainNode == nullptr) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode; // Move to the next subdomain level
    }

    // Store the webpage at the final node
    currentNode->children.push_back(webpage); // Assuming this stores the webpage in the node
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
    getDomainPagesHelper(domainNode, domainNode->key, results);

    return results;
}

// Helper function to recursively collect domain pages
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, vector<shared_ptr<Webpage>>& results)
{
    // If this is a leaf node, add the current path (webpage) to the results
    if (currDomain->children.empty()) {
        // Create a Webpage with the current path and add it to results
        auto completePath = currentPath + "." + currDomain->key; // Append current node key to path
        results.push_back(make_shared<Webpage>(completePath)); // Create a new Webpage object
    }
    else {
        for (const auto& child : currDomain->children) {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results); // Recurse with updated path
        }
    }
}

// Other functions remain unchanged...


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
