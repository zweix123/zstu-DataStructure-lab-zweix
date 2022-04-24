#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
//��Ҫ���ӻ��Ķ�����ת���ɡ���α���˳��洢����ʽ�������浽vector�У���������1��ʼ���ս����һ�������ܵ�ֵ���ڱ�flag 
/*
ת��������һ�ַ�����α���룺�����ʾ������ѵĵݹ���ʽд�ɷǵݹ���ʽ������ʽ����Ҳ�ǣ�lambdaʵ�ֿ��ڵݹ鲻�ᣬֻ����ջ���ֱ������� 
#define T ��������ֵ 
T flag = ������ֵ;
vector<T> vec(2 << ��������������, flag);  //��һ��������ͬ���߶ȵ����������Ľ����� 
{	
	�������ĸ�root 
	struct point {
		���Ľ�� 
		int k;  //������Ӧ�õı�� 
	};
	stack<point> st;
	st.push({root, 1});
	while (! st.empty()) {
		int node = st.top().node, k = st.top().k; st.pop();
		vec[k] = node������Ķ�������ֵ 
		if (�������) st.push({node�������, k << 1});
		if (���Ҷ���) st.push({node���Ҷ���, k << 1 | 1}); 
	}
}
cat(vec, flag);
*/
//Tip���������ʾ���ӣ����԰�Node����Ϊstring�������ӵı��ʽ����ñ�ɴ��Ž�ȥ 
template<typename Node>
void cat(std::vector<Node>& tree, Node flag) {  //tree������vector
	//���Ҫ���Ľ������ݵ���󳤶�
	int length = 0; for (int i = 1; i < tree.size(); ++ i) if (tree[i] != flag) {
		std::stringstream ss; ss << tree[i];
		length = std::max(length, (int)ss.str().size());
	} 
	//���������ȣ��������Ϊ1�������ݹ�ת�ǵݹ���ʽ 
	int n = 0;
	{//�������������ȷʵ����ֱ��ͨ��tree��size��ã�����Ϊ�˱�֤������������ʵ�� 
		int root = 1;
		struct bench {
			int node, dep;
		};
		std::stack<bench> st;
		st.push({root, 1});
		while (! st.empty()) {
			int node = st.top().node, dep = st.top().dep; st.pop();
			if (node >= tree.size() || tree[node] == flag) continue;
			n = std::max(n, dep);
			if (node * 2 < tree.size()) st.push({node * 2, dep + 1});
			if (node * 2 + 1 < tree.size()) st.push({node * 2 + 1, dep + 1});
		}
	}
	//�Ƶ��ҹ���
	int len = n - 1; if (n > 2) len = 3 * (1 << (n - 3)) - 1;  //���ڵ㵽�������ӵļ�ͷ���� 
	int dep = 1; if (n > 1) dep = 3 * (1 << (n - 2));  //����ͼ�ĸ� 
	int wid = 1; if (n > 1) wid = 3 * (1 << (n - 1)) - 1;  //����ͼ�Ŀ� 
	int mid = wid / 2 + 1;
	//���� 
	std::vector<std::vector<std::string> > g(dep + 2, std::vector<std::string>(wid + 2, std::string(length, ' ')));
	for (auto &str : g.front()) str = std::string(length, '='); for (auto &str : g.back()) str = std::string(length, '=');
	for (auto &vec : g) vec.front() = "||", vec.back() = "||\n";
	//��ͼ�����ݹ黯�ǵݹ� 
	{
		int root = 1;
		struct bench {
			int x, y, len, k; //��ǰ���ڵ�x��y���ꡢ�䣨����У����ӵļ�ͷ���ȣ���ǰ������k
		}; 
		std::stack<bench> st;
		st.push({1, mid, len, root});
		while (! st.empty()) {
			int x = st.top().x, y = st.top().y, len = st.top().len, k = st.top().k; st.pop();
			if (k >= tree.size()) continue;
			{ 
				std::stringstream str;
				str << tree[k];
				std::string res = str.str();
				for (int cnt = (length - res.size()) >> 1; cnt --; ) res = " " + res;
				for (int cnt = (length - res.size()) + 1 >> 1; cnt --; ) res += " ";
				g[x][y] = res;
			}
			if (len <= 0) continue;  //�޶��� 
			
			int temp = len;
			if (len == 1) len = 0;
			else if (len == 2) len = 1;
			else len = ((len + 1) >> 1) - 1;
			
			
			if (k * 2 < tree.size() && tree[k << 1] != flag) {
				std::string str(length, ' '); str[length >> 1] = '/';
				for (int i = 1; i <= temp; ++ i) g[x + i][y - i] = str;
				st.push({x + temp + 1, y - temp - 1, len, k << 1});
			} 
			if (k * 2 + 1 < tree.size() && tree[k << 1 | 1] != flag) {
				std::string str(length, ' '); str[length >> 1] = '\\';
				for (int i = 1; i <= temp; ++ i) g[x + i][y + i] = str;
				st.push({x + temp + 1, y + temp + 1, len, k << 1 | 1});
			}		
		}
	}
	//��� 
	for (auto &vec : g) {
		for (auto &sam : vec) std::cout << sam;	
		//std::cout << std::endl;
	}
}
/*
using namespace std;
int main() {
	vector<string> vec({"#", "A:1", "B:2", "C:3", "D:4...."});
	string flag = "#";
	cat(vec, flag);
}
*/
