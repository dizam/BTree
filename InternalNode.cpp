#include <iostream>
#include "InternalNode.h"
using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
                           InternalNode *p, BTreeNode *left, BTreeNode *right) :
BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  int i = 1;
  BTreeNode *ptr;
  while (value > keys[i])
  {
    if (i == getCount())
    {
      ptr = children[i-1]->insert(value);
      this->setMinKeys(i);
      if (ptr)
      {
        if (internalSize == getCount())
        {
          if (leftSibling && leftSibling->getCount() != internalSize)
          {
	    if (value < keys[0])
	      {
		((InternalNode* )(leftSibling))->insert(ptr);
		return NULL;
	      }
            ((InternalNode *) (leftSibling))->insert(children[0]);
            count--;
            this->decrementChildren();
            this->insert(ptr);
            return NULL;
          }
          if (rightSibling && rightSibling->getCount() != internalSize)
          {	 
	    if (value > keys[count - 1])
	      {
		((InternalNode* )(rightSibling))->insert(ptr);
		return NULL;
	      }
            ((InternalNode *) (rightSibling))->insert(children[getCount()-1]);
            count--;
            this->insert(ptr);
            return NULL;
          }
          InternalNode *right = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
          if (rightSibling)
          {
            rightSibling->setLeftSibling(right);
          }
          rightSibling = right;
          if (internalSize % 2 == 0)
          {
            this->setInternalValues(right, ptr, value);
            return right;
          }//even internalSize
          else
          {
            if (value < keys[internalSize/2])
            {
              for (int lhs = internalSize/2; lhs < internalSize; lhs++)
              {
                right->insert(children[lhs]);
              }
              count = 0;
              for (int left = 0; left < internalSize/2; left++)
              {
                insert(children[left]);
              }
              insert(ptr);
            }
            else
            {
              count-=internalSize/2;
              right->insert(ptr);
              for (int rhs = internalSize/2 + 1; rhs < internalSize; rhs++)
              {
                right->insert(children[rhs]);
              }
            }
            return right;
          }
        } //internal node split
        else
        {
          children[i] = ptr;
          keys[i] = children[i]->getMinimum();
          count++;
        }
        return NULL;
      }
      return NULL;
    }
    i++;
  }
  if (i == getCount())
  {
    ptr = children[i-1]->insert(value);
    this->setMinKeys(i);
    if (ptr)
    {
      if (internalSize == getCount())
      {
        if (leftSibling && leftSibling->getCount() != internalSize)
        {
	  if (value < keys[0])
	    {
	      ((InternalNode *)(leftSibling))->insert(ptr);
	      return NULL;
	    }
          ((InternalNode *) (leftSibling))->insert(children[0]);
          count--;
          this->decrementChildren();
          this->insert(ptr);
          return NULL;
        }
        if (rightSibling && rightSibling->getCount() != internalSize)
        {
	  if (value > keys[count - 1])
	    {
	      ((InternalNode *)(rightSibling))->insert(ptr);
	      return NULL;
	    }
          ((InternalNode *) (rightSibling))->insert(children[getCount()-1]);
          count--;
          this->insert(ptr);
          return NULL;
        }
        InternalNode *right = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
        if (rightSibling)
        {
          rightSibling->setLeftSibling(right);
        }
        rightSibling = right;
        if (internalSize % 2 == 0)
        {
          this->setInternalValues(right, ptr, value);
          return right;
        }//even internalSize
        else
        {
          return (this->returnRight(value, ptr, right)); 
        }
      } //internal node split
      else
      {
        children[i] = ptr;
        keys[i] = children[i]->getMinimum();
        count++;
      }
      return NULL;
    }
    return NULL;
  }
  ptr = children[i-1]->insert(value);
  this->findAndSetChildMinimum(i);
  getMinimum(this);
  if (ptr)
  {
    if (internalSize == getCount())
    {
        if (leftSibling && leftSibling->getCount() != internalSize)
        {
	  if (value < keys[0])
	    {
	      ((InternalNode *)(leftSibling))->insert(ptr);
	      return NULL;
	    }
          ((InternalNode *) (leftSibling))->insert(children[0]);
          count--;
          this->decrementChildren();
          this->insert(ptr);
          return NULL;
        }
        if (rightSibling && rightSibling->getCount() != internalSize)
        {
	  if (value > keys[count - 1])
	    {
	      ((InternalNode *)(rightSibling))->insert(ptr);
	      return NULL;
	    }
          ((InternalNode *) (rightSibling))->insert(children[getCount()-1]);
          count--;
          this->insert(ptr);
          return NULL;
        }
      return (this->decideSplit(value, ptr));
    } //internal node split
    else
      {
      this->setKeys(i, ptr);
      count++;
      return NULL;
      }
  }
  return NULL;
} // InternalNode::insert()

InternalNode* InternalNode::returnRight(int value, BTreeNode *temp, InternalNode *rh)
{
  if (value < keys[internalSize/2])
  {
    for (int lhs = internalSize/2; lhs < internalSize; lhs++)
    {
      rh->insert(children[lhs]);
    }
    count = 0;
    for (int left = 0; left < internalSize/2; left++)
    {
      insert(children[left]);
    }
    insert(temp);
  }
  else
  {
    count-=internalSize/2;
    rh->insert(temp);
    for (int rhs = internalSize/2 + 1; rhs < internalSize; rhs++)
    {
      rh->insert(children[rhs]);
    }
  }
  return rh;

}

void InternalNode::decrementChildren()
{
  for (int r = 0; r < getCount(); r++)
  {
    keys[r] = keys[r+1];
    children[r] = children[r+1];
  } // decremented count takes care of second insert
}

void InternalNode::setMinKeys(int counter)
{
  if (children[counter-1]->getMinimum() != keys[counter-1])
  {
    keys[counter-1] = children[counter-1]->getMinimum();
  }
  if (rightSibling)
  {
    for (int i = 0; i < rightSibling->getCount(); i++)
    {
      ((InternalNode *) rightSibling)->keys[i] = ((InternalNode *) rightSibling)->children[i]->getMinimum();
    }
  }
  if (leftSibling)
  {
    for (int i = 0; i < leftSibling->getCount(); i++)
    {
      ((InternalNode *) leftSibling)->keys[i] = ((InternalNode *) leftSibling)->children[i]->getMinimum();
    }
  }
  
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  oldRoot->setParent(this);
  node2->setParent(this);
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count+=2;
  // students must write this
} // InternalNode::insert()

void InternalNode::setInternalValues(InternalNode *rSide, BTreeNode *temp, int value)
{
  if (value < keys[internalSize/2 - 1])
  {
    for (int reven = internalSize/2 - 1; reven < internalSize; reven++)
    {
      rSide->insert(children[reven]);
    }
    count = 0;
    for (int leven = 0; leven < internalSize/2 - 1; leven++)
    {
      insert(children[leven]);
    }
    insert(value);
  }
  else
  {
    count -= internalSize/2;
    for (int rodd = internalSize/2; rodd < internalSize; rodd++)
    {
      rSide->insert(children[rodd]);
    }
    rSide->insert(temp);
  }
}

InternalNode* InternalNode::decideSplit(int value, BTreeNode *valid)
{
  InternalNode *right = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
  if (rightSibling)
  {
    rightSibling->setLeftSibling(right);
  }
  rightSibling = right;
  if (internalSize % 2 == 0)
  {
    if (value < keys[internalSize/2 - 1])
    {
      this->insertChildren(right, value, valid);
    }
    else
    {
      count -= internalSize/2;
      for (int rodd = internalSize/2; rodd < internalSize; rodd++)
      {
        right->insert(children[rodd]);
      }
      right->insert(valid);
    }
    return right;
  }//even internalSize
  else
  {
    this->splitInternalNode(value, valid, right);
    return right;
  }
}

void InternalNode::findAndSetChildMinimum(int iterator)
{
  if (children[iterator-1]->getMinimum() != keys[iterator-1])
  {
    keys[iterator-1] = children[iterator-1]->getMinimum();
  }
  if (iterator-2 >= 0)
  {
    if (children[iterator-2]->getMinimum() != keys[iterator-2])
    {
      keys[iterator-2] = children[iterator-2]->getMinimum();
    }
  }
  if (iterator < count)
  {
    if (children[iterator]->getMinimum() != keys[iterator])
    {
      keys[iterator] = children[iterator]->getMinimum();
    }
  }
}

void InternalNode::insertChildren(InternalNode *rightchild, int value, BTreeNode *left)
{
  for (int reven = internalSize/2 - 1; reven < internalSize; reven++)
  {
    rightchild->insert(children[reven]);
  }
  count = 0;
  for (int leven = 0; leven < internalSize/2 - 1; leven++)
  {
    insert(children[leven]);
  }
  insert(left);
}

void InternalNode::splitInternalNode(int value, BTreeNode *TBI, InternalNode *rightchild)
{
  if (value < keys[internalSize/2])
  {
    for (int lhs = internalSize/2; lhs < internalSize; lhs++)
    {
      rightchild->insert(children[lhs]);
    }
    count = 0;
    for (int left = 0; left < internalSize/2; left++)
    {
      insert(children[left]);
    }
    insert(TBI);
  }
  else
  {
    count-=internalSize/2;
    rightchild->insert(TBI);
    for (int rhs = internalSize/2 + 1; rhs < internalSize; rhs++)
    {
      rightchild->insert(children[rhs]);
    }
  }

}

void InternalNode::setKeys(int temp, BTreeNode *returned)
{
  for (int p = internalSize; p > temp; p--)
  {
    children[p] = children[p-1];
  }
  children[temp] = returned;
  for (int r = internalSize; r > temp; r--)
  {
    keys[r] = keys[r-1];
  }
  keys[temp] = children[temp]->getMinimum();
}

void InternalNode::getMinimum(BTreeNode *sibling)
{
  if (sibling->getRightSibling())
  {
    for (int i = 0; i < sibling->getRightSibling()->getCount(); i++)
    {
      ((InternalNode *) sibling->getRightSibling())->keys[i] = ((InternalNode *) sibling->getRightSibling())->children[i]->getMinimum();
    }
  }
  if (sibling->getLeftSibling())
  {
    for (int i = 0; i < sibling->getLeftSibling()->getCount(); i++)
    {
      ((InternalNode *) sibling->getLeftSibling())->keys[i] = ((InternalNode *) sibling->getLeftSibling())->children[i]->getMinimum();
    }
  }
}

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  if (getCount() == 0)
  {
    children[0] = newNode;
    keys[0] = newNode->getMinimum();
    count++;
    return;
  }
  int i = 0;
  int child = newNode->getMinimum();
  while (keys[i] < child)
  {
    i++;
    if (i == getCount())
    {
      children[i] = newNode;
      keys[i] = child;
      count++;
      return;
    }
  }
  if (i < getCount())
  {
    for (int p = getCount(); p > i; p--)
    {
      keys[p] = keys[p - 1];
      children[p] = children[p-1];
    }
    children[i] = newNode;
    keys[i] = child;
    count++;
    return;
  }
  // students may write this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;
  
  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;
  
  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);
  
} // InternalNode::print()


