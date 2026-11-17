
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>
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

    void insertStudent(Student s)
    {
        int index = hashFunction(s.id);

        table[index].push_back(s);
        ids.push_back(s.id);

        sort(ids.begin(), ids.end());
    }

    bool emailExists(string email)
    {
        for (auto &bucket : table)
        {
            for (auto &s : bucket)
            {
                if (s.email == email)
                    return true;
            }
        }

        return false;
    }

    bool rollExists(string rollNo)
    {
        for (auto &bucket : table)
        {
            for (auto &s : bucket)
            {
                if (s.rollNo == rollNo)
                    return true;
            }
        }

        return false;
    }

    bool validEmail(string email)
    {
        size_t at = email.find('@');
        size_t dot = email.find('.', at);

        return at != string::npos &&
               dot != string::npos &&
               at > 0 &&
               dot > at + 1 &&
               dot < email.length() - 1;
    }

    bool validPhone(string phone)
    {
        if (phone.length() != 10)
            return false;

        for (char c : phone)
        {
            if (!isdigit(c))
                return false;
        }

        return true;
    }

    bool validCGPA(float cgpa)
    {
        return cgpa >= 0 && cgpa <= 10;
    }

    bool validSemester(int semester)
    {
        return semester >= 1 && semester <= 12;
    }

    string generateStudentID()
    {
        int number = 1;
        string id;

        do
        {
            id = "CC" + to_string(number++);
        }
        while (binarySearch(id));

        return id;
    }

    void saveStudent(Student s)
    {
        ofstream file("students.txt", ios::app);

        if (!file)
        {
            cout << "Unable to open file!\n";
            return;
        }

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

        if (!file)
        {
            cout << "Unable to save records!\n";
            return;
        }

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

        if (!file)
            return;

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

        cout << "\nEnter Full Name: ";
        getline(cin, s.name);

        cout << "Enter Roll Number: ";
        getline(cin, s.rollNo);

        if (s.rollNo.empty() || rollExists(s.rollNo))
        {
            cout << "Invalid or Existing Roll Number!\n";
            return;
        }

        cout << "Enter Section: ";
        getline(cin, s.section);

        cout << "Enter Semester: ";

        if (!(cin >> s.semester) || !validSemester(s.semester))
        {
            cout << "Invalid Semester!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter CGPA: ";

        if (!(cin >> s.cgpa) || !validCGPA(s.cgpa))
        {
            cout << "Invalid CGPA!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Email: ";
        getline(cin, s.email);

        if (!validEmail(s.email) || emailExists(s.email))
        {
            cout << "Invalid or Existing Email!\n";
            return;
        }

        cout << "Enter Phone: ";
        getline(cin, s.phone);

        if (!validPhone(s.phone))
        {
            cout << "Invalid Phone Number!\n";
            return;
        }

        cout << "Create Password: ";
        getline(cin, s.password);

        if (s.password.empty())
        {
            cout << "Password Cannot Be Empty!\n";
            return;
        }

        s.id = generateStudentID();

        saveStudent(s);
        insertStudent(s);

        cout << "\nRegistration Successful!\n";
        cout << "Your Student ID: " << s.id << "\n";
    }

    bool login(string &loggedInID)
    {
        string id, password;

        cout << "\nEnter Student ID: ";
        getline(cin, id);

        if (!binarySearch(id))
        {
            cout << "Student ID Not Found!\n";
            return false;
        }

        Student *s = findStudent(id);

        for (int attempt = 1; attempt <= 3; attempt++)
        {
            cout << "Enter Password: ";
            getline(cin, password);

            if (s != nullptr && s->password == password)
            {
                loggedInID = s->id;

                cout << "\nLogin Successful! Welcome, "
                     << s->name << "!\n";

                return true;
            }

            cout << "Incorrect Password! Attempts Left: "
                 << 3 - attempt << "\n";
        }

        cout << "Maximum Login Attempts Reached!\n";
        return false;
    }

    void displayProfile(string id)
    {
        Student *s = findStudent(id);

        if (s == nullptr)
        {
            cout << "Profile Not Found!\n";
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

    void updateProfile(string id)
    {
        Student *s = findStudent(id);

        if (s == nullptr)
        {
            cout << "Profile Not Found!\n";
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
                cout << "Invalid Input!\n";
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

                    {
                        string roll;
                        getline(cin, roll);

                        if (roll.empty() ||
                            (rollExists(roll) && roll != s->rollNo))
                            cout << "Invalid or Existing Roll Number!\n";
                        else
                            s->rollNo = roll;
                    }
                    break;

                case 3:
                    cout << "Enter New Section: ";
                    getline(cin, s->section);
                    break;

                case 4:
                    cout << "Enter New Semester: ";

                    {
                        int semester;

                        if (cin >> semester && validSemester(semester))
                            s->semester = semester;
                        else
                        {
                            cout << "Invalid Semester!\n";
                            cin.clear();
                        }

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    break;

                case 5:
                    cout << "Enter New CGPA: ";

                    {
                        float cgpa;

                        if (cin >> cgpa && validCGPA(cgpa))
                            s->cgpa = cgpa;
                        else
                        {
                            cout << "Invalid CGPA!\n";
                            cin.clear();
                        }

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    break;

                case 6:
                    cout << "Enter New Email: ";

                    {
                        string email;
                        getline(cin, email);

                        if (validEmail(email) &&
                            (!emailExists(email) || email == s->email))
                            s->email = email;
                        else
                            cout << "Invalid or Existing Email!\n";
                    }
                    break;

                case 7:
                    cout << "Enter New Phone: ";

                    {
                        string phone;
                        getline(cin, phone);

                        if (validPhone(phone))
                            s->phone = phone;
                        else
                            cout << "Invalid Phone Number!\n";
                    }
                    break;

                case 8:
                {
                    string oldPassword, newPassword;

                    cout << "Enter Current Password: ";
                    getline(cin, oldPassword);

                    if (oldPassword != s->password)
                    {
                        cout << "Incorrect Current Password!\n";
                        break;
                    }

                    cout << "Enter New Password: ";
                    getline(cin, newPassword);

                    if (newPassword.empty())
                    {
                        cout << "Password Cannot Be Empty!\n";
                    }
                    else
                    {
                        s->password = newPassword;
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

    void searchByRoll()
    {
        string roll;

        cout << "\nEnter Roll Number: ";
        getline(cin, roll);

        for (auto &bucket : table)
        {
            for (auto &s : bucket)
            {
                if (s.rollNo == roll)
                {
                    cout << "\nStudent Found!\n";
                    displayProfile(s.id);
                    return;
                }
            }
        }

        cout << "Student Not Found!\n";
    }

    void displayStatistics()
    {
        int total = 0;
        float sum = 0;

        for (auto &bucket : table)
        {
            for (auto &s : bucket)
            {
                total++;
                sum += s.cgpa;
            }
        }

        cout << "\n===== STUDENT STATISTICS =====\n";
        cout << "Total Students : " << total << "\n";

        if (total > 0)
            cout << "Average CGPA   : " << sum / total << "\n";
        else
            cout << "No Student Records Available!\n";
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

        cout << "Enter Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Input!\n";
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
                        cout << "3. Search Student\n";
                        cout << "4. Student Statistics\n";
                        cout << "5. Logout\n";

                        cout << "Enter Choice: ";

                        if (!(cin >> option))
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid Input!\n";
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
                                manager.searchByRoll();
                                break;

                            case 4:
                                manager.displayStatistics();
                                break;

                            case 5:
                                cout << "Logged Out!\n";
                                break;

                            default:
                                cout << "Invalid Choice!\n";
                        }

                    } while (option != 5);
                }

                break;

            case 3:
                cout << "Thank You For Using CampusConnect!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 3);

    return 0;
}