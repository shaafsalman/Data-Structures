#ifndef STUDENTDB_H
#define STUDENTDB_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "tree.cpp"

using namespace std;

class Student {
public:
    // Member variables
    string rollNumber;          // Student's roll number [25020240]
    int batch;                  // Student's batch year, e.g., [25]
    string schoolCode;          // Student's school code [02]
    int schoolRollNumber;       // Student's internal number [0240]
    string name;                // Student's name
    int age;                    // Student's age
    float GPA;                  // Student's GPA
    string major;               // Student's major

    // Constructor
    Student() {
        rollNumber = "";
        name = "";
        age = 0;
        GPA = 0.0;
        major = "";
        batch = 0;
        schoolCode = "";
        schoolRollNumber = 0;
    }

    Student(string rollNumber, string name, int age, float GPA, string major)
        : rollNumber(rollNumber), name(name), age(age), GPA(GPA), major(major) {
        // Extract batch, school code, and school roll number from roll number
        batch = stoi(rollNumber.substr(0, 2));           // First 2 digits
        schoolCode = rollNumber.substr(2, 2);            // Next 2 digits
        schoolRollNumber = stoi(rollNumber.substr(4));   // Remaining digits
    }

    // Display function
    void display() const {
        cout << "Roll Number: " << rollNumber << "\n"
             << "Name: " << name << "\n"
             << "Age: " << age << "\n"
             << "GPA: " << GPA << "\n"
             << "Major: " << major << endl;
    }

    // Overload the output operator (<<)
    friend ostream &operator<<(ostream &os, const Student &student) {
        os << "Roll Number: " << student.rollNumber << ", "
           << "Name: " << student.name << ", "
           << "Age: " << student.age << ", "
           << "GPA: " << student.GPA << ", "
           << "Major: " << student.major;
        return os;
    }

    // Overload the equality operator (==)
    bool operator==(const Student &other) const {
        return rollNumber == other.rollNumber;
    }

    // Overload the inequality operator (!=)
    bool operator!=(const Student &other) const {
        return !(*this == other);
    }
};

class StudentDatabase {
private:
    Tree<int, vector<Tree<int, Student>>> LUMS; 
    // Stacked Trees for LUMS students
    // LUMS Tree has Keys as Batch Year and Values are a vector of Trees for each batch of school students 
    // Vector would be [HSS, SAHSOL, SSE, SDSB] for each batch year
    // Each Tree has Keys as School Roll Number and Values as Student objects
public:
    StudentDatabase();
    void addStudent(const Student &student);
    bool searchStudent(const string &rollNumber);
    void displayStudent(const string &rollNumber);
    void displayBatch(const int batch);
    void displayBatchAndSchool(const int batch, const string schoolCode);
    void displayAll();
    void read_csv(const string &filename); // Use CSV File given in Test Folder as it will be used
    void interface(); // User interface for the database All checking would be done here
};

#endif
