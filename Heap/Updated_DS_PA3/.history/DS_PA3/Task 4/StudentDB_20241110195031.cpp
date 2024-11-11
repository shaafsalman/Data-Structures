#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() : LUMS(make_shared<node<int, vector<Tree<int, Student>>>>(0, vector<Tree<int, Student>>())) {

    // Initialize any required resources if needed
    for (int batch = 20; batch <= 24; ++batch) {  // Example range for batches
        // Initialize houses with empty trees, each with a root node
        vector<Tree<int, Student>> houses(4, Tree<int, Student>(nullptr));  
        
        // Create a shared pointer for the batch node with the houses vector
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = make_shared<node<int, vector<Tree<int, Student>>>>(batch, houses);
        
        // Insert the batch node into the LUMS structure
        LUMS.insertChild(batchNode, 0);
    }
    
    read_csv("students.csv");
}



// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // Extract batch and school details from the student object
    int batch = student.batch;
    string schoolCode = student.schoolCode;
    int schoolRollNumber = student.schoolRollNumber;

    // Identify the correct index for the school based on the schoolCode
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    // Find the batch node
    auto batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "Error: Batch " << batch << " does not exist!" << endl;
        return;
    }

    // Get the corresponding school tree (based on the school index)
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];

    // Check for duplicate student (based on schoolRollNumber)
    if (schoolTree.findKey(schoolRollNumber)) {
        cout << "Duplicate entry for student: " << student.name 
             << " with roll number: " << student.rollNumber
             << " in batch " << batch << " and school " << schoolCode << endl;
        return;
    }

    // Create a node for the student and insert it into the corresponding school tree
    shared_ptr<node<int, Student>> studentNode = make_shared<node<int, Student>>(schoolRollNumber, student);
    schoolTree.insertChild(studentNode, 0);

    cout << "Student " << student.name << " added successfully." << endl;
}
// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    try {
        // Validate the roll number length
        if (rollNumber.size() < 6) {
            throw invalid_argument("Roll number is too short.");
        }

        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);

        // Validate roll number components
        if (batchStr.empty() || schoolRollNumberStr.empty()) {
            throw invalid_argument("Invalid roll number parts.");
        }

        // Validate if batch and school roll number are numeric
        if (!all_of(batchStr.begin(), batchStr.end(), ::isdigit) || !all_of(schoolRollNumberStr.begin(), schoolRollNumberStr.end(), ::isdigit)) {
            throw invalid_argument("Roll number parts must contain only digits.");
        }

        // Parse the batch and school roll number
        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        // Find the batch node
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        if (!batchNode) return false;

        // Determine the school index
        int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

        // Get the corresponding school tree
        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
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
    // Check if student exists
    if (!searchStudent(rollNumber)) {
        cout << "Student with roll number " << rollNumber << " not found.\n";
        return;
    }

    try {
        // Validate the roll number length
        if (rollNumber.size() < 6) {
            throw invalid_argument("Roll number is too short.");
        }

        string batchStr = rollNumber.substr(0, 2);
        string schoolCode = rollNumber.substr(2, 2);
        string schoolRollNumberStr = rollNumber.substr(4);

        // Validate roll number components
        if (batchStr.empty() || schoolRollNumberStr.empty()) {
            throw invalid_argument("Invalid roll number parts.");
        }

        // Parse the batch and school roll number
        int batch = stoi(batchStr);
        int schoolRollNumber = stoi(schoolRollNumberStr);

        // Find the batch node
        shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
        int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

        // Get the corresponding school tree
        Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
        shared_ptr<node<int, Student>> studentNode = schoolTree.findKey(schoolRollNumber);

        // Display student details if found
        if (studentNode) studentNode->value.display();
    } catch (const std::invalid_argument &e) {
        cout << "Error: Invalid roll number format (" << rollNumber << "). " << e.what() << endl;
    } catch (const std::out_of_range &e) {
        cout << "Error: Roll number value out of range (" << rollNumber << "). " << e.what() << endl;
    }
}

// Function to display all students in a specific batch
void StudentDatabase::displayBatch(const int batch) {
    // Find the batch node
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    // Iterate through all school trees in the batch
    for (int i = 0; i < 4; ++i) {
        Tree<int, Student> &schoolTree = batchNode->value[i];
        for (auto &studentNode : schoolTree.getAllChildren(0)) {
            studentNode->value.display();
        }
    }
}

// Function to display all students in a batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    // Find the batch node
    shared_ptr<node<int, vector<Tree<int, Student>>>> batchNode = LUMS.findKey(batch);
    if (!batchNode) {
        cout << "No students found for batch " << batch << ".\n";
        return;
    }

    // Determine the school index
    int schoolIndex = (schoolCode == "01") ? 0 : (schoolCode == "02") ? 1 : (schoolCode == "03") ? 2 : 3;

    // Get the corresponding school tree
    Tree<int, Student> &schoolTree = batchNode->value[schoolIndex];
    for (auto &studentNode : schoolTree.getAllChildren(0)) {
        studentNode->value.display();
    }
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    // Iterate over all batches in the LUMS B+ Tree
    for (shared_ptr<node<int, vector<Tree<int, Student>>>>& batchNode : LUMS.getAllChildren(0)) {
        cout << "Batch " << batchNode->key << ":" << endl;

        // Iterate over all schools in the batch
        int schoolIndex = 0;
        for (Tree<int, Student>& schoolTree : batchNode->value) {
            cout << "School " << (schoolIndex + 1) << ":" << endl;
            int studentIndex = 0;
            // Display all students in the school
            for (shared_ptr<node<int, Student>>& studentNode : schoolTree.getAllChildren(0)) {
                cout << "    Student " << (studentIndex + 1) << ": ";
                studentNode->value.display();
                studentIndex++;
            }
            schoolIndex++;
        }
    }
}

// Function to read student data from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line);  // Skip header line if there's one

    int rowCount = 0;  // Tracks rows read from the file
    int insertionCount = 0;  // Tracks students inserted into the tree

    while (getline(file, line)) {
        istringstream ss(line);
        string rollNumber, name, major;
        int age;
        float GPA;

        // Read and parse the data from the CSV line
        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        ss >> age;
        ss.ignore();  
        ss >> GPA;
        ss.ignore();  
        getline(ss, major);

        // Create a student object
        Student student(rollNumber, name, age, GPA, major);

        // Count the row
        rowCount++;  

        // Add the student to the database and check if the insertion was successful
        addStudent(student);
        insertionCount++;  // Increment only if insertion is successful
    }

    // Display the row count and insertions separately
    cout << "Total rows processed: " << rowCount << endl;
    cout << "Total students inserted: " << insertionCount << endl;

    // Count the number of students in the database after insertion
    int totalStudentCount = 0;
    for (auto &batchNode : LUMS.getAllChildren(0)) {
        for (auto &schoolTree : batchNode->value) {
            totalStudentCount += schoolTree.getAllChildren(0).size();  // Count the students in each school tree
        }
    }

    cout << "Total students in the database: " << totalStudentCount << endl;  // Display total students in the database

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