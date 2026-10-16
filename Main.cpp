#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
using namespace std;
string readText(const string& msg) {
    string s;
    do {
        cout << msg;
        if (!getline(cin, s)) { cout << "\nInput ended.\n"; exit(0); }
    } while (s.empty());
    return s;
}

double readNumber(const string& msg, double lo, double hi) {
    while (true) {
        string s = readText(msg);
        stringstream ss(s); double v; char extra;
        if (ss >> v && !(ss >> extra) && v >= lo && v <= hi) return v;
        cout << "Enter a value between " << lo << " and " << hi << ".\n";
    }
}

string toLower(string s) {
    for (char& c : s) c = tolower((unsigned char)c);
    return s;
}

//harsh verma work
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

int main()
{
    OpportunityHub hub;
    hub.menu();

    return 0;
}
