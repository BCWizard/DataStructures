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
			cout << "沒有節點" << endl;
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
	while (p) {															//root有值
		pp = p;
		if (thePair.first < p->data.first)		p = p->leftChild;		//當前pair < 父節點的數值
		else if (thePair.first > p->data.first)	p = p->rightChild;		//當前pair > 父節點的數值
		else { p->data.second = thePair.second;	return; }				//當前pair == 父節點的數值																
	}
	p = new TreeNode<pair<K, E>>(thePair);
	if (root)
		if (thePair.first < pp->data.first)		pp->leftChild = p;		//數值小於父節點 放左子節點
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
		if (deleteKey < p->data.first) {						//想找的數值 < 當前節點的數值
			pp = p;
			p = p->leftChild;		
			position = 0;
		}
		else if (deleteKey > p->data.first) {					//想找的數值 > 當前節點的數值
			pp = p;
			p = p->rightChild;		
			position = 1;
		}
		else { break; }											//想找的數值 == 當前節點的數值
	}
	parentNode = pp;				//目標父節點
	currentNode = p;				//目標點

	if (p->rightChild) {					//向右找最小
		p = p->rightChild;
		while (p)
		{
			if (p->leftChild) {
				pp = p;
				p = p->leftChild;
			}
			else{
				temp = p;					//找到最小
				break;
			}
		}
		currentNode->data = temp->data;			//交換數值
		if (temp->rightChild && pp)					//temp有右節點
			pp->leftChild = temp->rightChild;
		else if (!temp->rightChild && pp)
			pp->leftChild = 0;
		else if (temp->rightChild && !pp)			//目標位置為3,且右邊有節點
			root->rightChild = temp->rightChild;
		else if (root->rightChild->data.first == root->data.first)	//root右邊只有一個節點
			root->rightChild = 0;
		temp->~TreeNode();
		return;
	}
	else if(p->leftChild)					//只有左節點
	{
		if (!pp)
			root = p->leftChild;		//目標為root
		else if (position)
			parentNode->rightChild = p->leftChild;
		else
			parentNode->leftChild = p->leftChild;
		currentNode->~TreeNode();
		return;
	}
	else {									//都沒有子節點
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
	//亂數範圍
	const int rangeFrom = 0, rangeTo = 10000;
	//隨機設備
	random_device rd;
	//亂數產生器-梅森旋轉演算法
	mt19937 generator(rd());
	//離散型均勻分布
	uniform_int_distribution<int> uniform(rangeFrom, rangeTo);
	//控制輸入資料數量
	int inputDataNumber;
	cout << "請輸入資料數量" << endl;
	cin >> inputDataNumber;

	//產生離散型均勻分布的亂數
	for (int i = 0; i < inputDataNumber; i++) {
		int randomNumber = uniform(generator);
		cout << randomNumber << " ";

		pair<int, int> inputPair(randomNumber, i);
		testBST.Insert(inputPair);
	}
	cout << endl;

	
	while (1)
	{
		cout << "請輸入指令:Inorder,Delete,Exit" << endl;
		string command;
		cin >> command;

		if (command == "Inorder") {
			cout << "Inorder輸出結果:" << endl;
			testBST.Inorder();
			cout << endl;
		}
		else if (command == "Delete") {
			cout << "請輸入刪除值:" << endl;
			int deleteKey;
			cin >> deleteKey;
			testBST.Delete(deleteKey);
		}
		else if (command == "Exit")
			return 0;
	}
	
}