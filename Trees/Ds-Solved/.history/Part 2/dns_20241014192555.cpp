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
    auto page = make_shared<Webpage>(url); // Parse the URL into a Webpage object
    string tld = page->TLD;
    string domainKey = page->domain + "." + tld;

    // Ensure TLD exists
    auto tldNode = domainTree->findKey(tld);
    if (!tldNode) {
        tldNode = make_shared<node<string, string>>(tld, tld);
        domainTree->insertChild(tldNode, "ROOT"); // Insert TLD as a child of ROOT
    }

    // Ensure domain node exists under TLD
    auto domainNode = domainTree->findKey(domainKey);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(domainKey, domainKey);
        domainTree->insertChild(domainNode, tld); // Insert domain under the corresponding TLD
    }

    // Add subdomains
    string currentParentKey = domainKey;
    for (const auto& subdomain : page->subdomains) {
        auto subdomainNode = domainTree->findKey(subdomain);
        if (!subdomainNode) {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainTree->insertChild(subdomainNode, currentParentKey); // Insert subdomain under the domain
        }
        currentParentKey = subdomain; // Update current parent to the last subdomain
    }
}




int DNS::numRegisteredTLDs() {
    return domainTree->getAllChildren("ROOT").size(); // Return the size of TLD nodes directly
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD) {
    vector<shared_ptr<Webpage>> result;
    auto domains = domainTree->getAllChildren("ROOT");

    for (const auto& domainNode : domains) {
        if (domainNode->key.ends_with("." + TLD)) { // Check if domain ends with the specified TLD
            vector<string> pages; // Use a normal vector instead
            getDomainPagesHelper(domainNode, domainNode->key, pages);
            for (const auto& page : pages) {
                result.push_back(make_shared<Webpage>(page)); // Create Webpage objects from URLs
            }
        }
    }
    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain) {
    vector<shared_ptr<Webpage>> result;
    auto domainNode = domainTree->findKey(domain);

    if (domainNode) {
        vector<string> pages; // Use a normal vector instead
        getDomainPagesHelper(domainNode, domain, pages); // Populate pages under this domain
        for (const auto& page : pages) {
            result.push_back(make_shared<Webpage>(page)); // Create Webpage objects from URLs
        }
    }
    return result;
}



// Helper function to get all pages under a domain recursively
void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results) {
    if (currDomain->children.empty()) {
        results->push_back(currentPath); // If it's a leaf, add the path
    } else {
        for (const auto& child : currDomain->children) {
            getDomainPagesHelper(child, currentPath + "/" + child->key, results); // Recur for children
        }
    }
}

// Return the domain tree
shared_ptr<Tree<string, string>> DNS::getDomainTree() {
    return domainTree; // Return the entire tree
}

// Find the top-level domain (TLD) from a domain name
string DNS::findTLD(string domain) {
    size_t pos = domain.find_last_of('.');
    if (pos != string::npos) {
        return domain.substr(pos + 1); // Return the substring after the last dot
    }
    return ""; // Return empty string if no TLD found
}

