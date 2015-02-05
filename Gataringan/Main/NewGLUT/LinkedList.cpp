#include "LinkedList.h"

#include <iostream>

LinkedList::LinkedList()
{
	_head = nullptr;
	_listSize = 0;
}

LinkedList::~LinkedList(void){}

Node* LinkedList::NewNode(SceneObject* d)
{
	Node* n = new Node();
	n->data = d;
	n->next = nullptr;
	_listSize++;

	return n;
}

void LinkedList::InsertNodeAtFront(Node* newNode)
{

	if (_head == nullptr)
	{
		_head = newNode;
	}
	else
	{
		newNode->next = _head;
		_head = newNode;
	}

}

void LinkedList::InsertNodeSpecific(Node* pos, Node* newNode)
{
	newNode->next = pos->next;//"pos" is the node that the new node will be placed in front of
	pos->next = newNode;
}

void LinkedList::UpdateAll(Node* pos)
{

	if (pos != nullptr)
	{

		if (_head != 0)
		{

			if (pos->data != nullptr)
			{
				(pos->data)->Update();
				(pos->data)->UpdatePosition(0, 0, 0);
			}

			UpdateAll(pos->next);
		}

	}

}

void LinkedList::DrawAll(Node* pos)
{

	if (pos != nullptr)
	{

		if (_head != 0)
		{

			if (pos->data != nullptr)
				(pos->data)->Draw();

			DrawAll(pos->next);
		}

	}

}

int LinkedList::CountNodes(Node* pos, int count)
{

	if (pos != nullptr)
	{
		count++;
		count = CountNodes(pos->next, count++);
	}

	return count;
}


Node* LinkedList::FindNode(int s, int pos, Node* p)
{

	if (p != nullptr)
	{

		if (s == pos)
		{
			return p;
		}		
		else
		{
			s++;
			p = p->next;
			p = FindNode(s, pos, p);
		}
		
	}

	if (p != nullptr)
	{
		return p;
	}
	else
	{
		return nullptr; // if there's no valid node with the desire data to be found
	}

}

Node* LinkedList::FindData(SceneObject* d, Node* pos)
{

	if (pos != nullptr)
	{

		if (pos->data == d)
		{
			return pos;
		}		
		else
		{
			pos = FindData(d, pos->next);
		}		

	}

	if (pos != nullptr)
	{
		return pos;
	}
	else
	{
		return nullptr; // if there's no valid node with the desire data to be found
	}

}

void LinkedList::DeleteNode(Node* bPos) //This will delete the node in front of the node specified in the parameters
{
	Node* pTemp = new Node();

	if (bPos != nullptr && bPos->next != nullptr)
	{
		pTemp = bPos->next;
		bPos->next = pTemp->next;
		delete pTemp;
		pTemp = nullptr;
		_listSize--;
	}

}

void LinkedList::DeleteHead()
{
	delete _head;
	_head = nullptr;
}


void LinkedList::DeleteList()
{
	Node* pTemp = new Node();

	while (_head != nullptr)
	{
		pTemp = _head;
		_head = _head->next;
		delete pTemp;
		DeleteList();
		_listSize = 0;
	}

}