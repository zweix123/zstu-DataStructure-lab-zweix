#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
//把要可视化的二叉树转换成“顺序存储层次遍历的形式”并保存的vector中，其索引从1开始，vector的size到最后一个结点，重写code的中文部分即可使用
//Tip：如果结点表示复杂，可以把Node设置为string，将复杂的表达式处理好变成串放进去 
template<typename Node>
void cat(std::vector<Node>& tree) {  //tree存树的vector
	//获得要表达的结点的内容的最大长度
	int length = 0; for (int i = 1; i < tree.size(); ++ i) {
		std::stringstream ss; ss << tree[i];
		length = std::max(length, (int)ss.str().size());
	} 
	//获得树的深度（根的深度为1）——递归转非递归形式 
	int n = 0;
	{
		int root = 1;
		struct bench {
			int node, dep;
		};
		std::stack<bench> st;
		st.push({root, 1});
		while (! st.empty()) {
			int node = st.top().node, dep = st.top().dep; st.pop();
			if (node >= tree.size()) continue;
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
	std::vector<std::vector<std::string> > g(dep + 1, std::vector<std::string>(wid + 2, std::string(" ", length)));
	//for (auto &vec : g) vec.back() = "\n";
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
			
			
			if (k * 2 < tree.size()) {
				std::string str(" ", length); str[length >> 1] = '/';
				for (int i = 1; i <= temp; ++ i) g[x + i][y - i] = str;
				st.push({x + temp + 1, y - temp - 1, len, k << 1});
			} 
			if (k * 2 + 1 < tree.size()) {
				std::string str(" ", length); str[length >> 1] = '\\';
				for (int i = 1; i <= temp; ++ i) g[x + i][y + i] = str;
				st.push({x + temp + 1, y + temp + 1, len, k << 1 | 1});
			}		
		}
	}
	//输出 
	for (auto &vec : g) {
		for (auto &sam : vec) std::cout << sam;	
		std::cout << std::endl;
	}
}
/*
using namespace std;
int main() {
	vector<string> vec({"#", "A:1", "B:2", "C:3", "D:4...."});
	cat(vec);
}
*/
