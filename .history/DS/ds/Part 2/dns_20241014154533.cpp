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

    // Find or create the node for the domain in the tree
    shared_ptr<node<string, string>> domainNode = domainTree->findKey(domain);
    if (!domainNode) {
        domainNode = make_shared<node<string, string>>(domain, TLD); // Create new domain node
        domainTree->insertChild(domainNode, "ROOT"); // Assuming ROOT is the starting point
    }

    // Assuming we store webpages under their domain nodes
    domainNode->children.push_back(newPage); // Storing the webpage as a child of the domain node
}

int DNS::numRegisteredTLDs()
{
    set<string> TLDs;
    
    auto root = domainTree->getRoot();
    function<void(shared_ptr<node<string, string>>)> traverse = [&](shared_ptr<node<string, string>> node) {
        if (!node) return;

        // If the node represents a TLD, add it to the set
        TLDs.insert(node->data);
        
        // Traverse all children
        for (const auto& child : node->children) {
            traverse(child);
        }
    };
    
    traverse(root);
    
    return TLDs.size();
}

vector<shared_ptr<Webpage>> DNS::getAllWebpages(string TLD)
{
    vector<shared_ptr<Webpage>> result;

    auto root = domainTree->getRoot();
    
    function<void(shared_ptr<node<string, string>>)> traverse = [&](shared_ptr<node<string, string>> node) {
        if (!node) return;

        // Check if this node contains the desired TLD
        if (node->data == TLD) {
            for (const auto& page : node->children) {
                result.push_back(page); // Assuming children are Webpage pointers
            }
        }
        
        // Traverse all children
        for (const auto& child : node->children) {
            traverse(child);
        }
    };
    
    traverse(root);
    
    return result;
}

vector<shared_ptr<Webpage>> DNS::getDomainPages(string domain)
{
    vector<shared_ptr<Webpage>> result;
    
    auto domainNode = domainTree->findKey(domain);
    
    if (domainNode) {
        for (const auto& page : domainNode->children) {
            result.push_back(page); // Assuming children are Webpage pointers
        }
    }

    return result;
}

void DNS::getDomainPagesHelper(shared_ptr<node<string, string>> currDomain, string currentPath, shared_ptr<vector<string>> results)
{
    if (currDomain) {
        for (const auto& page : currDomain->children) {
            string fullPath = currentPath + page->domain + "." + page->TLD;
            results->push_back(fullPath);
        }

        for (const auto& child : currDomain->children) {
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
    auto domainNode = domainTree->findKey(domain);

    if (domainNode) {
        return domainNode->data; // Assuming the TLD is stored in the node's data
    }

    return ""; // Return empty string if the domain isn't found
}