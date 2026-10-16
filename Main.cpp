#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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
    int top; // index of top element, -1 when empty
public:
    RecentStack() : top(-1) {}
    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == CAPACITY - 1; }

    void push(int tipId)
    {
        // if already in stack, remove it so it moves to the top (no duplicates)
        for (int i = 0; i <= top; i++)
            if (arr[i] == tipId)
            {
                for (int j = i; j < top; j++)
                    arr[j] = arr[j + 1];
                top--;
                break;
            }
        // if full, drop the oldest (bottom) element
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
    int peekAt(int fromTop) const { return arr[top - fromTop]; } // 0 = top
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

int main()
{
}
