#ifndef CAMPUSCONNECT_H
#define CAMPUSCONNECT_H

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
    int Priority;
};
void NotificationAndRecommendationSystem();
void addNotification(int id ,string message,string category,int priority);
void notification();
void searchNotification();
void showCategories();


void recommendation();
void interestRecommendation();


void attendenceReminder();
void collegeProgram();
void searchOpportunity();


struct Task
{
    string taskname;
    string deadline;
    string completed;
};

void studyPlanner();
void viewStudyTasks();


#endif