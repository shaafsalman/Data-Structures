#include "StudentDB.h"
#include <fstream>
#include <sstream>

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize any required resources if needed
    // Read the data from the CSV file
    read_csv("students.csv");
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // Determine the batch year from the student object
    int batch = student.batch;

    // Find or create the batch tree
    auto batchTree = LUMS.findKey(batch);
    if (batchTree == nullptr) {
        // If the batch doesn't exist, create a new tree for it
        batchTree = make_shared<node<int, vector<Tree<int, Student>>>>(batch, vector<Tree<int, Student>>());
        LUMS.insertChild(batchTree, batch);
    }

    // Get the school tree for the student
    auto &schoolTrees = batchTree->value;
    auto schoolTree = find_if(schoolTrees.begin(), schoolTrees.end(),
                              [&student](const Tree<int, Student>& tree) {
                                  return tree.getRoot()->key == student.schoolCode;
                              });

    if (schoolTree == schoolTrees.end()) {
        // If the school doesn't exist, create a new tree for it
        Tree<int, Student> newSchoolTree(nullptr);
        schoolTrees.push_back(newSchoolTree);
        schoolTree = schoolTrees.end() - 1;
    }

    // Insert the student into the appropriate school tree
    schoolTree->insertChild(make_shared<node<int, Student>>(student.schoolRollNumber, student), student.schoolRollNumber);
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));

    auto batchTree = LUMS.findKey(batch);
    if (batchTree == nullptr) return false;

    auto &schoolTrees = batchTree->value;
    for (auto &schoolTree : schoolTrees) {
        auto studentNode = schoolTree.findKey(schoolRollNumber);
        if (studentNode != nullptr && studentNode->value.rollNumber == rollNumber) {
            return true;
        }
    }
    return false;
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    if (searchStudent(rollNumber)) {
        int batch = stoi(rollNumber.substr(0, 2));
        string schoolCode = rollNumber.substr(2, 2);
        int schoolRollNumber = stoi(rollNumber.substr(4));

        auto batchTree = LUMS.findKey(batch);
        if (batchTree != nullptr) {
            auto &schoolTrees = batchTree->value;
            for (auto &schoolTree : schoolTrees) {
                auto studentNode = schoolTree.findKey(schoolRollNumber);
                if (studentNode != nullptr && studentNode->value.rollNumber == rollNumber) {
                    studentNode->value.display();
                    return;
                }
            }
        }
    } else {
        cout << "Student not found!" << endl;
    }
}

// Function to display all students in a specific batch
void StudentDatabase::displayBatch(const int batch) {
    auto batchTree = LUMS.findKey(batch);
    if (batchTree == nullptr) {
        cout << "No students found in this batch!" << endl;
        return;
    }

    auto &schoolTrees = batchTree->value;
    for (auto &schoolTree : schoolTrees) {
        auto schoolRoot = schoolTree.getRoot();
        for (auto &studentNode : schoolRoot->children) {
            studentNode->value.display();
        }
    }
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    auto batchTree = LUMS.findKey(batch);
    if (batchTree == nullptr) {
        cout << "No students found in this batch!" << endl;
        return;
    }

    auto &schoolTrees = batchTree->value;
    for (auto &schoolTree : schoolTrees) {
        if (schoolTree.getRoot()->key == schoolCode) {
            auto schoolRoot = schoolTree.getRoot();
            for (auto &studentNode : schoolRoot->children) {
                studentNode->value.display();
            }
            return;
        }
    }

    cout << "No students found for this school in the given batch!" << endl;
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    for (auto &batchNode : LUMS.getRoot()->children) {
        auto &schoolTrees = batchNode->value;
        for (auto &schoolTree : schoolTrees) {
            auto schoolRoot = schoolTree.getRoot();
            for (auto &studentNode : schoolRoot->children) {
                studentNode->value.display();
            }
        }
    }
}

// Function to read student data from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    // Read each line and create Student objects
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
        getline(ss, major, ',');

        Student student(rollNumber, name, age, GPA, major);
        addStudent(student);
    }

    file.close();
}
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
            case 1: {
                string name, major;
                int age;
                float GPA;
                cout << "Enter roll number: ";
                cin >> rollNumber;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter age: ";
                cin >> age;
                cout << "Enter GPA: ";
                cin >> GPA;
                cout << "Enter major: ";
                cin.ignore();
                getline(cin, major);

                Student newStudent(rollNumber, name, age, GPA, major);
                addStudent(newStudent);
                cout << "Student added successfully!\n";
                break;
            }
            case 2: {
                cout << "Enter roll number to search: ";
                cin >> rollNumber;

                if (searchStudent(rollNumber)) {
                    cout << "Student found.\n";
                } else {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 3: {
                cout << "Enter roll number to display details: ";
                cin >> rollNumber;

                displayStudent(rollNumber);
                break;
            }
            case 4: {
                cout << "Enter batch number: ";
                cin >> batch;

                displayBatch(batch);
                break;
            }
            case 5: {
                cout << "Enter batch number: ";
                cin >> batch;
                cout << "Enter school code (e.g. 01, 02, 03, etc.): ";
                cin >> schoolCode;

                displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6: {
                displayAll();
                break;
            }
            case 7: {
                cout << "Exiting the system. Goodbye!\n";
                return;
            }
            case 8: {
                system("cls"); 
                break; 
            }
        }
    }
}

int main() {
    StudentDatabase db;
    db.interface();
    return 0;
}