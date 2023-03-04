#include <iostream>
#include <algorithm>
using namespace std;

template <class T>
class MinPQ {
public:
	virtual ~MinPQ() {}
	virtual bool IsEmpty() const = 0;
	virtual const T& Top()const = 0;
	virtual void Push(const T&) = 0;
	virtual void Pop() = 0;
};

template <class T>
class MinHeap {
private:
	T* heap;		//element array
	int heapSize;	//number of elements in heap
	int capacity;	//size of the array heap
public:
	MinHeap(int theCapacity = 10);
	//Constructor
	~MinHeap();
	//Destructor
	void Push(const T& e);
	void Pop();
	T& Top() const;
	bool IsEmpty() const { return heapSize == 0; }
	int Size() const { return heapSize; }
	void PrintHeap() const;
};
//Constructor
template <class T>
MinHeap<T>::MinHeap(int theCapacity) {
	if (theCapacity < 1) {
		cout << "Capacity must be >= 1" << endl;
		return;
	}
	capacity = theCapacity;
	heapSize = 0;
	heap = new T[capacity + 1];		//heap[0] is not used
}

//Destructor
template <class T>
MinHeap<T>::~MinHeap() {
	delete[] heap;
}

//Push
template <class T>
void ChangeSize1D(T*& a, const int oldSize, const int newSize) {
	if (newSize < 0) {
		cout << "New lenth must be >=0" << endl;
		return;
	}
	T* temp = new T[newSize + 1];
	int number = min(oldSize, newSize);
	copy(a, a + number + 1, temp);
	delete[] a;
	a = temp;
}
template <class T>
void MinHeap<T>::Push(const T& e) {
	//Insert e into the max heap
	if (heapSize == capacity) {		//double the capacity
		ChangeSize1D(heap, capacity, 2 * capacity);
		capacity *= 2;
	}
	int currentNode = ++heapSize;
	while (currentNode != 1 && heap[currentNode / 2] > e) {	//輸入值小於父節點
		heap[currentNode] = heap[currentNode / 2];			//move parent down
		currentNode /= 2;
	}
	heap[currentNode] = e;
}

//Pop
template <class T>
void MinHeap<T>::Pop() {
	//Delete min element
	if (IsEmpty()) {
		cout << "Heap is empty. Cannot delete." << endl;
		return;
	}
	heap[1].~T();	//delete max element

	//remove last element from heap
	T lastE = heap[heapSize--];

	//trickle down
	int currentNode = 1;					//currentNode = root 位置
	int child = 2;
	while (child <= heapSize) {
		if (child < heapSize && heap[child] > heap[child + 1]) child++;		//指向較小數值的節點
		if (lastE <= heap[child])break;

		heap[currentNode] = heap[child];			//父節點值 = 最小子節點值
		currentNode = child;
		child *= 2;									//移到子節點
	}
	heap[currentNode] = lastE;						//將最後一個節點補上空位
}

template <class T>
T& MinHeap<T>::Top()const {
	if (IsEmpty()) {
		cout << "Heap is empty. No top element." << endl;
	}
	return heap[1];
}

template<class T>
void MinHeap<T>::PrintHeap()const {
	for (int i = 1; i <= heapSize; i++)
		cout << heap[i] << " ";
	cout << endl;
}

int main() {
	MinHeap<int> testMinHeap(10);
	string command;
	int inputNumber;
	cout << "請輸入一個或多個數值 以空格區分 以-1為結尾" << endl;
	while (cin >> inputNumber) {
		if (inputNumber == EOF)
			break;
		testMinHeap.Push(inputNumber);
	}
	while (1) {
		cout << "請輸入指令:Push,Pop,Top,PrintHeap,Exit" << endl;
		cin >> command;
		if (command == "Push") {
			cout << "請輸入數值" << endl;
			cin >> inputNumber;
			testMinHeap.Push(inputNumber);
			cout << "成功輸入數值:" << inputNumber << endl;
		}
		if (command == "Pop") {
			testMinHeap.Pop();
		}
		if (command == "Top") {
			cout << testMinHeap.Top() << endl;
		}
		if (command == "PrintHeap") {
			testMinHeap.PrintHeap();
		}
		if (command == "Exit") {
			break;
		}
	}
}