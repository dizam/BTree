#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
                   BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;
  
} // LeafNode::getMinimum()

LeafNode* LeafNode::insert(int value)
{
  if (!getCount())
  {
    values[0] = value;
    count++;
    return NULL;
  } //inserting first value into empty list
  else if (!(getCount() == leafSize))
  {
    int i = 0;
    while (value > values[i])
    {
      i++;
      if (i == getCount())
      {
        values[i] = value;
        count++;
        return NULL;
      } //insert at the end of the leaf
    } //skip over values that are less than the element
    if (i < getCount())
    {
      for (int p = getCount()-1; p >= i; p--)
      {
        values[p+1] = values[p];
      } //duplicate values starting backwards
      values[i] = value;
      count++;
      return NULL;
    } //duplicate insert
  } //inserting when the leaf is not at full capacity, and not full
  else
  {
    if (leftSibling && leftSibling->getCount() != leafSize)
    {
      if (value < values[0])
      {
        leftSibling->insert(value);
      }
      else
      {
        leftSibling->insert(values[0]);
        count--;
        for (int r = 0; r < getCount(); r++)
        {
          values[r] = values[r+1];
        } // decremented count takes care of second insert
        this->insert(value);
      }
    } //pass left
    else if (rightSibling && rightSibling->getCount() != leafSize)
    {
      if (value > values[getCount() - 1])
      {
        rightSibling->insert(value);
      }
      else
      {
        rightSibling->insert(values[getCount() - 1]);
        count--;
        this->insert(value);
      }
    } //pass right
    else
    {
      LeafNode* right = new LeafNode(leafSize, parent, this, rightSibling);
      if (rightSibling)
      {
        rightSibling->setLeftSibling(right);
      }
      rightSibling = right;
      if (leafSize % 2 != 0)
      {
        if (value < values[leafSize/2])
        {
          for (int lhs = leafSize/2; lhs < leafSize; lhs++)
          {
            right->insert(values[lhs]);
          }
          count = 0;
          for (int left = 0; left < leafSize/2; left++)
          {
            insert(values[left]);
          }
          insert(value);
        }
        else if (value > values[leafSize/2])
        {
          count -= leafSize/2;
          right->insert(value);
          for (int rhs = leafSize/2 + 1; rhs < leafSize; rhs++)
          {
            right->insert(values[rhs]);
          }
        }
        return right;
      } //if leafSize is odd
      else
      {
        if (value < values[leafSize/2 - 1])
        {
          for (int reven = leafSize/2 - 1; reven < leafSize; reven++)
          {
            right->insert(values[reven]);
          }
          count = 0;
          for (int leven = 0; leven < leafSize/2 - 1; leven++)
          {
            insert(values[leven]);
          }
          insert(value);
        }
        else if (value > values[leafSize/2 - 1])
        {
          count -= leafSize/2;
          for (int rodd = leafSize/2; rodd < leafSize; rodd++)
          {
            right->insert(values[rodd]);
          }
          right->insert(value);
        }
        return right;
      } //even leafSize
    } //split
  } //Leaf Node is full
  // students must write this
  return NULL;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


