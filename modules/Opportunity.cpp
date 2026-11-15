#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

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
            cout << "\n===== CAMPUS CONNECT =====\n";
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

int main()
{
    OpportunityHub hub;
    hub.menu();

    return 0;
}