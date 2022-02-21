#include <iostream>
using namespace std;

template <class T>
class Tree;

template <class T>
class TreeNode{
friend class Tree<T>;
private:
	T data = 0;
	TreeNode<T> *leftChild = 0;
	TreeNode<T> *rightChild = 0;
public:
	TreeNode(T input){
		data = input;
	}
	void setNode(TreeNode *left,TreeNode *right){
		leftChild = left;
		rightChild = right;
	}
};

template <class T>
class Tree{
private:
	TreeNode<T> *root = 0;
public:
	Tree(TreeNode<T> *inputNode){
		root = inputNode;
	}
										//Preorder
	void Preorder(){
		Preorder(root);
	}
	void Preorder(TreeNode<T> *currentNode){
		if(currentNode){
			Visit(currentNode);
			Preorder(currentNode->leftChild);
			Preorder(currentNode->rightChild);
		}
	}
										//Inorder
	void Inorder(){
		Inorder(root);
	}
	void Inorder(TreeNode<T> *currentNode){
		if(currentNode){
			Inorder(currentNode->leftChild);
			Visit(currentNode);			
			Inorder(currentNode->rightChild);
		}
	}
										//Postorder
	void Postorder(){
		Postorder(root);
	}
	void Postorder(TreeNode<T> *currentNode){
		if(currentNode){
			Postorder(currentNode->leftChild);
			Postorder(currentNode->rightChild);
			Visit(currentNode);
		}
	}
	
	void Visit(TreeNode<T> *currentNode){
		cout << currentNode->data;
	}	
};

int main(){
	TreeNode<char>* nodeA = new TreeNode <char>('A');
	TreeNode<char>* nodeB = new TreeNode <char>('B');
	TreeNode<char>* nodeC = new TreeNode <char>('C');
	TreeNode<char>* nodeD = new TreeNode <char>('D');
	TreeNode<char>* nodeE = new TreeNode <char>('E');
	TreeNode<char>* nodeF = new TreeNode <char>('F');
	TreeNode<char>* nodeG = new TreeNode <char>('G');
	TreeNode<char>* nodeH = new TreeNode <char>('H');
	TreeNode<char>* nodeI = new TreeNode <char>('I');
	
	nodeA->setNode(nodeB,nodeC);
	nodeB->setNode(nodeD,nodeE);
	nodeE->setNode(nodeG,nodeH);
	nodeC->setNode(nodeF,NULL);
	nodeF->setNode(NULL,nodeI);
	
	Tree <char> tree(nodeA);
	
	cout << "Preorder:" << endl;
	tree.Preorder();
		cout << endl;
	cout << "Inorder:" << endl;
	tree.Inorder();
		cout << endl;
	cout << "Postorder:" << endl;
	tree.Postorder();
		cout << endl;
}
