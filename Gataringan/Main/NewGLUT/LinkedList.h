#pragma once

#include "SceneObject.h"
#include "Structures.h"

/*
   The node in our list will contain TWO variables, "data" and "next".
   data is whatever the node holds as informarion, whilst "next" is 
   for containing the next node on the list
*/

struct Node
{
	SceneObject* data; //This can be changed to contain any data type
	Node* next;
};

class LinkedList
{
	private:
		Node* _head; //the very beginning of the list
		int _listSize;
	public:
		LinkedList();
		~LinkedList(void);

		//Insert Node
		Node* NewNode(SceneObject* d);
		void InsertNodeAtFront(Node* node);
		void InsertNodeSpecific(Node* beforeNode, Node* node);

		//Update
		void UpdateAll(Node* pos);

		//Draw
		void DrawAll(Node* pos);

		//Search
		int CountNodes(Node* pos, int count);//Must enter start position (e.g. head) as first in order to count, and 0 for the recursion
		Node* FindNode(int s, int pos, Node* p);//s = starting search position, pos = requested position, will require the head of the list
		Node* FindData(SceneObject* d, Node* pos);

		//Delete
		void DeleteNode(Node* beforeNode);
		void DeleteHead();
		void DeleteList();

		//Get functions
		Node* GetFront() const { return _head; }; 
		int GetSize() const { return _listSize; };
};