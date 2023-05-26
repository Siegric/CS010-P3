#ifndef __NODE_H
#define __NODE_H

#include <string>

using namespace std;

class Node {

  friend class Tree;

private:
  string small;
  string large;

  Node *left;
  Node *middle;
  Node *right;
  Node *parent;

  Node();
  Node(const string&);
  bool isLeaf();
  bool isFull();
  bool hasKey(const string&);
  void insertKey(const string&);
};

#endif // __NODE_H
