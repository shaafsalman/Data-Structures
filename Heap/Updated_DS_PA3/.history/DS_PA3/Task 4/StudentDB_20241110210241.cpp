#include "StudentDB.h"

// Constructor for StudentDatabase
StudentDatabase::StudentDatabase() {
    // Initialize any required resources if needed
}

// Function to add a student to the database
void StudentDatabase::addStudent(const Student &student) {
    // TODO: Implement the logic to add the student to the appropriate B+ Tree based on the batch and school code.
    // 1. Determine the batch year from the student object.
    // 2. Insert the student into the corresponding vector of B+ Trees indexed by school code.
}

// Function to search for a student by roll number
bool StudentDatabase::searchStudent(const string &rollNumber) {
    // TODO: Implement the search functionality.
    // 1. Extract the batch and school code from the roll number.
    // 2. Search for the student in the corresponding B+ Tree using the school roll number as the key.
    return false; // Placeholder return value
}

// Function to display a student's details by roll number
void StudentDatabase::displayStudent(const string &rollNumber) {
    // TODO: Implement the display functionality.
    // 1. Use the searchStudent function to find the student by roll number.
    // 2. If found, call the student's display method to show their details.
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