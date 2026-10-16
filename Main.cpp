#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
using namespace std;
string readText(const string &msg)
{
    string s;
    do
    {
        cout << msg;
        if (!getline(cin, s))
        {
            cout << "\nInput ended.\n";
            exit(0);
        }
    } while (s.empty());
    return s;
}

double readNumber(const string &msg, double lo, double hi)
{
    while (true)
    {
        string s = readText(msg);
        stringstream ss(s);
        double v;
        char extra;
        if (ss >> v && !(ss >> extra) && v >= lo && v <= hi)
            return v;
        cout << "Enter a value between " << lo << " and " << hi << ".\n";
    }
}

string toLower(string s)
{
    for (char &c : s)
        c = tolower((unsigned char)c);
    return s;
}

// harsh verma work
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

        cout << "Enter Required Skill: ";
        getline(cin >> ws, skill);

        cout << "Enter Deadline (YYYYMMDD): ";
        cin >> deadline;
    }

    void display()
    {
        cout << "\nOpportunity ID: " << id;
        cout << "\nTitle: " << title;
        cout << "\nType: " << type;
        cout << "\nRequired Skill: " << skill;
        cout << "\nDeadline: " << deadline << "\n";
    }
};

class OpportunityHub
{
    vector<Opportunity> opportunities;

public:
    void addOpportunity()
    {
        Opportunity o;
        o.input();

        for (int i = 0; i < opportunities.size(); i++)
        {
            if (opportunities[i].id == o.id)
            {
                cout << "ID already exists!\n";
                return;
            }
        }

        opportunities.push_back(o);
        cout << "Opportunity Added Successfully!\n";
    }

    void displayOpportunities()
    {
        if (opportunities.empty())
        {
            cout << "No Opportunities Available.\n";
            return;
        }

        for (int i = 0; i < opportunities.size(); i++)
        {
            opportunities[i].display();
        }
    }

    void searchOpportunity()
    {
        int id;
        cout << "Enter Opportunity ID: ";
        cin >> id;

        for (int i = 0; i < opportunities.size(); i++)
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

        for (int i = 0; i < opportunities.size(); i++)
        {
            if (opportunities[i].id == id)
            {
                opportunities.erase(opportunities.begin() + i);
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

        for (int i = 0; i < opportunities.size(); i++)
        {
            if (opportunities[i].id == id)
            {
                cout << "Enter New Details:\n";
                opportunities[i].input();
                cout << "Opportunity Updated Successfully!\n";
                return;
            }
        }

        cout << "Opportunity Not Found.\n";
    }

    void sortByDeadline()
    {
        sort(opportunities.begin(), opportunities.end(),
             [](Opportunity a, Opportunity b)
             {
                 return a.deadline < b.deadline;
             });

        cout << "Opportunities Sorted by Deadline.\n";
        displayOpportunities();
    }

    void filterByType()
    {
        string type;
        int found = 0;

        cout << "Enter Type to Filter: ";
        cin >> type;

        for (int i = 0; i < opportunities.size(); i++)
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

        cout << "Enter Skill to Search: ";
        getline(cin >> ws, skill);

        for (int i = 0; i < opportunities.size(); i++)
        {
            if (opportunities[i].skill == skill)
            {
                opportunities[i].display();
                found = 1;
            }
        }

        if (!found)
            cout << "No Opportunities Found for This Skill.\n";
    }

    void menu()
    {
        int choice;

        do
        {
            cout << "\n===== CAMPUS CONNECT =====\n";
            cout << "       OPPORTUNITY HUB\n";
            cout << "1. Add Opportunity\n";
            cout << "2. Display All Opportunities\n";
            cout << "3. Search by ID\n";
            cout << "4. Delete Opportunity\n";
            cout << "5. Update Opportunity\n";
            cout << "6. Sort by Deadline\n";
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

class RecentStack
{
    static const int CAPACITY = 5;
    int arr[CAPACITY];
    int top;

public:
    RecentStack() : top(-1) {}
    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == CAPACITY - 1; }

    void push(int tipId)
    {

        for (int i = 0; i <= top; i++)
            if (arr[i] == tipId)
            {
                for (int j = i; j < top; j++)
                    arr[j] = arr[j + 1];
                top--;
                break;
            }

        if (isFull())
        {
            for (int j = 0; j < top; j++)
                arr[j] = arr[j + 1];
            top--;
        }
        arr[++top] = tipId;
    }

    bool pop(int &tipId)
    {
        if (isEmpty())
            return false;
        tipId = arr[top--];
        return true;
    }

    int size() const { return top + 1; }
    int peekAt(int fromTop) const { return arr[top - fromTop]; }
};

struct Tip
{
    int id;
    string subject;
    string category;
    string keywords;
    string text;
};

class StudyTips
{
    vector<Tip> tips;
    RecentStack recent;
    string fileName;

    void printTip(const Tip &t) const
    {
        cout << "[" << t.id << "] (" << t.subject << " - " << t.category << ")\n    "
             << t.text << "\n";
    }

    const Tip *findTip(int id) const
    {
        for (const Tip &t : tips)
            if (t.id == id)
                return &t;
        return nullptr;
    }

    string normalizeSubject(const string &subject) const
    {
        string s = toLower(subject);
        if (s.find("dsa") != string::npos || s.find("data struct") != string::npos || s == "ds")
            return "dsa";
        if (s.find("oop") != string::npos || s.find("c++") != string::npos ||
            s.find("cpp") != string::npos || s.find("object") != string::npos)
            return "oop";
        return s;
    }

    bool keywordMatch(const Tip &t, const string &text) const
    {
        stringstream ss(t.keywords);
        string kw;
        while (getline(ss, kw, ','))
            if (!kw.empty() && text.find(kw) != string::npos)
                return true;
        return false;
    }

    vector<string> subjectList() const
    {
        vector<string> list;
        for (const Tip &t : tips)
            if (find(list.begin(), list.end(), t.subject) == list.end())
                list.push_back(t.subject);
        return list;
    }

public:
    StudyTips(const string &file = "tips.txt") : fileName(file) { loadFromFile(); }

    void loadFromFile()
    {
        tips.clear();
        ifstream in(fileName);
        if (!in)
        {
            cout << "Warning: " << fileName << " not found. No tips loaded.\n";
            return;
        }
        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string f[5];
            for (int i = 0; i < 5; i++)
                getline(ss, f[i], '|');
            try
            {
                if (f[4].empty())
                    continue;
                tips.push_back({stoi(f[0]), f[1], f[2], toLower(f[3]), f[4]});
            }
            catch (...)
            {
            }
        }
    }

    int count() const { return (int)tips.size(); }

    void displayAll() const
    {
        if (tips.empty())
        {
            cout << "No tips available.\n";
            return;
        }
        for (const Tip &t : tips)
            printTip(t);
    }

    void filterBySubject() const
    {
        vector<string> subs = subjectList();
        if (subs.empty())
        {
            cout << "No tips available.\n";
            return;
        }
        for (size_t i = 0; i < subs.size(); i++)
            cout << i + 1 << ". " << subs[i] << "\n";
        int c = (int)readNumber("Choose subject: ", 1, subs.size()) - 1;
        for (const Tip &t : tips)
            if (t.subject == subs[c])
                printTip(t);
    }

    void filterByCategory() const
    {
        const string cats[] = {"Concept", "Practice", "Revision", "Productivity"};
        for (int i = 0; i < 4; i++)
            cout << i + 1 << ". " << cats[i] << "\n";
        int c = (int)readNumber("Choose category: ", 1, 4) - 1;
        bool any = false;
        for (const Tip &t : tips)
            if (t.category == cats[c])
            {
                printTip(t);
                any = true;
            }
        if (!any)
            cout << "No tips in this category.\n";
    }

    void searchByKeyword() const
    {
        string key = toLower(readText("Keyword (e.g. stack, inheritance): "));
        bool any = false;
        for (const Tip &t : tips)
            if (t.keywords.find(key) != string::npos || toLower(t.text).find(key) != string::npos)
            {
                printTip(t);
                any = true;
            }
        if (!any)
            cout << "No tips found for \"" << key << "\".\n";
    }

    void viewTip()
    {
        const Tip *t = findTip((int)readNumber("Tip ID: ", 1, 100000));
        if (!t)
        {
            cout << "Tip not found.\n";
            return;
        }
        printTip(*t);
        recent.push(t->id);
    }

    void showRecent() const
    {
        if (recent.isEmpty())
        {
            cout << "No recently viewed tips.\n";
            return;
        }
        cout << "Recently viewed (latest first):\n";
        for (int i = 0; i < recent.size(); i++)
        {
            const Tip *t = findTip(recent.peekAt(i));
            if (t)
                printTip(*t);
        }
    }

    void goBack()
    {
        int id;
        if (!recent.pop(id))
        {
            cout << "Nothing to go back to.\n";
            return;
        }
        if (recent.isEmpty())
        {
            cout << "No earlier tip.\n";
            return;
        }
        cout << "Previous tip:\n";
        const Tip *t = findTip(recent.peekAt(0));
        if (t)
            printTip(*t);
    }

    void suggestForTask(const string &subject, const string &topic,
                        const string &event, int limit = 2)
    {
        string subj = normalizeSubject(subject);
        string text = toLower(topic);
        vector<pair<int, int>> scored;

        for (size_t i = 0; i < tips.size(); i++)
        {
            const Tip &t = tips[i];
            int score = 0;
            string ts = toLower(t.subject);
            if (ts == subj)
                score += 2;
            else if (ts == "general")
                score += 1;
            else
                continue;
            if (keywordMatch(t, text))
                score += 3;
            if (event == "done" && t.category == "Revision")
                score += 2;
            if (event == "new" && (t.category == "Concept" || t.category == "Practice"))
                score += 2;
            scored.push_back({score, (int)i});
        }
        stable_sort(scored.begin(), scored.end(),
                    [](const pair<int, int> &a, const pair<int, int> &b)
                    { return a.first > b.first; });

        if (scored.empty())
            return;
        cout << "\n--- Study tips for this " << (event == "done" ? "completed" : "new") << " task ---\n";
        for (int i = 0; i < (int)scored.size() && i < limit; i++)
        {
            printTip(tips[scored[i].second]);
            recent.push(tips[scored[i].second].id);
        }
    }

    void menu()
    {
        while (true)
        {
            cout << "\n===== STUDY TIPS (" << tips.size() << " loaded) =====\n"
                 << "1. Display all tips\n"
                 << "2. Filter by subject\n"
                 << "3. Filter by category\n"
                 << "4. Search by keyword\n"
                 << "5. Open a tip by ID\n"
                 << "6. Recently viewed tips (stack)\n"
                 << "7. Go back to previous tip (pop)\n"
                 << "8. Reload tips from file\n"
                 << "0. Back\n";
            switch ((int)readNumber("Choice: ", 0, 8))
            {
            case 1:
                displayAll();
                break;
            case 2:
                filterBySubject();
                break;
            case 3:
                filterByCategory();
                break;
            case 4:
                searchByKeyword();
                break;
            case 5:
                viewTip();
                break;
            case 6:
                showRecent();
                break;
            case 7:
                goBack();
                break;
            case 8:
                loadFromFile();
                cout << tips.size() << " tips loaded.\n";
                break;
            case 0:
                return;
            }
        }
    }
};
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

struct TaskNode
{
    Task data;
    TaskNode *next;
    TaskNode(const Task &t) : data(t), next(nullptr) {}
};

class StudyPlanner
{
    TaskNode *head;
    TaskNode *tail;
    int count;
    int nextId;
    string fileName;
    queue<int> pendingQueue;

    void appendTask(const Task &t)
    {
        TaskNode *node = new TaskNode(t);
        if (!head)
            head = tail = node;
        else
        {
            tail->next = node;
            tail = node;
        }
        count++;
    }
    void clearList()
    {
        while (head)
        {
            TaskNode *tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        count = 0;
    }

    bool validDate(const string &d)
    {
        int y, m, day;
        char c1, c2;
        stringstream ss(d);
        return (ss >> y >> c1 >> m >> c2 >> day) && c1 == '-' && c2 == '-' && m >= 1 && m <= 12 && day >= 1 && day <= 31;
    }
    string readDate(const string &msg)
    {
        while (true)
        {
            string d = readText(msg);
            if (validDate(d))
                return d;
            cout << "Use format YYYY-MM-DD.\n";
        }
    }
    Task *findTask(int id)
    {
        for (TaskNode *p = head; p; p = p->next)
            if (p->data.id == id)
                return &p->data;
        return nullptr;
    }

    void loadFromFile()
    {
        ifstream in(fileName);
        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string f[8];
            for (int i = 0; i < 8; i++)
                getline(ss, f[i], '|');
            try
            {
                Task t{stoi(f[0]), f[1], f[2], f[3], f[4], stod(f[5]), stod(f[6]), f[7] == "1"};
                appendTask(t);
                if (!t.completed)
                    pendingQueue.push(t.id);
                if (t.id >= nextId)
                    nextId = t.id + 1;
            }
            catch (...)
            {
            }
        }
    }
    void saveToFile()
    {
        ofstream out(fileName);
        for (TaskNode *p = head; p; p = p->next)
        {
            const Task &t = p->data;
            out << t.id << '|' << t.type << '|' << t.subject << '|' << t.topic << '|'
                << t.date << '|' << t.targetHours << '|' << t.hoursDone << '|'
                << t.completed << '\n';
        }
    }

    void printHeader()
    {
        cout << left << setw(4) << "ID" << setw(8) << "Type" << setw(16) << "Subject"
             << setw(28) << "Topic" << setw(12) << "Date" << "Progress\n"
             << string(85, '-') << "\n";
    }
    void printTask(const Task &t)
    {
        cout << left << setw(4) << t.id << setw(8) << t.type << setw(16) << t.subject
             << setw(28) << t.topic.substr(0, 26) << setw(12) << t.date
             << fixed << setprecision(1) << t.hoursDone << "/" << t.targetHours << " hrs  "
             << (t.completed ? "Done" : "Pending") << "\n";
        cout.unsetf(ios::fixed);
    }

    static bool lessThan(const Task *a, const Task *b, int sortBy)
    {
        if (sortBy == 1)
            return a->date < b->date;
        if (sortBy == 2)
            return (a->targetHours - a->hoursDone) < (b->targetHours - b->hoursDone);
        return a->subject < b->subject;
    }
    static void mergeSort(vector<Task *> &v, int lo, int hi, int sortBy)
    {
        if (lo >= hi)
            return;
        int mid = (lo + hi) / 2;
        mergeSort(v, lo, mid, sortBy);
        mergeSort(v, mid + 1, hi, sortBy);
        vector<Task *> tmp;
        int i = lo, j = mid + 1;
        while (i <= mid && j <= hi)
            tmp.push_back(lessThan(v[j], v[i], sortBy) ? v[j++] : v[i++]);
        while (i <= mid)
            tmp.push_back(v[i++]);
        while (j <= hi)
            tmp.push_back(v[j++]);
        for (int k = 0; k < (int)tmp.size(); k++)
            v[lo + k] = tmp[k];
    }

public:
    StudyPlanner(const string &file = "study_tasks.txt")
        : head(nullptr), tail(nullptr), count(0), nextId(1), fileName(file)
    {
        loadFromFile();
    }
    ~StudyPlanner() { clearList(); }
    StudyPlanner(const StudyPlanner &) = delete;
    StudyPlanner &operator=(const StudyPlanner &) = delete;

    void addTask(const string &type)
    {
        Task t;
        t.id = nextId++;
        t.type = type;
        t.subject = readText("Subject (e.g. OOP, DSA): ");
        t.topic = readText("Topic (e.g. Unit 2 - Linked List): ");
        t.date = readDate(type == "Daily" ? "Date (YYYY-MM-DD): " : "Week start date (YYYY-MM-DD): ");
        t.targetHours = readNumber("Target study hours: ", 0.5, type == "Daily" ? 24 : 100);
        t.hoursDone = 0;
        t.completed = false;
        for (char &c : t.subject)
            if (c == '|')
                c = '/';
        for (char &c : t.topic)
            if (c == '|')
                c = '/';
        appendTask(t);
        pendingQueue.push(t.id);
        saveToFile();
        cout << type << " task added with ID " << t.id << ".\n";
    }

    void viewTasks(const string &type)
    {
        bool any = false;
        for (TaskNode *p = head; p; p = p->next)
        {
            const Task &t = p->data;
            if (type != "All" && t.type != type)
                continue;
            if (!any)
            {
                printHeader();
                any = true;
            }
            printTask(t);
        }
        if (!any)
            cout << "No tasks found.\n";
    }

    void viewSorted()
    {
        if (!head)
        {
            cout << "No tasks found.\n";
            return;
        }
        cout << "Sort by:\n 1. Date\n 2. Hours left (least first)\n 3. Subject\n";
        int sortBy = (int)readNumber("Choice: ", 1, 3);

        vector<Task *> arr;
        for (TaskNode *p = head; p; p = p->next)
            arr.push_back(&p->data);
        mergeSort(arr, 0, (int)arr.size() - 1, sortBy);

        printHeader();
        for (Task *t : arr)
            printTask(*t);
    }

    void markComplete()
    {
        Task *t = findTask((int)readNumber("Task ID to mark complete: ", 1, 100000));
        if (!t)
        {
            cout << "Task not found.\n";
            return;
        }
        if (t->completed)
        {
            cout << "Task is already complete.\n";
            return;
        }
        t->completed = true;
        saveToFile();
        cout << "Task " << t->id << " marked complete.\n";
    }

    void logStudyHours()
    {
        Task *t = findTask((int)readNumber("Task ID: ", 1, 100000));
        if (!t)
        {
            cout << "Task not found.\n";
            return;
        }
        t->hoursDone += readNumber("Hours studied: ", 0.1, 24);
        saveToFile();
        cout << "Total for this task: " << t->hoursDone << "/" << t->targetHours << " hrs.\n";
        if (t->hoursDone >= t->targetHours && !t->completed)
            cout << "Target reached! You can mark this task complete.\n";
    }

    void studyReport()
    {
        if (!head)
        {
            cout << "No tasks yet.\n";
            return;
        }
        double target = 0, done = 0;
        int completed = 0;
        vector<string> subjects;
        vector<double> subjectHours;
        for (TaskNode *p = head; p; p = p->next)
        {
            const Task &t = p->data;
            target += t.targetHours;
            done += t.hoursDone;
            if (t.completed)
                completed++;
            size_t i = 0;
            while (i < subjects.size() && subjects[i] != t.subject)
                i++;
            if (i == subjects.size())
            {
                subjects.push_back(t.subject);
                subjectHours.push_back(0);
            }
            subjectHours[i] += t.hoursDone;
        }
        cout << fixed << setprecision(1)
             << "\nCompleted tasks: " << completed << "/" << count << "\n"
             << "Hours studied  : " << done << " of " << target << " target hrs\n"
             << "Hours by subject:\n";
        for (size_t i = 0; i < subjects.size(); i++)
            cout << "  " << left << setw(16) << subjects[i] << subjectHours[i] << " hrs\n";
        cout.unsetf(ios::fixed);
    }

    void showQueue()
    {
        if (pendingQueue.empty())
        {
            cout << "Pending queue is empty.\n";
            return;
        }
        queue<int> copy = pendingQueue;
        bool any = false;
        while (!copy.empty())
        {
            Task *t = findTask(copy.front());
            copy.pop();
            if (!t || t->completed)
                continue;
            if (!any)
            {
                printHeader();
                any = true;
            }
            printTask(*t);
        }
        if (!any)
            cout << "Pending queue is empty.\n";
    }

    void processNextTask()
    {
        while (!pendingQueue.empty())
        {
            int id = pendingQueue.front();
            pendingQueue.pop();
            Task *t = findTask(id);
            if (!t || t->completed)
                continue;
            cout << "Next task in queue:\n";
            printHeader();
            printTask(*t);
            string ans = readText("Log study hours for it now? (y/n): ");
            if (ans == "y" || ans == "Y")
            {
                t->hoursDone += readNumber("Hours studied: ", 0.1, 24);
                if (t->hoursDone >= t->targetHours)
                {
                    t->completed = true;
                    cout << "Target reached - task marked complete.\n";
                    saveToFile();
                    return;
                }
                pendingQueue.push(t->id);
                saveToFile();
            }
            else
            {
                pendingQueue.push(t->id);
            }
            return;
        }
        cout << "No pending tasks in the queue.\n";
    }

    void menu()
    {
        while (true)
        {
            cout << "\n===== STUDY PLANNER =====\n"
                 << "1. Add daily task\n"
                 << "2. Add weekly task\n"
                 << "3. View daily tasks\n"
                 << "4. View weekly tasks\n"
                 << "5. View all tasks\n"
                 << "6. Mark task complete\n"
                 << "7. Log study hours\n"
                 << "8. Study hours report\n"
                 << "9. View tasks sorted\n"
                 << "10. Show pending queue\n"
                 << "11. Process next task from queue\n"
                 << "0. Back\n";
            switch ((int)readNumber("Choice: ", 0, 11))
            {
            case 1:
                addTask("Daily");
                break;
            case 2:
                addTask("Weekly");
                break;
            case 3:
                viewTasks("Daily");
                break;
            case 4:
                viewTasks("Weekly");
                break;
            case 5:
                viewTasks("All");
                break;
            case 6:
                markComplete();
                break;
            case 7:
                logStudyHours();
                break;
            case 8:
                studyReport();
                break;
            case 9:
                viewSorted();
                break;
            case 10:
                showQueue();
                break;
            case 11:
                processNextTask();
                break;
            case 0:
                return;
            }
        }
    }
};

int main()
{
}
