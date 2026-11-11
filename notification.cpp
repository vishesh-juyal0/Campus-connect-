#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<unordered_map>
#include<set>
using namespace std;




struct Notification
{
    int id;
    string message;
    string category;
    int priority;
};






struct compare
{
    bool operator()(Notification a,Notification b)
    {
        return a.priority < b. priority;
    }
};



priority_queue<Notification,
vector< Notification>,compare>
    notifications;
unordered_map<int , Notification>
notificationData;
set<string> categories;





 
void addNotification(int id ,string message, string category,int priority)
{
    Notification n = {id, message, 
        category, priority};

        notifications.push(n);
        notificationData[id] = n;
        categories.insert(category);
}





    void notification ()
    {
        addNotification(101, "C++ Internship deadline in 3 days", "Internship",2);
        addNotification(102, "DSA Hackathon deadline in 2 days", "Hackathon",3 );
        addNotification(103,"Technical Workshop", "Workshop",1);
        addNotification(104,"Cultural Fest","Event",1);

        cout<< "\n -----notifications ----\n";
        while(!notifications.empty())
        {
            cout<< notifications.top().message << endl;
            notifications.pop();
        }
    }




void recommendation()
{
    string skill;

    cout<< "\n ---Recommendation system ---\n";
    cout<< "Enter your skill (C++,DSA,HTML and CSS)"<<endl;
    cin >> skill;

    if(skill == "C++")
    {
        cout<<"Recommended: C++ Internship \n"<<endl;
    }

    else if(skill == "DSA")
    {
        cout<<"Recommended: DSA Hackathon \n"<<endl;
    }

    else if(skill == "HTML")
    {
        cout<<"Recommended: Web Development job \n"<<endl;
    }

    else if(skill == "CSS")
    {
        cout<<"Recommended: web Development job \n"<<endl;
    }

    else
    {
        cout<<"No matching opportunity found \n"<<endl;

    }
    
}





void interestRecommendation()
{
    string interest;

    cout<< "\n Enter your interset coding, web, AI"<<endl;
    cin >> interest;

    if(interest == "coding")
    {
        cout<<"Recommended: DSA Hackathon\n"<<endl;
        cout<<"Recommended: coding\n"<<endl;
    }
    else if(interest == "web")
    {
        cout<<"Recommended: DSA  hackathon\n"<<endl;
        cout<<"Recommended: HTML Workshop\n"<<endl;
    }
    else if(interest == "AI")
    {
        cout<<"Recommended : AI Internship\n"<<endl;
        cout<<"Recommended : AI Workshop\n"<<endl;
    }
    else
    {
        cout<<"No matching recommended found\n"<<endl;

    }

}





void attendenceReminder()
{
    int total,attended;
    float percentage;

    cout<< "\n----Attendence Reminder ---\n"<<endl;
    cout<< "Enter a total classes:"<<endl;
    cin >> total;
    cout<<"Enter attended class:"<<endl;
    cin >> attended;

    if(total <= 0 || attended < 0 || attended > total)
    {
        cout<<"Invalid attendance data\n"<<endl;
        return;
    }

    percentage = (attended * 100.0)/total;

    cout<<"Attendance "<<percentage << "%\n";
   
    if(percentage < 75)
    {
       cout<<"Warning: Attendence is below 75%\n"<<endl;

    }
    else
    {
        cout<< "your attendence is sufficient \n";

    }
}




void collegeProgram()
{
    cout<< "\n---College  Programs ----\n"<<endl;

    cout<< "1. Technical Workshop\n";
    cout<< "2. Cultural fest \n";
    cout<< "3. Coding Competition\n";
    cout<< "4. Hackathon\n";
    cout<< "5. AI workshop\n";
}






struct opportunity
{
    string title;
    string skill;
};




vector<opportunity> opportunities =
{
    {"C++ Intership", "C++"},
    {"DSA Hackathon" , "DSA"},
    {"Web Workshop", "CSS"},
    {"AI Intership" , "AI"},
};




void searchOpportunity()
{
    string skill;
    bool found = false;

    cout << "\n Enter skill to search:"<<endl;
    cin >> skill;
    for(int i=0;i<opportunities.size();i++)
    {
        if (opportunities[i].skill == skill)
        {
            cout<< "Found :"<<opportunities[i].title << endl;
            found = true;
        }
    }
    if(!found)
    cout << "No opportunity found\n" <<endl;
}



void searchNotification()
{
    int id;

    cout<<"\n Enter notification ID:"<<endl;
    cin>>id;

    auto it = notificationData .find(id);

    if(it != notificationData.end())
    {
        cout<< "Found :"<<it->second.message <<endl;
    }
    else
    {
        cout << "Notification not found\n"<<endl;

    }
}




void showCategories()
{
    cout<< "\n -----Notification Categories----\n"<<endl;

    if(categories.empty())
    {
        cout << "First Select Notification from the menu\n"<<endl;
        return;
    }
    for(string category : categories)
    {
        cout << category<<endl;
    }
}

struct Task
{
    string taskname;
    string deadline;
    bool completed;
};

vector<Task> tasks;

void studyPlanner()
{
    int choice;

    do{
        cout <<  "\n---Daily Study Planner---\n"<<endl;
        cout << "1. Add Task\n"<<endl;
        cout << "2.Show Task\n"<<endl;
        cout << "3.Mark Task Completed\n"<<endl;
        cout << "4. Back to Main Menu\n"<<endl;
        cout << "Enter a Choice:"<<endl;
        cin>>choice;

        if(choice == 1)
        {
            Task t;

            cout<<"Enter task name:"<<endl;
            cin.ignore();
            getline(cin,t.taskname);

            cout<< "Enter deadline:" << endl;
            getline(cin, t.deadline);

            t.completed = false;
            tasks.push_back(t);
            cout<<"Task added succesfully!\n"<<endl;
        }
        else if(choice == 2)
        {
            if(tasks.empty())
            {
                cout << "No task available \n"<<endl;
            }
            else
            {
                for(int i=0;i< tasks.size();i++)
                {
                    cout<<"\nTask"<< i+1<< ":"  <<   tasks[i].taskname << endl;
                    cout<<"Deadline:"<<tasks[i].deadline << endl;
                
                if(tasks[i].completed)
                {
                    cout<<"Status:Completed\n"<<endl;
                }
                else
                {
                    cout<<"status: Pending\n"<<endl;

                }

            }
        }
    }
    else if(choice == 3)
    {
        int n;
        cout<<"Enter task number :"<<endl;

        cin >> n;
        if(n>= 1 && n<= tasks.size())
        {
            tasks[n-1].completed = true;
            cout<<"Task marked as completed!\n"<<endl;
        }
        else
        {
            cout<<"Invalid task number\n"<<endl;
        }
    }
    else if(choice!=4)
    {
        cout<<"Invalid Choice.\n"<<endl;
    }
  }while(choice!=4);
}



void viewStudyTasks()
{
    if(tasks.empty())
    {
        cout<< "No study task available\n"<<endl;
        return;
    }

    cout<< "\n---StudyPlanner----\n"<<endl;

   for(int i=0; i < tasks.size(); i++)
    {
       cout<<"Task: " << tasks[i].taskname << endl;
       cout <<"Deadline:" << tasks[i].deadline<<endl;

       if(tasks[i].completed)
       cout<< "Status Completed\n"<<endl;
       else
          cout<< "Status: pending\n"<<endl;
    }
}

