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
    size_t pos = url.find('.');
    if (pos == string::npos) return;

    string domain = url.substr(0, pos);
    string tld = url.substr(pos + 1);

    auto newWebpage = make_shared<Webpage>(url);

    shared_ptr<node<string, string>> currNode = domainTree->getRoot();
    bool domainFound = false;

    for (auto &child : currNode->children)
    {
        if (child->key == domain)
        {
            child->subdomains.push_back(newWebpage);
            domainFound = true;
            break;
        }
    }

    if (!domainFound)
    {
        auto newDomainNode = make_shared<node<string, string>>(domain, "");
        newDomainNode->subdomains.push_back(newWebpage);
        currNode->children.push_back(newDomainNode);
    }
}

int DNS::numRegisteredTLDs()
{
    int count = 0;
    shared_ptr<node<string, string>> root = domainTree->getRoot();
    
    for (const auto& child : root->children)
    {
        count += child->subdomains.size();
    }
    return count;
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;
    shared_ptr<node<string, string>> root = domainTree->getRoot();
    
    for (const auto& child : root->children)
    {
        if (child->key == TLD)
        {
            result.insert(result.end(), child->subdomains.begin(), child->subdomains.end());
        }
    }
    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;
    shared_ptr<node<string, string>> currNode = domainTree->getRoot();

    for (const auto& child : currNode->children)
    {
        if (child->key == domain)
        {
            result = child->subdomains;
            break;
        }
    }
    return result;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (!currDomain) return;

    for (const auto& subdomain : currDomain->subdomains)
    {
        results->push_back(currentPath + subdomain->domain + "." + subdomain->TLD);
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
    size_t pos = domain.find('.');
    return (pos != string::npos) ? domain.substr(pos + 1) : "";
}
