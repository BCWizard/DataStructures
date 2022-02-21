#include <iostream>
#include <Windows.h>
using namespace std;
class Polynomial;

class Term {
	friend Polynomial;
	friend ostream& operator<<(ostream& os, Polynomial& poly);
	friend istream& operator>>(istream& is, Polynomial& poly);
private:
	float coef;
	int exp;
};

class Polynomial {
	friend istream& operator>>(istream& is, Polynomial& poly);
	friend ostream& operator<<(ostream& os, Polynomial& poly);
private:
	Term* termArray;
	int capacity;			//termArray的容量
	int terms;				//項數
public:
	Polynomial(int term);
	Polynomial();
	~Polynomial();
	Polynomial Add(Polynomial poly);
	Polynomial Mult(Polynomial poly);
	void NewTerm(const float theCoeff, const int theExp);
	//float Eval(float f);
};
//輸入式子
istream& operator>>(istream& is, Polynomial& poly) {
	for (int i = 0; i < poly.terms; i++) {
		cin >> poly.termArray[i].coef >> poly.termArray[i].exp;
	}
	return is;
}
//輸出
ostream& operator<<(ostream& os, Polynomial& poly) {
	for (int i = 1; i < poly.terms; i++) {
		//係數不為0,1 或 項次等於0
		if ((poly.termArray[i].coef != 0 && poly.termArray[i].coef != 1) || poly.termArray[i].exp == 0)
			cout << poly.termArray[i].coef;
		//項次不為0 並 係數不等於0
		if(poly.termArray[i].exp != 0 && poly.termArray[i].coef != 0)
			cout << "x^" << poly.termArray[i].exp;
		//不為最後一項
		if (i + 1 != poly.terms) {
			cout << " + ";
		}
	}
	cout << endl;
	return os;
}
//建構子與解構子
Polynomial::Polynomial() {
	this->terms = 1;
	this->capacity = 1;
	termArray = new Term[terms];
	termArray[0].coef = 0;
	termArray[0].exp = 0;
}
Polynomial::Polynomial(int term) {
	this->capacity = term;
	this->terms = term;					//非零項數量
	termArray = new Term[terms];
}
Polynomial::~Polynomial() {
	//delete [] this->termArray;
}

Polynomial Polynomial::Add(Polynomial b) {
	Polynomial c;
	int aPos = 0, bPos = 0;
	while ((aPos < terms) && (bPos < b.terms)) {
		//如果兩式次方相等
		if (termArray[aPos].exp == b.termArray[bPos].exp) {
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if (t)	c.NewTerm(t, termArray[aPos].exp);
			aPos++; bPos++;
		}
		//a式當前項的次方 小於 b式當前項的次方
		else if (termArray[aPos].exp < b.termArray[bPos].exp) {
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		//a式當前項的次方 大於 b式當前項的次方
		else {
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}
	//其中一方已抵達極限 另一方未抵達
	for (; aPos < terms; aPos++)
		c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
	for (; bPos < b.terms; bPos++)
		c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
	return c;
}

Polynomial Polynomial::Mult(Polynomial b) {
	Polynomial c;
	int aPos = 0, bPos = 0, pos = 0;
	int coef = 0, exp = 0;				//運算出的係數及次方
	bool flag = 1;						//未被執行存入
	//a的每一項與b的每一項相乘
	for (aPos = 0; aPos < terms;aPos++)
		for (bPos = 0; bPos < b.terms; bPos++) {
			//係數相乘	次方相加
			coef = termArray[aPos].coef * b.termArray[bPos].coef;
			exp = termArray[aPos].exp + b.termArray[bPos].exp;
			//原項係數有值
			for (int pos = 0; pos < c.terms; pos++) {
				//項次相等
				if (c.termArray[pos].exp == exp) {
					c.termArray[pos].coef += coef;
					flag = 0;
					break;
				}
			}
			//找不到符合目前的項次
			if(flag)
				c.NewTerm(coef, exp);
			flag = 1;
		}
	return c;
}

void Polynomial::NewTerm(const float theCoeff, const int theExp) {
	if (terms == capacity) {
		capacity *= 2;
		Term* temp = new Term[capacity];
		copy(termArray, termArray + terms, temp);
		delete[]termArray;
		termArray = temp;
	}
	termArray[terms].coef = theCoeff;
	termArray[terms++].exp = theExp;	//terms做完後+=1
}

int main() {
	int term;										//非零項的數量
	Polynomial c;
													//輸入a式
	cout << "請輸入式子a非零項的數量" << endl;
	cin >> term;
	Polynomial a(term);
	cout << "請降冪輸入式子a" << endl;
	cin >> a;
													//輸入b式
	cout << "請輸入式子b非零項的數量" << endl;
	cin >> term;
	Polynomial b(term);
	cout << "請降冪輸入式子b" << endl;
	cin >> b;

	LARGE_INTEGER startTime, endTime, fre;
	QueryPerformanceFrequency(&fre);		//取得CPU頻率
	double timesAdd,timesMult;				//花費時間
																		//Add()
	cout << "兩式相加結果:" << endl;
	QueryPerformanceCounter(&startTime);	//開始計數CPU次數
	c = a.Add(b);
	QueryPerformanceCounter(&endTime);		//結束計數CPU次數
	cout << '\t' << c << endl;
	timesAdd = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;//累積次數/頻率
	
																		//Mult()
	cout << "兩式相乘結果:" << endl;
	QueryPerformanceFrequency(&fre);		//取得CPU頻率
	QueryPerformanceCounter(&startTime);	//開始計數CPU次數
	c = a.Mult(b);
	QueryPerformanceCounter(&endTime);		//結束計數CPU次數
	cout << '\t' << c << endl;
	timesMult = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	
	cout << "Add()花費時間:" << fixed << timesAdd << 's' << endl
		<< "Mult()花費時間:" << timesMult << 's' << endl;

	system("pause");
}
