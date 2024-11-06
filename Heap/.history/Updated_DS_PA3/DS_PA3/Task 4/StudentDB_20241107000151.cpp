#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize LUMS root with an empty node
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    int schoolIndex;

    if (student.schoolCode == "HSS") schoolIndex = 0;
    else if (student.schoolCode == "SAHSOL") schoolIndex = 1;
    else if (student.schoolCode == "SSE") schoolIndex = 2;
    else if (student.schoolCode == "SDSB") schoolIndex = 3;
    else return;

    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) {
        vector<Tree<int, Student>> schoolTrees(4);
        LUMS.insertChild(make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees), 0);
        batchNode = LUMS.findKey(batch);
    }

    auto &schoolTree = batchNode->value[schoolIndex];
    schoolTree.insertChild(make_shared<node<int, Student>>(schoolRollNumber, student), 0);
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));
    int schoolIndex;

    if (schoolCode == "HSS") schoolIndex = 0;
    else if (schoolCode == "SAHSOL") schoolIndex = 1;
    else if (schoolCode == "SSE") schoolIndex = 2;
    else if (schoolCode == "SDSB") schoolIndex = 3;
    else return false;

    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) return false;

    auto studentNode = batchNode->value[schoolIndex].findKey(schoolRollNumber);
    return studentNode != nullptr;
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));
    int schoolIndex;

    if (schoolCode == "HSS") schoolIndex = 0;
    else if (schoolCode == "SAHSOL") schoolIndex = 1;
    else if (schoolCode == "SSE") schoolIndex = 2;
    else if (schoolCode == "SDSB") schoolIndex = 3;
    else {
        cout << "Invalid school code.\n";
        return;
    }

    auto batchNode = LUMS.findKey(batch);
    if (batchNode != nullptr) {
        auto studentNode = batchNode->value[schoolIndex].findKey(schoolRollNumber);
        if (studentNode != nullptr) {
            studentNode->value.display();
            return;
        }
    }
    cout << "Student not found.\n";
}

// Function to display all students in a specific batch
void StudentDatabase::displayBatch(const int batch) {
    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    for (int i = 0; i < 4; i++) {
        auto &schoolTree = batchNode->value[i];
        for (auto studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
        }
    }
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    int schoolIndex;
    if (schoolCode == "HSS") schoolIndex = 0;
    else if (schoolCode == "SAHSOL") schoolIndex = 1;
    else if (schoolCode == "SSE") schoolIndex = 2;
    else if (schoolCode == "SDSB") schoolIndex = 3;
    else {
        cout << "Invalid school code.\n";
        return;
    }

    auto batchNode = LUMS.findKey(batch);
    if (batchNode != nullptr) {
        auto &schoolTree = batchNode->value[schoolIndex];
        for (auto studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
        }
    } else {
        cout << "No students found for batch " << batch << " and school " << schoolCode << ".\n";
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    for (auto batchNode : LUMS.getAllChildren(0)) {
        for (int i = 0; i < 4; i++) {
            auto &schoolTree = batchNode->value[i];
            for (auto studentNode : schoolTree.getAllChildren(0)) {
                studentNode->value.display();
            }
        }
    }
}

// Function to read student data from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string rollNumber, name, ageStr, gpaStr, major;
        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, gpaStr, ',');
        getline(ss, major, ',');
        int age = stoi(ageStr);
        float gpa = stof(gpaStr);
        Student student(rollNumber, name, age, gpa, major);
        addStudent(student);
    }
    file.close();
}



