#include "dns.h"

using namespace std;

// Constructor to initialize the DNS tree with the ROOT node
DNS::DNS()
{
    domainTree = make_shared<Tree<string, string>>(make_shared<node<string, string>>("ROOT", "ROOT"));
}

// Generate output to a file for a list of webpages
void DNS::generateOutput(vector<shared_ptr<Webpage>> pages)
{
    ofstream file("output.txt", ios::out);
    if (!file.is_open()) {
        cerr << "Error opening output.txt file" << endl;
        return;
    }

    int length = pages.size();
    file << length << endl;  // Write the number of pages

    for (int i = 0; i < length; i++)
    {
        auto ptr = pages.at(i);
        string line = ptr->domain + "." + ptr->TLD;

        if (!ptr->subdomains.empty())
        {
            for (const auto &subdomain : ptr->subdomains)
            {
                line += "/" + subdomain;
            }
        }

        file << line << endl;
    }

    file.close();
}

// Add a new webpage to the DNS hierarchy
void DNS::addWebpage(string url)
{
    shared_ptr<Webpage> webpage = make_shared<Webpage>(url);

    // Step 1: Add or find the TLD node
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(webpage->TLD);
    if (tldNode == nullptr)
    {
        // If the TLD doesn't exist, create a new node for the TLD
        tldNode = make_shared<node<string, string>>(webpage->TLD, webpage->TLD);
        domainTree->insertChild(tldNode, "ROOT"); // Insert under ROOT
    }

    // Step 2: Add or find the domain node under the TLD
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(webpage->domain);
    if (domainNode == nullptr)
    {
        // If the domain doesn't exist, create a new node for the domain
        domainNode = make_shared<node<string, string>>(webpage->domain, webpage->domain);
        domainTree->insertChild(domainNode, webpage->TLD); // Insert under TLD
    }

    // Step 3: Add the subdomains, if any, under the domain node
    shared_ptr<node<string, string>> currentNode = domainNode;
    for (const auto &subdomain : webpage->subdomains)
    {
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(subdomain);
        if (subdomainNode == nullptr)
        {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentNode->key);
        }
        currentNode = subdomainNode; // Move to next subdomain
    }
}

// Return the number of registered TLDs (nodes under ROOT)
int DNS::numRegisteredTLDs()
{
    return domainTree->getAllChildren("ROOT").size();
}

// Retrieve all webpages under a specific TLD
vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> tldNode = domainTree->findKey(TLD);

    if (tldNode == nullptr)
    {
        return results; // TLD not found, return empty vector
    }

    for (const auto &domainNode : tldNode->children)
    {
        getDomainPagesHelper(domainNode, domainNode->key + "." + TLD, results);
    }

    return results;
}

// Retrieve all webpages under a specific domain
vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> results;
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);

    if (domainNode == nullptr)
    {
        return results; // Domain not found, return empty vector
    }

    getDomainPagesHelper(domainNode, domainNode->key, results);

    return results;
}

// Helper function to recursively collect pages under a domain
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currNode, string currentPath, vector<shared_ptr<Webpage>> &results)
{
    if (currNode->children.empty())
    {
        // If this is a leaf node, create a webpage and add it to results
        vector<string> subdomains = split(currentPath, '/'); // Assuming 'split' is implemented
        shared_ptr<Webpage> webpage = make_shared<Webpage>(subdomains.back(), subdomains, currNode->key);
        results.push_back(webpage);
    }
    else
    {
        for (const auto &child : currNode->children)
        {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results);
        }
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
    auto tlds = domainTree->getAllChildren("ROOT");

    for (const auto &tldNode : tlds)
    {
        for (const auto &domainNode : tldNode->children)
        {
            if (domainNode->key == domain)
            {
                return tldNode->key; // Return the TLD if domain found
            }
        }
    }

    return ""; // Domain not found, return empty string
}
