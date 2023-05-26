#ifndef __TREE_H
#define __TREE_H

#include "Node.h"

class Tree {
private:
  Node *root;

public:
  Tree( );
  ~Tree( );
  //Inserts a value into tree while following 2-3 tree properties
  void insert(const string &);
  //Prints tree in pre order
  void preOrder( ) const;
  //Prints tree in order
  void inOrder( ) const;
  //Prints tree in post order
  void postOrder( ) const;
  //Removes node from tree while following 2-3 tree properties
  void remove(const string &);
  //Returns true if key exists in any node in tree
  bool search (const string &) const;

private:
    void destroyTree(Node*);//destructor helper
    bool searchNode(Node*, const string&) const;//search helper
    Node* splitNode(Node*, Node*, const string&);//insert helper
    Node* insert(Node*, const string& key);//insert helper
    void insertChildren(Node*, const string&, Node*, Node*);//insert helper
    void insertLeaf(Node*, const string&);//insert helper
    void preOrderTraversal(Node*) const;
    void inOrderTraversal(Node*) const;
    void postOrderTraversal(Node*) const;
    Tree* operator=(Tree* rhs);//Assignment operator
    Tree(const Tree& rhs); // Copy Constructor
    Node* remove(Node*, const string&); //remove helper
};

#endif //__TREE_H
