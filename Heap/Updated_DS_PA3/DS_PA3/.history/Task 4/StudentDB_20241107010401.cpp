#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() 
    : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // Determine the batch and school from the student roll number
    int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    string schoolCode = student.schoolCode;

    // Find or create the batch node in the LUMS tree
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        // Initialize a vector with empty trees for each school
        vector<Tree<int, Student>> schoolTrees = {
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student()))
        };
        batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees);
        LUMS.insertChild(batchNode, 0);  // Insert the new batch node
    }

    // Map school codes to their indices in the vector
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    // Check if the student is already in the school tree
    auto &schoolTree = batchNode->value[schoolIndex];
    if (schoolTree.findKey(schoolRollNumber) != nullptr) {
        cout << "Student with roll number " << student.rollNumber << " already exists.\n";
        return;
    }

    // Insert the student into the correct school tree
    auto studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, 0);
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));

    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) return false;

    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;
    auto &schoolTree = batchNode->value[schoolIndex];
    return schoolTree.findKey(schoolRollNumber) != nullptr;
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }
    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));

    auto batchNode = LUMS.findKey(batch);
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;
    auto &schoolTree = batchNode->value[schoolIndex];
    auto studentNode = schoolTree.findKey(schoolRollNumber);

    if (studentNode) studentNode->value.display();
}

// Function to display all students in a specific batch
void StudentDatabase::displayBatch(const int batch) {
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }
    for (auto &schoolTree : batchNode->value) {
        for (auto &studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
        }
    }
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;
    auto &schoolTree = batchNode->value[schoolIndex];

    for (auto &studentNode : schoolTree.getAllChildren(0)) {
        studentNode->value.display();
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    for (auto &batchNode : LUMS.getAllChildren(0)) {
        for (auto &schoolTree : batchNode->value) {
            for (auto &studentNode : schoolTree.getAllChildren(0)) {
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

        Student student(rollNumber, name, age, GPA, major);
        addStudent(student);
    }
    file.close();
}
