#include <cstdio>
#include <iostream>
#include <initializer_list>
#include <set>

using namespace std;

template<typename T>
class List {
	struct point {
		T data;
		point *next;
		point(T x = 0) : data(x), next(nullptr) {}
	};
	int len;
	point *head;
	point* flip(int l, int r);  //翻转指定区间的链表，内部使用 
public :
	List() : len(0)  //创建空链表 
		{ head = new point; }
	List(istream &in, int n);  //从IO输入创建链表 
	List(initializer_list<T> vec);  //从内存输入创建链表 
	~List() {  //销毁链表
		this -> clear();
		delete head;
	}
	friend istream& operator >> (istream &in, List &t) {  //重载>>从IO输入创建链表 
		T x;
		while (in >> x) t.insert(t.len + 1, x);
		return in;
	}
	int size() //返回链表中元素个数 
		{ return len; }
	void push_front(T x) { //头插法插入元素 
		point *now = new point(x),
			  *temp = head -> next;
		head -> next = now;
		if (temp) now -> next = temp;
	}
	void pop_front() {  //头插法删除第一个元素 
		point *temp = nullptr;
		if (head -> next) {
			temp = head -> next;
			head -> next = temp -> next;
		}
		delete temp;
	} 
	bool insert(int inde, T x); //在1 ~ size() + 1各个位置中插入 
	bool remove(int inde);      //在1 ~ size()各个位置删除元素 
	T get(int inde);          //获得某个索引的元素 
	T operator [] (int inde)  //重载[]运算符获得任意位置 
		{ return get(inde); }
	void output(ostream &out)                              //打印链表 
		{ out << (*this); }
	friend ostream& operator << (ostream &out, List &t) {  //重载<<运算符打印链表 
		out << "[";
		for (point *now = t.head -> next; now; now = now -> next) out << now -> data << (now -> next ? ", " : "");
		out << "]";
		return out;
	}
	void flip();   //翻转链表 
	void flip2();  //另一个算法翻转链表 
	//void flip(int l, int r);  //翻转指定区间的链表 
	void unique();  //链表去重 
	void clear();  //清空链表
	bool pj();  //判断回文 
};

template<typename T>
bool List<T>::pj() {
	point *l = head -> next,
		  *r = this -> flip((len & 1 ? len / 2 + 1 : len / 2), len);
	
	int flag = 1;
	for (; l && r; l = l -> next, r = r -> next) 
		if (l -> data != r -> data) {
			flag = 0;
			break;
		}
	flip((len & 1 ? len / 2 + 1 : len / 2), len);
	return flag;
}


template<typename T>
List<T>::List(istream &in, int n) : List() {
	T x;
	for (int i = 1; i <= n; ++ i) {
		in >> x;
		insert(len + 1, x);
	}
}
template<typename T>
List<T>::List(initializer_list<T> vec) : List() {
	for (auto it = vec.begin(); it != vec.end(); ++ it) insert(len + 1, *it);
}

template<typename T>
bool List<T>::insert(int inde, T x) {
	point *now = head, *node = new point(x);
	for (; -- inde && now -> next; now = now -> next);
	if (inde != 0) return false;
	node -> next = now -> next;
	now -> next = node;
	++ len;
}

template<typename T>
bool List<T>::remove(int inde) {
	point *now = head, *father = now;
	for (; -- inde && now -> next; father = now, now = now -> next);
	if (inde != 0) return false;
	now = father -> next;
	father -> next = now -> next;
	-- len;
	delete now;
	return true;
}

template<typename T>
T List<T>::get(int inde) {
	point *now = head;
	for (; -- inde && now -> next; now = now -> next);
	if (inde != 0) return 0;
	return now -> date;
}

template<typename T>
void List<T>::flip() {
	point *now = head -> next, *father = nullptr;
	for (point *temp; now; ) {
		temp = now -> next;
		now -> next = father;
		father = now;
		now = temp;
	}
	head -> next = father;
}
template<typename T>
void List<T>::flip2() {
	point *now = head -> next;
	head -> next = nullptr;
	while (now) {
		point *temp = now -> next;
		now -> next = nullptr;
		if (head -> next) now -> next = head -> next;
		head -> next = now;
		now = temp;
	}
}

template<typename T>
typename List<T>::point* List<T>::flip(int l, int r) {
	point *pl, *pr;
	
	int i = 1;
	point *now = head -> next, *father = head;
	for (; now; ++ i, father = now, now = now -> next) {
		if (i == l) pl = father;
		if (i == r) pr = now;
	}
	
	point *res = pr;
	
	father = pl -> next; pr = pr -> next;
	
	now = pl -> next;
	pl -> next = nullptr;
	while (now != pr) {
		point *temp = now -> next;
		now -> next = nullptr;
		if (pl -> next) now -> next = pl -> next;
		pl -> next = now;
		now = temp;
	}
	father -> next = pr;
	
	return res;
}

template<typename T>
void List<T>::unique() {
	set<T> t;
	for (point *now = head -> next, *father = head; now; ) {
		if (t.count(now -> data)) {
			father -> next = now -> next;
			point *temp = now;
			now = now -> next;
			delete temp;
			
		} else {
			t.insert(now -> data);
			father = now;
			now = now -> next;
		}
	}
	
}

template<typename T>
void List<T>::clear() {
	point *now = head -> next;
	for (point *temp; now; ) {
		temp = now -> next;
		delete now;
		now = temp;
	}
	head -> next = nullptr;
}

//int am[] = {20, 20, 17, 16, 15, 15, 11, 10, 8, 7, 7, 5, 4, -1};

int main() {
	//freopen("in.txt", "r", stdin);
	
	List<int> sam({20, 20, 17, 16, 15, 15, 11, 10, 8, 7, 7, 5, 4, -1});
	
	cout << sam << endl;
	/*
	//for (int i = 0; am[i] != -1; ++ i) sam.insert(1, am[i]);
	cout << sam << endl;
	sam.flip();
	cout << sam << endl;
	sam.unique();
	cout << sam << endl;
	sam.flip2();
	cout << sam << endl;
	*/
	//sam.flip(2, 3);
	//sam.pop_front();
	
	//cout << sam << endl;
	
	cout << sam.pj() << endl;
	
	
	cout << sam << endl;
	/*
	
	List<int> sam({23, 23 , 324, 545 ,6 }); 
	//cin >> sam;
	//int n; cin >> n; List<int> sam(cin, n);
	cout << sam.size() << endl;
	
	sam.insert(1, 1);
	cout << sam << endl;
	sam.insert(2, 2);
	cout << sam << endl;
	sam.insert(3, 3);
	cout << sam << endl;
	sam.insert(4, 4);
	cout << sam << endl;
	sam.remove(1);
	cout << sam << endl;
	
	
	//sam.flip();
	
	//sam.output(cout);
	cout << sam << endl;
	*/
	return 0;
}
