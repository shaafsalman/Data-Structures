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
    string domain = newPage->domain;
    string TLD = newPage->TLD;

    // Find or create the node for the domain
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    if (!domainNode)
    {
        domainNode = make_shared<node<string, string>>(domain, ""); // Create new domain node
        domainTree->insertChild(domainNode, "ROOT"); // Insert under the root
    }

    // Assuming that the node can hold multiple pages, we can add the new webpage to it
    domainNode->children.push_back(newPage); // Adding the webpage to the domain node
}

int DNS::numRegisteredTLDs()
{
    set<string> TLDs;
    
    function<void(shared_ptr<node<string, string>>)> traverse = [&](shared_ptr<node<string, string>> node) {
        if (!node) return;
        for (const auto& child : node->children)
        {
            TLDs.insert(child->data); // Assuming `data` holds the TLDs
            traverse(child);
        }
    };

    traverse(domainTree->getRoot());
    return TLDs.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;
    
    function<void(shared_ptr<node<string, string>>)> traverse = [&](shared_ptr<node<string, string>> node) {
        if (!node) return;
        for (const auto& child : node->children)
        {
            if (child->data == TLD) // Check if the node's TLD matches
            {
                result.push_back(make_shared<Webpage>(child->key + "." + TLD)); // Create a Webpage for each match
            }
            traverse(child);
        }
    };

    traverse(domainTree->getRoot());
    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;

    // Find the domain node
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    if (domainNode)
    {
        for (const auto& page : domainNode->children)
        {
            result.push_back(make_shared<Webpage>(page->key + "." + domainNode->data)); // Construct the full webpage
        }
    }
    return result;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (!currDomain) return;
    
    for (const auto& page : currDomain->children)
    {
        results->push_back(currentPath + page->key + "." + currDomain->data); // Construct and add to results
    }

    for (const auto& child : currDomain->children)
    {
        getDomainPagesHelper(child, currentPath + child->key + "/", results);
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

string DNS::findTLD(string domain)
{
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    if (domainNode)
    {
        return domainNode->data; // Assuming data contains the TLD
    }
    return ""; // Return empty if not found
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