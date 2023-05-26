#include <iostream>
#include <string>
using namespace std;

#include "Node.h"

Node::Node() : small(""), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

Node::Node(const string& value) : small(value), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

 bool Node::isLeaf() {
    return (left == nullptr && middle == nullptr && right == nullptr);
 }

 bool Node::isFull() {
    return (!small.empty() && !large.empty());
 }

 bool Node::hasKey(const string& key) {
   return (small == key || large == key);
 }

 void Node::insertKey(const string& key) {
   if (!isFull()) {
      if (small.empty()) {
         small = key;
      } else if (large.empty()) {
         large = key;
      }
   }
   if (small > large) {
      swap(small, large);
   }
 }