#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize any required resources if needed        
    LUMS = Tree<int, vector<Tree<int, Student>>>(make_shared<node<int, vector<Tree<int, Student>>>>(0));

}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // TODO: Implement the logic to add the student to the appropriate B+ Tree based on the batch and school code.
    // 1. Determine the batch year from the student object.
    // 2. Insert the student into the corresponding vector of B+ Trees indexed by school code.
     int batch = student.batch;
    int schoolRollNumber = student.schoolRollNumber;
    int schoolIndex;

    // Map school code to an index (e.g., HSS = 0, SAHSOL = 1, SSE = 2, SDSB = 3)
    if (student.schoolCode == "HSS") schoolIndex = 0;
    else if (student.schoolCode == "SAHSOL") schoolIndex = 1;
    else if (student.schoolCode == "SSE") schoolIndex = 2;
    else if (student.schoolCode == "SDSB") schoolIndex = 3;
    else return; // Invalid school code

    // Check if batch already exists
    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) {
        // Create new vector of Trees for each school if batch does not exist
        vector<Tree<int, Student>> schoolTrees(4);
        LUMS.insertChild(make_shared<node<int, vector<Tree<int, Student>>>>(batch, schoolTrees), 0);
        batchNode = LUMS.findKey(batch);
    }

    // Insert the student in the appropriate school tree for the batch
    auto &schoolTree = batchNode->value[schoolIndex];
    schoolTree.insertChild(make_shared<node<int, Student>>(schoolRollNumber, student), 0);


}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    // TODO: Implement the search functionality.
    // 1. Extract the batch and school code from the roll number.
    // 2. Search for the student in the corresponding B+ Tree using the school roll number as the key.
    
    
    // return false; // Placeholder return value


    int batch = stoi(rollNumber.substr(0, 2));
    string schoolCode = rollNumber.substr(2, 2);
    int schoolRollNumber = stoi(rollNumber.substr(4));
    int schoolIndex;

    if (schoolCode == "HSS") schoolIndex = 0;
    else if (schoolCode == "SAHSOL") schoolIndex = 1;
    else if (schoolCode == "SSE") schoolIndex = 2;
    else if (schoolCode == "SDSB") schoolIndex = 3;
    else return false;

    // Find the batch node in the LUMS Tree
    auto batchNode = LUMS.findKey(batch);
    if (batchNode == nullptr) return false;

    // Search in the appropriate school tree for the student
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
}

// Function to display all students in a specific batch and school
void StudentDatabase::displayBatchAndSchool(const int batch, const string schoolCode) {
    // TODO: Implement the logic to display students for a specific batch and school.
    // 1. Find the index of the school code to access the correct B+ Tree.
    // 2. Display students from that B+ Tree based on the batch.
}

// Function to display all students in the database
void StudentDatabase::displayAll() {
    // TODO: Implement the logic to display all students.
    // 1. Iterate over all batches in the LUMS B+ Tree.
    // 2. For each batch, iterate over the corresponding vector of B+ Trees and display each student.
}

// Function to read student data from a CSV file
void StudentDatabase::read_csv(const string &filename) {
    // TODO: Implement the CSV reading functionality.
    // 1. Open the file and read each line, parsing the details into a Student object.
    // 2. Add each student to the database using the addStudent function.
}


void displayMenu() {
    cout << "\n--- Student Database Menu ---\n";
    cout << "1. Add Student\n";
    cout << "2. Search Student by Roll Number\n";
    cout << "3. Display Student by Roll Number\n";
    cout << "4. Display All Students in a Batch\n";
    cout << "5. Display All Students in a Batch and School\n";
    cout << "6. Display All Students\n";
    cout << "7. Load Students from CSV\n";
    cout << "0. Exit\n";
    cout << "Select an option: ";
}

int main() {
    StudentDatabase db;
    int choice;
    string rollNumber;
    int batch;
    string schoolCode;
    string filename;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string rollNumber, name, major;
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

                Student newStudent(rollNumber, name, age, GPA, major);
                db.addStudent(newStudent);
                cout << "Student added successfully.\n";
                break;
            }
            case 2: {
                cout << "Enter Roll Number to Search: ";
                cin >> rollNumber;
                bool found = db.searchStudent(rollNumber);
                if (found) {
                    cout << "Student found.\n";
                } else {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 3: {
                cout << "Enter Roll Number to Display: ";
                cin >> rollNumber;
                db.displayStudent(rollNumber);
                break;
            }
            case 4: {
                cout << "Enter Batch to Display: ";
                cin >> batch;
                db.displayBatch(batch);
                break;
            }
            case 5: {
                cout << "Enter Batch: ";
                cin >> batch;
                cout << "Enter School Code: ";
                cin >> schoolCode;
                db.displayBatchAndSchool(batch, schoolCode);
                break;
            }
            case 6: {
                db.displayAll();
                break;
            }
            case 7: {
                cout << "Enter CSV Filename: ";
                cin >> filename;
                db.read_csv(filename);
                cout << "Students loaded from CSV.\n";
                break;
            }
            case 0: {
                cout << "Exiting the program.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}