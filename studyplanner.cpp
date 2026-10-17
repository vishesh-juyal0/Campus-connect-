#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype>
#include <sstream>
using namespace std;

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
    // Basic YYYY-MM-DD format and range checking.
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
    string type;       // Daily or Weekly
    string subject;
    string topic;
    string date;       // YYYY-MM-DD
    double targetHours;
    double hoursDone;
    bool completed;
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

    void loadFromFile()
    {
        tasks.clear();
        pending.clear();
        nextID = 1;

        ifstream in(fileName);
        if (!in)
            return; // The file will be created when tasks are saved.

        string line;
        while (getline(in, line))
        {
            string fields[8];
            // Split the record using the pipe character.
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
        int choice = readNumber("Choose: ", 1, 3)
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
            cout << "\n========== STUDY PLANNER ==========\n";
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
            cout << "0. Exit planner\n";

            int choice = readNumber("Enter choice: ", 0, 10);

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
            else return;
        }
    }
};

int main()
{
    StudyPlanner planner("study_tasks.txt");
    planner.menu();
    return 0;
}
