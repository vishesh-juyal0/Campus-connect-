
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

class Student
{
public:
    string id, name, rollNo, section, email, phone, password;
    int semester;
    float cgpa;
};

class StudentManager
{
    static const int SIZE = 101;
    vector<Student> table[SIZE];
    vector<string> ids;

    int hashFunction(string id)
    {
        int hash = 0;

        for (char c : id)
            hash = (hash * 31 + c) % SIZE;

        return hash;
    }

    Student* findStudent(string id)
    {
        int index = hashFunction(id);

        for (auto &s : table[index])
        {
            if (s.id == id)
                return &s;
        }

        return nullptr;
    }

    void insertStudent(Student s)
    {
        int index = hashFunction(s.id);

        table[index].push_back(s);
        ids.push_back(s.id);

        sort(ids.begin(), ids.end());
    }

    bool binarySearch(string id)
    {
        int low = 0, high = (int)ids.size() - 1;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;

            if (ids[mid] == id)
                return true;

            if (ids[mid] < id)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return false;
    }

    void saveStudent(Student s)
    {
        ofstream file("students.txt", ios::app);

        file << s.id << "|"
             << s.name << "|"
             << s.rollNo << "|"
             << s.section << "|"
             << s.semester << "|"
             << s.cgpa << "|"
             << s.email << "|"
             << s.phone << "|"
             << s.password << "\n";

        file.close();
    }

    void saveAllStudents()
    {
        ofstream file("students.txt");

        for (const auto &bucket : table)
        {
            for (const auto &s : bucket)
            {
                file << s.id << "|"
                     << s.name << "|"
                     << s.rollNo << "|"
                     << s.section << "|"
                     << s.semester << "|"
                     << s.cgpa << "|"
                     << s.email << "|"
                     << s.phone << "|"
                     << s.password << "\n";
            }
        }

        file.close();
    }

    void loadStudents()
    {
        ifstream file("students.txt");
        Student s;

        while (getline(file, s.id, '|'))
        {
            getline(file, s.name, '|');
            getline(file, s.rollNo, '|');
            getline(file, s.section, '|');

            file >> s.semester;
            file.ignore(numeric_limits<streamsize>::max(), '|');

            file >> s.cgpa;
            file.ignore(numeric_limits<streamsize>::max(), '|');

            getline(file, s.email, '|');
            getline(file, s.phone, '|');
            getline(file, s.password);

            if (!s.id.empty() && findStudent(s.id) == nullptr)
                insertStudent(s);
        }

        file.close();
    }

public:

    StudentManager()
    {
        loadStudents();
    }

    void registerStudent()
    {
        Student s;

        cout << "\nEnter Student ID: ";
        getline(cin, s.id);

        if (s.id.empty() || findStudent(s.id) != nullptr)
        {
            cout << "Invalid or existing Student ID!\n";
            return;
        }

        cout << "Enter Full Name: ";
        getline(cin, s.name);

        cout << "Enter Roll Number: ";
        getline(cin, s.rollNo);

        cout << "Enter Section: ";
        getline(cin, s.section);

        cout << "Enter Semester: ";
        if (!(cin >> s.semester))
        {
            cout << "Invalid semester!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter CGPA: ";
        if (!(cin >> s.cgpa))
        {
            cout << "Invalid CGPA!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Email: ";
        getline(cin, s.email);

        cout << "Enter Phone: ";
        getline(cin, s.phone);

        cout << "Create Password: ";
        getline(cin, s.password);

        if (s.password.empty())
        {
            cout << "Password cannot be empty!\n";
            return;
        }

        saveStudent(s);
        insertStudent(s);

        cout << "\nRegistration Successful!\n";
    }

    bool login(string &loggedInID)
    {
        string id, password;

        cout << "\nEnter Student ID: ";
        getline(cin, id);

        if (!binarySearch(id))
        {
            cout << "Student ID not found!\n";
            return false;
        }

        Student *s = findStudent(id);

        cout << "Enter Password: ";
        getline(cin, password);

        if (s != nullptr && s->password == password)
        {
            loggedInID = s->id;

            cout << "\nLogin Successful! Welcome, "
                 << s->name << "!\n";

            return true;
        }

        cout << "Incorrect Password!\n";
        return false;
    }

    void displayProfile(string id)
    {
        Student *s = findStudent(id);

        if (s == nullptr)
        {
            cout << "Profile not found!\n";
            return;
        }

        cout << "\n--- STUDENT PROFILE ---\n";
        cout << "Student ID : " << s->id << "\n";
        cout << "Name       : " << s->name << "\n";
        cout << "Roll No    : " << s->rollNo << "\n";
        cout << "Section    : " << s->section << "\n";
        cout << "Semester   : " << s->semester << "\n";
        cout << "CGPA       : " << s->cgpa << "\n";
        cout << "Email      : " << s->email << "\n";
        cout << "Phone      : " << s->phone << "\n";
    }

    // NEW: Update Student Profile
    void updateProfile(string id)
    {
        Student *s = findStudent(id);

        if (s == nullptr)
        {
            cout << "Profile not found!\n";
            return;
        }

        int choice;

        do
        {
            cout << "\n===== UPDATE STUDENT PROFILE =====\n";
            cout << "1. Update Name\n";
            cout << "2. Update Roll Number\n";
            cout << "3. Update Section\n";
            cout << "4. Update Semester\n";
            cout << "5. Update CGPA\n";
            cout << "6. Update Email\n";
            cout << "7. Update Phone\n";
            cout << "8. Change Password\n";
            cout << "9. Save and Return\n";

            cout << "Enter Choice: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1:
                    cout << "Enter New Name: ";
                    getline(cin, s->name);
                    break;

                case 2:
                    cout << "Enter New Roll Number: ";
                    getline(cin, s->rollNo);
                    break;

                case 3:
                    cout << "Enter New Section: ";
                    getline(cin, s->section);
                    break;

                case 4:
                    cout << "Enter New Semester: ";

                    if (!(cin >> s->semester) ||
                        s->semester < 1 || s->semester > 12)
                    {
                        cout << "Invalid semester!\n";
                        cin.clear();
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;

                case 5:
                    cout << "Enter New CGPA: ";

                    if (!(cin >> s->cgpa) ||
                        s->cgpa < 0 || s->cgpa > 10)
                    {
                        cout << "Invalid CGPA! Enter between 0 and 10.\n";
                        cin.clear();
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;

                case 6:
                    cout << "Enter New Email: ";
                    getline(cin, s->email);
                    break;

                case 7:
                    cout << "Enter New Phone: ";
                    getline(cin, s->phone);
                    break;

                case 8:
                {
                    string oldPassword;

                    cout << "Enter Current Password: ";
                    getline(cin, oldPassword);

                    if (oldPassword != s->password)
                    {
                        cout << "Incorrect Current Password!\n";
                        break;
                    }

                    cout << "Enter New Password: ";
                    getline(cin, s->password);

                    if (s->password.empty())
                    {
                        cout << "Password cannot be empty!\n";
                        cout << "Password change cancelled.\n";
                        // The previous password is restored below.
                        // See note: use a temporary variable for this case.
                    }
                    else
                    {
                        cout << "Password Updated!\n";
                    }
                    break;
                }

                case 9:
                    saveAllStudents();
                    cout << "Profile Updated Successfully!\n";
                    break;

                default:
                    cout << "Invalid Choice!\n";
            }

            if (choice != 9)
                saveAllStudents();

        } while (choice != 9);
    }
};

int main()
{
    StudentManager manager;
    string loggedInID;
    int choice;

    do
    {
        cout << "\n===== CAMPUSCONNECT =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                manager.registerStudent();
                break;

            case 2:
                if (manager.login(loggedInID))
                {
                    int option;

                    do
                    {
                        cout << "\n===== STUDENT DASHBOARD =====\n";
                        cout << "1. View Profile\n";
                        cout << "2. Update Profile\n";
                        cout << "3. Logout\n";
                        cout << "Enter choice: ";

                        if (!(cin >> option))
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input!\n";
                            continue;
                        }

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (option)
                        {
                            case 1:
                                manager.displayProfile(loggedInID);
                                break;

                            case 2:
                                manager.updateProfile(loggedInID);
                                break;

                            case 3:
                                cout << "Logged Out!\n";
                                break;

                            default:
                                cout << "Invalid Choice!\n";
                        }

                    } while (option != 3);
                }
                break;

            case 3:
                cout << "Thank you for using CampusConnect!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 3);

    return 0;
}
