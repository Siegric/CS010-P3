#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#include "Tree.h"

// Default constructor
Tree::Tree() {
    root = nullptr;
}

Tree::~Tree() {
    // Call a private helper function to delete all nodes recursively
    destroyTree(root);
}

//Wrapper function for searchNode
bool Tree::search(const string& value) const {
    return searchNode(root, value);
}

//Recursively searches through the tree to find matching value
bool Tree::searchNode(Node* node, const string& key) const {
    if (node == nullptr) {
        return false; //Empty tree or reaches the end of tree
    }
    
    if (node->small == key || node->large == key) {
        return true;
    }
    
    if (node->small.empty()) {
        //Should never happen so returns false
        return false;
    }
    
    //Recursively searches through the tree
    if (key < node->small) {
        return searchNode(node->left, key);
    } else if (node->large.empty() || key > node->large) {
        return searchNode(node->right, key);
    } else {
        return searchNode(node->middle, key);
    }
}

//Helper that recursively frees all node in tree
void Tree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->middle);
        destroyTree(node->right);
        delete node;
    }
}

//Wrapper for insertion
void Tree::insert(const string& key) {
    if (search(key)) {
        return; //Does not insert anything if there already exists node with key data in the tree
    }
    //Case 1: Inserting into empty tree
    if (root == nullptr) {
        root = new Node(key); //Simply makes a new node and set root to point to it
    } else {
        insert(root, key);
    }
}

//Insertion
Node* Tree::insert(Node* curr, const string& key) {
    if (search(key)) {
        return nullptr; //Does not insert if duplicate value found
    }
    //Premptively splits nodes if encountered while inserting
    if (curr->isFull()) {
        curr = splitNode(curr, curr->parent, key);
    } 
    //Locating sutable leaf node for the insertion
    if (!curr->isLeaf()) {
        if (key < curr->small) {
            return insert(curr->left, key);
        } else if (curr->large == "" || key < curr->large) {
            return insert(curr->middle, key);
        } else {
            return insert(curr->right, key);
        }
    } else {
        insertLeaf(curr, key);
        return curr;
    }
}

void Tree::insertLeaf(Node* curr, const string& key) {
    if (search(key)) {
        return;
    } else if (key < curr->small) {
        curr->large = curr->small;
        curr->small = key;
    } else if (key > curr->small) {
        curr->large = key;
    }
}
//Helper for splitting up nodes
Node* Tree::splitNode(Node* curr, Node* parent, const string& key) {
    if (!curr->isFull()) {
        return nullptr; //Does nothing if curr is not full hence no need to split
    }

    //Declaring points to store new nodes allocated
    Node* splitLeft = nullptr;
    Node* splitRight = nullptr;
    string midStr;

    //Determines which value gets to be promoted to parent
    if (key < curr->small) {
        midStr = curr->small;
        splitLeft = new Node(key);
        splitRight = new Node(curr->large);
    } else if (key > curr->small && key < curr->large) {
        midStr = key;
        splitLeft = new Node(curr->small);
        splitRight = new Node(curr->large);
    } else {
        midStr = curr->large;
        splitLeft = new Node(curr->small);
        splitRight = new Node(key);
    }

    //If split node was root
    if (parent != nullptr) {
        insertChildren(parent, midStr, splitLeft, splitRight);
    } else {
        parent = new Node(midStr);
        parent->left = splitLeft;
        parent->middle = splitRight;
        root = parent;
        splitLeft->parent = root;
        splitRight->parent = root;
    }
    return parent;
}

//Handles insertion to a node with children that was split as well
void Tree::insertChildren(Node* parent, const string& key, Node* splitLeft, Node* splitRight) {
    if (key < parent->small) {
        parent->large = parent->small;
        parent->small = key;
        if (splitLeft->small < parent->small) {
            parent->left = splitLeft;
            parent->right = parent->middle;
            parent->middle = splitRight;
        } else {
            parent->middle = splitLeft;
            parent->right = splitRight;
        }
        splitLeft->parent = parent;
        splitRight->parent = parent;

    } else {
        parent->large = key;
        if (splitLeft->small < parent->small) {
            parent->left = splitLeft;
            parent->right = parent->middle;
            parent->middle = splitRight;
        } else {
            parent->middle = splitLeft;
            parent->right = splitRight;
        }
        splitLeft->parent = parent;
        splitRight->parent = parent;
    }
}

//Wrapper for preOrder
void Tree::preOrder() const {
    preOrderTraversal(root);
    cout<<endl;
}
//Standard pre order traversal
void Tree::preOrderTraversal(Node* node) const {
    if (node != nullptr) {
        cout << node->small << ", ";
        
        preOrderTraversal(node->left);

        if (!node->large.empty()) {
            cout << node->large << ", ";
        }
       preOrderTraversal(node->middle);

        preOrderTraversal(node->right);
    }
}
//Wrapper for postOrder
void Tree::postOrder() const {
    postOrderTraversal(root);
    cout<<endl;
}
//Standard post order traversal
void Tree::postOrderTraversal(Node* node) const {
    if (node != nullptr) {
        postOrderTraversal(node->left);
        postOrderTraversal(node->middle);

        cout << node->small << ", ";
        
        postOrderTraversal(node->right);
        
        if (!node->large.empty()) {
            cout << node->large << ", ";
        }
    }
}
//Wrapper for in order
void Tree::inOrder() const {
    inOrderTraversal(root);
    cout<<endl;
}
//Standard in order travsersal
void Tree::inOrderTraversal(Node* node) const {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        
        cout << node->small << ", ";
        
        inOrderTraversal(node->middle);
        if (!node->large.empty()) {
            cout << node->large << ", ";
        }
        
        inOrderTraversal(node->right);

    }
}
//Remove implementation
void Tree::remove(const string& key) {
    //Case 1: Tree is empty
    if (root == nullptr) {
        return;
    }
    if (!root->isLeaf()) {
        //Case 5.1: Root with only 1 data
        if (!root->isFull()) {
            //Case 5.1.1: Root has 1 key and has children that are 1 data leaf
            if (root->hasKey(key) && !root->left->isFull() && !root->middle->isFull()) {
                root->small = root->left->small;
                root->large = root->middle->small;
                delete root->left;
                delete root->middle;
                root->left = nullptr;
                root->middle = nullptr;
                return;
            } else if (root->hasKey(key) && !root->middle->isFull()) {
                root->small = root->left->large;
                root->left->large = "";
                return;
            } else if (root->hasKey(key) && !root->left->isFull()) {
                root->small = root->middle->small;
                root->middle->small = root->middle->large;
                root->middle->large = "";
                return;
            }
            if (root->left->hasKey(key)) {
                if (!root->left->isFull()) {
                    root->large = root->middle->small;
                    delete root->left;
                    delete root->middle;
                    root->left = nullptr;
                    root->middle = nullptr;
                    return;
                } else {
                    if (root->left->small == key) {
                        root->left->small = root->left->large;
                        root->left->large = "";
                    } else if (root->left->large == key) {
                        root->left->large = "";
                    }
                    return;
                }
            } else if (root->middle->hasKey(key)) {
                if (!root->middle->isFull()) {
                    root->large = root->small;
                    root->small = root->left->small;
                    delete root->left;
                    delete root->middle;
                    root->left = nullptr;
                    root->middle = nullptr;
                    return;
                } else {
                    if (root->middle->small == key) {
                        root->middle->small = root->middle->large;
                        root->middle->large = "";
                    } else if (root->middle->large == key) {
                        root->middle->large = "";
                    }
                    return;
                }
            }
        } 
    }
    //Case 2: Key is not found in tree
    if (!search(key)) {
        return;
    }
    //Case 3: Removing key in 1 data root
    if (root->isLeaf() && root->hasKey(key) &&!root->isFull()) {
        delete root;
        root = nullptr;
        return;
    }
    //Case 4: Remove key in 2 data root
    if (root->isLeaf() && root->isFull() && root->hasKey(key)) {
        if (root->small == key) {
            Node* newRoot = new Node(root->large);
            delete root;
            root = newRoot;
        } else if (root->large == key) {
            Node* newRoot = new Node(root->small);
            delete root;
            root = newRoot;
            
        }
        return;
    }
}

Node* Tree::remove(Node* curr, const string& key) {
    // Helper function to remove a key from a node
    // Returns the new node that replaces the current node

    // Find the child node to proceed with the removal
    Node* child;
    if (key < curr->small) {
        child = curr->left;
    } else if (curr->large == "" || key < curr->large) {
        child = curr->middle;
    } else {
        child = curr->right;
    }

    if (child->isLeaf()) {
        // Case 5.1: Remove key from a leaf child node

        if (!child->isFull()) {
            // Case 5.1.1: Leaf child node becomes empty after removal
            if (child->small == key) {
                delete child;
                curr->left = nullptr;
                curr->small = curr->large;
                curr->large = "";
            } else {
                delete child;
                curr->middle = nullptr;
                curr->large = "";
            }

            // Check if the current node becomes empty after removal
            if (curr->small == "") {
                delete curr;
                return nullptr; // Signal to replace the root node
            }
        } else {
            // Case 5.1.2: Remove key from a full leaf child node
            if (child->small == key) {
                child->small = child->large;
                child->large = "";
            } else {
                child->large = "";
            }
        }
    } else {
        // Case 5.2: Remove key from an internal node with children
        curr = remove(child, key);

        // Check if the current node becomes empty after removal
        if (curr != nullptr && curr->small == "") {
            delete curr;
            return nullptr; // Signal to replace the root node
        }
    }

    return curr; // Return the updated current node
}