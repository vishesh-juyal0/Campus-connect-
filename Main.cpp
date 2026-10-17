#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
        stringstream ss(readText(msg));
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
 
template <class T>
class List
{
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };
    Node *head = nullptr, *tail = nullptr;
    int n = 0;
 
    template <class F>
    static Node *mergeSort(Node *h, F less)
    {
        if (!h || !h->next)
            return h;
        Node *slow = h, *fast = h->next;
        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node *b = slow->next;
        slow->next = nullptr;
        Node *a = mergeSort(h, less);
        b = mergeSort(b, less);
        Node *res = nullptr, **t = &res;
        while (a && b)
        {
            Node *&pick = less(b->data, a->data) ? b : a;
            *t = pick;
            pick = pick->next;
            t = &(*t)->next;
        }
        *t = a ? a : b;
        return res;
    }
 
public:
    List() {}
    List(const List &) = delete;
    List &operator=(const List &) = delete;
    ~List() { clear(); }
 
    int size() const { return n; }
    bool empty() const { return n == 0; }
 
    void push_back(const T &d)
    {
        Node *x = new Node(d);
        if (tail)
            tail->next = x;
        else
            head = x;
        tail = x;
        n++;
    }
    void push_front(const T &d)
    {
        Node *x = new Node(d);
        x->next = head;
        head = x;
        if (!tail)
            tail = x;
        n++;
    }
    bool pop_front(T &out)
    {
        if (!head)
            return false;
        Node *x = head;
        out = x->data;
        head = x->next;
        if (!head)
            tail = nullptr;
        delete x;
        n--;
        return true;
    }
    bool pop_back(T &out)
    {
        if (!head)
            return false;
        out = tail->data;
        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
        }
        else
        {
            Node *p = head;
            while (p->next != tail)
                p = p->next;
            delete tail;
            tail = p;
            p->next = nullptr;
        }
        n--;
        return true;
    }
    bool front(T &out) const
    {
        if (!head)
            return false;
        out = head->data;
        return true;
    }
    void clear()
    {
        T tmp;
        while (pop_front(tmp))
        {
        }
    }
    const T &at(int i) const
    {
        Node *p = head;
        while (i-- > 0)
            p = p->next;
        return p->data;
    }
    template <class F>
    T *find(F pred)
    {
        for (Node *p = head; p; p = p->next)
            if (pred(p->data))
                return &p->data;
        return nullptr;
    }
    template <class F>
    bool removeIf(F pred) // removes first match
    {
        Node *prev = nullptr;
        for (Node *p = head; p; prev = p, p = p->next)
            if (pred(p->data))
            {
                (prev ? prev->next : head) = p->next;
                if (p == tail)
                    tail = prev;
                delete p;
                n--;
                return true;
            }
        return false;
    }
    template <class F>
    void each(F f) const
    {
        for (Node *p = head; p; p = p->next)
            f(p->data);
    }
    template <class F>
    void sort(F less) 
    {
        head = mergeSort(head, less);
        tail = head;
        while (tail && tail->next)
            tail = tail->next;
    }
};
 
struct Opportunity
{
    int id = 0;
    string title, type, skill, deadline;
 
    void input()
    {
        id = (int)readNumber("Opportunity ID: ", 1, 1000000);
        title = readText("Title: ");
        type = readText("Type (Job/Internship/Hackathon/Quiz): ");
        skill = readText("Required Skill: ");
        deadline = readText("Deadline (YYYYMMDD): ");
    }
    void display() const
    {
        cout << "\nOpportunity ID: " << id << "\nTitle: " << title << "\nType: " << type
             << "\nRequired Skill: " << skill << "\nDeadline: " << deadline << "\n";
    }
};
 
class OpportunityHub
{
    List<Opportunity> list;
 
    Opportunity *byId(const string &msg)
    {
        int id = (int)readNumber(msg, 1, 1000000);
        return list.find([&](const Opportunity &o) { return o.id == id; });
    }
    template <class F>
    void showWhere(F pred, const string &none)
    {
        bool any = false;
        list.each([&](const Opportunity &o) {
            if (pred(o))
            {
                o.display();
                any = true;
            }
        });
        if (!any)
            cout << none << "\n";
    }
 
public:
    void add()
    {
        Opportunity o;
        o.input();
        if (list.find([&](const Opportunity &x) { return x.id == o.id; }))
            cout << "ID already exists!\n";
        else
        {
            list.push_back(o);
            cout << "Opportunity Added Successfully!\n";
        }
    }
    void displayAll() { showWhere([](const Opportunity &) { return true; }, "No Opportunities Available."); }
    void search()
    {
        if (Opportunity *o = byId("Opportunity ID: "))
            o->display();
        else
            cout << "Opportunity Not Found.\n";
    }
    void remove()
    {
        int id = (int)readNumber("Opportunity ID to Delete: ", 1, 1000000);
        cout << (list.removeIf([&](const Opportunity &o) { return o.id == id; })
                     ? "Opportunity Deleted Successfully!\n"
                     : "Opportunity Not Found.\n");
    }
    void update()
    {
        if (Opportunity *o = byId("Opportunity ID to Update: "))
        {
            cout << "Enter New Details:\n";
            o->input();
            cout << "Opportunity Updated Successfully!\n";
        }
        else
            cout << "Opportunity Not Found.\n";
    }
    void sortByDeadline()
    {
        list.sort([](const Opportunity &a, const Opportunity &b) { return a.deadline < b.deadline; });
        cout << "Opportunities Sorted by Deadline.\n";
        displayAll();
    }
    void filterByType()
    {
        string t = toLower(readText("Type to Filter: "));
        showWhere([&](const Opportunity &o) { return toLower(o.type) == t; }, "No Matching Opportunities Found.");
    }
    void searchBySkill()
    {
        string s = toLower(readText("Skill to Search: "));
        showWhere([&](const Opportunity &o) { return toLower(o.skill) == s; }, "No Opportunities Found for This Skill.");
    }
    void menu()
    {
        while (true)
        {
            cout << "\n===== CAMPUS CONNECT - OPPORTUNITY HUB =====\n"
                    "1. Add Opportunity\n2. Display All\n3. Search by ID\n4. Delete\n"
                    "5. Update\n6. Sort by Deadline\n7. Filter by Type\n8. Search by Skill\n0. Back\n";
            switch ((int)readNumber("Choice: ", 0, 8))
            {
            case 1: add(); break;
            case 2: displayAll(); break;
            case 3: search(); break;
            case 4: remove(); break;
            case 5: update(); break;
            case 6: sortByDeadline(); break;
            case 7: filterByType(); break;
            case 8: searchBySkill(); break;
            case 0: return;
            }
        }
    }
};
 
class RecentStack
{
    static const int CAPACITY = 5;
    List<int> s;
 
public:
    bool isEmpty() const { return s.empty(); }
    void push(int id)
    {
        s.removeIf([&](int x) { return x == id; }); 
        int dropped;
        if (s.size() == CAPACITY)
            s.pop_back(dropped);
        s.push_front(id);
    }
    bool pop(int &id) { return s.pop_front(id); }
    bool peek(int &id) const { return s.front(id); }
    template <class F>
    void each(F f) const { s.each(f); } // latest first
};
 
struct Tip
{
    int id = 0;
    string subject, category, keywords, text;
};
 
class StudyTips
{
    List<Tip> tips;
    RecentStack recent;
    string fileName;
 
    static void printTip(const Tip &t)
    {
        cout << "[" << t.id << "] (" << t.subject << " - " << t.category << ")\n    " << t.text << "\n";
    }
    const Tip *findTip(int id)
    {
        return tips.find([&](const Tip &t) { return t.id == id; });
    }
    template <class F>
    void printWhere(F pred, const string &none)
    {
        bool any = false;
        tips.each([&](const Tip &t) {
            if (pred(t))
            {
                printTip(t);
                any = true;
            }
        });
        if (!any)
            cout << none << "\n";
    }
    static string normalizeSubject(const string &subject)
    {
        string s = toLower(subject);
        if (s.find("dsa") != string::npos || s.find("data struct") != string::npos || s == "ds")
            return "dsa";
        if (s.find("oop") != string::npos || s.find("c++") != string::npos ||
            s.find("cpp") != string::npos || s.find("object") != string::npos)
            return "oop";
        return s;
    }
    static bool keywordMatch(const Tip &t, const string &text)
    {
        stringstream ss(t.keywords);
        string kw;
        while (getline(ss, kw, ','))
            if (!kw.empty() && text.find(kw) != string::npos)
                return true;
        return false;
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
                Tip t;
                t.id = stoi(f[0]);
                t.subject = f[1];
                t.category = f[2];
                t.keywords = toLower(f[3]);
                t.text = f[4];
                tips.push_back(t);
            }
            catch (...)
            {
            }
        }
    }
 
    void displayAll() { printWhere([](const Tip &) { return true; }, "No tips available."); }
 
    void filterBySubject()
    {
        List<string> subs; // distinct subjects
        tips.each([&](const Tip &t) {
            if (!subs.find([&](const string &s) { return s == t.subject; }))
                subs.push_back(t.subject);
        });
        if (subs.empty())
        {
            cout << "No tips available.\n";
            return;
        }
        int i = 0;
        subs.each([&](const string &s) { cout << ++i << ". " << s << "\n"; });
        string pick = subs.at((int)readNumber("Choose subject: ", 1, subs.size()) - 1);
        printWhere([&](const Tip &t) { return t.subject == pick; }, "No tips.");
    }
 
    void filterByCategory()
    {
        const string cats[] = {"Concept", "Practice", "Revision", "Productivity"};
        for (int i = 0; i < 4; i++)
            cout << i + 1 << ". " << cats[i] << "\n";
        string c = cats[(int)readNumber("Choose category: ", 1, 4) - 1];
        printWhere([&](const Tip &t) { return t.category == c; }, "No tips in this category.");
    }
 
    void searchByKeyword()
    {
        string key = toLower(readText("Keyword (e.g. stack, inheritance): "));
        printWhere([&](const Tip &t) {
            return t.keywords.find(key) != string::npos || toLower(t.text).find(key) != string::npos;
        },
                   "No tips found for \"" + key + "\".");
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
 
    void showRecent()
    {
        if (recent.isEmpty())
        {
            cout << "No recently viewed tips.\n";
            return;
        }
        cout << "Recently viewed (latest first):\n";
        recent.each([&](int id) {
            if (const Tip *t = findTip(id))
                printTip(*t);
        });
    }
 
    void goBack()
    {
        int id;
        if (!recent.pop(id))
        {
            cout << "Nothing to go back to.\n";
            return;
        }
        if (!recent.peek(id))
        {
            cout << "No earlier tip.\n";
            return;
        }
        cout << "Previous tip:\n";
        if (const Tip *t = findTip(id))
            printTip(*t);
    }
 
    // event: "new" or "done". Prints the best `limit` tips for a task.
    void suggestForTask(const string &subject, const string &topic, const string &event, int limit = 2)
    {
        struct Scored
        {
            int score = 0;
            const Tip *tip = nullptr;
        };
        string subj = normalizeSubject(subject), text = toLower(topic);
        List<Scored> scored;
        tips.each([&](const Tip &t) {
            int score = 0;
            string ts = toLower(t.subject);
            if (ts == subj)
                score += 2;
            else if (ts == "general")
                score += 1;
            else
                return;
            if (keywordMatch(t, text))
                score += 3;
            if (event == "done" && t.category == "Revision")
                score += 2;
            if (event == "new" && (t.category == "Concept" || t.category == "Practice"))
                score += 2;
            Scored s;
            s.score = score;
            s.tip = &t;
            scored.push_back(s);
        });
        if (scored.empty())
            return;
        scored.sort([](const Scored &a, const Scored &b) { return a.score > b.score; });
        cout << "\n--- Study tips for this " << (event == "done" ? "completed" : "new") << " task ---\n";
        int shown = 0;
        scored.each([&](const Scored &s) {
            if (shown++ < limit)
            {
                printTip(*s.tip);
                recent.push(s.tip->id);
            }
        });
    }
 
    void menu()
    {
        while (true)
        {
            cout << "\n===== STUDY TIPS (" << tips.size() << " loaded) =====\n"
                    "1. Display all tips\n2. Filter by subject\n3. Filter by category\n"
                    "4. Search by keyword\n5. Open a tip by ID\n6. Recently viewed tips (stack)\n"
                    "7. Go back to previous tip (pop)\n8. Reload tips from file\n0. Back\n";
            switch ((int)readNumber("Choice: ", 0, 8))
            {
            case 1: displayAll(); break;
            case 2: filterBySubject(); break;
            case 3: filterByCategory(); break;
            case 4: searchByKeyword(); break;
            case 5: viewTip(); break;
            case 6: showRecent(); break;
            case 7: goBack(); break;
            case 8: loadFromFile(); cout << tips.size() << " tips loaded.\n"; break;
            case 0: return;
            }
        }
    }
};
 
struct Task
{
    int id = 0;
    string type, subject, topic, date;
    double targetHours = 0, hoursDone = 0;
    bool completed = false;
};
 
class StudyPlanner
{
    List<Task> tasks;      
    List<int> pending;    
    int nextId = 1;
    string fileName;
    StudyTips *tips;      
 
    static bool validDate(const string &d)
    {
        int y, m, day;
        char c1, c2;
        stringstream ss(d);
        return (ss >> y >> c1 >> m >> c2 >> day) && c1 == '-' && c2 == '-' &&
               m >= 1 && m <= 12 && day >= 1 && day <= 31;
    }
    static string readDate(const string &msg)
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
        return tasks.find([&](const Task &t) { return t.id == id; });
    }
    Task *askTask(const string &msg)
    {
        Task *t = findTask((int)readNumber(msg, 1, 100000));
        if (!t)
            cout << "Task not found.\n";
        return t;
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
                Task t;
                t.id = stoi(f[0]);
                t.type = f[1];
                t.subject = f[2];
                t.topic = f[3];
                t.date = f[4];
                t.targetHours = stod(f[5]);
                t.hoursDone = stod(f[6]);
                t.completed = f[7] == "1";
                tasks.push_back(t);
                if (!t.completed)
                    pending.push_back(t.id);
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
        tasks.each([&](const Task &t) {
            out << t.id << '|' << t.type << '|' << t.subject << '|' << t.topic << '|' << t.date << '|'
                << t.targetHours << '|' << t.hoursDone << '|' << t.completed << '\n';
        });
    }
 
    static void printHeader()
    {
        cout << left << setw(4) << "ID" << setw(8) << "Type" << setw(16) << "Subject" << setw(28) << "Topic"
             << setw(12) << "Date" << "Progress\n" << string(85, '-') << "\n";
    }
    static void printTask(const Task &t)
    {
        cout << left << setw(4) << t.id << setw(8) << t.type << setw(16) << t.subject
             << setw(28) << t.topic.substr(0, 26) << setw(12) << t.date << fixed << setprecision(1)
             << t.hoursDone << "/" << t.targetHours << " hrs  " << (t.completed ? "Done" : "Pending") << "\n";
        cout.unsetf(ios::fixed);
    }
    void finish(Task &t)
    {
        t.completed = true;
        if (tips)
            tips->suggestForTask(t.subject, t.topic, "done");
    }
 
public:
    StudyPlanner(const string &file = "study_tasks.txt", StudyTips *tp = nullptr)
        : fileName(file), tips(tp) { loadFromFile(); }
 
    void addTask(const string &type)
    {
        Task t;
        t.id = nextId++;
        t.type = type;
        t.subject = readText("Subject (e.g. OOP, DSA): ");
        t.topic = readText("Topic (e.g. Unit 2 - Linked List): ");
        t.date = readDate(type == "Daily" ? "Date (YYYY-MM-DD): " : "Week start date (YYYY-MM-DD): ");
        t.targetHours = readNumber("Target study hours: ", 0.5, type == "Daily" ? 24 : 100);
        for (char &c : t.subject) if (c == '|') c = '/';
        for (char &c : t.topic) if (c == '|') c = '/';
        tasks.push_back(t);
        pending.push_back(t.id);
        saveToFile();
        cout << type << " task added with ID " << t.id << ".\n";
        if (tips)
            tips->suggestForTask(t.subject, t.topic, "new");
    }
 
    void viewTasks(const string &type)
    {
        bool any = false;
        tasks.each([&](const Task &t) {
            if (type != "All" && t.type != type)
                return;
            if (!any)
                printHeader();
            any = true;
            printTask(t);
        });
        if (!any)
            cout << "No tasks found.\n";
    }
 
    void viewSorted()
    {
        if (tasks.empty())
        {
            cout << "No tasks found.\n";
            return;
        }
        cout << "Sort by:\n 1. Date\n 2. Hours left (least first)\n 3. Subject\n";
        int by = (int)readNumber("Choice: ", 1, 3);
        tasks.sort([by](const Task &a, const Task &b) {
            if (by == 1) return a.date < b.date;
            if (by == 2) return (a.targetHours - a.hoursDone) < (b.targetHours - b.hoursDone);
            return a.subject < b.subject;
        });
        viewTasks("All");
    }
 
    void markComplete()
    {
        Task *t = askTask("Task ID to mark complete: ");
        if (!t)
            return;
        if (t->completed)
        {
            cout << "Task is already complete.\n";
            return;
        }
        finish(*t);
        saveToFile();
        cout << "Task " << t->id << " marked complete.\n";
    }
 
    void logStudyHours()
    {
        Task *t = askTask("Task ID: ");
        if (!t)
            return;
        t->hoursDone += readNumber("Hours studied: ", 0.1, 24);
        saveToFile();
        cout << "Total for this task: " << t->hoursDone << "/" << t->targetHours << " hrs.\n";
        if (t->hoursDone >= t->targetHours && !t->completed)
            cout << "Target reached! You can mark this task complete.\n";
    }
 
    void studyReport()
    {
        if (tasks.empty())
        {
            cout << "No tasks yet.\n";
            return;
        }
        struct SubjectHours
        {
            string name;
            double hrs = 0;
        };
        List<SubjectHours> bySubject;
        double target = 0, done = 0;
        int completed = 0;
        tasks.each([&](const Task &t) {
            target += t.targetHours;
            done += t.hoursDone;
            completed += t.completed;
            SubjectHours *s = bySubject.find([&](const SubjectHours &x) { return x.name == t.subject; });
            if (!s)
            {
                SubjectHours n;
                n.name = t.subject;
                bySubject.push_back(n);
                s = bySubject.find([&](const SubjectHours &x) { return x.name == t.subject; });
            }
            s->hrs += t.hoursDone;
        });
        cout << fixed << setprecision(1) << "\nCompleted tasks: " << completed << "/" << tasks.size()
             << "\nHours studied  : " << done << " of " << target << " target hrs\nHours by subject:\n";
        bySubject.each([](const SubjectHours &s) { cout << "  " << left << setw(16) << s.name << s.hrs << " hrs\n"; });
        cout.unsetf(ios::fixed);
    }
 
    void showQueue()
    {
        bool any = false;
        pending.each([&](int id) {
            Task *t = findTask(id);
            if (!t || t->completed)
                return;
            if (!any)
                printHeader();
            any = true;
            printTask(*t);
        });
        if (!any)
            cout << "Pending queue is empty.\n";
    }
 
    void processNextTask()
    {
        int id;
        while (pending.pop_front(id)) 
        {
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
                    finish(*t);
                    cout << "Target reached - task marked complete.\n";
                    saveToFile();
                    return;
                }
                saveToFile();
            }
            pending.push_back(t->id); 
            return;
        }
        cout << "No pending tasks in the queue.\n";
    }
 
    void menu()
    {
        while (true)
        {
            cout << "\n===== STUDY PLANNER =====\n"
                    "1. Add daily task\n2. Add weekly task\n3. View daily tasks\n4. View weekly tasks\n"
                    "5. View all tasks\n6. Mark task complete\n7. Log study hours\n8. Study hours report\n"
                    "9. View tasks sorted\n10. Show pending queue\n11. Process next task from queue\n0. Back\n";
            switch ((int)readNumber("Choice: ", 0, 11))
            {
            case 1: addTask("Daily"); break;
            case 2: addTask("Weekly"); break;
            case 3: viewTasks("Daily"); break;
            case 4: viewTasks("Weekly"); break;
            case 5: viewTasks("All"); break;
            case 6: markComplete(); break;
            case 7: logStudyHours(); break;
            case 8: studyReport(); break;
            case 9: viewSorted(); break;
            case 10: showQueue(); break;
            case 11: processNextTask(); break;
            case 0: return;
            }
        }
    }
};
 
int main()
{
    OpportunityHub hub;
    StudyTips tips;
    StudyPlanner planner("study_tasks.txt", &tips);
    while (true)
    {
        cout << "\n===== CAMPUS CONNECT =====\n1. Opportunity Hub\n2. Study Planner\n3. Study Tips\n0. Exit\n";
        switch ((int)readNumber("Choice: ", 0, 3))
        {
        case 1: hub.menu(); break;
        case 2: planner.menu(); break;
        case 3: tips.menu(); break;
        case 0: return 0;
        }
    }
}
