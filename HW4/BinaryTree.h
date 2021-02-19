#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include <iostream>
#include "Stack.h"

using namespace std;

class BinaryTree
{
 public:
  //Constructor
  BinaryTree();
  //Copy Constructor
  BinaryTree(const BinaryTree&);
  //Destructor
  ~BinaryTree();
  
  // Insertion method
  void insert(int);
  TreeNode* getRoot();
  
  //operators
  const BinaryTree& operator= (const BinaryTree& rhs);
  bool operator== (const BinaryTree& rhs);
  bool operator!= (const BinaryTree& rhs);
  void operator+= (const BinaryTree& rhs);
  BinaryTree operator+ (const BinaryTree& rhs);
  void operator+= (int number);
  BinaryTree operator+(int number);
  

  friend BinaryTree operator+(int number, const BinaryTree&);
  friend ostream& operator<<(ostream&, const BinaryTree&);

 private:
  //The root of the tree
  TreeNode* root;

  friend class Iterator;
};

class Iterator{
 public:
  //Constructor
  Iterator();

  void Init(const BinaryTree& );
  bool hasNext();
  TreeNode* Next(); //Actually the same with InOrderNext()
  TreeNode* InOrderNext(); 
  TreeNode* PreOrderNext(); 
  
 private:
  TreeNode* myCurrent;
  Stack* stack;
  Stack* stack2;
};

#endif
