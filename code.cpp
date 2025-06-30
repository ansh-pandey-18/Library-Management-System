#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <stack>
#include <cctype>
#include <limits.h>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int publicationYear;
    int availableCopies;

    //Book() {}
    Book(int id, string title, string author, int year, int copies)
        : id(id), title(title), author(author), publicationYear(year), availableCopies(copies) {}

    string getTitle() const {
        return title;
    }

    int getPublicationYear() const {
        return publicationYear;
    }
};


struct Node {
    Book data;
    Node* next;
    Node(Book b) : data(b), next(nullptr) {}
};


class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

    void insert(Book b) {
        Node* newNode = new Node(b);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << "Book ID: " << temp->data.id << ", Title: " << temp->data.title
                 << ", Author: " << temp->data.author << ", Year: " << temp->data.publicationYear
                 << ", Available Copies: " << temp->data.availableCopies << endl;
            temp = temp->next;
        }
    }

    void remove(int bookID) {
        if (!head) return;
        if (head->data.id == bookID) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* prev = head;
        Node* curr = head->next;
        while (curr != nullptr && curr->data.id != bookID) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != nullptr) {
            prev->next = curr->next;
            delete curr;
        }
    }

    Book* searchByID(int bookID) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == bookID) {
                return &temp->data;
            }
            temp = temp->next;
        }
        return nullptr;
    }
};

queue<string> waitingList;

stack<Book> bookHistory;

struct BSTNode {
    Book data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Book b) : data(b), left(nullptr), right(nullptr) {}
};

class BST {
public:
    BSTNode* root;
    BST() : root(nullptr) {}

    void insert(Book b) {
        root = insertRec(root, b);
    }

    BSTNode* insertRec(BSTNode* node, Book b) {
        if (node == nullptr) {
            return new BSTNode(b);
        }
        if (b.id < node->data.id) {
            node->left = insertRec(node->left, b);
        } else {
            node->right = insertRec(node->right, b);
        }
        return node;
    }

    void inOrder(BSTNode* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << "Book ID: " << node->data.id << ", Title: " << node->data.title
                 << ", Author: " << node->data.author << ", Year: " << node->data.publicationYear
                 << ", Available Copies: " << node->data.availableCopies << endl;
            inOrder(node->right);
        }
    }

    BSTNode* searchByID(BSTNode* node, int bookID) {
    if (node == nullptr || node->data.id == bookID) {
        return node;
    }
    if (bookID < node->data.id) {
        return searchByID(node->left, bookID);
    } else {
        return searchByID(node->right, bookID);
        }
    }

    void display() {
        inOrder(root);
    }
};

void selectionSort(vector<Book>& books) {
    int n = books.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (books[j].title < books[minIdx].title) {
                minIdx = j;
            }
        }
        swap(books[i], books[minIdx]);
    }
    for(auto i:books)
    cout << "Book ID: " << i.id << ", Title: " << i.title
                 << ", Author: " << i.author << ", Year: " << i.publicationYear
                 << ", Available Copies: " << i.availableCopies << endl;
}

string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return result;
}

int linearSearch(vector<Book>& books, string title) {
    string searchTitle = toLowerCase(title);
    for (int i = 0; i < books.size(); i++) {
        if (toLowerCase(books[i].title) == searchTitle) {
            return i;
        }
    }
    return -1;
}

int binarySearch(vector<Book>& books, string title) {
    string searchTitle = toLowerCase(title);
    int low = 0, high = books.size() - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        string midTitle = toLowerCase(books[mid].title);

        if (midTitle == searchTitle) return mid;
        if (midTitle < searchTitle) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

map<int, queue<string>> issuedBooksHistory;

map<int, queue<string>> waitingLists;

void issueBook(LinkedList& list, BST& tree, int bookID, string user) {
    Book* book = list.searchByID(bookID);
    if (book != nullptr) {
        if (book->availableCopies > 0) {
            book->availableCopies--;

            BSTNode* bookTreeNode = tree.searchByID(tree.root, bookID);
            if (bookTreeNode != nullptr) {
                bookTreeNode->data.availableCopies--;
            }

            cout << "Book issued successfully to " << user << endl;

            bookHistory.push(*book);
            if (bookHistory.size() > 5) {
                bookHistory.pop();
            }

            issuedBooksHistory[bookID].push(user);

        } else {
            cout << "No copies available. Adding " << user << " to the waiting list for book ID "
                 << bookID << "." << endl;
            waitingLists[bookID].push(user);
        }
    } else {
        cout << "Book not found." << endl;
    }
}

void returnBook(LinkedList& list, BST& tree, int bookID, string user) {

    if (issuedBooksHistory.find(bookID) == issuedBooksHistory.end() || issuedBooksHistory[bookID].empty()) {
        cout << "This book was not issued to anyone or is invalid." << endl;
        return;
    }

    queue<string>& issuedQueue = issuedBooksHistory[bookID];
    bool isUserValid = false;

    queue<string> tempQueue = issuedQueue;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == user) {
            isUserValid = true;
            break;
        }
        tempQueue.pop();
    }

    if (!isUserValid) {
        cout << "This book was not issued to " << user << ". Return rejected." << endl;
        return;
    }

    Book* bookList = list.searchByID(bookID);
    BSTNode* bookTree = tree.searchByID(tree.root, bookID);

    if (bookList != nullptr && bookTree != nullptr) {

        bookList->availableCopies++;
        bookTree->data.availableCopies++;

        cout << "Book returned successfully by " << user << "." << endl;

        bookHistory.push(*bookList);
        if (bookHistory.size() > 5) {
            bookHistory.pop();
        }

        issuedQueue = issuedBooksHistory[bookID];
        while (!issuedQueue.empty()) {
            if (issuedQueue.front() == user) {
                issuedQueue.pop();
                break;
            }
            issuedQueue.pop();
        }

        if (issuedQueue.empty()) {
            issuedBooksHistory.erase(bookID);
        }

        if (waitingLists.find(bookID) != waitingLists.end() && !waitingLists[bookID].empty()) {
            string waitingUser = waitingLists[bookID].front();
            cout << "Notifying " << waitingUser << " from the waiting list for book ID " << bookID << "." << endl;
            waitingLists[bookID].pop();

            if (waitingLists[bookID].empty()) {
                waitingLists.erase(bookID);
            }
        }
    } else {
        cout << "Book not found in the system." << endl;
    }
}

int naiveStringMatching(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            return i;
        }
    }
    return -1;
}

void addBook(vector<Book>& books, LinkedList& list, BST& tree, Book b) {
    books.push_back(b);
    list.insert(b);
    tree.insert(b);
}

int LCS(string str1, string str2) {
    int m = str1.length();
    int n = str2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[m][n];
}


void compareBooksByTitle(string book1, string book2) {
    int lcsLength = LCS(book1, book2);
    cout << "Longest Common Subsequence Length between '" << book1 << "' and '" << book2 << "': " << lcsLength << endl;
}


void floydWarshall(vector<vector<int>>& dist, int V) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void deliverBook(const vector<vector<int>>& dist, int source, int destination) {
    if (dist[source][destination] == INT_MAX) {
        cout << "Delivery not possible between these locations." << endl;
    } else {
        cout << "Shortest delivery path from branch " << source << " to location " << destination
             << " is " << dist[source][destination] << " units." << endl;
    }
}

void issueBookWithDelivery(LinkedList& list, int bookID, string user, const vector<vector<int>>& dist, int userLocation, int libraryBranch) {
    Book* book = list.searchByID(bookID);
    if (book != nullptr) {
        if (book->availableCopies > 0) {
            book->availableCopies--;
            cout << "Book issued successfully to " << user << endl;
            bookHistory.push(*book);
            if (bookHistory.size() > 5) {
                bookHistory.pop();
            }

            deliverBook(dist, libraryBranch, userLocation);
        } else {
            cout << "No copies available. Adding " << user << " to the waiting list." << endl;
            waitingList.push(user);
        }
    } else {
        cout << "Book not found." << endl;
    }
}

vector<int> rabinKarpSearch(const string& text, const string& pattern) {
    vector<int> result;
    int m = pattern.length();
    int n = text.length();
    const int d = 256;
    const int q = 101;

    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    for (i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }


    for (i = 0; i <= n - m; i++) {
        if (p == t) {
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }

            if (j == m) {
                result.push_back(i);
            }
        }

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;

            if (t < 0) {
                t = t + q;
            }
        }
    }

    return result;
}


    vector<int> searchBooksByYear(const vector<Book>& books, const string& yearQuery) {
    vector<int> matchedBookIndexes;
    string combinedText = "";
    vector<int> bookStartIndices;
    int currentIndex = 0;
    for (int i = 0; i < books.size(); i++) {
        string year = to_string(books[i].getPublicationYear());
        combinedText += year + " ";
        bookStartIndices.push_back(currentIndex);
        currentIndex += year.size() + 1;
    }

    vector<int> result = rabinKarpSearch(combinedText, yearQuery);

    for (int position : result) {
        for (int i = 0; i < bookStartIndices.size(); i++) {
            if (position >= bookStartIndices[i] &&
                (i == bookStartIndices.size() - 1 || position < bookStartIndices[i + 1])) {
                matchedBookIndexes.push_back(i);
                break;
            }
        }
    }

    return matchedBookIndexes;
}



int main() {
    vector<Book> books;
    LinkedList list;
    BST tree;

    int V = 4;
    vector<vector<int>> dist = {
        {0, 5, INT_MAX, 10},
        {5, 0, 3, INT_MAX},
        {INT_MAX, 3, 0, 1},
        {10, INT_MAX, 1, 0}
    };


    floydWarshall(dist, V);

    addBook(books, list, tree, Book(1, "The God of Small Things", "Arundhati Roy", 1997, 2));
    addBook(books, list, tree, Book(2, "Autobiography Of Ansh", "Ansh Pandey", 2016, 1));
    addBook(books, list, tree, Book(3, "The Ramayana", "Valmiki", 500, 3));
    addBook(books, list, tree, Book(4, "The Guide", "R.K. Narayan", 1968, 4));
    addBook(books, list, tree, Book(5, "Gitanjali", "Rabindranath Tagore", 1910, 0));

    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Display Books (Linked List)\n";
        cout << "3. Display Books (BST)\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Search Book (Linear Search)\n";
        cout << "7. Search Book (Binary Search)\n";
        cout << "8. Sort Books (Selection Sort)\n";
        cout << "9. Display Waiting List\n";
        cout << "10. View Last 5 Book Operations (History)\n";
        cout << "11. Compare two book titles by LCS\n";
        cout << "12. Issue a Book with Delivery\n";
        cout << "13. Search Books by Year (Rabin-Karp)\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, year, copies;
                string title, author;
                cout << "Enter Book ID: "; cin >> id;
                cout << "Enter Title: "; cin.ignore(); getline(cin, title);
                cout << "Enter Author: "; getline(cin, author);
                cout << "Enter Publication Year: "; cin >> year;
                cout << "Enter Available Copies: "; cin >> copies;
                Book b(id, title, author, year, copies);
                addBook(books, list, tree, b);
                cout<<"Book successfully added to Library."<<endl;
                break;
            }
            case 2:
                list.display();
                break;
            case 3:
                tree.display();
                break;
            case 4: {
                int id;
                string user;
                cout << "Enter Book ID to issue: "; cin >> id;
                cout << "Enter User Name: "; cin.ignore(); getline(cin, user);
                issueBook(list, tree, id, user);
                break;
            }
            case 5: {
                int id;
                string user;
                cout << "Enter Book ID to return: "; cin >> id;
                cout << "Enter User Name: "; cin.ignore(); getline(cin, user);
                returnBook(list, tree, id, user);
                break;
            }
            case 6: {
                string title;
                cout << "Enter title to search: "; cin.ignore(); getline(cin, title);
                int index = linearSearch(books, title);
                if (index != -1) cout << "Book found: " << books[index].title << endl;
                else cout << "Book not found.\n";
                break;
            }
            case 7: {
                string title;
                cout << "Enter title to search: "; cin.ignore(); getline(cin, title);
                sort(books.begin(), books.end(), [](Book a, Book b) { return a.title < b.title; });
                int index = binarySearch(books, title);
                if (index != -1) cout << "Book found: " << books[index].title << endl;
                else cout << "Book not found.\n";
                break;
            }
            case 8:
                selectionSort(books);
                sort(books.begin(), books.end(), [](Book a, Book b) { return a.title < b.title; });
                cout << "Books sorted by title.\n";
                break;
            case 9:
            cout << "Waiting List:\n";
            if (waitingLists.empty()) {
            cout << "No one in the waiting list.\n";
            } else {
            for (const auto& entry : waitingLists) {
            int bookID = entry.first;
            const queue<string>& waitingQueue = entry.second;

            cout << "Book ID: " << bookID << "\n";
            queue<string> tempQueue = waitingQueue;
            while (!tempQueue.empty()) {
                cout << "Username - " << tempQueue.front() << endl;
                tempQueue.pop();
                }
            }
            }
                break;
            case 10: {
                cout << "Last 5 Book Operations (Issued/Returned):\n";
                stack<Book> tempHistory = bookHistory;
                int count = 0;
                while (!tempHistory.empty() && count < 5) {
                    Book b = tempHistory.top();
                    cout << "Book ID: " << b.id << ", Title: " << b.title
                         << ", Author: " << b.author << ", Year: " << b.publicationYear << endl;
                    tempHistory.pop();
                    count++;
                }
                break;
            }
            case 11: {
                string book1, book2;
                cout << "Enter first book title: ";
                cin.ignore();
                getline(cin, book1);
                cout << "Enter second book title: ";
                getline(cin, book2);
                compareBooksByTitle(book1, book2);
                break;
            }
            case 12: {
            int id;
            string user;
            int userLocation;
            cout << "Enter Book ID to issue: "; cin >> id;
            cout << "Enter User Name: "; cin.ignore(); getline(cin, user);
            cout << "Enter User Delivery Location (0-3): "; cin >> userLocation;
            issueBookWithDelivery(list, id, user, dist, userLocation, 0);
            break;
        }
        case 13: {
    string yearQuery;
    cout << "Enter publication year to search for: ";
    cin.ignore(); getline(cin, yearQuery);
    vector<int> foundBooks = searchBooksByYear(books, yearQuery);
    if (foundBooks.empty()) {
        cout << "No books found for the year " << yearQuery << ".\n";
    } else {
        cout << "Books found for the year " << yearQuery << ":\n";
        for (int index : foundBooks) {
            cout << books[index].getTitle() << endl;
        }
    }
    break;
}
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
