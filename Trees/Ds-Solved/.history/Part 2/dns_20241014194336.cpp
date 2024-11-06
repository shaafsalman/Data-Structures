#include "dns.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <vector>

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
    vector<string> parts = split(url, '/');
    if (parts.empty()) return;

    string domain = parts[0];
    vector<string> domainParts = split(domain, '.');

    reverse(domainParts.begin(), domainParts.end());
    shared_ptr<node<string, string>> currentNode = domainTree->getRoot();

    for (const auto& part : domainParts) {
        if (!domainTree->insertChild(currentNode, part)) {
            currentNode = domainTree->findKey(part);
        } else {
            currentNode = domainTree->findKey(part);
        }
    }

    string tld = domainParts.front();
    auto webpagePtr = make_shared<Webpage>(url);
    tldMap[tld].push_back(webpagePtr);
}

int DNS::numRegisteredTLDs()
{
    return tldMap.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    if (tldMap.find(TLD) != tldMap.end()) {
        return tldMap[TLD];
    }
    return {};
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    if (tldMap.find(domain) != tldMap.end()) {
        return tldMap[domain];
    }
    return {};
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (!currDomain) return;

    string fullPath = currentPath + (currentPath.empty() ? "" : ".") + currDomain->key;

    if (currDomain->children.empty()) {
        results->push_back(fullPath);
    }

    for (const auto& child : currDomain->children) {
        getDomainPagesHelper(child, fullPath, results);
    }
}

shared_ptr<Tree<string, string>> DNS::getDomainTree()
{
    return domainTree;
}

string DNS::findTLD(string domain)
{
    auto pos = domain.rfind('.');
    return (pos != string::npos) ? domain.substr(pos + 1) : "";
}
