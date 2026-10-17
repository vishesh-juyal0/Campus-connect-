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
        addNotification(103,"technical Workshop","Workshop", 1);
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




void attendanceReminder()
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




void ShowCategories()
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

int main()
{
    int choice;

    do
    {
        cout << " \n =====CAMPUS CONNECT=====\n"<<endl;
        cout << "1.Notificiation\n"<<endl;
        cout << "2. Skill Recommendation\n"<<endl;
        cout << "3. InterestRecommended\n"<<endl;
        cout << "4. Attendence Reminder\n"<<endl;
        cout << "5. College program\n"<<endl;
        cout << "6. search oppournities \n"<<endl;
        cout << "7. Search Notification by ID\n"<<endl;
        cout << "8. Show Notification categories\n"<<endl;
        cout << "9. Exit\n"<<endl; 

        cout << "Enter a choice "<<endl;
        cin >> choice;

        switch(choice)
        {
            case 1:
                notification();
                break;

            case 2:
                recommendation();
                break;

            case 3:
                interestRecommendation();
                break;

            case 4:
                attendanceReminder();
                break;

            case 5:
                collegeProgram();
                break;

           case 6:
                searchOpportunity();
                break;

            case 7:
                searchNotification();
                break;

            case 8:
                ShowCategories();
                break;    

            case 9:
            cout << "Thank you for using campus connect!!\n"<<endl;
            break;

            default:
            cout<<"Invalid choice\n"<<endl;
        } 
    }while(choice != 9);
    return 0;
}
    
    
