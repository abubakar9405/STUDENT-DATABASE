#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
//mini project
// Student class with attributes
class student {
public:
    int roll_NO;
    string section, Name;
    float GPA;

    student() {} // Default constructor
    student(string name, int rollNumber, string section, float gpa)
        : Name(name), roll_NO(rollNumber), section(section), GPA(gpa) {}
};

// Tree node struct
class tree {
public:
    student obj;
    tree* lptr;
    tree* rptr;
    int height;

    tree(const student& obj1) : obj(obj1), height(1), lptr(nullptr), rptr(nullptr) {}
};

// AVL Tree class
class AVLTreeNode {
public:
    tree* root;

    // Constructor
    AVLTreeNode() : root(nullptr) {}

    // Helper functions
    tree* insert(tree* node, const student& student);
    tree* rotateLeft(tree* node);
    tree* rotateRight(tree* node);
    int getHeight(tree* node);
    int getBalanceFactor(tree* node);
    void inOrderTraversal(tree* node);
    tree* deleteNode(tree* root, int roll_NO);
    tree* search(tree* node, int roll_NO);
    tree* minValueNode(tree* node);
};

// Function to get the height of a node
int AVLTreeNode::getHeight(tree* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Function to get the balance factor of a node
int AVLTreeNode::getBalanceFactor(tree* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->lptr) - getHeight(node->rptr);
}

// Right rotation function
tree* AVLTreeNode::rotateRight(tree* y) {
    tree* x = y->lptr;
    tree* T2 = x->rptr;

    // Perform rotation
    x->rptr = y;
    y->lptr = T2;

    // Update heights
    y->height = max(getHeight(y->lptr), getHeight(y->rptr)) + 1;
    x->height = max(getHeight(x->lptr), getHeight(x->rptr)) + 1;

    return x; // New root after rotation
}

// Left rotation function
tree* AVLTreeNode::rotateLeft(tree* x) {
    tree* y = x->rptr;
    tree* T2 = y->lptr;

    // Perform rotation
    y->lptr = x;
    x->rptr = T2;

    // Update heights
    x->height = max(getHeight(x->lptr), getHeight(x->rptr)) + 1;
    y->height = max(getHeight(y->lptr), getHeight(y->rptr)) + 1;

    return y; // New root after rotation
}

// Insert function for AVL tree
tree* AVLTreeNode::insert(tree* node, const student& student) {
    if (node == nullptr) {
        return new tree(student); // Create a new node if position is found
    }

    if (student.roll_NO < node->obj.roll_NO) {
        node->lptr = insert(node->lptr, student);
    } else if (student.roll_NO > node->obj.roll_NO) {
        node->rptr = insert(node->rptr, student);
    } else {
        // Duplicate roll numbers are not allowed
        return node;
    }

    node->height = 1 + max(getHeight(node->lptr), getHeight(node->rptr));

    int balance = getBalanceFactor(node);

    // Left Left (LL) Case
    if (balance > 1 && student.roll_NO < node->lptr->obj.roll_NO)
        return rotateRight(node);

    // Right Right (RR) Case
    if (balance < -1 && student.roll_NO > node->rptr->obj.roll_NO)
        return rotateLeft(node);

    // Left Right (LR) Case
    if (balance > 1 && student.roll_NO > node->lptr->obj.roll_NO) {
        node->lptr = rotateLeft(node->lptr);
        return rotateRight(node);
    }

    // Right Left (RL) Case
    if (balance < -1 && student.roll_NO < node->rptr->obj.roll_NO) {
        node->rptr = rotateRight(node->rptr);
        return rotateLeft(node);
    }

    return node;
}

// In-order traversal to display students
void AVLTreeNode::inOrderTraversal(tree* node) {
    if (node == nullptr)
        return;
    inOrderTraversal(node->lptr);
    cout << "Roll No: " << node->obj.roll_NO << ", Name: " << node->obj.Name 
         << ", Section: " << node->obj.section << ", GPA: " << node->obj.GPA << endl;
    inOrderTraversal(node->rptr);
}

// Get the minimum value node in the tree
tree* AVLTreeNode::minValueNode(tree* node) {
    tree* current = node;
    while (current && current->lptr != nullptr)
        current = current->lptr;
    return current;
}

// Delete a student node
tree* AVLTreeNode::deleteNode(tree* root, int roll_NO) {
    if (root == nullptr) {
        cout << "Student not found!" << endl;
        return root;
    }

    if (roll_NO < root->obj.roll_NO) {
        root->lptr = deleteNode(root->lptr, roll_NO);
    } else if (roll_NO > root->obj.roll_NO) {
        root->rptr = deleteNode(root->rptr, roll_NO);
    } else {
        // Node to be deleted found
        if (root->lptr == nullptr || root->rptr == nullptr) {
            tree* temp = root->lptr ? root->lptr : root->rptr;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp; // Copy the contents of the non-empty child
            }
            delete temp;
        } else {
            // Node with two children: Get the inorder successor
            tree* temp = minValueNode(root->rptr);
            root->obj = temp->obj; // Copy the inorder successor's content to this node
            root->rptr = deleteNode(root->rptr, temp->obj.roll_NO); // Delete the inorder successor
        }
    }

    if (root == nullptr) {
        return root;
    }

    root->height = 1 + max(getHeight(root->lptr), getHeight(root->rptr));

    int balance = getBalanceFactor(root);

    // Left Left (LL) Case
    if (balance > 1 && getBalanceFactor(root->lptr) >= 0)
        return rotateRight(root);

    // Right Right (RR) Case
    if (balance < -1 && getBalanceFactor(root->rptr) <= 0)
        return rotateLeft(root);

    // Left Right (LR) Case
    if (balance > 1 && getBalanceFactor(root->lptr) < 0) {
        root->lptr = rotateLeft(root->lptr);
        return rotateRight(root);
    }

    // Right Left (RL) Case
    if (balance < -1 && getBalanceFactor(root->rptr) > 0) {
        root->rptr = rotateRight(root->rptr);
        return rotateLeft(root);
    }

    return root;
}

// Search a student by roll number
tree* AVLTreeNode::search(tree* node, int roll_NO) {
    if (node == nullptr || node->obj.roll_NO == roll_NO)
        return node;

    if (roll_NO < node->obj.roll_NO)
        return search(node->lptr, roll_NO);

    return search(node->rptr, roll_NO);
};

class StudentsDatabase {
public:
    AVLTreeNode studentTree;

    void addStudent();
    void removeStudent();
    void searchStudent();
    void displayAllStudents();
};

// Function implementations
void StudentsDatabase::addStudent() {
    string name, section;
    int roll_NO;
    float GPA;

    cout << "Enter student's name: ";
    getline(cin, name);
    cout << "Enter student's roll number: ";
    cin >> roll_NO;
    cin.ignore(); // To clear the newline left by cin
    cout << "Enter student's section: ";
    getline(cin, section);
    cout << "Enter student's GPA: ";
    cin >> GPA;
    cin.ignore(); // To clear the newline left by cin

    student newStudent(name, roll_NO, section, GPA);
    studentTree.root = studentTree.insert(studentTree.root, newStudent);
}

void StudentsDatabase::removeStudent() {
    int roll_NO;
    cout << "Enter roll number of student to remove: ";
    cin >> roll_NO;
    studentTree.root = studentTree.deleteNode(studentTree.root, roll_NO);
}

void StudentsDatabase::searchStudent() {
    int roll_NO;
    cout << "Enter roll number to search: ";
    cin >> roll_NO;

    tree* foundStudent = studentTree.search(studentTree.root, roll_NO);
    if (foundStudent != nullptr) {
        cout << "Student found! Name: " << foundStudent->obj.Name << ", Section: "
             << foundStudent->obj.section << ", GPA: " << foundStudent->obj.GPA << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

void StudentsDatabase::displayAllStudents() {
    studentTree.inOrderTraversal(studentTree.root);
}

int main() {
    StudentsDatabase db;
    int choice;

    while (true) {
        cout << "\n1. Add Student\n2. Remove Student\n3. Search Student\n4. Display All Students\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To clear any leftover newline

        switch (choice) {
            case 1:
                db.addStudent();
                break;
            case 2:
                db.removeStudent();
                break;
            case 3:
                db.searchStudent();
                break;
            case 4:
                db.displayAllStudents();
                break;
            case 5:
                exit(0);
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
