#include <iostream>
#include <random>
using namespace std;

template <class K, class E>
class Dictionary {
public:
	virtual bool IsEmpty() const = 0;
	virtual pair<K, E>* Get(const K&) const = 0;
	virtual void Insert(const pair<K, E>&) = 0;
	virtual void Delete(const K&) = 0;
};

template <class K, class E>
class BST;

template <class T>
class TreeNode {
	template <class K, class E>
	friend class BST;
private:
	T data;
	TreeNode<T>* leftChild = 0;
	TreeNode<T>* rightChild = 0;
public:
	TreeNode(T _data) {
		data = _data;
	}
	TreeNode(T _data, TreeNode <T>* lC, TreeNode <T>* rC) {
		data = _data;
		leftChild = lC;
		rightChild = rC;
	}
	~TreeNode() {
	}
	void setNode(TreeNode* left, TreeNode* right) {
		leftChild = left;
		rightChild = right;
	}
};

template <class K, class E>
class BST {
	template<class T>
	friend class TreeNode;
private:
	TreeNode<pair<K, E>>* root = 0;
public:
	BST() { root = 0; };
	~BST() {};
	//bool IsEmpty() const = 0;
	pair<K, E>* Get(const K& k) {
		return Get(root, k);
	}
	pair<K, E>* Get(TreeNode<pair<K, E>>* p, const K& k) {
		if (!p)					return 0;							//0
		if (k < p->data.first)	return Get(p->leftChild, k);
		if (k > p->data.first)	return Get(p->rightChild, k);
		return &p->data;
	}
	void Insert(const pair<K, E>&);
	void Delete(const K&);

	BST(TreeNode<pair<K, E>>* inputNode) {
		root = inputNode;
	}
	//Inorder
	void Inorder() {
		if (root)
			Inorder(root);
		else
			cout << "�S���`�I" << endl;
	}
	void Inorder(TreeNode<pair<K, E>>* currentNode) {
		if (currentNode) {
			Inorder(currentNode->leftChild);
			Visit(currentNode);
			Inorder(currentNode->rightChild);
		}
	}
	void Visit(TreeNode<pair<K, E>>* currentNode) {
		cout << currentNode->data.first << ' ';
	}
};

template <class K, class E>
void BST<K, E>::Insert(const pair<K, E>& thePair) {
	TreeNode <pair<K, E>>* p = root, * pp = 0;
	while (p) {															//root����
		pp = p;
		if (thePair.first < p->data.first)		p = p->leftChild;		//��epair < ���`�I���ƭ�
		else if (thePair.first > p->data.first)	p = p->rightChild;		//��epair > ���`�I���ƭ�
		else { p->data.second = thePair.second;	return; }				//��epair == ���`�I���ƭ�																
	}
	p = new TreeNode<pair<K, E>>(thePair);
	if (root)
		if (thePair.first < pp->data.first)		pp->leftChild = p;		//�ƭȤp����`�I �񥪤l�`�I
		else									pp->rightChild = p;
	else root = p;
}

template<class K, class E>
void BST<K, E>::Delete(const K& deleteKey) {
	TreeNode <pair<K, E>>* currentNode = root, * parentNode = 0, *temp = 0;
	TreeNode <pair<K, E>>* p = root, * pp = 0;
	bool position = 0;
	while (p)
	{
		if (deleteKey < p->data.first) {						//�Q�䪺�ƭ� < ��e�`�I���ƭ�
			pp = p;
			p = p->leftChild;		
			position = 0;
		}
		else if (deleteKey > p->data.first) {					//�Q�䪺�ƭ� > ��e�`�I���ƭ�
			pp = p;
			p = p->rightChild;		
			position = 1;
		}
		else { break; }											//�Q�䪺�ƭ� == ��e�`�I���ƭ�
	}
	parentNode = pp;				//�ؼФ��`�I
	currentNode = p;				//�ؼ��I

	if (p->rightChild) {					//�V�k��̤p
		p = p->rightChild;
		while (p)
		{
			if (p->leftChild) {
				pp = p;
				p = p->leftChild;
			}
			else{
				temp = p;					//���̤p
				break;
			}
		}
		currentNode->data = temp->data;			//�洫�ƭ�
		if (temp->rightChild && pp)					//temp���k�`�I
			pp->leftChild = temp->rightChild;
		else if (!temp->rightChild && pp)
			pp->leftChild = 0;
		else if (temp->rightChild && !pp)			//�ؼЦ�m��3,�B�k�䦳�`�I
			root->rightChild = temp->rightChild;
		else if (root->rightChild->data.first == root->data.first)	//root�k��u���@�Ӹ`�I
			root->rightChild = 0;
		temp->~TreeNode();
		return;
	}
	else if(p->leftChild)					//�u�����`�I
	{
		if (!pp)
			root = p->leftChild;		//�ؼЬ�root
		else if (position)
			parentNode->rightChild = p->leftChild;
		else
			parentNode->leftChild = p->leftChild;
		currentNode->~TreeNode();
		return;
	}
	else {									//���S���l�`�I
		if (!pp)
			root = 0;
		else if (position)
			parentNode->rightChild = 0;
		else
			parentNode->leftChild = 0;
		currentNode->~TreeNode();
		return;
	}
}

int main() {
	BST<int, int> testBST;
	pair<int, int> inputPair();
	//�üƽd��
	const int rangeFrom = 0, rangeTo = 10000;
	//�H���]��
	random_device rd;
	//�üƲ��;�-���˱���t��k
	mt19937 generator(rd());
	//���������ä���
	uniform_int_distribution<int> uniform(rangeFrom, rangeTo);
	//�����J��Ƽƶq
	int inputDataNumber;
	cout << "�п�J��Ƽƶq" << endl;
	cin >> inputDataNumber;

	//�������������ä������ü�
	for (int i = 0; i < inputDataNumber; i++) {
		int randomNumber = uniform(generator);
		cout << randomNumber << " ";

		pair<int, int> inputPair(randomNumber, i);
		testBST.Insert(inputPair);
	}
	cout << endl;

	
	while (1)
	{
		cout << "�п�J���O:Inorder,Delete,Exit" << endl;
		string command;
		cin >> command;

		if (command == "Inorder") {
			cout << "Inorder��X���G:" << endl;
			testBST.Inorder();
			cout << endl;
		}
		else if (command == "Delete") {
			cout << "�п�J�R����:" << endl;
			int deleteKey;
			cin >> deleteKey;
			testBST.Delete(deleteKey);
		}
		else if (command == "Exit")
			return 0;
	}
	
}