#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
    read_csv("students.csv"); 

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
        // cout << "Student with roll number " << student.rollNumber << " already exists.\n";
        return;
    }

    // Insert the student into the correct school tree
    auto studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, 0);
}

// Function to search for a student by roll number
// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    try {
        if (rollNumber.size() < 6) {
            throw invalid_argument("Roll number is too short.");
        }

        // Debug: Print roll number and its substrings
        cout << "Roll number: " << rollNumber << endl;
        
        // Extract and print each part of the roll number for debugging
        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);
        
        cout << "Batch: " << batchStr << ", School Code: " << schoolCode << ", School Roll Number: " << schoolRollNumberStr << endl;

        // Check if substrings are valid numbers
        if (batchStr.empty() || schoolRollNumberStr.empty()) {
            throw invalid_argument("Invalid roll number parts.");
        }

        // Validate if substrings contain only digits
        if (!all_of(batchStr.begin(), batchStr.end(), ::isdigit) || !all_of(schoolRollNumberStr.begin(), schoolRollNumberStr.end(), ::isdigit)) {
            throw invalid_argument("Roll number parts must contain only digits.");
        }

        // Attempt to convert substrings to integers
        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        // Debug: Print parsed values
        cout << "Parsed Batch: " << batch << ", Parsed School Roll Number: " << schoolRollNumber << endl;

        auto batchNode = LUMS.findKey(batch);
        if (!batchNode) return false;

        int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;
        auto &schoolTree = batchNode->value[schoolIndex];
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

    try {
        if (rollNumber.size() < 6) {
            throw invalid_argument("Roll number is too short.");
        }

        // Debug: Print roll number and its substrings
        cout << "Roll number: " << rollNumber << endl;
        
        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);

        cout << "Batch: " << batchStr << ", School Code: " << schoolCode << ", School Roll Number: " << schoolRollNumberStr << endl;

        if (batchStr.empty() || schoolRollNumberStr.empty()) {
            throw invalid_argument("Invalid roll number parts.");
        }

        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        // Debug: Print parsed values
        cout << "Parsed Batch: " << batch << ", Parsed School Roll Number: " << schoolRollNumber << endl;

        auto batchNode = LUMS.findKey(batch);
        int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;
        auto &schoolTree = batchNode->value[schoolIndex];
        auto studentNode = schoolTree.findKey(schoolRollNumber);

        if (studentNode) studentNode->value.display();
    } catch (const std::invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
    } catch (const std::out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
    }
}



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

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        istringstream ss(line);
        string rollNumber, name, major;
        int age;
        float GPA;

        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        ss >> age;
        ss.ignore();  // Ignore the comma after age
        ss >> GPA;
        ss.ignore();  // Ignore the comma after GPA
        getline(ss, major);

        // Create the student object and add it to the database
        Student student(rollNumber, name, age, GPA, major);
        addStudent(student);
    }
    file.close();
}


void alert(const string &message) {
    cout << "\n--- " << message << " ---\n";
}

void displayMenu() {
 cout << "\n--- Student Database Menu ---\n";
    cout << "1. Add Student\n";
    cout << "2. Search Student by Roll Number\n";
    cout << "3. Display Student Details\n";
    cout << "4. Display All Students in a Batch\n";
    cout << "5. Display Students in a Batch and School\n";
    cout << "6. Display All Students\n";
    cout << "7. Exit\n";
    cout << "Enter your choice (1-7): ";
}

int main() {
    StudentDatabase db;

    int choice;
    string rollNumber;
    int batch;
    string schoolCode;
    string filename;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                // Add Student
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
                db.addStudent(newStudent);
                cout << "Student added successfully!\n";
                break;
            }
            case 2: {
                // Search Student by Roll Number
                cout << "Enter roll number to search: ";
                cin >> rollNumber;

                if (db.searchStudent(rollNumber)) {
                    cout << "Student found.\n";
                } else {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 3: {
                // Display Student Details
                cout << "Enter roll number to display details: ";
                cin >> rollNumber;

                db.displayStudent(rollNumber);
                break;
            }
            case 4: {
                // Display All Students in a Batch
                cout << "Enter batch number: ";
                cin >> batch;

                db.displayBatch(batch);
                break;
            }
            case 5: {
                // Display Students in a Batch and School
                cout << "Enter batch number: ";
                cin >> batch;
                cout << "Enter school code (e.g. 01, 02, 03, etc.): ";
                cin >> schoolCode;

                db.displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6: {
                // Display All Students
                db.displayAll();
                break;
            }
            case 7: {
                // Read Students from CSV File
                cout << "Enter CSV file path: ";
                cin >> filename;

                db.read_csv(filename);
                cout << "Students added from CSV file.\n";
                break;
            }
            case 8: {
                // Exit
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}