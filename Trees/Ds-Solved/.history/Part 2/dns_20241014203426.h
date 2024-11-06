#ifndef __DNS_H
#define __DNS_H

#include "../Part 1/tree.cpp"
#include <string>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

// Structure representing a webpage
struct Webpage
{
    string TLD; // Top-level domain
    string domain; // Domain name
    vector<string> subdomains; // List of subdomains

    // Constructor that accepts a single URL and parses it into its components
    Webpage(const string& url)
    {
        size_t domainEnd = url.find('.');
        domain = url.substr(0, domainEnd); // Extract domain

        size_t tldStart = domainEnd + 1; // Start of TLD
        size_t tldEnd = url.find('/', tldStart); // End of TLD

        if (tldEnd == string::npos) {
            TLD = url.substr(tldStart); // No subdomains, just get the TLD
        } else {
            TLD = url.substr(tldStart, tldEnd - tldStart); // Extract TLD
        }

        // Extract subdomains if any
        size_t subdomainStart = tldEnd + 1; // Start of subdomains
        if (subdomainStart < url.length()) {
            string temp;
            for (size_t k = subdomainStart; k < url.length(); ++k)
            {
                if (url[k] == '/') {
                    if (!temp.empty()) {
                        subdomains.push_back(temp);
                        temp.clear();
                    }
                } else {
                    temp += url[k]; // Build the subdomain string
                }
            }
            if (!temp.empty()) { // Add the last subdomain if exists
                subdomains.push_back(temp);
            }
        }
    }

    // New constructor that accepts domain, subdomains, and TLD
    Webpage(const string& domain, const vector<string>& subdomains, const string& TLD)
        : domain(domain), subdomains(subdomains), TLD(TLD) {}
};

// Class representing the DNS structure
class DNS
{
    shared_ptr<Tree<string, string>> domainTree; // The tree structure for storing domains

    // Private helper method to collect domain pages
    void collectDomainPages(shared_ptr<node<string,string>> currDomain, const string& currentPath, vector<shared_ptr<Webpage>>& results);

public:
    DNS(); // Constructor to initialize the DNS structure
    void addWebpage(const string& url); // Method to add a new webpage
    int numRegisteredTLDs(); // Method to count registered TLDs
    vector<shared_ptr<Webpage>> getDomainPages(const string& domain); // Retrieve pages by domain
    vector<shared_ptr<Webpage>> getAllWebpages(const string& TLD); // Retrieve all pages by TLD
    shared_ptr<Tree<string, string>> getDomainTree(); // Get the domain tree
    string findTLD(const string& domain); // Find the TLD corresponding to a domain name
    void generateOutput(const vector<shared_ptr<Webpage>>& pages); // Generate output to a file
};

#endif // __DNS_H
