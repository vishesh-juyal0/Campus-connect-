#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <cctype>

using namespace std;

// Simple ANSI colors for the console interface.
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"


class Note {
public:
    int id;
    string title, subject, content;
    Note *next, *hashNext;
};

class Resource {
public:
    string title, subject, link;
    Resource *next;
};

Note *head = NULL;
Resource *rhead = NULL;

const int SIZE = 100;
Note *table[SIZE] = {NULL};

int hashID(int id) {
    return (id % SIZE + SIZE) % SIZE;
}

void insertHash(Note *n) {
    int i = hashID(n->id);
    n->hashNext = table[i];
    table[i] = n;
}

Note* searchID(int id) {
    Note *p = table[hashID(id)];

    while (p != NULL) {
        if (p->id == id) return p;
        p = p->hashNext;
    }
    return NULL;
}

void saveNotes() {
    ofstream f("notes.txt");

    for (Note *p = head; p != NULL; p = p->next)
        f << p->id << "|" << p->title << "|"
          << p->subject << "|" << p->content << "\n";

    f.close();
}

void saveResources() {
    ofstream f("resources.txt");

    for (Resource *p = rhead; p != NULL; p = p->next)
        f << p->title << "|" << p->subject
          << "|" << p->link << "\n";

    f.close();
}

void loadData() {
    string line, a, b, c, d;
    ifstream f("notes.txt");

    while (getline(f, line)) {
        stringstream ss(line);

        getline(ss, a, '|');
        getline(ss, b, '|');
        getline(ss, c, '|');
        getline(ss, d);

        if (a.empty()) continue;

        int id = stoi(a);
        if (searchID(id) != NULL) continue;

        Note *n = new Note{id, b, c, d, head, NULL};
        head = n;
        insertHash(n);
    }
    f.close();

    ifstream r("resources.txt");

    while (getline(r, line)) {
        stringstream ss(line);

        getline(ss, a, '|');
        getline(ss, b, '|');
        getline(ss, c);

        if (a.empty()) continue;

        Resource *p = new Resource{a, b, c, rhead};
        rhead = p;
    }
    r.close();
}

void createNote() {
    Note *n = new Note;

    cout << "Enter ID: ";
    cin >> n->id;

    if (searchID(n->id)) {
        cout << "ID already exists!\n";
        delete n;
        return;
    }

    cin.ignore();

    cout << "Title: ";
    getline(cin, n->title);

    cout << "Subject: ";
    getline(cin, n->subject);

    cout << "Content: ";
    getline(cin, n->content);

    n->next = head;
    n->hashNext = NULL;
    head = n;

    insertHash(n);
    saveNotes();

    cout << "Note added!\n";
}

void displayNotes() {
    Note *p = head;

    while (p != NULL) {
        cout << "\nID: " << p->id
             << "\nTitle: " << p->title
             << "\nSubject: " << p->subject
             << "\nContent: " << p->content << "\n";

        p = p->next;
    }
}

void searchNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    Note *p = searchID(id);

    if (p)
        cout << p->title << "\n" << p->subject
             << "\n" << p->content << "\n";
    else
        cout << "Note not found!\n";
}

void searchSubject() {
    string s;
    cin.ignore();

    cout << "Enter subject: ";
    getline(cin, s);

    bool found = false;

    for (Note *p = head; p != NULL; p = p->next) {
        if (p->subject == s) {
            cout << p->id << " " << p->title << "\n";
            found = true;
        }
    }

    if (!found) cout << "No notes found!\n";
}

void editNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;
    cin.ignore();

    Note *p = searchID(id);

    if (!p) {
        cout << "Note not found!\n";
        return;
    }

    cout << "New title: ";
    getline(cin, p->title);

    cout << "New subject: ";
    getline(cin, p->subject);

    cout << "New content: ";
    getline(cin, p->content);

    saveNotes();
    cout << "Note updated!\n";
}

void deleteNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    Note *p = head, *prev = NULL;

    while (p && p->id != id) {
        prev = p;
        p = p->next;
    }

    if (!p) {
        cout << "Note not found!\n";
        return;
    }

    if (prev) prev->next = p->next;
    else head = p->next;

    int i = hashID(id);
    Note *q = table[i], *old = NULL;

    while (q && q != p) {
        old = q;
        q = q->hashNext;
    }

    if (old) old->hashNext = q->hashNext;
    else table[i] = q->hashNext;

    delete p;
    saveNotes();

    cout << "Note deleted!\n";
}

void sortNotes() {
    for (Note *p = head; p; p = p->next) {
        for (Note *q = p->next; q; q = q->next) {
            if (p->title > q->title) {
                swap(p->id, q->id);
                swap(p->title, q->title);
                swap(p->subject, q->subject);
                swap(p->content, q->content);
            }
        }
    }

    for (int i = 0; i < SIZE; i++)
        table[i] = NULL;

    for (Note *p = head; p; p = p->next)
        insertHash(p);

    saveNotes();
    displayNotes();
}

void addResource() {
    Resource *r = new Resource;
    cin.ignore();

    cout << "Resource title: ";
    getline(cin, r->title);

    cout << "Subject: ";
    getline(cin, r->subject);

    cout << "Link: ";
    getline(cin, r->link);

    r->next = rhead;
    rhead = r;

    saveResources();
    cout << "Resource added!\n";
}

void displayResources() {
    for (Resource *p = rhead; p; p = p->next) {
        cout << "\nTitle: " << p->title
             << "\nSubject: " << p->subject
             << "\nLink: " << p->link << "\n";
    }
}

struct Notification
{
    int id;
    string message;
    string category;
    int priority;
};

struct compare
{
    bool operator()(Notification a, Notification b)
    {
        return a.priority < b.priority;
    }
};

priority_queue<Notification,
               vector<Notification>, compare>
    notifications;
unordered_map<int, Notification>
    notificationData;
set<string> categories;

void addNotification(int id, string message, string category, int priority)
{
    Notification n = {id, message,
                      category, priority};

    notifications.push(n);
    notificationData[id] = n;
    categories.insert(category);
}

void notification()
{
    addNotification(101, "C++ Internship deadline in 3 days", "Internship", 2);
    addNotification(102, "DSA Hackathon deadline in 2 days", "Hackathon", 3);
    addNotification(103, "Technical Workshop", "Workshop", 1);
    addNotification(104, "Cultural Fest", "Event", 1);

    cout << "\n -----notifications ----\n";
    while (!notifications.empty())
    {
        cout << notifications.top().message << endl;
        notifications.pop();
    }
}

void recommendation()
{
    string skill;

    cout << "\n ---Recommendation system ---\n";
    cout << "Enter your skill (C++,DSA,HTML and CSS)" << endl;
    cin >> skill;

    if (skill == "C++")
    {
        cout << "Recommended: C++ Internship \n"
             << endl;
    }

    else if (skill == "DSA")
    {
        cout << "Recommended: DSA Hackathon \n"
             << endl;
    }

    else if (skill == "HTML")
    {
        cout << "Recommended: Web Development job \n"
             << endl;
    }

    else if (skill == "CSS")
    {
        cout << "Recommended: web Development job \n"
             << endl;
    }

    else
    {
        cout << "No matching opportunity found \n"
             << endl;
    }
}

void interestRecommendation()
{
    string interest;

    cout << "\n Enter your interset coding, web, AI" << endl;
    cin >> interest;

    if (interest == "coding")
    {
        cout << "Recommended: DSA Hackathon\n"
             << endl;
        cout << "Recommended: coding\n"
             << endl;
    }
    else if (interest == "web")
    {
        cout << "Recommended: DSA  hackathon\n"
             << endl;
        cout << "Recommended: HTML Workshop\n"
             << endl;
    }
    else if (interest == "AI")
    {
        cout << "Recommended : AI Internship\n"
             << endl;
        cout << "Recommended : AI Workshop\n"
             << endl;
    }
    else
    {
        cout << "No matching recommended found\n"
             << endl;
    }
}

void attendenceReminder()
{
    int total, attended;
    float percentage;

    cout << "\n----Attendence Reminder ---\n"
         << endl;
    cout << "Enter a total classes:" << endl;
    cin >> total;
    cout << "Enter attended class:" << endl;
    cin >> attended;

    if (total <= 0 || attended < 0 || attended > total)
    {
        cout << "Invalid attendance data\n"
             << endl;
        return;
    }

    percentage = (attended * 100.0) / total;

    cout << "Attendance " << percentage << "%\n";

    if (percentage < 75)
    {
        cout << "Warning: Attendence is below 75%\n"
             << endl;
    }
    else
    {
        cout << "your attendence is sufficient \n";
    }
}

void collegeProgram()
{
    cout << "\n---College  Programs ----\n"
         << endl;

    cout << "1. Technical Workshop\n";
    cout << "2. Cultural fest \n";
    cout << "3. Coding Competition\n";
    cout << "4. Hackathon\n";
    cout << "5. AI workshop\n";
}

struct opportunity
{
    string title;
    string skill;
};

vector<opportunity> opportunities =
    {
        {"C++ Intership", "C++"},
        {"DSA Hackathon", "DSA"},
        {"Web Workshop", "CSS"},
        {"AI Intership", "AI"},
};

void searchOpportunity()
{
    string skill;
    bool found = false;

    cout << "\n Enter skill to search:" << endl;
    cin >> skill;
    for (int i = 0; i < opportunities.size(); i++)
    {
        if (opportunities[i].skill == skill)
        {
            cout << "Found :" << opportunities[i].title << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No opportunity found\n"
             << endl;
}

void searchNotification()
{
    int id;

    cout << "\n Enter notification ID:" << endl;
    cin >> id;

    auto it = notificationData.find(id);

    if (it != notificationData.end())
    {
        cout << "Found :" << it->second.message << endl;
    }
    else
    {
        cout << "Notification not found\n"
             << endl;
    }
}

void showCategories()
{
    cout << "\n -----Notification Categories----\n"
         << endl;

    if (categories.empty())
    {
        cout << "First Select Notification from the menu\n"
             << endl;
        return;
    }
    for (string category : categories)
    {
        cout << category << endl;
    }
}

class Opportunity
{
public:
    int id;
    string title, type, deadline, skill;

    void input()
    {
        cout << "Enter Opportunity ID: ";
        cin >> id;

        cout << "Enter Title: ";
        getline(cin >> ws, title);

        cout << "Enter Type (Job/Internship/Hackathon/Quiz): ";
        cin >> type;

        cout << "Enter Required Skill/Criteria: ";
        getline(cin >> ws, skill);

        cout << "Enter Deadline (YYYYMMDD): ";
        cin >> deadline;
    }

    void display()
    {
        cout << "\nOpportunity ID: " << id;
        cout << "\nTitle: " << title;
        cout << "\nType: " << type;
        cout << "\nRequired Skill/Criteria: " << skill;
        cout << "\nDeadline: " << deadline << "\n";
    }
};

class OpportunityHub
{
    vector<Opportunity> opportunities;
    string filename = "opportunities.txt";

    void merge(vector<Opportunity> &arr, int left, int mid, int right)
    {
        vector<Opportunity> temp;

        int i = left;
        int j = mid + 1;

        while (i <= mid && j <= right)
        {
            if (arr[i].deadline <= arr[j].deadline)
                temp.push_back(arr[i++]);
            else
                temp.push_back(arr[j++]);
        }

        while (i <= mid)
            temp.push_back(arr[i++]);

        while (j <= right)
            temp.push_back(arr[j++]);

        for (int k = 0; k < (int)temp.size(); k++)
            arr[left + k] = temp[k];
    }

    void mergeSort(vector<Opportunity> &arr, int left, int right)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }

public:
    OpportunityHub()
    {
        loadFromFile();
    }

    void loadFromFile()
    {
        opportunities.clear();

        ifstream file(filename.c_str());

        if (!file)
        {
            cout << "Unable to open opportunities.txt\n";
            return;
        }

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string field;

            Opportunity o;

            getline(ss, field, '|');
            o.id = atoi(field.c_str());

            getline(ss, o.title, '|');
            getline(ss, o.type, '|');
            getline(ss, o.skill, '|');
            getline(ss, o.deadline);

            opportunities.push_back(o);
        }

        file.close();

        cout << opportunities.size()
             << " Opportunities Loaded Successfully!\n";
    }

    void saveToFile()
    {
        ofstream file(filename.c_str());

        if (!file)
        {
            cout << "Error Saving File!\n";
            return;
        }

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            file << opportunities[i].id << "|"
                 << opportunities[i].title << "|"
                 << opportunities[i].type << "|"
                 << opportunities[i].skill << "|"
                 << opportunities[i].deadline << "\n";
        }

        file.close();
    }

    void addOpportunity()
    {
        Opportunity o;
        o.input();

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].id == o.id)
            {
                cout << "ID Already Exists!\n";
                return;
            }
        }

        opportunities.push_back(o);
        saveToFile();

        cout << "Opportunity Added Successfully!\n";
    }

    void displayOpportunities()
    {
        if (opportunities.empty())
        {
            cout << "No Opportunities Available.\n";
            return;
        }

        for (int i = 0; i < (int)opportunities.size(); i++)
            opportunities[i].display();
    }

    void searchOpportunity()
    {
        int id;

        cout << "Enter Opportunity ID: ";
        cin >> id;

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].id == id)
            {
                cout << "Opportunity Found!\n";
                opportunities[i].display();
                return;
            }
        }

        cout << "Opportunity Not Found.\n";
    }

    void deleteOpportunity()
    {
        int id;

        cout << "Enter Opportunity ID to Delete: ";
        cin >> id;

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].id == id)
            {
                opportunities.erase(opportunities.begin() + i);
                saveToFile();

                cout << "Opportunity Deleted Successfully!\n";
                return;
            }
        }

        cout << "Opportunity Not Found.\n";
    }

    void updateOpportunity()
    {
        int id;

        cout << "Enter Opportunity ID to Update: ";
        cin >> id;

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].id == id)
            {
                cout << "Enter New Details:\n";

                opportunities[i].input();
                saveToFile();

                cout << "Opportunity Updated Successfully!\n";
                return;
            }
        }

        cout << "Opportunity Not Found.\n";
    }

    void sortByDeadline()
    {
        if (!opportunities.empty())
            mergeSort(opportunities, 0, (int)opportunities.size() - 1);

        cout << "Sorted Using Merge Sort!\n";

        displayOpportunities();
    }

    void filterByType()
    {
        string type;
        int found = 0;

        cout << "Enter Type to Filter: ";
        cin >> type;

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].type == type)
            {
                opportunities[i].display();
                found = 1;
            }
        }

        if (!found)
            cout << "No Matching Opportunities Found.\n";
    }

    void searchBySkill()
    {
        string skill;
        int found = 0;

        cout << "Enter Skill/Criteria to Search: ";
        getline(cin >> ws, skill);

        for (int i = 0; i < (int)opportunities.size(); i++)
        {
            if (opportunities[i].skill.find(skill) != string::npos)
            {
                opportunities[i].display();
                found = 1;
            }
        }

        if (!found)
            cout << "No Opportunities Found.\n";
    }

    void menu()
    {
        int choice;

        do
        {
            cout << "\n" << CYAN << BOLD << "===== CAMPUS CONNECT : OPPORTUNITY HUB =====" << RESET << "\n";
            cout << "       OPPORTUNITY HUB\n";
            cout << "1. Add Opportunity\n";
            cout << "2. Display All Opportunities\n";
            cout << "3. Search by ID\n";
            cout << "4. Delete Opportunity\n";
            cout << "5. Update Opportunity\n";
            cout << "6. Sort by Deadline (Merge Sort)\n";
            cout << "7. Filter by Type\n";
            cout << "8. Search by Skill\n";
            cout << "0. Exit\n";

            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                addOpportunity();
                break;

            case 2:
                displayOpportunities();
                break;

            case 3:
                searchOpportunity();
                break;

            case 4:
                deleteOpportunity();
                break;

            case 5:
                updateOpportunity();
                break;

            case 6:
                sortByDeadline();
                break;

            case 7:
                filterByType();
                break;

            case 8:
                searchBySkill();
                break;

            case 0:
                cout << "Exiting Opportunity Hub.\n";
                break;

            default:
                cout << "Invalid Choice!\n";
            }

        } while (choice != 0);
    }
};

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

string lowerText(string s)
{
    for (int i = 0; i < (int)s.size(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

string readText(string message)
{
    string s;
    cout << message;
    getline(cin >> ws, s);
    return s;
}

int readNumber(string message, int low, int high)
{
    int n;
    while (true)
    {
        cout << message;
        if (cin >> n && n >= low && n <= high)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return n;
        }
        cout << "Invalid input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readHours(string message)
{
    double h;
    while (true)
    {
        cout << message;
        if (cin >> h && h >= 0)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return h;
        }
        cout << "Enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool validDate(string date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit((unsigned char)date[i]))
            return false;
    }

    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    return true;
}

struct Task
{
    int id;
    string type;       
    string subject;
    string topic;
    string date;       
    double targetHours;
    double hoursDone;
    bool completed;
};

class TaskPriorityQueue
{
private:
    vector<Task> heap;

    bool higherPriority(Task a, Task b)
    {
        if(a.date != b.date)
            return a.date < b.date;

        return a.id < b.id;
    }

    void heapifyUp(int i)
    {
        while(i > 0)
        {
            int parent = (i - 1) / 2;

            if(higherPriority(heap[i], heap[parent]))
            {
                swap(heap[i], heap[parent]);
                i = parent;
            }
            else
                break;
        }
    }

    void heapifyDown(int i)
    {
        int n = heap.size();

        while(true)
        {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if(left < n &&
               higherPriority(heap[left], heap[smallest]))
                smallest = left;

            if(right < n &&
               higherPriority(heap[right], heap[smallest]))
                smallest = right;

            if(smallest != i)
            {
                swap(heap[i], heap[smallest]);
                i = smallest;
            }
            else
                break;
        }
    }

public:
    void push(Task t)
    {
        if(t.completed)
            return;

        heap.push_back(t);
        heapifyUp(heap.size() - 1);
    }

    Task top()
    {
        if(heap.empty())
        {
            cout << "Priority Queue is empty.\n";
            return Task{};
        }

        return heap[0];
    }

    void pop()
    {
        if(heap.empty())
            return;

        heap[0] = heap.back();
        heap.pop_back();

        if(!heap.empty())
            heapifyDown(0);
    }

    bool empty()
    {
        return heap.empty();
    }

    int size()
    {
        return heap.size();
    }

    void display()
    {
        if(heap.empty())
        {
            cout << "No pending tasks.\n";
            return;
        }

        TaskPriorityQueue temp = *this;

        cout << "\nTasks in priority order:\n";

        while(!temp.empty())
        {
            Task t = temp.top();

            cout << "ID: " << t.id
                 << " | Subject: " << t.subject
                 << " | Topic: " << t.topic
                 << " | Deadline: " << t.date << "\n";

            temp.pop();
        }
    }
};

class StudyPlanner
{
private:
    vector<Task> tasks;
    vector<int> pending;
    int nextID;
    string fileName;

    int findIndex(int id)
    {
        for (int i = 0; i < (int)tasks.size(); i++)
            if (tasks[i].id == id)
                return i;
        return -1;
    }

    void saveToFile()
    {
        ofstream out(fileName);
        if (!out)
        {
            cout << "Could not save tasks to file.\n";
            return;
        }

        for (int i = 0; i < (int)tasks.size(); i++)
        {
            out << tasks[i].id << '|'
                << tasks[i].type << '|'
                << tasks[i].subject << '|'
                << tasks[i].topic << '|'
                << tasks[i].date << '|'
                << tasks[i].targetHours << '|'
                << tasks[i].hoursDone << '|'
                << tasks[i].completed << '\n';
        }
        out.close();
    }

 void showPriorityTasks()
{
    TaskPriorityQueue pq;

    for(int i = 0; i < (int)tasks.size(); i++)
    {
        if(!tasks[i].completed)
            pq.push(tasks[i]);
    }

    pq.display();
}

    void loadFromFile()
    {
        tasks.clear();
        pending.clear();
        nextID = 1;

        ifstream in(fileName);
        if (!in)
            return; 

        string line;
        while (getline(in, line))
        {
            string fields[8];
            int field = 0;
            string value;
            for (int i = 0; i <= (int)line.size(); i++)
            {
                if (i == (int)line.size() || line[i] == '|')
                {
                    if (field < 8)
                        fields[field] = value;
                    field++;
                    value.clear();
                }
                else
                    value += line[i];
            }

            if (field < 8)
                continue;

            try
            {
                Task t;
                t.id = stoi(fields[0]);
                t.type = fields[1];
                t.subject = fields[2];
                t.topic = fields[3];
                t.date = fields[4];
                t.targetHours = stod(fields[5]);
                t.hoursDone = stod(fields[6]);
                t.completed = (fields[7] == "1");

                tasks.push_back(t);
                if (!t.completed)
                    pending.push_back(t.id);

                if (t.id >= nextID)
                    nextID = t.id + 1;
            }
            catch (...)
            {
                cout << "Skipping invalid task record.\n";
            }
        }
        in.close();
    }

    void printTask(const Task &t)
    {
        cout << "\nID: " << t.id << "\n";
        cout << "Type: " << t.type << "\n";
        cout << "Subject: " << t.subject << "\n";
        cout << "Topic: " << t.topic << "\n";
        cout << "Date: " << t.date << "\n";
        cout << "Target hours: " << t.targetHours << "\n";
        cout << "Hours completed: " << t.hoursDone << "\n";
        cout << "Hours remaining: "
             << (t.targetHours > t.hoursDone ? t.targetHours - t.hoursDone : 0)
             << "\n";
        cout << "Status: " << (t.completed ? "Completed" : "Pending") << "\n";
    }

    void addTask(string type)
    {
        Task t;
        t.id = nextID++;
        t.type = type;
        t.subject = readText("Enter subject: ");
        t.topic = readText("Enter topic: ");

        do
        {
            t.date = readText("Enter date (YYYY-MM-DD): ");
            if (!validDate(t.date))
                cout << "Invalid date format or range.\n";
        } while (!validDate(t.date));

        t.targetHours = readHours("Enter target study hours: ");
        t.hoursDone = 0;
        t.completed = false;

        tasks.push_back(t);
        pending.push_back(t.id);
        saveToFile();

        cout << "Task added. ID: " << t.id << "\n";
    }

    void removePendingID(int id)
    {
        for (int i = 0; i < (int)pending.size(); i++)
        {
            if (pending[i] == id)
            {
                pending.erase(pending.begin() + i);
                return;
            }
        }
    }

    void finishTask(Task &t)
    {
        t.completed = true;
        removePendingID(t.id);
        saveToFile();
        cout << "Task marked as completed.\n";
    }

public:
    StudyPlanner(string file = "study_tasks.txt")
    {
        fileName = file;
        nextID = 1;
        loadFromFile();
    }

    void displayAll()
    {
        if (tasks.empty())
        {
            cout << "No tasks available.\n";
            return;
        }

        for (int i = 0; i < (int)tasks.size(); i++)
            printTask(tasks[i]);
    }

    void displayByType()
    {
        cout << "1. Daily tasks\n2. Weekly tasks\n";
        int choice = readNumber("Choose: ", 1, 2);
        string type = (choice == 1 ? "Daily" : "Weekly");
        bool found = false;

        for (int i = 0; i < (int)tasks.size(); i++)
        {
            if (tasks[i].type == type)
            {
                printTask(tasks[i]);
                found = true;
            }
        }

        if (!found)
            cout << "No tasks of this type.\n";
    }

    void addDailyTask()
    {
        addTask("Daily");
    }

    void addWeeklyTask()
    {
        addTask("Weekly");
    }

    void sortTasks()
    {
        if (tasks.empty())
        {
            cout << "No tasks available.\n";
            return;
        }

        cout << "Sort by:\n1. Date\n2. Hours remaining\n3. Subject\n";
        int choice = readNumber("Choose: ", 1, 3);
        for (int i = 0; i < (int)tasks.size() - 1; i++)
        {
            for (int j = 0; j < (int)tasks.size() - i - 1; j++)
            {
                bool swapNeeded = false;

                if (choice == 1 && tasks[j].date > tasks[j + 1].date)
                    swapNeeded = true;
                else if (choice == 2)
                {
                    double left = tasks[j].targetHours - tasks[j].hoursDone;
                    double right = tasks[j + 1].targetHours - tasks[j + 1].hoursDone;
                    if (left > right)
                        swapNeeded = true;
                }
                else if (choice == 3 &&
                         lowerText(tasks[j].subject) >
                         lowerText(tasks[j + 1].subject))
                    swapNeeded = true;

                if (swapNeeded)
                {
                    Task temp = tasks[j];
                    tasks[j] = tasks[j + 1];
                    tasks[j + 1] = temp;
                }
            }
        }

        displayAll();
    }

    void markCompleted()
    {
        int id = readNumber("Enter task ID to mark completed: ", 1, 1000000);
        int index = findIndex(id);

        if (index == -1)
        {
            cout << "Task not found.\n";
            return;
        }

        finishTask(tasks[index]);
    }

    void logHours()
    {
        int id = readNumber("Enter task ID: ", 1, 1000000);
        int index = findIndex(id);

        if (index == -1)
        {
            cout << "Task not found.\n";
            return;
        }

        Task &t = tasks[index];

        if (t.completed)
        {
            cout << "This task is already completed.\n";
            return;
        }

        double hours = readHours("Enter hours studied now: ");
        t.hoursDone += hours;

        if (t.hoursDone >= t.targetHours)
            finishTask(t);
        else
            saveToFile();

        cout << "Updated hours: " << t.hoursDone << "\n";
    }

    void showReport()
    {
        double totalTarget = 0;
        double totalDone = 0;
        int completedCount = 0;

        for (int i = 0; i < (int)tasks.size(); i++)
        {
            totalTarget += tasks[i].targetHours;
            totalDone += tasks[i].hoursDone;
            if (tasks[i].completed)
                completedCount++;
        }

        cout << "\n========== STUDY REPORT ==========\n";
        cout << "Total tasks: " << tasks.size() << "\n";
        cout << "Completed tasks: " << completedCount << "\n";
        cout << "Pending tasks: " << tasks.size() - completedCount << "\n";
        cout << fixed << setprecision(2);
        cout << "Target hours: " << totalTarget << "\n";
        cout << "Hours studied: " << totalDone << "\n";
        cout << "Hours remaining: "
             << (totalTarget > totalDone ? totalTarget - totalDone : 0)
             << "\n";
    }

    void showPendingQueue()
    {
        if (pending.empty())
        {
            cout << "No pending tasks in the queue.\n";
            return;
        }

        cout << "Pending task IDs (next task first):\n";
        for (int i = 0; i < (int)pending.size(); i++)
            cout << pending[i] << "\n";
    }

    void processNextTask()
    {
        while (!pending.empty())
        {
            int id = pending.front();
            pending.erase(pending.begin());

            int index = findIndex(id);
            if (index == -1 || tasks[index].completed)
                continue;

            Task &t = tasks[index];
            printTask(t);

            double hours = readHours("Enter hours studied in this session: ");
            t.hoursDone += hours;

            if (t.hoursDone >= t.targetHours)
            {
                t.completed = true;
                cout << "Target reached. Task completed.\n";
            }
            else
            {
                pending.push_back(t.id);
                cout << "Task is still pending and moved to the end of the queue.\n";
            }

            saveToFile();
            return;
        }

        cout << "No pending tasks to process.\n";
    }

    void menu()
    {
        while (true)
        {
            cout << "\n" << GREEN << BOLD << "========== STUDY PLANNER ==========" << RESET << "\n";
            cout << "1. Add daily task\n";
            cout << "2. Add weekly task\n";
            cout << "3. Display all tasks\n";
            cout << "4. Display tasks by type\n";
            cout << "5. Sort tasks\n";
            cout << "6. Mark task completed\n";
            cout << "7. Log study hours\n";
            cout << "8. Study report\n";
            cout << "9. Show pending queue\n";
            cout << "10. Process next task\n";
        cout << "11. Show priority tasks\n";
            cout << "0. Exit planner\n";

            int choice = readNumber("Enter choice: ", 0, 11);

            if (choice == 1) addDailyTask();
            else if (choice == 2) addWeeklyTask();
            else if (choice == 3) displayAll();
            else if (choice == 4) displayByType();
            else if (choice == 5) sortTasks();
            else if (choice == 6) markCompleted();
            else if (choice == 7) logHours();
            else if (choice == 8) showReport();
            else if (choice == 9) showPendingQueue();
            else if (choice == 10) processNextTask();
             else if(choice == 11) showPriorityTasks();
            else return;
        }
    }
};

string toLowerCase(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);

    return s;
}

struct Tip
{
    int id;
    string subject;
    string category;
    string keywords;
    string text;
};

class RecentStack
{
private:
    vector<int> ids;
    int capacity;

public:
    RecentStack()
    {
        capacity = 5;
    }

    bool isEmpty()
    {
        return ids.empty();
    }

    void push(int id)
    {
        if ((int)ids.size() == capacity)
            ids.erase(ids.begin());

        ids.push_back(id);
    }

    bool pop(int &id)
    {
        if (ids.empty())
            return false;

        id = ids.back();
        ids.pop_back();

        return true;
    }

    bool peek(int &id)
    {
        if (ids.empty())
            return false;

        id = ids.back();
        return true;
    }

    void display()
    {
        for (int i = (int)ids.size() - 1; i >= 0; i--)
            cout << "Tip ID: " << ids[i] << "\n";
    }
};

class StudyTips
{
private:
    vector<Tip> tips;
    RecentStack recent;
    string fileName;

    int findTipIndex(int id)
    {
        for (int i = 0; i < (int)tips.size(); i++)
        {
            if (tips[i].id == id)
                return i;
        }

        return -1;
    }

    void printTip(const Tip &t, bool addToHistory = true)
    {
        cout << "\n[" << t.id << "] "
             << "(" << t.subject << " - "
             << t.category << ")\n";

        cout << t.text << "\n";

        if (addToHistory)
            recent.push(t.id);
    }

public:
    StudyTips(string file = "tips.txt")
    {
        fileName = file;
        loadFromFile();
    }

    void loadFromFile()
    {
        tips.clear();

        ifstream input(fileName);

        if (!input)
        {
            cout << "Warning: Could not open "
                 << fileName << ".\n";
            return;
        }

        string line;

        while (getline(input, line))
        {
            string fields[5];
            stringstream ss(line);
            bool valid = true;

            for (int i = 0; i < 5; i++)
            {
                if (!getline(ss, fields[i], '|'))
                {
                    valid = false;
                    break;
                }
            }

            if (!valid || fields[4].empty())
                continue;

            try
            {
                Tip t;

                t.id = stoi(fields[0]);
                t.subject = fields[1];
                t.category = fields[2];
                t.keywords = toLowerCase(fields[3]);
                t.text = fields[4];

                tips.push_back(t);
            }
            catch (...)
            {
                cout << "Skipping an invalid line.\n";
            }
        }

        input.close();

        cout << tips.size() << " tips loaded.\n";
    }

    void displayAll()
    {
        if (tips.empty())
        {
            cout << "No tips available.\n";
            return;
        }

        for (int i = 0; i < (int)tips.size(); i++)
            printTip(tips[i]);
    }

    void filterBySubject()
    {
        vector<string> subjects;

        for (int i = 0; i < (int)tips.size(); i++)
        {
            bool found = false;

            for (int j = 0; j < (int)subjects.size(); j++)
            {
                if (subjects[j] == tips[i].subject)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
                subjects.push_back(tips[i].subject);
        }

        if (subjects.empty())
        {
            cout << "No tips available.\n";
            return;
        }

        cout << "\nSubjects:\n";

        for (int i = 0; i < (int)subjects.size(); i++)
            cout << i + 1 << ". " << subjects[i] << "\n";

        int choice = readNumber(
            "Choose subject: ", 1, (int)subjects.size());

        string selected = subjects[choice - 1];

        for (int i = 0; i < (int)tips.size(); i++)
        {
            if (tips[i].subject == selected)
                printTip(tips[i]);
        }
    }

    void filterByCategory()
    {
        string categories[4] = {
            "Concept", "Practice", "Revision", "Productivity"};

        cout << "\nCategories:\n";

        for (int i = 0; i < 4; i++)
            cout << i + 1 << ". " << categories[i] << "\n";

        int choice = readNumber("Choose category: ", 1, 4);

        string selected = categories[choice - 1];
        bool found = false;

        for (int i = 0; i < (int)tips.size(); i++)
        {
            if (tips[i].category == selected)
            {
                printTip(tips[i]);
                found = true;
            }
        }

        if (!found)
            cout << "No tips in this category.\n";
    }

    void searchByKeyword()
    {
        string keyword = toLowerCase(
            readText("Enter a keyword: "));

        bool found = false;

        for (int i = 0; i < (int)tips.size(); i++)
        {
            if (tips[i].keywords.find(keyword) != string::npos ||
                toLowerCase(tips[i].text).find(keyword) != string::npos)
            {
                printTip(tips[i]);
                found = true;
            }
        }

        if (!found)
            cout << "No tips found for that keyword.\n";
    }

    void viewTip()
    {
        int id = readNumber("Enter tip ID: ", 1, 100000);

        int index = findTipIndex(id);

        if (index == -1)
        {
            cout << "Tip not found.\n";
            return;
        }

        printTip(tips[index]);
    }

    void showRecent()
    {
        if (recent.isEmpty())
        {
            cout << "No recently viewed tips.\n";
            return;
        }

        cout << "\nRecently viewed (latest first):\n";
        recent.display();
    }

    void goBack()
    {
        int currentID;
        int previousID;

        if (!recent.pop(currentID))
        {
            cout << "No recent tip history.\n";
            return;
        }

        if (!recent.peek(previousID))
        {
            cout << "No earlier tip.\n";
            return;
        }

        int index = findTipIndex(previousID);

        if (index != -1)
        {
            cout << "\nPrevious tip:\n";

            printTip(tips[index], false);
        }
    }

    void menu()
    {
        while (true)
        {
            cout << "\n" << MAGENTA << BOLD << "===== STUDY TIPS ("
                 << tips.size() << " loaded) =====" << RESET << "\n";

            cout << "1. Display all tips\n";
            cout << "2. Filter by subject\n";
            cout << "3. Filter by category\n";
            cout << "4. Search by keyword\n";
            cout << "5. Open tip by ID\n";
            cout << "6. Recently viewed tips\n";
            cout << "7. Go back to previous tip\n";
            cout << "8. Reload tips from file\n";
            cout << "0. Back\n";

            int choice = readNumber("Choice: ", 0, 8);

            if (choice == 1)
                displayAll();

            else if (choice == 2)
                filterBySubject();

            else if (choice == 3)
                filterByCategory();

            else if (choice == 4)
                searchByKeyword();

            else if (choice == 5)
                viewTip();

            else if (choice == 6)
                showRecent();

            else if (choice == 7)
                goBack();

            else if (choice == 8)
                loadFromFile();

            else
                return;
        }
    }
};


// Notes and Resources submenu.
void notesMenu()
{
    int choice;

    do
    {
        cout << "\n" << BLUE << BOLD << "========== NOTES & RESOURCES ==========" << RESET << "\n";
        cout << "1. Create Note\n";
        cout << "2. Display Notes\n";
        cout << "3. Search Note by ID\n";
        cout << "4. Search Notes by Subject\n";
        cout << "5. Edit Note\n";
        cout << "6. Delete Note\n";
        cout << "7. Sort Notes\n";
        cout << "8. Add Resource\n";
        cout << "9. Display Resources\n";
        cout << "0. Back to Dashboard\n";
        cout << YELLOW << "Enter choice: " << RESET;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input!\n" << RESET;
            continue;
        }

        switch (choice)
        {
        case 1: createNote(); break;
        case 2: displayNotes(); break;
        case 3: searchNote(); break;
        case 4: searchSubject(); break;
        case 5: editNote(); break;
        case 6: deleteNote(); break;
        case 7: sortNotes(); break;
        case 8: addResource(); break;
        case 9: displayResources(); break;
        case 0: break;
        default: cout << RED << "Invalid choice!\n" << RESET;
        }

    } while (choice != 0);
}

// Notifications submenu.
void notificationMenu()
{
    int choice;

    do
    {
        cout << "\n" << YELLOW << BOLD << "========== NOTIFICATIONS ==========" << RESET << "\n";
        cout << "1. View Notifications\n";
        cout << "2. Skill Based Recommendation\n";
        cout << "3. Interest Based Recommendation\n";
        cout << "4. Attendance Reminder\n";
        cout << "5. College Programs\n";
        cout << "6. Search Opportunities\n";
        cout << "7. Search Notification by ID\n";
        cout << "8. Show Notification Categories\n";
        cout << "0. Back to Dashboard\n";
        cout << YELLOW << "Enter choice: " << RESET;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input!\n" << RESET;
            continue;
        }

        switch (choice)
        {
        case 1: notification(); break;
        case 2: recommendation(); break;
        case 3: interestRecommendation(); break;
        case 4: attendenceReminder(); break;
        case 5: collegeProgram(); break;
        case 6: searchOpportunity(); break;
        case 7: searchNotification(); break;
        case 8: showCategories(); break;
        case 0: break;
        default: cout << RED << "Invalid choice!\n" << RESET;
        }

    } while (choice != 0);
}

int main()
{
    StudentManager manager;
    OpportunityHub opportunityHub;
    StudyPlanner planner("study_tasks.txt");
    StudyTips studyTips("tips.txt");

    // Load the saved notes and resources once when the program starts.
    loadData();

    string loggedInID;
    int choice;

    do
    {
        cout << "\n" << CYAN << BOLD;
        cout << "============================================\n";
        cout << "              CAMPUS CONNECT                \n";
        cout << "       Student Academic Management          \n";
        cout << "============================================\n" << RESET;

        cout << GREEN << "1. Register\n" << RESET;
        cout << BLUE << "2. Login\n" << RESET;
        cout << RED << "0. Exit\n" << RESET;
        cout << YELLOW << "Enter choice: " << RESET;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input!\n" << RESET;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            manager.registerStudent();
        }
        else if (choice == 2)
        {
            if (manager.login(loggedInID))
            {
                int option;

                do
                {
                    cout << "\n" << CYAN << BOLD;
                    cout << "============================================\n";
                    cout << "              STUDENT DASHBOARD             \n";
                    cout << "============================================\n" << RESET;

                    cout << "1. View Student Profile\n";
                    cout << "2. Update Student Profile\n";
                    cout << "3. Opportunity Hub\n";
                    cout << "4. Study Planner\n";
                    cout << "5. Study Tips\n";
                    cout << "6. Notes & Resources\n";
                    cout << "7. Notifications & Recommendations\n";
                    cout << "8. Logout\n";
                    cout << YELLOW << "Enter choice: " << RESET;

                    if (!(cin >> option))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << RED << "Invalid input!\n" << RESET;
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
                        opportunityHub.menu();
                        break;

                    case 4:
                        planner.menu();
                        break;

                    case 5:
                        studyTips.menu();
                        break;

                    case 6:
                        notesMenu();
                        break;

                    case 7:
                        notificationMenu();
                        break;

                    case 8:
                        cout << GREEN << "Logged out successfully.\n" << RESET;
                        break;

                    default:
                        cout << RED << "Invalid choice!\n" << RESET;
                    }

                } while (option != 8);
            }
        }
        else if (choice == 0)
        {
            cout << GREEN << "\nThank you for using Campus Connect!\n" << RESET;
        }
        else
        {
            cout << RED << "Invalid choice!\n" << RESET;
        }

    } while (choice != 0);

    return 0;
}
