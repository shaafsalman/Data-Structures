#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize any required resources if needed
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // TODO: Implement the logic to add the student to the appropriate B+ Tree based on the batch and school code.
    // 1. Determine the batch year from the student object.
    // 2. Insert the student into the corresponding vector of B+ Trees indexed by school code.
    
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
    // TODO: Implement the search functionality.
    // 1. Extract the batch and school code from the roll number.
    // 2. Search for the student in the corresponding B+ Tree using the school roll number as the key.
    
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
    // TODO: Implement the display functionality.
    // 1. Use the searchStudent function to find the student by roll number.
    // 2. If found, call the student's display method to show their details.
    
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
    // TODO: Implement the display functionality for all students in a given batch.
    // 1. Iterate through the vector of B+ Trees corresponding to the batch.
    // 2. For each school, display all students in that B+ Tree.
    
    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    for (int i = 0; i < 4; i++) {
        auto &schoolTree = batchNode->value[i];
        schoolTree.inorderTraversal([](Student &student) {
            student.display();
        });
    }
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    // TODO: Implement the logic to display students for a specific batch and school.
    // 1. Find the index of the school code to access the correct B+ Tree.
    // 2. Display students from that B+ Tree based on the batch.
    
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
        schoolTree.inorderTraversal([](Student &student) {
            student.display();
        });
    } else {
        cout << "No students found for batch " << batch << " and school " << schoolCode << ".\n";
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    // TODO: Implement the logic to display all students.
    // 1. Iterate over all batches in the LUMS B+ Tree.
    // 2. For each batch, iterate over the corresponding vector of B+ Trees and display each student.
    
    LUMS.inorderTraversal([](vector<Tree<int, Student>> &batchTrees) {
        for (int i = 0; i < 4; i++) {
            batchTrees[i].inorderTraversal([](Student &student) {
                student.display();
            });
        }
    });
}

// Function to read student data from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    // TODO: Implement the CSV reading functionality.
    // 1. Open the file and read each line, parsing the details into a Student object.
    // 2. Add each student to the database using the addStudent function.
    
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
