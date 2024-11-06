#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize LUMS root with an empty node
    shared_ptr<node<int, vector<Tree<int, Student>>>> root = make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>());  // Initialize with key 0 and empty vector
    db = Tree<int, vector<Tree<int, Student>>>(root); }

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




// Function to display the menu
void showMenu() {
    cout << "\n--- Student Database Menu ---\n";
    cout << "1. Add Student\n";
    cout << "2. Search for Student\n";
    cout << "3. Display Student Details\n";
    cout << "4. Display All Students in a Batch\n";
    cout << "5. Display All Students in a Batch and School\n";
    cout << "6. Display All Students\n";
    cout << "7. Load Students from CSV File\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

// Function to handle operations based on user input
void operationHandler(int choice, StudentDatabase &db) {
    string rollNumber;
    int batch;
    string schoolCode;
    string filename;

    switch (choice) {
        case 1:
            {
                // Input student details
                string name, major;
                int age;
                float GPA;
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Age: ";
                cin >> age;
                cout << "Enter GPA: ";
                cin >> GPA;
                cout << "Enter Major: ";
                cin.ignore();
                getline(cin, major);

                // Create student object and add it
                Student student(rollNumber, name, age, GPA, major);
                db.addStudent(student);
                cout << "Student added successfully!\n";
            }
            break;
        case 2:
            {
                // Search for student by roll number
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                if (db.searchStudent(rollNumber)) {
                    cout << "Student found.\n";
                } else {
                    cout << "Student not found.\n";
                }
            }
            break;
        case 3:
            {
                // Display student details by roll number
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                db.displayStudent(rollNumber);
            }
            break;
        case 4:
            {
                // Display all students in a specific batch
                cout << "Enter Batch Year: ";
                cin >> batch;
                db.displayBatch(batch);
            }
            break;
        case 5:
            {
                // Display all students in a specific batch and school
                cout << "Enter Batch Year: ";
                cin >> batch;
                cout << "Enter School Code: ";
                cin >> schoolCode;
                db.displayBatchAndSchool(batch, schoolCode);
            }
            break;
        case 6:
            // Display all students in the database
            db.displayAll();
            break;
        case 7:
            {
                // Load students from a CSV file
                cout << "Enter CSV Filename: ";
                cin >> filename;
                db.read_csv(filename);
                cout << "CSV file loaded successfully!\n";
            }
            break;
        case 0:
            // Exit the program
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

int main() {
    StudentDatabase db;
    int choice;

    do {
        showMenu();
        cin >> choice;
        operationHandler(choice, db);
    } while (choice != 0);

    cout << "Exiting the program...\n";
    return 0;
}