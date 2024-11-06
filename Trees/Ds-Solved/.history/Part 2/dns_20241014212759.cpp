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
    file << length << endl; // Output the total count first

    for (const auto& webpage : pages)
    {
        string line = webpage->domain + "." + webpage->TLD; // Construct the base URL
        if (!webpage->subdomains.empty())
        {
            for (const auto& subdomain : webpage->subdomains)
                line += "/" + subdomain; // Append subdomains to the URL
        }
        file << line << endl; // Output the constructed URL
    }

    file.close();
}

void DNS::addWebpage(string url)
{
    // Parse the URL into TLD, domain, and subdomains using the Webpage struct
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (tldNode == nullptr) {
        // Create a new node for the TLD
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT");
    }

    // Step 2: Add or find the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (domainNode == nullptr) {
        // Create a new node for the domain
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

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    
    if (domainNode == nullptr) {
        return results; // Return empty vector if domain not found
    }
    
    // Use the helper function to retrieve all pages under the domain
    getDomainPagesHelper(domainNode, domainNode->key + "." + domainNode->TLD, results);
    
    return results;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, vector<shared_ptr<Webpage>>& results)
{
    if (currDomain->children.empty()) {
        // If this is a leaf node, create a new Webpage object
        auto webpage = make_shared<Webpage>(currentPath); // Ensure to pass the full path
        results.push_back(webpage); // Store the webpage in results
    }
    
    for (const auto& child : currDomain->children) {
        getDomainPagesHelper(child, currentPath + "/" + child->key, results);
    }
}

int DNS::numRegisteredTLDs()
{
    return domainTree->getAllChildren("ROOT").size();
}

string DNS::findTLD(string domain)
{
    vector<shared_ptr<node<string, string>>> tlds = domainTree->getAllChildren("ROOT");
    
    for (const auto& tldNode : tlds) {
        for (const auto& domainNode : tldNode->children) {
            if (domainNode->key == domain) {
                return tldNode->key; // Return the TLD if the domain is found
            }
        }
    }
    
    return ""; // Return an empty string if the domain is not found
}
