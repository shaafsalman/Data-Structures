#include "StudentsDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize LUMS as an empty M-ary tree with int keys and vector<Tree<int, Student>> values
}

// Adds a new student to the appropriate tree within the database.
void StudentDatabase::addStudent(const Student &student) {
    int batch = student.batch;
    string schoolCode = student.schoolCode;
    int schoolRollNumber = student.schoolRollNumber;

    // Find or create the batch node in the LUMS tree
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, vector<Tree<int, Student>>());
        LUMS.insertChild(batchNode, LUMS.getRoot() ? LUMS.getRoot()->key : batch); // Insert under root if present
    }

    // Initialize the vector for schools if it's empty (1 per batch for each school)
    if (batchNode->value.empty()) {
        batchNode->value.resize(4); // Assuming 4 schools
    }

    // Map school code to index
    int schoolIndex = stoi(schoolCode) - 1; // Assuming HSS=01, SAHSOL=02, SSE=03, SDSB=04
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    // Insert the student in the appropriate school tree
    shared_ptr<node<int, Student>> studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, schoolTree.getRoot() ? schoolTree.getRoot()->key : schoolRollNumber);
}

// Searches for a student by their roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    int schoolIndex = stoi(rollNumber.substr(2, 2)) - 1;
    int schoolRollNumber = stoi(rollNumber.substr(4));

    auto batchNode = LUMS.findKey(batch);
    if (!batchNode || batchNode->value.size() <= schoolIndex) return false;

    auto schoolTree = batchNode->value[schoolIndex];
    return schoolTree.findKey(schoolRollNumber) != nullptr;
}

// Displays the details of a specific student
void StudentDatabase::displayStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    int schoolIndex = stoi(rollNumber.substr(2, 2)) - 1;
    int schoolRollNumber = stoi(rollNumber.substr(4));

    auto batchNode = LUMS.findKey(batch);
    if (batchNode && batchNode->value.size() > schoolIndex) {
        auto studentNode = batchNode->value[schoolIndex].findKey(schoolRollNumber);
        if (studentNode) {
            studentNode->value.display();
            return;
        }
    }
    cout << "Student not found" << endl;
}

// Displays all students from a specific batch
void StudentDatabase::displayBatch(const int batch) {
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found in batch " << batch << endl;
        return;
    }
    for (auto &schoolTree : batchNode->value) {
        for (auto studentNode : schoolTree.getAllChildren(schoolTree.getRoot()->key)) {
            studentNode->value.display();
        }
    }
}

// Displays students from a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    int schoolIndex = stoi(schoolCode) - 1;

    auto batchNode = LUMS.findKey(batch);
    if (batchNode && batchNode->value.size() > schoolIndex) {
        for (auto studentNode : batchNode->value[schoolIndex].getAllChildren(batchNode->value[schoolIndex].getRoot()->key)) {
            studentNode->value.display();
        }
    } else {
        cout << "No students found in batch " << batch << " for school " << schoolCode << endl;
    }
}

// Displays all student records in the database
void StudentDatabase::displayAll() {
    if (!LUMS.getRoot()) {
        cout << "No student records found." << endl;
        return;
    }
    LUMS.printTree();
}

// Reads student data from a CSV file and populates the database
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip the header

    while (getline(file, line)) {
        stringstream ss(line);
        string rollNumber, name, ageStr, GPAStr, major;
        int age;
        float GPA;

        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, GPAStr, ',');
        getline(ss, major, ',');

        age = stoi(ageStr);
        GPA = stof(GPAStr);

        Student student(rollNumber, name, age, GPA, major);
        addStudent(student);
    }
}

// Interface for user interaction with the database
void StudentDatabase::interface() {
    int choice;
    do {
        cout << "1. Add Student\n2. Search Student\n3. Display Student\n";
        cout << "4. Display Batch\n5. Display Batch and School\n";
        cout << "6. Display All\n7. Load from CSV\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string rollNumber, name, major;
                int age;
                float GPA;
                cout << "Enter Roll Number, Name, Age, GPA, and Major: ";
                cin >> rollNumber >> name >> age >> GPA >> major;
                addStudent(Student(rollNumber, name, age, GPA, major));
                break;
            }
            case 2: {
                string rollNumber;
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << (searchStudent(rollNumber) ? "Student found" : "Student not found") << endl;
                break;
            }
            case 3: {
                string rollNumber;
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                displayStudent(rollNumber);
                break;
            }
            case 4: {
                int batch;
                cout << "Enter Batch: ";
                cin >> batch;
                displayBatch(batch);
                break;
            }
            case 5: {
                int batch;
                string schoolCode;
                cout << "Enter Batch and School Code: ";
                cin >> batch >> schoolCode;
                displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6:
                displayAll();
                break;
            case 7: {
                string filename;
                cout << "Enter CSV filename: ";
                cin >> filename;
                read_csv(filename);
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);
}
