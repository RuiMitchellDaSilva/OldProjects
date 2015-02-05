#pragma once

#include "MovableObject.h"
#include "Structures.h"
#include <sstream>
/*
Same structure to a Linked List, except there is the option to 
attach more than one node to each individual node, plus there is
a hierarchical system implemented
*/

struct TreeNode
{
	MovableObject* data;
	TreeNode* left;
	TreeNode* right;
};

class SceneGraph
{
	private:
		TreeNode* _root;
	public:
		SceneGraph();
		~SceneGraph(void);

		//Insert Node
		TreeNode* NewNode(MovableObject* d);
		TreeNode* NewNode(); //For easier linking 
		void InsertRootNode(TreeNode* newNode);

		//Update
		void UpdateTree(TreeNode* pos); //Can choose where to update
		void UpdatePositionOfTree(TreeNode* pos, float x, float y, float z);
		void RotateTree(TreeNode* pos, float xRV, float yRV, float zRV, float r);

		//Draw
		void DrawTree(TreeNode* pos); //Traverse to left TreeNode first, then print data before traversing to right TreeNode
	
		TreeNode* DepthFirstSearch(TreeNode* pos, MovableObject* searchData);
		//The searches Returns pointer to the TreeNode containing searchData (if data exists in the tree)
	
		//Delete
		void DeleteBranch(TreeNode* pos); //This will delete the branch as well as all connecting left / right TreeNodes 
		void DeleteTree(TreeNode* pos); //Hovae to input the _root first
		void DeleteLeftNode(TreeNode* pos);
		void DeleteRightNode(TreeNode* pos);

		TreeNode* GetFront() const { return _root; };
};