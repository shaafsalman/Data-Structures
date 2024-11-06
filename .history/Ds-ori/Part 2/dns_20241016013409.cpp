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
    shared_ptr<node<string, string>> domainNode = nullptr;

    // Check if the domain already exists under the TLD node
    for (const auto& child : tldNode->children) {
        if (child->key == webpage->domain) {
            domainNode = child;
            break;
        }
    }

    if (domainNode == nullptr) {
        // If the domain doesn't exist, create a new node for the domain
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, tldNode->key); // Insert under the TLD
    }

    // Step 3: Add the subdomains, if any, under the domain node
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto& subdomain : webpage->subdomains) {
        shared_ptr<node<string, string>> subdomainNode = nullptr;

        // Check if the subdomain already exists under the current node
        for (const auto& child : currentNode->children) {
            if (child->key == subdomain) {
                subdomainNode = child;
                break;
            }
        }

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
