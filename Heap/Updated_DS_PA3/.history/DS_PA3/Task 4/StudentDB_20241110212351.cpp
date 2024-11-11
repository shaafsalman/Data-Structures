#include "StudentDB.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

// Function to get school index based on school code
int getSchoolIndex(const string &schoolCode) {
    if (schoolCode == "02") return 0;  
    if (schoolCode == "10") return 1;  
    if (schoolCode == "11") return 2;  
    if (schoolCode == "09") return 3;  
    return -1;  
}

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase()
    : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
    // Initialize the batches
    for (int batch = 0; batch < 4; ++batch) {
        vector<Tree<int, Student>> schoolTrees = {
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student()))
        };
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = 
            make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees);

        LUMS.insertChild(batchNode, 0);
    }

    // Read CSV data and insert students
    read_csv("students.csv");
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    string rollNumber = student.rollNumber;
    int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    string schoolCode = student.schoolCode;

    // Debugging: Output student info being added
    cout << "Adding student: " << student.name << ", Roll Number: " << rollNumber << ", Batch: " << batch << ", School Code: " << schoolCode << endl;

    // Find the pre-initialized batch node
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Batch node not found for batch " << batch << endl;
        return;
    }

    // Find the appropriate house tree based on roll number
    int houseIndex = getSchoolIndex(rollNumber);
    if (houseIndex == -1) {
        cout << "Invalid house code in roll number: " << rollNumber << endl;
        return;
    }

    Tree<int, Student> &houseTree = batchNode->value[houseIndex];

    // Ensure no duplicate student (check by school roll number)
    if (houseTree.findKey(schoolRollNumber) != nullptr) {
        cout << "Duplicate student detected, not adding: " << schoolRollNumber << endl;
        return;
    }

    // Create a new student node and insert it into the correct house tree
    shared_ptr<node<int, Student>> studentNode = 
        make_shared<node<int, Student>>(schoolRollNumber, student);
    houseTree.insertChild(studentNode, 0);

    cout << "Student added: " << student.name << endl;
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    try {
        if (rollNumber.size() < 6) throw invalid_argument("Roll number is too short.");

        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);

        if (batchStr.empty() || schoolRollNumberStr.empty()) throw invalid_argument("Invalid roll number parts.");
        if (!all_of(batchStr.begin(), batchStr.end(), ::isdigit) || 
            !all_of(schoolRollNumberStr.begin(), schoolRollNumberStr.end(), ::isdigit)) 
            throw invalid_argument("Roll number parts must contain only digits.");

        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        if (!batchNode) return false;

        int schoolIndex = getSchoolIndex(schoolCode);
        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
        return schoolTree.findKey(schoolRollNumber) != nullptr;
    } catch (const std::invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
        return false;
    } catch (const std::out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
        return false;
    }
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }

    if (rollNumber.size() < 6) {
        cout << "Roll number is too short";
        return;
    }

    string batchStr = rollNumber.substr(0, 2);
    string schoolCode = rollNumber.substr(2, 2);
    string schoolRollNumberStr = rollNumber.substr(4);

    if (batchStr.empty() || schoolRollNumberStr.empty()) {
        cout << "Invalid roll number parts.";
        return;
    }

    int batch = stoi(batchStr);
    int schoolRollNumber = stoi(schoolRollNumberStr);
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);

    int schoolIndex = getSchoolIndex(schoolCode);
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

    if (studentNode) studentNode->value.display();
}

// Function to display all students in a specific batch
void StudentDatabase::displayBatch(const int batch) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }
    for (size_t i = 0; i < batchNode->value.size(); i++) {
        Tree<int, Student> &schoolTree = batchNode->value[i];
        vector<shared_ptr<node<int, Student>>> students = schoolTree.getAllChildren(0);
        for (size_t j = 0; j < students.size(); j++) {
            students[j]->value.display();
        }
    }
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    int schoolIndex = getSchoolIndex(schoolCode);
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    vector<shared_ptr<node<int, Student>>> students = schoolTree.getAllChildren(0);
    for (size_t i = 0; i < students.size(); i++) {
        students[i]->value.display();
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    vector<shared_ptr<node<int, vector<Tree<int, Student>>>>> batches = LUMS.getAllChildren(0);
    for (size_t i = 0; i < batches.size(); i++) {
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = batches[i];
        for (size_t j = 0; j < batchNode->value.size(); j++) {
            Tree<int, Student> &schoolTree = batchNode->value[j];
            vector<shared_ptr<node<int, Student>>> students = schoolTree.getAllChildren(0);
            for (size_t k = 0; k < students.size(); k++) {
                students[k]->value.display();
            }
        }
    }
}

// Function to read and add students from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line);

    int rowCount = 0;
    int insertionCount = 0;

    // Read and process each line of the CSV file
    while (getline(file, line)) {
        istringstream ss(line);
        string rollNumber, name, major;
        int age;
        float GPA;

        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        ss >> age;
        ss.ignore();
        ss >> GPA;
        ss.ignore();
        getline(ss, major);

        // Create a student object
        Student student(rollNumber, name, age, GPA, major);

        // Increment row count and add student to the database
        rowCount++;
        addStudent(student);
        insertionCount++;
    }

    // Display row count
    cout << "Rows Counted: " << rowCount << endl;

    // Display the LUMS tree entries
     int totalStudents = 0;
    vector<shared_ptr<node<int, vector<Tree<int, Student>>>>> batches = LUMS.getAllChildren(0);
    for (size_t i = 0; i < batches.size(); i++) {
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = batches[i];
        for (size_t j = 0; j < batchNode->value.size(); j++) {
            Tree<int, Student> &schoolTree = batchNode->value[j];
            totalStudents += schoolTree.getAllChildren(0).size(); // Count students in each school
        }
    }

    cout << "Total Students in LUMS Tree: " << totalStudents << endl;
}

// Interface for user input
void StudentDatabase::interface() {
    int choice;
    string rollNumber;
    int batch;
    string schoolCode;

    while (true) {
        cout << ":------------------------------------:\n";
        cout << "|  Lums Database management System\n";
        cout << "|  1. Add Student\n";
        cout << "|  2. Search Student by Roll Number\n";
        cout << "|  3. Display Student Details\n";
        cout << "|  4. Display All Students in a Batch\n";
        cout << "|  5. Display Students in a Batch and School\n";
        cout << "|  6. Display All Students\n";
        cout << "|  7. Exit\n";
        cout << "|  8. Clear Screen\n";
        cout << ":------------------------------------:\n";

        while (!(cin >> choice) || choice < 1 || choice > 8) {
            cout << "Invalid choice. Please enter a number between 1 and 8: ";
            cin.clear(); 
            cin.ignore(10000, '\n');  
        }

        switch (choice) {
            case 1: { // Add Student
                string name, rollNumber, major;
                int age;
                float GPA;
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Age: ";
                cin >> age;
                cout << "Enter GPA: ";
                cin >> GPA;
                cout << "Enter Major: ";
                cin >> major;
                addStudent(Student(rollNumber, name, age, GPA, major));
                break;
            }
            case 2: { // Search Student
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                if (searchStudent(rollNumber)) cout << "Student found.\n";
                else cout << "Student not found.\n";
                break;
            }
            case 3: { // Display Student
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                displayStudent(rollNumber);
                break;
            }
            case 4: { // Display All in Batch
                cout << "Enter Batch Number: ";
                cin >> batch;
                displayBatch(batch);
                break;
            }
            case 5: { // Display Batch and School
                cout << "Enter Batch Number: ";
                cin >> batch;
                cout << "Enter School Code: ";
                cin >> schoolCode;
                displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6: { // Display All
                displayAll();
                break;
            }
            case 7: { // Exit
                return;
            }
            case 8: { // Clear Screen
                system("CLS");
                break;
            }
            default:
                cout << "Invalid choice, please try again.\n";
                break;
        }
    }
}

int main() {
    StudentDatabase db;
    db.interface();
    return 0;
}
