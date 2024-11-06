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

void DNS::addWebpage(string url)
{
    shared_ptr<Webpage> newPage = make_shared<Webpage>(url);
    string TLD = newPage->TLD;
    string domain = newPage->domain;

    // We need to add the webpage to the domain in the tree
    // Assuming the Tree has a method to insert a domain and add a webpage to it
    if (!domainTree->insert(domain, newPage)) {
        // If insertion failed, handle the case where the domain might already exist
        // Assuming Tree class has a way to get the node for the domain
        auto existingNode = domainTree->getNode(domain);
        if (existingNode) {
            existingNode->pages.push_back(newPage); // Add the new webpage to the existing node
        }
    }
}

int DNS::numRegisteredTLDs()
{
    set<string> TLDs;

    // Assuming Tree class has a method to traverse nodes
    domainTree->traverse([&TLDs](const shared_ptr<node<string, string>>& currNode) {
        for (const auto& page : currNode->pages) {
            TLDs.insert(page->TLD);
        }
    });

    return TLDs.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;

    // Assuming Tree class has a method to traverse nodes
    domainTree->traverse([&result, TLD](const shared_ptr<node<string, string>>& currNode) {
        for (const auto& page : currNode->pages) {
            if (page->TLD == TLD) {
                result.push_back(page);
            }
        }
    });

    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;

    // Assuming Tree class has a method to get a node by domain
    auto domainNode = domainTree->getNode(domain);
    if (domainNode) {
        result = domainNode->pages; // Directly access pages from the node
    }

    return result;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (currDomain) {
        for (const auto& page : currDomain->pages) {
            string fullPath = currentPath + page->domain + "." + page->TLD;
            results->push_back(fullPath);
        }

        for (const auto& child : currDomain->children) {
            getDomainPagesHelper(child.second, currentPath + child.first + "/", results);
        }
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

string DNS::findTLD(string domain)
{
    // Assuming Tree class has a method to get a node by domain
    auto domainNode = domainTree->getNode(domain);
    if (domainNode) {
        return domainNode->TLD; // Assuming TLD is accessible from the node
    }

    return ""; // Return an empty string if TLD not found
}