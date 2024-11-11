




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