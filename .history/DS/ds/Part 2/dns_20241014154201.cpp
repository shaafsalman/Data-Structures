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

    // Create a node for the domain if it doesn't exist
    auto rootNode = domainTree->root;
    if (!rootNode->children.count(domain)) {
        rootNode->children[domain] = make_shared<node<string, string>>(domain, TLD);
    }

    // Add the webpage to the domain's list
    rootNode->children[domain]->pages.push_back(newPage);
}

int DNS::numRegisteredTLDs()
{
    set<string> TLDs;
    auto rootNode = domainTree->root;

    for (const auto& child : rootNode->children) {
        const auto& pages = child.second->pages;
        for (const auto& page : pages) {
            TLDs.insert(page->TLD);
        }
    }

    return TLDs.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;
    auto rootNode = domainTree->root;

    for (const auto& child : rootNode->children) {
        const auto& pages = child.second->pages;
        for (const auto& page : pages) {
            if (page->TLD == TLD) {
                result.push_back(page);
            }
        }
    }

    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;
    auto rootNode = domainTree->root;

    if (rootNode->children.count(domain)) {
        const auto& pages = rootNode->children[domain]->pages;
        for (const auto& page : pages) {
            result.push_back(page);
        }
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
    auto rootNode = domainTree->root;

    if (rootNode->children.count(domain)) {
        return rootNode->children[domain]->TLD;
    }

    return ""; // or some default value if TLD is not found
}