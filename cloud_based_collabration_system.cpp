#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <algorithm>

using namespace std;

class Document {
public:
    string title;
    string content;
    int version;
    string lastModifiedBy;
    time_t lastModifiedTime;

    Document(string t, string c) {
        title = t;
        content = c;
        version = 1;
        lastModifiedBy = "None";
        lastModifiedTime = time(nullptr);
    }

    void editContent(string newContent, string user) {
        content = newContent;
        version++;
        lastModifiedBy = user;
        lastModifiedTime = time(nullptr);
    }

    void displayInfo() {
        cout << "Document: " << title << endl;
        cout << "Version: " << version << endl;
        cout << "Last Modified By: " << lastModifiedBy << endl;
        cout << "Last Modified Time: " << ctime(&lastModifiedTime);
        cout << "Content: " << content << endl;
    }
};

class CloudService {
public:
    unordered_map<string, Document> documents;

    void createDocument(string title, string content) {
        if (documents.find(title) != documents.end()) {
            cout << "Document already exists!" << endl;
        } else {
            documents[title] = Document(title, content);
            cout << "Document '" << title << "' created successfully." << endl;
        }
    }

    void editDocument(string title, string newContent, string user) {
        if (documents.find(title) != documents.end()) {
            documents[title].editContent(newContent, user);
            cout << "Document '" << title << "' updated by " << user << endl;
        } else {
            cout << "Document not found!" << endl;
        }
    }

    void shareDocument(string title, string user) {
        if (documents.find(title) != documents.end()) {
            cout << "Document '" << title << "' shared with " << user << endl;
        } else {
            cout << "Document not found!" << endl;
        }
    }

    void deleteDocument(string title) {
        if (documents.find(title) != documents.end()) {
            documents.erase(title);
            cout << "Document '" << title << "' deleted successfully." << endl;
        } else {
            cout << "Document not found!" << endl;
        }
    }

    void viewDocument(string title) {
        if (documents.find(title) != documents.end()) {
            documents[title].displayInfo();
        } else {
            cout << "Document not found!" << endl;
        }
    }

    void listDocuments() {
        if (documents.empty()) {
            cout << "No documents available." << endl;
            return;
        }
        for (auto& doc : documents) {
            cout << doc.first << endl;
        }
    }
};

class User {
private:
    string username;
    CloudService& cloudService;

public:
    User(string user, CloudService& cloud) : username(user), cloudService(cloud) {}

    void createDocument(string title, string content) {
        cloudService.createDocument(title, content);
    }

    void editDocument(string title, string newContent) {
        cloudService.editDocument(title, newContent, username);
    }

    void shareDocument(string title, string user) {
        cloudService.shareDocument(title, user);
    }

    void deleteDocument(string title) {
        cloudService.deleteDocument(title);
    }

    void viewDocument(string title) {
        cloudService.viewDocument(title);
    }

    void listDocuments() {
        cloudService.listDocuments();
    }
};

int main() {
    CloudService cloudService;

    User user1("Alice", cloudService);
    User user2("Bob", cloudService);

    user1.createDocument("Document1", "This is the content of Document 1.");
    user1.createDocument("Document2", "This is the content of Document 2.");

    user2.createDocument("Document3", "This is the content of Document 3.");

    user1.listDocuments();
    user2.listDocuments();

    user1.editDocument("Document1", "Updated content of Document 1", "Alice");
    user2.editDocument("Document2", "Updated content of Document 2", "Bob");

    user1.shareDocument("Document1", "Bob");

    user1.viewDocument("Document1");
    user2.viewDocument("Document2");

    user1.deleteDocument("Document2");

    user1.listDocuments();

    return 0;
}
