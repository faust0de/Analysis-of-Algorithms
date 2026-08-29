#include <iostream>
using namespace std;

//9. Create your own Binary Tree data structure and test the insertion of elements. Insertion will
//need its own algorithm, research on how to implement it.

class Node{
    public:
        int data;
        Node* left;
        Node* right;

        Node(int value) {
            data = value;
            left = nullptr;
            right = nullptr;
        }
};

class BinaryTree {
    private:
        Node* root;

        void insert(Node*& node, int value) {
            if (node == nullptr) {
                node = new Node(value);
            } else if (value < node->data) {
                insert(node->left, value);
            } else {
                insert(node->right, value);
            }
        }

        void print(Node* node){
            if (node !=nullptr){
                print(node->left);
                cout << node->data << " ";
                print(node->right);
            }
        }

    public:
        BinaryTree() {
            root = nullptr;
        }

        void insert(int value) {
            insert(root, value);
        }
        
        void print() {
            print(root);
            cout << endl;
        }
};

int main() {
    BinaryTree tree;

    // Inserting elements into the binary tree
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    //Printing the elements of the binary tree in order
    tree.print(); 

    return 0;
}