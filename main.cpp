#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

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
int main()
{
    OpportunityHub hub;
    hub.menu();

    return 0;
}

 
