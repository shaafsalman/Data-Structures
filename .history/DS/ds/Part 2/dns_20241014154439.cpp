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

    // Create a new node for the domain if it doesn't exist
    auto domainNode = domainTree->findKey(domain);
    if (!domainNode)
    {
        // If the domain doesn't exist, create a new node and insert it
        auto newDomainNode = make_shared<node<string, string>>(domain, TLD);
        domainTree->insertChild(newDomainNode, "ROOT");
        domainNode = newDomainNode; // Update domainNode to the newly created node
    }

    // If we already have a node, just add the webpage to it
    domainNode->children.push_back(newPage); // Assuming we can directly add pages
}

int DNS::numRegisteredTLDs()
{
    set<string> TLDs;

    // Get all children of the root node
    auto rootChildren = domainTree->getAllChildren("ROOT");
    for (const auto& child : rootChildren)
    {
        // Assuming child->data contains the TLD
        TLDs.insert(child->data); // Add the TLD to the set for uniqueness
    }

    return TLDs.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;

    // Get all children of the root node
    auto rootChildren = domainTree->getAllChildren("ROOT");
    for (const auto& child : rootChildren)
    {
        // Assuming child->data contains the TLD
        if (child->data == TLD)
        {
            // Assuming each child has a vector of Webpage pointers
            result.insert(result.end(), child->children.begin(), child->children.end());
        }
    }

    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;

    // Find the node corresponding to the domain
    auto domainNode = domainTree->findKey(domain);
    if (domainNode)
    {
        result = domainNode->children; // Assuming children contains the webpages
    }

    return result;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (currDomain)
    {
        // Construct the full path and add it to the results
        for (const auto& page : currDomain->children)
        {
            string fullPath = currentPath + page->domain + "." + page->data; // Assuming data is TLD
            results->push_back(fullPath);
        }

        // Recursively visit all children
        for (const auto& child : currDomain->children)
        {
            getDomainPagesHelper(child, currentPath + child->key + "/", results);
        }
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

string DNS::findTLD(string domain)
{
    // Find the node corresponding to the domain
    auto domainNode = domainTree->findKey(domain);
    if (domainNode)
    {
        return domainNode->data; // Assuming data contains the TLD
    }
    return ""; // Return empty string if not found
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
                for (auto& element : ptr->subdomains)
                    line += "/" + element;
            }
            file << line << endl;
        }
    }

    file.close();
}