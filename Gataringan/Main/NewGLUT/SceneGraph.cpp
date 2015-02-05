#include "SceneGraph.h"

#include <iostream>

SceneGraph::SceneGraph()
{
	_root = nullptr;
}

SceneGraph::~SceneGraph(void){}

TreeNode* SceneGraph::NewNode(MovableObject* d)
{
	TreeNode* n = new TreeNode();
	n->data = d; //The new TreeNode's data is initialised once created
	n->left = nullptr; //Both branches are set to NULL
	n->right = nullptr;

	return n;
}

TreeNode* SceneGraph::NewNode() 
{
	TreeNode* n = new TreeNode();
	n->data = nullptr;
	n->left = nullptr; 
	n->right = nullptr;

	return n;
}

void SceneGraph::InsertRootNode(TreeNode* newNode)
{

	if (_root != nullptr)
	{
		DeleteBranch(_root);
	}	

	_root = newNode;
}

void SceneGraph::UpdateTree(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{
			UpdateTree(pos->left);
		}		

		if (pos->right != nullptr)
		{
			UpdateTree(pos->right);
		}			

		if (pos->data != nullptr)
		{
			pos->data->Update();
		}
			
	}

}

void SceneGraph::DrawTree(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{
			DrawTree(pos->left);
		}		

		if (pos->right != nullptr)
		{
			DrawTree(pos->right);
		}
			
		if (pos->data != nullptr)
		{
			pos->data->Draw();
		}			

	}

}

void SceneGraph::UpdatePositionOfTree(TreeNode* pos, float x, float y, float z)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{
			UpdatePositionOfTree(pos->left, x, y, z);
		}		

		if (pos->right != nullptr)
		{
			UpdatePositionOfTree(pos->right, x, y, z);
		}
			
		if (pos->data != nullptr)
		{
			pos->data->UpdatePosition(x, y, z);
		}
			
	}

}

void SceneGraph::RotateTree(TreeNode* pos, float xRV, float yRV, float zRV, float r)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{
			RotateTree(pos->left, xRV, yRV, zRV, r);
		}
			
		if (pos->right != nullptr)
		{
			RotateTree(pos->right, xRV, yRV, zRV, r);
		}
			

		if (pos->data != nullptr)
		{
			pos->data->SetRotation(xRV, yRV, zRV, r);
		}		

	}

}

TreeNode* SceneGraph::DepthFirstSearch(TreeNode* pos, MovableObject* searchData)
{

	if (pos != nullptr)
	{

		TreeNode* result = nullptr; //This is needed for the recursion

		if (pos->data == searchData) //checks data on the current TreeNode
		{
			return pos;
		}
			
		if (pos->left != nullptr)
		{
			result = DepthFirstSearch(pos->left, searchData); // search left TreeNode first
		}			

		if (result == nullptr && pos->right != nullptr) // If left TreeNode results in a nullptr, check right TreeNode
		{
			result = DepthFirstSearch(pos->right, searchData);
		}
			
		return result; //Return the final result from all searches
	}

}

void SceneGraph::DeleteBranch(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{

			if (pos->left->data == nullptr && pos->left->left == nullptr &&
				pos->left->right == nullptr)
			{
				DeleteLeftNode(pos);
			}	
			else
			{
				DeleteBranch(pos->left);
			}
				
		}	

		if (pos->right != nullptr)
		{

			if (pos->right->data == nullptr && pos->right->left == nullptr &&
				pos->right->right == nullptr)
			{
				DeleteRightNode(pos);
			}			
			else
			{
				DeleteBranch(pos->right);
			}
				
		}
			
		if (pos->data != nullptr)
		{
			std::cout << pos->data->_name << " deleted" << std::endl;
			delete pos->data;
			pos->data = nullptr;
		}
			
		delete pos;
		pos = nullptr;
	}

}

void SceneGraph::DeleteLeftNode(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{
			pos->left = nullptr;
		}		

	}

}

void SceneGraph::DeleteRightNode(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->right != nullptr)
		{
			pos->right = nullptr;
		}

	}

}

void SceneGraph::DeleteTree(TreeNode* pos)
{

	if (pos != nullptr)
	{

		if (pos->left != nullptr)
		{

			if (pos->left->data == nullptr && pos->left->left == nullptr &&
				pos->left->right == nullptr)
			{
				DeleteLeftNode(pos);
			}			
			else
			{
				DeleteBranch(pos->left);
			}
				
		}		

		if (pos->right != nullptr)
		{

			if (pos->right->data == nullptr && pos->right->left == nullptr &&
				pos->right->right == nullptr)
			{
				DeleteRightNode(pos);
			}			
			else
			{
				DeleteBranch(pos->right);
			}
				
		}


		if (pos->data != nullptr)
		{
			std::cout << pos->data->_name << " deleted" << std::endl;
			delete pos->data;
			pos->data = nullptr;
		}

		DeleteLeftNode(pos);
		DeleteRightNode(pos);

		delete pos;
		pos = nullptr;
	}

}

