#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <stack>
using namespace std;

//#define seq_pre_vec seq_pre_vec
template<typename T, typename calT = int>  //���ʽ��ֻ������T = string���ͣ��ڶ������Ͳ���calTΪ���ʽ�Ľ������ 
class BiTree {
	struct BiTNode {
		T data;
		BiTNode *lchild, *rchild;
		BiTNode(T val) : data(val), lchild(nullptr), rchild(nullptr) {}
	};
	BiTNode* root;
	
	vector<BiTNode*> seq_pre;  //˳��洢��ǰ׺�����ı� 
	vector<BiTNode*> seq_in;  //˳��洢��������ʽ ��������ģ���� 
	//��������1����AB#C##D#EF###ת��Ϊ˳��ǰ׺��
	void pre_pta1(string str) {
		for (auto &c : str) {
			if (c == '#') seq_pre.push_back(nullptr);
			else seq_pre.push_back(new BiTNode(T() + c)); //�����T() + c��Ϊ�˼���string���ͣ���Ϊstring����û�в���Ϊ�ַ��Ĺ��캯�� 
		}
	}
	//��������2����A(B(,D),C(E))ת��Ϊ˳��ǰ׺��
	void pre_pta2(string str) {  //����ת��Ϊ����1�����ɸ�������1������ת�� 
		string temp = "";
		pre_pta2_pre(temp, str);
		pre_pta1(temp);
	}
	void pre_pta2_pre(string& res, string str) {
		if (str.empty()) return ;
		res += str[0];
		if (str.size() == 1) {
			res += "##";
			return ;
		}
		int l = 1, mid = l, r = str.size() - 1; //l, r -> seat of '(' and ')'
		for (int sum = 0; mid <= r; ++ mid) {
			if (str[mid] == '(') ++ sum;
			if (str[mid] == ')') -- sum;
			if (str[mid] == ',' && sum == 1) break;
		}
		if (mid <= r) {
			if (mid == l + 1) {  //(, r)
				res += '#';
				pre_pta2_pre(res, str.substr(mid + 1, r - mid - 1));
			} else {  //(l, r)
				pre_pta2_pre(res, str.substr(l + 1, mid - l - 1));
				pre_pta2_pre(res, str.substr(mid + 1, r - mid - 1));
			}
		} else {  //(l)
			pre_pta2_pre(res, str.substr(l + 1, r - l - 1));
			res += '#';
		}
	}
 	//��������3�������ʽת������
	void pre_expression(string str) { } //ֻ����string����ʵ���� 
	//��������build��ͨ������õ�˳��ǰ׺������
	BiTNode* debug;
	typename vector<BiTNode*>::iterator ite;  //�������� ˳��ǰ׺�� 
	void build(BiTNode* &node) {
		if (ite == seq_pre.end()) return ;
		node = *ite ++;
		if (node) {
			if (debug == nullptr) debug = node;
			build(node -> lchild);
			build(node -> rchild);
		}	
	}
	void chain_to_seq(BiTNode* node, int k) {  //��1��ʼ���� 
		seq_in[k] = node;
		//����Ǵ�1��ʼ��������k * 2 + 1, k * 2 + 2; 
		if (node -> lchild) chain_to_seq(node -> lchild, k << 1);
		if (node -> rchild) chain_to_seq(node -> rchild, k << 1 | 1);
	}
public : void picture() {
	int n = max_depth();
	int len = (n - 1 == 1 ? 0 : 1); if (n > 2) len = 3 * (1 << (n - 3)) - 1;
	int dep = 1; if (n > 1) dep = 3 * (1 << (n - 2));
	int wid = 1; if (n > 1) wid = 3 * (1 << (n - 1)) - 1;
	int mid = wid / 2 + 1;
	
	vector<vector<T> > g(dep + 1, vector<T>(wid + 1, 0));
	picture_fuvu(g, 1, mid, len, 1);
	for (auto vec : g) {
		for (auto c : vec) cout << c;
		cout << endl;
	}
	
}
private: void picture_fuvu(vector<vector<T> >& g, int x, int y, int len, int k) {
	if (seq_in[k] == nullptr) return ;
	g[x][y] = seq_in[k] -> data;
	
	int y1 = y, y2 = y;
	if (len <= 0) return ;
	if (seq_in[k] -> lchild) for (int i = 1; i <= len; ++ i) g[x + i][y1 - i] = T() + '/';
	if (seq_in[k] -> rchild) for (int i = 1; i <= len; ++ i) g[x + i][y2 + i] = T() + '\\';
	
	int temp = len;
	if (len == 1) len = 0;
	else if (len == 2) len = 1;
	else len = ((len + 1) >> 1) - 1;
	
	if (seq_in[k] -> lchild) picture_fuvu(g, x + temp + 1, y1 - temp - 1, len, k << 1);
	if (seq_in[k] -> rchild) picture_fuvu(g, x + temp + 1, y2 + temp + 1, len, k << 1 | 1);
}
public :
	
	BiTree(int op, string str) : root(nullptr) {
		switch (op) {
			case 1 : pre_pta1(str); break;  //����1 "AB#C##D#EF###" 
			case 2 : pre_pta2(str); break;  //����2 "A(B(,D),C(E))" 
			case 3 : pre_expression(str); break;  //����3 "(2 * 3) - (2 + 3)"  //ֻ������string��ģ���� 
		}
		ite = seq_pre.begin();  //׼����ʼ����
		build(root);  //���� 
		//ת����˳��洢 
		seq_in.resize((1 << max_depth() + 1));
		chain_to_seq(root, 1); 
	}
public: void prefix() { prefix_(root);  }
private:void prefix_(BiTNode* node) {
		if (node == nullptr) return ;
		cout << node -> data;
		prefix_(node -> lchild);
		prefix_(node -> rchild);
	}
public: void infix() { infix_(root); }
private:void infix_(BiTNode* node) {
		if (node == nullptr) return ;
		infix_(node -> lchild);
		cout << node -> data;
		infix_(node -> rchild);
	}			
public: void postfix() { postfix_(root); }
private:void postfix_(BiTNode* node) {
		if (node == nullptr) return ;
		postfix_(node -> lchild);
		postfix_(node -> rchild);
		cout << node -> data;
	}			
public: void levelfix() { levelfix_(root); }
private:void levelfix_(BiTNode* node) {
		if (node == nullptr) return ;
		queue<BiTNode*> qu;
		qu.push(root);
		while (! qu.empty()) {
			auto now = qu.front(); qu.pop();
			if (now == nullptr) continue;
			cout << now -> data;
			qu.push(now -> lchild);
			qu.push(now -> rchild);
		}
	}
public: int size() { return size_(root); }
private:int size_(BiTNode* node) {
		if (node == nullptr) return 0;
		return 1 + size_(node -> lchild) + size_(node -> rchild);
	}
public: int max_depth() { return max_depth_(root); }
private:int max_depth_(BiTNode* node) {
		if (node == nullptr) return 0;
		return max(1, max(max_depth_(node -> lchild), max_depth_(node -> rchild)) + 1);
	}
public :calT calculation() { return calculation_(root); }
private:calT calculation_(BiTNode* node) {  //�ݹ���⣬���ʻ��Ǹ����ҵĵݹ���ʽ�������ǰ׺��Ϳ�����stackɨ���ˣ�Ҳ����ת���ɲ���׺���ʽ�� 
		if (node == nullptr) return 0;
		if (node -> lchild == nullptr && node -> rchild == nullptr) {
			int num = 0;
			for (auto &c : node -> data) num = num * 10 + c - 48;
			return num;
		}
		int a = calculation_(node -> lchild);
		int b = calculation_(node -> rchild);
		char c = node -> data[0];
		switch(c) {
			case '+' : return a + b;
			case '-' : return a - b;
			case '*' : return a * b;
			case '/' : return a / b;
		}
	}
};

template<>
void BiTree<string>::pre_expression(string str) {
		//��ȡ���ʽtoken
		vector<string> pre;
		str += ';';
		string temp = "";
		for (auto &c : str) if (c != ' ') {
			if (c >= '0' && c <= '9') temp += c;
			else {
				if (! temp.empty()) pre.push_back(temp);
				temp = c, pre.push_back(temp), temp = "";
			}
		}
		pre.pop_back();
		//for (auto &sam : pre) cout << sam; cout << endl;  //test 
		//�����ʽת��Ϊ���棩����ʽ���������Ҹ���
		//reverse(pre.begin(), pre.end()); for (auto &sam : pre) if (sam[0] == '(' || sam[0] == ')') sam[0] ^= 1;
		map<string, int> op{{"*", 1}, {"/", 1}, {"+", 0}, {"-", 0}, {"(", -1}};  //���ʽ���ȼ� 
		 
		stack<string> bench;  //ά����ջ 
		vector<string> res;  //������
		for (auto &sam : pre) {
			if (sam[0] >= '0' && sam[0] <= '9') //if num, push stack
				{ res.push_back(sam); continue; }
			if (bench.empty()) bench.push(sam);  //if stakc is empty, push stack
			else if (sam == "(") { bench.push(sam); }  //if it is (, push stack
			else if (sam == ")") { for (; ! bench.empty() && bench.top() != "("; bench.pop()) res.push_back(bench.top()); bench.pop(); }  //if it is ), pop stack until ( and keep them
			else {  //else operator
				if (bench.top() == "(") bench.push(sam);
				else if (op[sam] > op[bench.top()]) bench.push(sam);  //greater push
				else {  //else pop stack until it is greater or (  //���﷽�㴦��(��Ϊ���͵����ȼ� 
					for (; ! bench.empty() && ! (op[sam] > op[bench.top()]); bench.pop()) res.push_back(bench.top());
					bench.push(sam);
				}
			}
		} 
		for (; ! bench.empty(); bench.pop()) res.push_back(bench.top());
		//for (auto &sam : res) cout << sam; cout << endl;  //test;
		//���Ѿ������ʽ������沨��ʽ/��׺���ʽ
		/*��������ֱ�ӽ������� 
		vector<string> bench = res;
		stack<BiTNode*> st;
		for (auto &sam : bench) { 
			BiTNode *node = new BiTNode(sam);
			if (sam[0] >= '0' && sam[0] <= '9') st.push(node);
			else {
				node -> rchild = st.top(); st.pop();
				node -> lchild = st.top(); st.pop();
				st.push(node);
			}
		}
		root = st.top();
		*/
		/*��������ֱ�Ӽ���������ʽ�Ľ�� 
		vector<string> bench = res
		stack<int> st; 
		for (auto &sam : bench) {
			if (sam[0] >= '0' && sam[0] <= '9') {
				int num = 0;
				for (auto &c : sam) num = num * 10 + c - 48;
				st.push(num);
			} else {
				char c = sam[0];
				int b = st.top(); st.pop();
				int a = st.top(); st.pop();
				switch (c) {
					case '+' : st.push(a + b); break;
					case '-' : st.push(a - b); break;
					case '*' : st.push(a * b); break;
					case '/' : st.push(a / b); break;
				}
			}
		}
		int ans = st.top();
		*/
		//��ɵ�Ƶ��һ�Ҫ��취�������˳��ǰ׺��ʽ
		//ֱ������������Ȼ�� �ǵݹ� ǰ�� �������ѽ������vector�� 
		//���������⣬�С�����������ע�ͣ�char��string���͵�ģ����г�ͻ����û�����ô��� 
		stack<BiTNode*> st;
		for (auto &sam : res) {  //������ʽ���Ҹ���Ҷ�Ӿ������֣���ǰ��ջ���������ǵ�ǰ���Ķ��ӣ����ջ��ʣһ��
			BiTNode *node = new BiTNode(sam);
			//BiTNode *node = nullptr;  //new BiTNode(T() + sam);
			if (sam[0] >= '0' && sam[0] <= '9') st.push(node);
			else {
				node -> rchild = st.top(); st.pop();
				node -> lchild = st.top(); st.pop();
				st.push(node);
			}
		}
		//��ʱջ�����Ǹ� 
		while(! st.empty()) {
			auto now = st.top(); st.pop();
			seq_pre.push_back(now);
			if (now -> data[0] >= '0' && now -> data[0] <= '9') seq_pre.push_back(nullptr), seq_pre.push_back(nullptr);  //��Ҷ���ṩ�ն��� 
			if (now -> rchild) st.push(now -> rchild);
			if (now -> lchild) st.push(now -> lchild);
			//���Ƚ������ٽ���֮���ٴ����ң��Ӷ�ʵ�ָ����� 
		}
	}

template<>
void BiTree<string>::picture() {
		int n = max_depth();
	int len = (n - 1 == 1 ? 0 : 1); if (n > 2) len = 3 * (1 << (n - 3)) - 1;
	int dep = 1; if (n > 1) dep = 3 * (1 << (n - 2));
	int wid = 1; if (n > 1) wid = 3 * (1 << (n - 1)) - 1;
	int mid = wid / 2 + 1;
	vector<vector<string> > g(dep + 1, vector<string>(wid + 1, " "));
	picture_fuvu(g, 1, mid, len, 1);
	for (auto vec : g) {
		for (auto c : vec) cout << c;
		cout << endl;
	}
}

int main() {
	
	
	
	BiTree<char> sam1(1, "AB#C##D#EF###");
	sam1.prefix(); cout << endl;
	sam1.infix(); cout << endl;
	sam1.postfix(); cout << endl;
	sam1.levelfix(); cout << endl;
	cout << sam1.size() << endl;
	cout << sam1.max_depth() << endl;
	sam1.picture();
	
	cout << "\n-----\n";
	
	BiTree<char> sam2(2, "A(B(,D),C(E))");
	sam2.prefix(); cout << endl;
	sam2.infix(); cout << endl;
	sam2.postfix(); cout << endl;
	sam2.levelfix(); cout << endl;
	cout << sam2.size() << endl;
	cout << sam2.max_depth() << endl;
	
	cout << "\n-----\n";
	
	
	BiTree<string> sam3(3, "((2 * 3) - (2 + 3)) + ((1 * 2) - (1 / 2))");
	cout << sam3.calculation() << endl;
	sam3.picture();
	
	
	
	return 0;
}
