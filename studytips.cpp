#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>
using namespace std;

// Convert string to lowercase
string toLowerCase(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);

    return s;
}

// Read string input
string readText(string message)
{
    string value;
    cout << message;
    getline(cin >> ws, value);
    return value;
}

// Read valid number
int readNumber(string message, int low, int high)
{
    int number;

    while (true)
    {
        cout << message;

        if (cin >> number && number >= low && number <= high)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }

        cout << "Invalid input. Enter a number from "
             << low << " to " << high << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Tip structure
struct Tip
{
    int id;
    string subject;
    string category;
    string keywords;
    string text;
};

// Stack for recently viewed tip IDs
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

    // Push every viewed tip ID
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

// Study Tips Manager
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

    // Display tip and optionally add its ID to stack
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

    // Load tips from file
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

    // Display all tips
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

    // Filter by subject
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

    // Filter by category
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

    // Search by keyword
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

    // Open tip using ID
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

    // Display recent history
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

    // Go back to previous tip
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

            // Do not push this tip again
            printTip(tips[index], false);
        }
    }

    // Main menu
    void menu()
    {
        while (true)
        {
            cout << "\n===== STUDY TIPS ("
                 << tips.size() << " loaded) =====\n";

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

int main()
{
    StudyTips tips("tips.txt");

    tips.menu();

    return 0;
}