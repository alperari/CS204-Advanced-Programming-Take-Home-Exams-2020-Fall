#include "BinaryTree.h"
#include <iostream>

using namespace std;

//Constructor
BinaryTree::BinaryTree(){
  root = nullptr;
}

//Copy Constructor
BinaryTree::BinaryTree(const BinaryTree& rhs){
  
  root = nullptr;
  
  Iterator iter;
  iter.Init(rhs);
    
  while(iter.hasNext()){
    this->insert(iter.InOrderNext()->value);
  }
}

//Destructor
BinaryTree::~BinaryTree(){
  Iterator iter;
  iter.Init(*this);
    
  while(iter.hasNext()){
    delete iter.InOrderNext();
  }
}

TreeNode* BinaryTree::getRoot(){
  return this->root;
}

//First create the new node with the given value
void BinaryTree::insert(int num) {
  TreeNode *newNode,	// Pointer to a new node
    *nodePtr;	// Pointer to traverse the tree
  
  // Create a new node
  newNode = new TreeNode;
  newNode->value = num;
  newNode->left = newNode->right = nullptr;
  
  if (!root)	// Is the tree empty?
    root = newNode;
  else 
    {
      //we have a root, need to find the location for the next item
      nodePtr = root; 
      while (nodePtr) {
	if (num < nodePtr->value) {
	  if (nodePtr->left)
	    nodePtr = nodePtr->left;
	  else {
	    nodePtr->left = newNode;
	    break;
	  }
	}
	else if (num > nodePtr->value) {
	  if (nodePtr->right)
	    nodePtr = nodePtr->right;
	  else {
	    nodePtr->right = newNode;
	    break;
	  }
	}
	else { //num is not greater or smaller. So it is equal
	  cout << "Duplicate value found in tree.\n";
	  break;
	}
      }		
    }
}

//Operators
const BinaryTree& BinaryTree::operator= (const BinaryTree& rhs){
  if(this!=&rhs)
    {
      this->~BinaryTree();
      this->root = nullptr;
      
      Iterator iter;
      iter.Init(rhs);
      while (iter.hasNext()) {
	this->insert(iter.Next()->value);
      }
    }
  return *this;
}
bool BinaryTree::operator== (const BinaryTree& rhs){
  if(this != &rhs){
    Iterator iter_lhs, iter_rhs;
    iter_lhs.Init(*this);
    iter_rhs.Init(rhs);
    while (iter_lhs.hasNext() && iter_rhs.hasNext()) {
      if(iter_lhs.Next()->value != iter_rhs.Next()->value){
	return false;
      }
    }
    if(iter_lhs.hasNext() || iter_rhs.hasNext()){
      return false;
    }
  }
  return true;
}
bool BinaryTree::operator!= (const BinaryTree& rhs){
  return !operator==(rhs);
}
void BinaryTree::operator+=(const BinaryTree& rhs){
  Iterator iter;
  iter.Init(rhs);
  while (iter.hasNext()) {
    this->insert(iter.Next()->value);
  }
}
BinaryTree BinaryTree::operator+(const BinaryTree& rhs){
  BinaryTree temp(*this);
  temp += rhs;
  return temp;
}
void BinaryTree::operator+=(int number){
  this->insert(number);
}
BinaryTree BinaryTree::operator+(int number){
  BinaryTree temp(*this);
  temp.insert(number);
  return temp;
}
BinaryTree operator+(int number, const BinaryTree& tree){
  BinaryTree temp(tree);
  temp.insert(number);
  return temp;
}

ostream & operator<< (ostream & os, const BinaryTree & rhs)
{
  Iterator iter;
  iter.Init(rhs);
  while (iter.hasNext()) {
    os << iter.Next()->value << " ";
  }
  os << endl;
  return os;
}


/*
 *  ITERATOR CLASS
 */

//Constructor
Iterator::Iterator()
  :myCurrent(nullptr), stack(nullptr), stack2(nullptr)
{}

void Iterator::Init(const BinaryTree &tree)
{
  myCurrent = tree.root;
  stack = new Stack();
}

bool Iterator::hasNext()
{
  return (!stack->isEmpty() || myCurrent != nullptr);
}

TreeNode* Iterator::Next()
{
  while (myCurrent != nullptr) {
    stack->push(myCurrent);
    myCurrent = myCurrent->left;
  }
  
  myCurrent = stack->pop();
  TreeNode* node = myCurrent;
  myCurrent = myCurrent->right;
  
  return node;
}

TreeNode* Iterator::InOrderNext()
{
  while (myCurrent != nullptr) {
    stack->push(myCurrent);
    myCurrent = myCurrent->left;
  }
  
  myCurrent = stack->pop();
  TreeNode* node = myCurrent;
  myCurrent = myCurrent->right;
  
  return node;
}


//This is just to show. Iterative post-order is much more complex.
TreeNode* Iterator::PreOrderNext()
{
  if(myCurrent->left != nullptr || myCurrent->right != nullptr){
    stack->push(myCurrent);
  }
  
  TreeNode* node = myCurrent;

  if(myCurrent->left == nullptr && myCurrent->right == nullptr){
    if(!stack->isEmpty()){
      myCurrent = stack->pop();
    }
    else{
      myCurrent = nullptr;
      return node;
    }
    if(myCurrent->right){
      myCurrent = myCurrent->right;
    }
    else{
      while(!myCurrent->right){
	myCurrent = stack->pop();
	myCurrent = myCurrent->right;
      }
    }   
  }
  else if(myCurrent->left){
    myCurrent = myCurrent -> left;
  }
  else if(myCurrent->right){
    myCurrent = stack->pop();
    myCurrent = myCurrent -> right;
  }

  return node;
}


  
