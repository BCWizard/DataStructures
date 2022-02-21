#include <iostream>
#include <iomanip>
using namespace std;

struct Term
{
	int coef;
	int exp;
	Term Set(int c = 0, int e = -1) { coef = c; exp = e; return *this; };
};

template<class T> class LinkList;
class Polynomial;

template<class T>
class LinkNode {
	friend class LinkList<T>;
	friend class Polynomial;
private:
	T data;
	LinkNode<T>* link;
																	//建構子
	LinkNode(const T& _data = T(), LinkNode<T>* _next = NULL) {
		data = _data;
		link = _next;
	}
};

template<class T>
class LinkList {
private:
	LinkNode<T>* Head;			//coef = 0, exp = -1, link = 下一個LinkNode
	LinkNode<T>* Last;			//最後的LinkNode
public:
	LinkList() { Head = 0;	Last = 0; }

	class Iterator {
		friend LinkList;
		friend Polynomial;
	private:
		LinkNode<T>* current;
	public:
		Iterator(LinkNode<T>* startNode = 0) {
			current = startNode;
		}
		T& operator*()const { return current->data; }
		T* operator->()const { return &current->data; }

		Iterator& operator++() {		//preincrement
			current = current->link;	return *this;
		}
		Iterator operator++(int) {		//postincrement
			Iterator old = *this;
			current = current->link;
			return old;
		}

		bool operator!=(const Iterator right)const {
			return current != right.current;
		}
		bool operator==(const Iterator right)const {
			return current == right.current;
		}
	};
														//return 第一個有值的節點
	Iterator begin()const {					
		return Iterator(Head->link);
	}
														//return 最後一個節點
	Iterator end()const {					
		return Iterator(Last);
	}
														//InserBack
	void InserBack(const T& e) {
		LinkNode<T>* newNode = new LinkNode<T>(e);
		Term temp;
		temp.Set(0, -1);
		if (Head) {
			Last->link = newNode;
			newNode->link = Head;						//指回Head
			Last = Last->link;							//Last = newNode
		}
		else {
			Last = newNode;
			Head = new LinkNode<T>(temp, newNode);		//Head coef = 0, exp = -1 link = newNode
			newNode->link = Head;						//指回Head
		}			
	}
														//InserMid
	void InserMid(LinkNode<T>* pointNode, const T& e) {	//pointNode:前一個節點
		LinkNode<T>* newNode = new LinkNode<T>(e);
		
		newNode->link = pointNode->link;				//指向下一節點
		pointNode->link = newNode;						//指向新節點
	}
};

class Polynomial {
	friend istream& operator>>(istream& in, Polynomial& x);
	friend ostream& operator<<(ostream& os, Polynomial& x);
private:
	LinkList<Term> poly;
public:
	Polynomial() {	}
	Polynomial(const Polynomial& a) {			//copy
		poly = a.poly;
	}
	~Polynomial() {};
	Polynomial operator+(const Polynomial& b)const {
		Term temp;
		LinkList<Term>::Iterator ai = poly.begin(),			//一個節點
									bi = b.poly.begin();
		Polynomial c;
		while (1){
												//指數相等時
			if (ai->exp == bi->exp) {
				if (ai->exp == -1)	return c;
				int sum = ai->coef + bi->coef;	//係數相加值
				if (sum)	
					c.poly.InserBack(temp.Set(sum, ai->exp));
				ai++;	bi++;
			}
												//指數小於
			else if (ai->exp < bi->exp) {
				c.poly.InserBack(temp.Set(bi->coef, bi->exp));
				bi++;
			}
			else {
				c.poly.InserBack(temp.Set(ai->coef, ai->exp));
				ai++;
			}
		}
	}
	
	Polynomial operator*(const Polynomial& b)const {
		LinkNode<Term>* pointNode;					//存前一個節點
		Term temp;
		int coef, exp;
		bool aiFirst = 1;							//尚未建立任意節點

		Polynomial c;
		LinkList<Term>::Iterator ai, bi, ci;
													//歷遍所有相乘組合
		for(ai = poly.begin();ai->exp != -1;ai++)
			for (bi = b.poly.begin(); bi->exp != -1; bi++) {
				coef = ai->coef * bi->coef;			//儲存運算後的係數
				exp = ai->exp + bi->exp;			//儲存運算後的次方

				if (aiFirst){						//尚未建立任意節點
					c.poly.InserBack(temp.Set(coef, exp));
					aiFirst = 0;
				}
				else {
					bool stored = 0;				//新節點已建立
													//尋找是否有相同的次方
					for (ci = c.poly.begin(); ci->exp != -1; ci++){
						pointNode = ci.current;
													//找到相同次方
						if (exp == ci->exp){
							ci->coef += coef;
							stored = 1;
							break;
						}
													//發現比已知的次方還大
						else if(exp > ci->exp){
												//在當前節點前面新增
							c.poly.InserMid(pointNode, temp.Set(coef, exp));
							stored = 1;
							break;
						}
					}
													//比已知的次方還小
					if(!stored)
						c.poly.InserBack(temp.Set(coef, exp));
				}
			}
		return c;
	}

	Polynomial operator-(const Polynomial& b)const {
		Term temp;
		LinkList<Term>::Iterator ai = poly.begin(),				//第一個有值的節點
									bi = b.poly.begin();
		Polynomial c;
		while (1) {
			int coef = 0;
												//兩式結束
			if (ai->exp == -1 && bi->exp == -1)
				return c;
												//次方大於 直接建立節點
			if (ai->exp > bi->exp) {
				c.poly.InserBack(temp.Set(ai->coef, ai->exp));
				ai++;
			}
												//次方相等 兩係數相減
			else if (ai->exp == bi->exp) {
				coef = ai->coef - bi->coef;
				if(coef != 0)
					c.poly.InserBack(temp.Set(coef, ai->exp));
				ai++;	bi++;
			}
												//次方小於 b的係數加負號再建立
			else{
				c.poly.InserBack(temp.Set(-coef, bi->exp));
				bi++;
			}

		}
	}

	const Polynomial& operator=(const Polynomial& b) {//copy
		if (this == &b) {
			return *this;
		}
		poly = b.poly;
		return *this;
	}
};

istream& operator>>(istream& in, Polynomial& x) {
	Term temp;
	int n, c, e;
	cout << "請輸入多項式項數:" << endl;
	cin >> n;
	cout << "請降冪輸入多項式:" << endl;
	while(n){
		cin >> c >> e;
		temp.Set(c, e);
		x.poly.InserBack(temp);
		n--;
	}
	return in;
}

ostream& operator<<(ostream& os, Polynomial& x) {
	LinkList<Term>::Iterator ai = x.poly.begin();
	bool flagFirst = 1;					//是否為第一節點
	while (1) {
										//式子結束
		if (ai->exp == -1) {
			cout << endl;
			return os;
		}
										//次方為0 輸出係數
		if (ai->exp == 0)
			cout << ai->coef;
		else if (ai->coef && ai->exp) {
			(flagFirst) ?
									//係數不須輸出符號
				cout << ai->coef << "x^" << ai->exp : 
									//resetiosflags #include <iomanip> 清除指定格式
				cout << showpos << ai->coef << "x^" 
					<< resetiosflags(ios::showpos) << ai->exp;
		}
			
		flagFirst = 0;
		ai++;
	}
}

int main() {
	Polynomial a,b,c;

	cin >> a;
	cout << a;

	cin >> b;
	cout << b;

	c = a + b;
	cout << "兩式相加結果:" << endl << c;

	c = a * b;
	cout << "兩式相乘結果:" << endl << c;

	c = a - b;
	cout << "兩式相減結果:" << endl << c;
}
