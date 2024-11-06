#include "StudentDB.h"
using namespace std;

// Constructor: Initializes the root of the LUMS tree.
StudentDatabase::StudentDatabase() {
    // The LUMS tree is initialized with batch 0 as the root.
    LUMS = Tree<int, vector<Tree<int, Student>>>(
        make_shared<node<int, vector<Tree<int, Student>>>>(0, {}));
}

// Add a new student to the database.
void StudentDatabase::addStudent(const Student &student) {
    int batch = student.batch;
    string schoolCode = student.schoolCode;

    // Find or create the batch node.
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, {});
        LUMS.insertChild(batchNode);
    }

    // Find or create the school tree for the given school code.
    auto &schoolTrees = batchNode->value;
    bool schoolFound = false;
    for (auto &schoolTree : schoolTrees) {
        if (schoolTree.getRoot()->key == schoolCode) {
            // Add the student to the existing school tree.
            schoolTree.insertChild(make_shared<node<int, Student>>(student.schoolRollNumber, student));
            schoolFound = true;
            break;
        }
    }

    if (!schoolFound) {
        // If the school code was not found, create a new tree for the school and add the student.
        Tree<int, Student> newSchoolTree(make_shared<node<int, Student>>(student.schoolRollNumber, student));
        schoolTrees.push_back(newSchoolTree);
    }
}

// Search for a student in the database by roll number.
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));  // First 2 digits
    string schoolCode = rollNumber.substr(2, 2);  // Next 2 digits
    int schoolRollNumber = stoi(rollNumber.substr(4));  // Remaining digits

    // Find the batch node and then the school node.
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) return false;

    for (auto &schoolTree : batchNode->value) {
        if (schoolTree.getRoot()->key == schoolCode) {
            // Search the school tree for the student.
            return schoolTree.findKey(schoolRollNumber) != nullptr;
        }
    }
    return false;
}

// Display the information of a student by roll number.
void StudentDatabase::displayStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));  // First 2 digits
    string schoolCode = rollNumber.substr(2, 2);  // Next 2 digits
    int schoolRollNumber = stoi(rollNumber.substr(4));  // Remaining digits

    // Find the batch node and the corresponding school tree.
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Batch not found!" << endl;
        return;
    }

    for (auto &schoolTree : batchNode->value) {
        if (schoolTree.getRoot()->key == schoolCode) {
            auto studentNode = schoolTree.findKey(schoolRollNumber);
            if (studentNode) {
                studentNode->value.display();
            } else {
                cout << "Student not found!" << endl;
            }
            return;
        }
    }
    cout << "School not found!" << endl;
}

// Display all students in a specific batch.
void StudentDatabase::displayBatch(const int batch) {
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Batch not found!" << endl;
        return;
    }

    for (auto &schoolTree : batchNode->value) {
        for (auto &studentNode : schoolTree.getRoot()->children) {
            cout << studentNode->value << endl;
        }
    }
}

// Display all students in a specific batch and school.
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Batch not found!" << endl;
        return;
    }

    for (auto &schoolTree : batchNode->value) {
        if (schoolTree.getRoot()->key == schoolCode) {
            for (auto &studentNode : schoolTree.getRoot()->children) {
                cout << studentNode->value << endl;
            }
            return;
        }
    }
    cout << "School not found!" << endl;
}

// Display all students in the entire database.
void StudentDatabase::displayAll() {
    for (auto &batchNode : LUMS.getRoot()->children) {
        for (auto &schoolTree : batchNode->value) {
            for (auto &studentNode : schoolTree.getRoot()->children) {
                cout << studentNode->value << endl;
            }
        }
    }
}

// Read student data from a CSV file and add it to the database.
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string rollNumber, name, major;
        int age;
        float GPA;

        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        ss >> age;
        ss.ignore(1, ',');
        ss >> GPA;
        ss.ignore(1, ',');
        getline(ss, major);

        Student student(rollNumber, name, age, GPA, major);
        addStudent(student);
    }
}

// User interface to interact with the student database.
void StudentDatabase::interface() {
    int choice;
    while (true) {
        cout << "Student Database Menu:\n";
        cout << "1. Add Student\n";
        cout << "2. Search Student\n";
        cout << "3. Display Student\n";
        cout << "4. Display Batch\n";
        cout << "5. Display Batch and School\n";
        cout << "6. Display All Students\n";
        cout << "7. Read Students from CSV\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string rollNumber, name, major;
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

                Student student(rollNumber, name, age, GPA, major);
                addStudent(student);
                break;
            }
            case 2: {
                string rollNumber;
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                if (searchStudent(rollNumber)) {
                    cout << "Student found!\n";
                } else {
                    cout << "Student not found!\n";
                }
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
                cout << "Enter Batch: ";
                cin >> batch;
                cout << "Enter School Code: ";
                cin >> schoolCode;
                displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6: {
                displayAll();
                break;
            }
            case 7: {
                string filename;
                cout << "Enter CSV filename: ";
                cin >> filename;
                read_csv(filename);
                break;
            }
            case 8:
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

int main() {
    StudentDatabase db;
    db.interface();
    return 0;
}
