#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

class Note {
public:
    int id;
    string title, subject, content;
    Note *next, *hashNext;
};

class Resource {
public:
    string title, subject, link;
    Resource *next;
};

Note *head = NULL;
Resource *rhead = NULL;

const int SIZE = 100;
Note *table[SIZE] = {NULL};

int hashID(int id) {
    return (id % SIZE + SIZE) % SIZE;
}

void insertHash(Note *n) {
    int i = hashID(n->id);
    n->hashNext = table[i];
    table[i] = n;
}

Note* searchID(int id) {
    Note *p = table[hashID(id)];

    while (p != NULL) {
        if (p->id == id) return p;
        p = p->hashNext;
    }
    return NULL;
}

void saveNotes() {
    ofstream f("notes.txt");

    for (Note *p = head; p != NULL; p = p->next)
        f << p->id << "|" << p->title << "|"
          << p->subject << "|" << p->content << "\n";

    f.close();
}

void saveResources() {
    ofstream f("resources.txt");

    for (Resource *p = rhead; p != NULL; p = p->next)
        f << p->title << "|" << p->subject
          << "|" << p->link << "\n";

    f.close();
}

void loadData() {
    string line, a, b, c, d;
    ifstream f("notes.txt");

    while (getline(f, line)) {
        stringstream ss(line);

        getline(ss, a, '|');
        getline(ss, b, '|');
        getline(ss, c, '|');
        getline(ss, d);

        if (a.empty()) continue;

        int id = stoi(a);
        if (searchID(id) != NULL) continue;

        Note *n = new Note{id, b, c, d, head, NULL};
        head = n;
        insertHash(n);
    }
    f.close();

    ifstream r("resources.txt");

    while (getline(r, line)) {
        stringstream ss(line);

        getline(ss, a, '|');
        getline(ss, b, '|');
        getline(ss, c);

        if (a.empty()) continue;

        Resource *p = new Resource{a, b, c, rhead};
        rhead = p;
    }
    r.close();
}

void createNote() {
    Note *n = new Note;

    cout << "Enter ID: ";
    cin >> n->id;

    if (searchID(n->id)) {
        cout << "ID already exists!\n";
        delete n;
        return;
    }

    cin.ignore();

    cout << "Title: ";
    getline(cin, n->title);

    cout << "Subject: ";
    getline(cin, n->subject);

    cout << "Content: ";
    getline(cin, n->content);

    n->next = head;
    n->hashNext = NULL;
    head = n;

    insertHash(n);
    saveNotes();

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

    if (p)
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

    bool found = false;

    for (Note *p = head; p != NULL; p = p->next) {
        if (p->subject == s) {
            cout << p->id << " " << p->title << "\n";
            found = true;
        }
    }

    if (!found) cout << "No notes found!\n";
}

void editNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;
    cin.ignore();

    Note *p = searchID(id);

    if (!p) {
        cout << "Note not found!\n";
        return;
    }

    cout << "New title: ";
    getline(cin, p->title);

    cout << "New subject: ";
    getline(cin, p->subject);

    cout << "New content: ";
    getline(cin, p->content);

    saveNotes();
    cout << "Note updated!\n";
}

void deleteNote() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    Note *p = head, *prev = NULL;

    while (p && p->id != id) {
        prev = p;
        p = p->next;
    }

    if (!p) {
        cout << "Note not found!\n";
        return;
    }

    if (prev) prev->next = p->next;
    else head = p->next;

    int i = hashID(id);
    Note *q = table[i], *old = NULL;

    while (q && q != p) {
        old = q;
        q = q->hashNext;
    }

    if (old) old->hashNext = q->hashNext;
    else table[i] = q->hashNext;

    delete p;
    saveNotes();

    cout << "Note deleted!\n";
}

void sortNotes() {
    for (Note *p = head; p; p = p->next) {
        for (Note *q = p->next; q; q = q->next) {
            if (p->title > q->title) {
                swap(p->id, q->id);
                swap(p->title, q->title);
                swap(p->subject, q->subject);
                swap(p->content, q->content);
            }
        }
    }

    for (int i = 0; i < SIZE; i++)
        table[i] = NULL;

    for (Note *p = head; p; p = p->next)
        insertHash(p);

    saveNotes();
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

    saveResources();
    cout << "Resource added!\n";
}

void displayResources() {
    for (Resource *p = rhead; p; p = p->next) {
        cout << "\nTitle: " << p->title
             << "\nSubject: " << p->subject
             << "\nLink: " << p->link << "\n";
    }
}

int main() {
    loadData();

    int choice;

    do {
        cout << "\n--- CAMPUSCONNECT ---\n";
        cout << "1. Create Note\n";
        cout << "2. Display Notes\n";
        cout << "3. Search by ID\n";
        cout << "4. Search by Subject\n";
        cout << "5. Edit Note\n";
        cout << "6. Delete Note\n";
        cout << "7. Sort Notes\n";
        cout << "8. Add Resource\n";
        cout << "9. Display Resources\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
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
            case 0: cout << "Thank you!\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}