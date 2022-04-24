#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
//把要可视化的二叉树转换成“层次遍历顺序存储的形式”并保存到vector中，其索引从1开始，空结点用一个不可能的值做哨兵flag 
/*
转换方法的一种方法的伪代码：（本质就是深搜的递归形式写成非递归形式）（正式代码也是，lambda实现块内递归不会，只能用栈这种笨方法） 
#define T 二叉树的值 
T flag = 不可能值;
vector<T> vec(2 << 二叉树的最大深度, flag);  //与一个二叉树同样高度的满二叉树的结点个数 
{	
	定义树的根root 
	struct point {
		树的结点 
		int k;  //这个结点应该的编号 
	};
	stack<point> st;
	st.push({root, 1});
	while (! st.empty()) {
		int node = st.top().node, k = st.top().k; st.pop();
		vec[k] = node这个结点的二叉树的值 
		if (有左儿子) st.push({node的左儿子, k << 1});
		if (有右儿子) st.push({node的右儿子, k << 1 | 1}); 
	}
}
cat(vec, flag);
*/
//Tip：如果结点表示复杂，可以把Node设置为string，将复杂的表达式处理好变成串放进去 
template<typename Node>
void cat(std::vector<Node>& tree, Node flag) {  //tree存树的vector
	//获得要表达的结点的内容的最大长度
	int length = 0; for (int i = 1; i < tree.size(); ++ i) if (tree[i] != flag) {
		std::stringstream ss; ss << tree[i];
		length = std::max(length, (int)ss.str().size());
	} 
	//获得树的深度（根的深度为1）——递归转非递归形式 
	int n = 0;
	{//满二叉树的深度确实可以直接通过tree的size求得，但是为了保证非满二叉树的实现 
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
	//推导找规律
	int len = n - 1; if (n > 2) len = 3 * (1 << (n - 3)) - 1;  //根节点到两个儿子的箭头长度 
	int dep = 1; if (n > 1) dep = 3 * (1 << (n - 2));  //整个图的高 
	int wid = 1; if (n > 1) wid = 3 * (1 << (n - 1)) - 1;  //整个图的宽 
	int mid = wid / 2 + 1;
	//存结果 
	std::vector<std::vector<std::string> > g(dep + 2, std::vector<std::string>(wid + 2, std::string(length, ' ')));
	for (auto &str : g.front()) str = std::string(length, '='); for (auto &str : g.back()) str = std::string(length, '=');
	for (auto &vec : g) vec.front() = "||", vec.back() = "||\n";
	//建图——递归化非递归 
	{
		int root = 1;
		struct bench {
			int x, y, len, k; //当前所在的x，y坐标、其（如果有）儿子的箭头长度，当前点索引k
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
			if (len <= 0) continue;  //无儿子 
			
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
	//输出 
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
