#include "dns.h"

using namespace std;

// You aren't allowed to edit these method declarations or declare global variables
// .....................................................................................

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

// .....................................................................................
// To Implement

// Add a webpage to the DNS tree
void DNS::addWebpage(string url) {
    shared_ptr<Webpage> page = make_shared<Webpage>(url); // Create a new Webpage object
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
        string fullSubdomain = subdomain + "." + domainKey; // Create the full subdomain key
        shared_ptr<node<string, string>> subdomainNode = domainTree->findKey(fullSubdomain);
        if (subdomainNode == nullptr) {
            subdomainNode = make_shared<node<string, string>>(fullSubdomain, fullSubdomain);
            domainTree->insertChild(subdomainNode, parentKey);
        }
        parentKey = fullSubdomain; // Update parentKey to the last added subdomain
    }
}

// Return the number of registered top-level domains (TLDs) in the DNS tree
int DNS::numRegisteredTLDs() {
    vector<shared_ptr<node<string, string>>> children = domainTree->getAllChildren("ROOT");
    int count = 0;

    for (const auto& child : children) {
        if (child->key.find('.') != string::npos) { // Check if the child has a TLD
            count++;
        }
    }
    return count;
}

// Retrieve all webpages with the specified top-level domain (TLD)
vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD) {
    vector<shared_ptr<Webpage>> result;
    vector<shared_ptr<node<string, string>>> domains = domainTree->getAllChildren("ROOT");
    string tldWithDot = "." + TLD; // Prepend '.' for comparison

    for (const auto& domainNode : domains) {
        // Check if the domain ends with the specified TLD
        if (domainNode->key.size() >= tldWithDot.size() && 
            domainNode->key.compare(domainNode->key.size() - tldWithDot.size(), tldWithDot.size(), tldWithDot) == 0) {
            
            shared_ptr<vector<string>> pages = make_shared<vector<string>>();
            getDomainPagesHelper(domainNode, domainNode->key, pages);
            for (const auto& page : *pages) {
                result.push_back(make_shared<Webpage>(page)); // Create Webpage objects
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
        for (const auto& page : *pages) {
            result.push_back(make_shared<Webpage>(page)); // Create Webpage objects
        }
    }
    return result;
}

// Helper function to get all pages under a domain recursively
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) {
    if (currDomain->children.empty()) {
        results->push_back(currentPath); // Add the current path if no children
    } else {
        for (const auto& child : currDomain->children) {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results); // Recursive call
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
        return domain.substr(pos + 1); // Return the TLD
    }
    return "";
}
