#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void getMinimum(BTreeNode *sibling);
  void setKeys(int temp, BTreeNode *returned);
  void splitInternalNode(int value, BTreeNode *TBI, InternalNode *rhs);
  void insertChildren(InternalNode *rightchild, int value, BTreeNode *left);
  void findAndSetChildMinimum(int iterator);
  InternalNode* decideSplit(int value, BTreeNode *valid);
  void setMinKeys(int counter);
  void setInternalValues(InternalNode *rSide, BTreeNode *temp, int value);
  void decrementChildren();
  InternalNode* returnRight(int value, BTreeNode *temp, InternalNode *rh);
}; // InternalNode class

#endif
