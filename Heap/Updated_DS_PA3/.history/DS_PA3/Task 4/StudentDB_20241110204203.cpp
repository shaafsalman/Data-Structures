#include "StudentDB.h"

int getSchoolIndex(const string &schoolCode) {
    if (schoolCode == "01") return 0;
    if (schoolCode == "02") return 1;
    if (schoolCode == "03") return 2;
    if (schoolCode == "04") return 3;
    return -1; 
}

StudentDatabase::StudentDatabase() : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
    // Initialize the database with school trees for each batch
    for (int batch = 0; batch <= 99; ++batch) {
        vector<Tree<int, Student>> schoolTrees = {
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),  // HSS
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),  // SAHSOL
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),  // SSE
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student()))   // SDSB
        };
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees);
        LUMS.insertChild(batchNode, 0);
    }

    // Populate from CSV file
    read_csv("students.csv");
}

void StudentDatabase::addStudent(const Student &student) {
    int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    string schoolCode = student.schoolCode;

    try {
        // Find the batch node
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        if (!batchNode) return;

        int schoolIndex = getSchoolIndex(schoolCode);
        if (schoolIndex == -1) return;

        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

        // Avoid duplicate entries for the same batch and school roll number
        if (schoolTree.findKey(schoolRollNumber) != nullptr) {
            shared_ptr<node<int, Student>> existingNode = schoolTree.findKey(schoolRollNumber);
            if (existingNode && existingNode->value.batch == batch) {
                return;  // Student already exists for this batch and roll number
            }
        }

        shared_ptr<node<int, Student>> studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
        schoolTree.insertChild(studentNode, 0);

    } catch (const exception &e) {
        cout << "Error while adding student: " << e.what() << endl;
    }
}

bool StudentDatabase::searchStudent(const string &rollNumber) {
    try {
        if (rollNumber.size() < 6) {
            throw invalid_argument("Roll number is too short.");
        }

        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);

        if (batchStr.empty() || schoolRollNumberStr.empty()) {
            throw invalid_argument("Invalid roll number parts.");
        }

        if (!all_of(batchStr.begin(), batchStr.end(), ::isdigit) || !all_of(schoolRollNumberStr.begin(), schoolRollNumberStr.end(), ::isdigit)) {
            throw invalid_argument("Roll number parts must contain only digits.");
        }

        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        if (!batchNode) return false;

        int schoolIndex = getSchoolIndex(schoolCode);
        if (schoolIndex == -1) return false;

        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
        shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

        return studentNode && studentNode->value.batch == batch;

    } catch (const std::invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
        return false;
    } catch (const std::out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
        return false;
    }
}

void StudentDatabase::displayStudent(const string &rollNumber) {
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }

    string batchStr = rollNumber.substr(0, 2);
    string schoolCode = rollNumber.substr(2, 2);
    string schoolRollNumberStr = rollNumber.substr(4);

    int batch = stoi(batchStr);
    int schoolRollNumber = stoi(schoolRollNumberStr);
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);

    int schoolIndex = getSchoolIndex(schoolCode);
    if (schoolIndex == -1) return;

    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

    if (studentNode && studentNode->value.batch == batch) {
        studentNode->value.display();
    }
}

void StudentDatabase::displayBatch(const int batch) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    for (auto &schoolTree : batchNode->value) {
        for (auto &studentNode : schoolTree.getAllChildren(0)) {
            if (studentNode->value.batch == batch) {
                studentNode->value.display();
            }
        }
    }
}

void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    int schoolIndex = getSchoolIndex(schoolCode);
    if (schoolIndex == -1) return;

    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    for (shared_ptr<node<int, Student>> &studentNode : schoolTree.getAllChildren(0)) {
        if (studentNode->value.batch == batch) {
            studentNode->value.display();
        }
    }
}

void StudentDatabase::displayAll() {
    for (auto &batchNode : LUMS.getAllChildren(0)) {
        for (auto &schoolTree : batchNode->value) {
            for (auto &studentNode : schoolTree.getAllChildren(0)) {
                studentNode->value.display();
            }
        }
    }
}

void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line);  

    int rowCount = 0; 
    int insertionCount = 0;  

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

        rowCount++;  

        addStudent(student);
        insertionCount++;  
    }

    cout << "Total rows processed: " << rowCount << endl;
    cout << "Total students inserted: " << insertionCount << endl;

    int totalStudentCount = 0;
    for (auto &batchNode : LUMS.getAllChildren(0)) {
        for (auto &schoolTree : batchNode->value) {
            totalStudentCount += schoolTree.getAllChildren(0).size();
        }
    }

    cout << "Total students in the database: " << totalStudentCount << endl; 

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