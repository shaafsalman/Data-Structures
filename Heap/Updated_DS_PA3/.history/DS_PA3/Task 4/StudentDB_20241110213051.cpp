#include "StudentDB.h"

StudentDatabase::StudentDatabase() : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
    read_csv("students.csv");
}

int getSchoolIndex(const string &schoolCode) {
    if (schoolCode == "02") return 0;
    else if (schoolCode == "09") return 1;
    else if (schoolCode == "10") return 2;
        else if (schoolCode == "11") return 2;
    return 3; 
}

void StudentDatabase::addStudent(const Student &student) {
    int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    string schoolCode = student.schoolCode;

    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        vector<Tree<int, Student>> schoolTrees = {
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student())),
            Tree<int, Student>(make_shared<node<int, Student>>(0, Student()))
        };
        batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees);
        LUMS.insertChild(batchNode, 0);
    }

    int schoolIndex = getSchoolIndex(schoolCode);

    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    if (schoolTree.findKey(schoolRollNumber) != nullptr) {
        return;
    }

    shared_ptr<node<int, Student>> studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, 0);
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

        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
        return schoolTree.findKey(schoolRollNumber) != nullptr;
    } catch (const invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
        return false;
    } catch (const out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
        return false;
    }
}

void StudentDatabase::displayStudent(const string &rollNumber) {
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }

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

        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        int schoolIndex = getSchoolIndex(schoolCode);

        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
        shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

        if (studentNode) studentNode->value.display();
    } catch (const invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
    } catch (const out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
    }
}

void StudentDatabase::displayBatch(const int batch) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }
    for (int i = 0; i < 4; ++i) {
        Tree<int, Student> &schoolTree = batchNode->value[i];
        for (shared_ptr<node<int, Student>> &studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
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

    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    for (shared_ptr<node<int, Student>> &studentNode : schoolTree.getAllChildren(0)) {
        studentNode->value.display();
    }
}

void StudentDatabase::displayAll() {
    for (shared_ptr<node<int, vector<Tree<int, Student>>>> &batchNode : LUMS.getAllChildren(0)) {
        for (Tree<int, Student> &schoolTree : batchNode->value) {
            for (shared_ptr<node<int, Student>> &studentNode : schoolTree.getAllChildren(0)) {
                studentNode->value.display();
            }
        }
    }
}

void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line);

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



// Main interface function
void StudentDatabase::interface() {
    int choice;
    string rollNumber;
    int batch;
    string schoolCode;

    while (true) {
        cout << ":------------------------------------:\n";
        cout << "|  LUMS Database management System\n";
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
                break;
            }
            case 2:
                cout << "Enter roll number to search: ";
                cin >> rollNumber;
                if (searchStudent(rollNumber)) {
                    cout << "Student found.\n";
                } else {
                    cout << "Student not found.\n";
                }
                break;
            case 3:
                cout << "Enter roll number to display details: ";
                cin >> rollNumber;
                displayStudent(rollNumber);
                break;
            case 4:
                cout << "Enter batch number: ";
                cin >> batch;
                displayBatch(batch);
                break;
            case 5:
                cout << "Enter batch number: ";
                cin >> batch;
                cout << "Enter school code: ";
                cin >> schoolCode;
                displayBatchAndSchool(batch, schoolCode);
                break;
            case 6:
                displayAll();
                break;
            case 7:
                cout << "Exiting...\n";
                return;
            case 8:
                system("CLS");
                break;
        }
    }
}


int main() {
    StudentDatabase db;
    db.interface();
    return 0;
}