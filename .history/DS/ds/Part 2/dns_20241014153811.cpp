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

    // Traverse through the tree to place the new webpage
    shared_ptr<node<string, string>> curr = domainTree->root;

    // First, check if the TLD exists
    shared_ptr<node<string, string>> TLDNode = domainTree->find(curr, newPage->TLD);

    // If the TLD does not exist, create it
    if (!TLDNode)
    {
        TLDNode = make_shared<node<string, string>>(newPage->TLD, newPage->TLD);
        curr->children.push_back(TLDNode);
    }

    // Move to the domain
    shared_ptr<node<string, string>> domainNode = domainTree->find(TLDNode, newPage->domain);

    if (!domainNode)
    {
        domainNode = make_shared<node<string, string>>(newPage->domain, newPage->domain);
        TLDNode->children.push_back(domainNode);
    }

    // Add subdomains if any
    for (auto &subdomain : newPage->subdomains)
    {
        shared_ptr<node<string, string>> subdomainNode = domainTree->find(domainNode, subdomain);
        if (!subdomainNode)
        {
            subdomainNode = make_shared<node<string, string>>(subdomain, subdomain);
            domainNode->children.push_back(subdomainNode);
        }
        domainNode = subdomainNode;
    }
}

int DNS::numRegisteredTLDs()
{
    return domainTree->root->children.size();
}


vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> resultPages;
    shared_ptr<node<string, string>> TLDNode = domainTree->find(domainTree->root, TLD);

    if (!TLDNode)
        return resultPages; // If the TLD is not found, return an empty result

    for (auto &domainNode : TLDNode->children)
    {
        string domain = domainNode->data;
        shared_ptr<Webpage> page = make_shared<Webpage>(domain + "." + TLD);
        resultPages.push_back(page);
    }

    return resultPages;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
}

string DNS::findTLD(string domain)
{
}
