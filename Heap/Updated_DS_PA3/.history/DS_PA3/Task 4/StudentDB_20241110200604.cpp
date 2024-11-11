#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {
    // Initialize batches and houses (schools)
    for (int batch = 20; batch <= 24; ++batch) {
        vector<Tree<int, Student>> houses(4, Tree<int, Student>(nullptr));  // 4 schools per batch (e.g., HSS, SAHSOL, etc.)
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, houses);
        LUMS.insertChild(batchNode, 0);
    }
    
    read_csv("students.csv");  // Load student data from CSV
}

// Function to parse roll number into batch, schoolCode, and schoolRollNumber

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    int batch, schoolRollNumber;
    string schoolCode;

    // Parse the roll number to extract batch, school code, and school roll number
    if (!parseRollNumber(student.rollNumber, batch, schoolCode, schoolRollNumber)) {
        cout << "Invalid roll number: " << student.rollNumber << endl;
        return;
    }

    // Find the batch node
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Error: Batch " << batch << " does not exist!" << endl;
        return;
    }

    // Identify the school index based on schoolCode
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    // Get the corresponding school tree for the batch and school
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    // Check for duplicate student based on school roll number
    if (schoolTree.findKey(schoolRollNumber)) {
        cout << "Duplicate entry for student: " << student.name 
             << " with roll number: " << student.rollNumber
             << " in batch " << batch << " and school " << schoolCode << endl;
        return;
    }

    // Create and insert the student node into the tree
    shared_ptr<node<int, Student>> studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, 0);

    cout << "Student " << student.name << " added successfully." << endl;
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    int batch, schoolRollNumber;
    string schoolCode;

   
    // Find the batch node
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) return false;

    // Determine the school index based on the school code
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    // Get the corresponding school tree
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    // Check if the student exists in the school tree
    return schoolTree.findKey(schoolRollNumber) != nullptr;
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }

    int batch, schoolRollNumber;
    string schoolCode;

   
    

    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

    if (studentNode) {
        studentNode->value.display();
    }
}

// Function to display all students in a batch
void StudentDatabase::displayBatch(const int batch) {
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    for (int i = 0; i < 4; ++i) {  // Iterate over schools
        Tree<int, Student> &schoolTree = batchNode->value[i];
        for (auto &studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
        }
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    for (auto &batchNode : LUMS.getAllChildren(0)) {
        cout << "Batch " << batchNode->key << ":\n";
        for (int i = 0; i < 4; ++i) {
            cout << "  School " << (i + 1) << ":\n";
            Tree<int, Student> &schoolTree = batchNode->value[i];
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
    getline(file, line);  // Skip header line

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
        addStudent(student);
        rowCount++;
        insertionCount++;
    }

    cout << "Total rows processed: " << rowCount << endl;
    cout << "Total students inserted: " << insertionCount << endl;

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