
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct Note {
    int id;
    string title, subject, content;
    Note *next;
};

struct Resource {
    string title, subject, link;
    Resource *next;
};

Note *head = NULL;
Resource *rhead = NULL;

const int SIZE = 100;
Note *table[SIZE] = {NULL};


int hashID(int id) {
    return id % SIZE;
}

Note* searchID(int id) {
    Note *p = table[hashID(id)];

    while (p != NULL) {
        if (p->id == id)
            return p;
        p = p->next;
    }
    return NULL;
}


void createNote() {
    Note *n = new Note;

    cout << "Enter ID: ";
    cin >> n->id;

    if (searchID(n->id) != NULL) {
        cout << "ID already exists!\n";
        delete n;
        return;
    }

    cin.ignore();

    cout << "Enter title: ";
    getline(cin, n->title);

    cout << "Enter subject: ";
    getline(cin, n->subject);

    cout << "Enter content: ";
    getline(cin, n->content);

    n->next = head;
    head = n;

    
    n->next = head->next;
    table[hashID(n->id)] = n;

    ofstream f("notes.txt", ios::app);
    f << n->id << "|" << n->title << "|"
      << n->subject << "|" << n->content << "\n";
    f.close();

    cout << "Note added!\n";
}


void displayNotes() {
    
    Note *p = head;

    while (p != NULL) {
        cout << "\nID: " << p->id
             << "\nTitle: " << p->title
             << "\nSubject: " << p->subject
             << "\nContent: " << p->content << "\n";
        p = p->next;
    }
}


void searchNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    Note *p = searchID(id);

    if (p != NULL)
        cout << p->title << "\n" << p->subject
             << "\n" << p->content << "\n";
    else
        cout << "Note not found!\n";
}


void searchSubject() {
    string s;
    cin.ignore();

    cout << "Enter subject: ";
    getline(cin, s);

    Note *p = head;
    while (p != NULL) {
        if (p->subject == s)
            cout << p->id << " " << p->title << "\n";
        p = p->next;
    }
}


void editNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;
    cin.ignore();

    Note *p = searchID(id);

    if (p == NULL) {
        cout << "Note not found!\n";
        return;
    }

    cout << "New title: ";
    getline(cin, p->title);

    cout << "New subject: ";
    getline(cin, p->subject);

    cout << "New content: ";
    getline(cin, p->content);

    cout << "Note updated!\n";
}

void deleteNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    Note *p = head, *prev = NULL;

    while (p != NULL && p->id != id) {
        prev = p;
        p = p->next;
    }

    if (p == NULL) {
        cout << "Note not found!\n";
        return;
    }

    if (prev == NULL)
        head = p->next;
    else
        prev->next = p->next;

    table[hashID(id)] = NULL;
    delete p;

    cout << "Note deleted!\n";
}


void sortNotes() {
    for (Note *p = head; p != NULL; p = p->next) {
        for (Note *q = p->next; q != NULL; q = q->next) {
            if (p->title > q->title) {
                swap(p->id, q->id);
                swap(p->title, q->title);
                swap(p->subject, q->subject);
                swap(p->content, q->content);
            }
        }
    }

    displayNotes();
}


void addResource() {
    Resource *r = new Resource;
    cin.ignore();

    cout << "Resource title: ";
    getline(cin, r->title);

    cout << "Subject: ";
    getline(cin, r->subject);

    cout << "Link: ";
    getline(cin, r->link);

    r->next = rhead;
    rhead = r;

    ofstream f("resources.txt", ios::app);
    f << r->title << "|" << r->subject
      << "|" << r->link << "\n";
    f.close();

    cout << "Resource added!\n";
}


void displayResources() {
    Resource *p = rhead;

    while (p != NULL) {
        cout << "\n" << p->title
             << "\n" << p->subject
             << "\n" << p->link << "\n";
        p = p->next;
    }
}

int main() {
    int choice;

    do {
        cout << "\n--- CampusConnect ---\n";
        cout << "1. Create note\n";
        cout << "2. Display notes\n";
        cout << "3. Search by ID\n";
        cout << "4. Search by subject\n";
        cout << "5. Edit note\n";
        cout << "6. Delete note\n";
        cout << "7. Sort notes\n";
        cout << "8. Add resource\n";
        cout << "9. Display resources\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createNote(); break;
            case 2: displayNotes(); break;
            case 3: searchNote(); break;
            case 4: searchSubject(); break;
            case 5: editNote(); break;
            case 6: deleteNote(); break;
            case 7: sortNotes(); break;
            case 8: addResource(); break;
            case 9: displayResources(); break;
        }

    } while (choice != 0);

    return 0;
}
