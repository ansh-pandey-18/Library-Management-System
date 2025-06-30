# 📚 Library Management System in C++

This project implements a **Library Management System** using core **C++ data structures** like Linked Lists, Binary Search Trees (BST), Stacks, Queues, and arrays, while incorporating classical algorithms like **Rabin-Karp**, **Naive String Matching**, **LCS**, **Selection Sort**, and **Floyd-Warshall** for extended functionalities.

---

## 🔧 Features

- **Add Books** with metadata (ID, title, author, year, copies).
- **Display Books** via:
  - Singly Linked List (Insertion Order)
  - Binary Search Tree (Sorted by Book ID)
- **Issue Books** and maintain:
  - Availability count
  - Book issue history (last 5 via stack)
  - Issued records using maps
  - Waiting list for unavailable books
- **Return Books** with:
  - Waiting list handling
  - Automatic notification
- **Book Search**:
  - Linear Search (case-insensitive)
  - Binary Search (requires sorting)
  - Rabin-Karp Search (for publication year)
- **Sorting**:
  - Selection Sort by title
- **Title Comparison**:
  - Longest Common Subsequence (LCS)
- **Delivery**:
  - Shortest path calculation via **Floyd-Warshall Algorithm**
  - Book delivery simulation based on user location

---

## 🧱 Data Structures Used

| Data Structure | Purpose |
|----------------|---------|
| `LinkedList`   | Maintains book insertion order |
| `BST`          | Enables sorted book storage by ID |
| `Stack`        | Tracks last 5 book issue/return events |
| `Queue`        | Manages waiting lists per book |
| `Map`          | Tracks issued history and waiting queues |

---

## 🧠 Algorithms Implemented

- **Naive String Matching** – For simple substring match
- **Rabin-Karp Algorithm** – For fast publication year search
- **Selection Sort** – For sorting book titles
- **Binary Search** – On sorted book titles
- **Longest Common Subsequence (LCS)** – Compares similarity between titles
- **Floyd-Warshall Algorithm** – All-pairs shortest path for delivery simulation

---

## 🖥️ Program Flow (Menu-Driven CLI)

```text
1. Add Book
2. Display Books (Linked List)
3. Display Books (BST)
4. Issue Book
5. Return Book
6. Search Book (Linear Search)
7. Search Book (Binary Search)
8. Sort Books (Selection Sort)
9. Display Waiting List
10. View Last 5 Book Operations (History)
11. Compare Two Book Titles (LCS)
12. Issue a Book with Delivery
13. Search Books by Year (Rabin-Karp)
0. Exit