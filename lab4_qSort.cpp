#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <set>

template<typename T>
void swap(T &a, T &b) {
	T tmp = a; a = b; b = tmp;
}

//快速排序升序 
template<typename T>
void sort(T a[], int l, int r) {
	if (l >= r) return ;
	int i = l - 1, j = r + 1, x = a[l + r >> 1];
	while (i < j) {
		do ++ i; while (a[i] > x);
		do -- j; while (a[j] < x);
		if (i < j) swap(a[i], a[j]);
	}
	sort(a, l, j);
	sort(a, j + 1, r);
}

//重载比较的快速排序升序 
template<typename T>
void sort(T a[], int l, int r, bool (*cmp)(T a, T b)) {  //cmp为含义重载<运算符 
	if (l >= r) return ;
	int i = l - 1, j = r + 1, x = a[l + r >> 1];
	while (i < j) {
		do ++ i; while (cmp(a[i], x));
		do -- j; while (cmp(x, a[j]));
		if (i < j) swap(a[i], a[j]);
	}
	sort(a, l, j, cmp);
	sort(a, j + 1, r, cmp);
}


template<typename T>
class Sort {
	std::vector<T> vec;     //保存原序列，用于性能测试
	T* a;                   //原序列的指针，用于修改 
	int st, ed;             //序列的起末（闭区间） 
	bool (*cmp)(T a, T b);  //比较函数，默认std::less（升序） 
	std::vector<std::vector<std::pair<int, int> > > res;  //存储排序过程中的信息，pair的二维矩阵，first保存序列值，second保存模式
														  //模式： 
	int num_cmp, num_swap;  //保存比较次数和交换次数 
	std::set<std::pair<int, int> > vis;  //对于“基准数的<升序，最大>/<降序，最小>会导致递归区间无法缩小”用来保存搜索过的区间 
public :
	Sort(T aa[], int ll, int rr, bool (*cmp_)(T a, T b)) : a(aa), st(ll), ed(rr), cmp(cmp_) {  //数据成员初始化
		vec.resize(ed + 1); for (int i = st; i <= ed; ++ i) vec[i] = a[i];  //保存原序列
		num_cmp = num_swap = 0;
	}
	Sort(T aa[], int ll, int rr) : Sort(aa, ll, rr, std::less<T>() ) {}  //默认初始化
	 
	friend std::ostream& operator << (std::ostream& out, const Sort& t) {  //辅助函数：输出当前处理的序列的状态 
		out << "["; for (int i = t.st; i <= t.ed; ++ i) out << t.a[i] << " ]"[i == t.ed];
		return out;
	}
private: 
void dfs(int l, int r, int k, int op) {  //算法核心：l和r用于排序，k和op是辅助变量，k表示递归层数，op表示排序模式
	while (k + 1 > res.size()) res.push_back(std::vector<std::pair<int, int> >(ed + 1, {0, 0}));  //动态扩展新的存储矩阵层数
	if (l >= r) {  //递归边界 
		return ;
	}
	if (vis.count({l, r})) return ;
	vis.insert({l, r});   //搜索空间的记录，避免重复进入同一区间
	
	//算法核心 
	int i = l - 1, j = r + 1;
	T mid = a[l + r >> 1];
	
	if (op == 0) ;  //默认模式，选中间的
	else if (op == 1) for (int o = l; o <= r; ++ o) mid = std::min(mid, a[o]);
	else if (op == 2) for (int o = l; o <= r; ++ o) mid = std::max(mid, a[o]);
	
	while (i < j) {	
		do {++ i; ++ num_cmp;} while (cmp(a[i], mid));
		do {-- j; ++ num_cmp;} while (cmp(mid, a[j]));
		if (i < j) { std::swap(a[i], a[j]);  ++ num_swap; }
	}
	//mid == i == j
	
	
	for (int o = l; o <= r; ++ o) res[k][o].first = a[o];
	
	res[k][l].second = res[k][j + 1].second = -1;
	res[k][j].second = res[k][r].second = 1;
	
	//if (r - l == 1) res[k][l].second = res[k][r].second = 2;
	
	if (l == j) res[k][l].second = 2;
	if (j + 1 == r) res[k][r].second = 2;
	
	dfs(l, j, k + 1, op);
	dfs(j + 1, r, k + 1, op); 
}

public :
void work(int op) {
	vis.clear();
	for (int i = st; i <= ed; ++ i) a[i] = vec[i];
	num_cmp = num_swap = 0;
	dfs(st, ed, 1, op);
}
void show() {
	std::cout << "比较次数：" << num_cmp << "， " << "交换次数： " << num_swap << std::endl;
	for (int i = 1; i < res.size(); ++ i) {
		for (int j = st; j <= ed; ++ j) {
			std::stringstream ss;
			if (res[i][j].second == -1 || res[i][j].second == 2) ss << "(";
			else ss << " ";
			
			if (res[i][j].first == 0) ss << a[j];
			else ss << res[i][j].first;
			
			if (res[i][j].second == 1 || res[i][j].second == 2) ss << ")";
			else ss << " ";
			
			std::cout << ss.str();
		}
		std::cout << std::endl;
	}
}

};

bool cmp_min(int a, int b) {
	return a < b;
}
bool cmp_max(int a, int b) {
	return a > b;
}

int a[10007], b[10007];

using namespace std;
int main() {
	//std::ios::sync_with_stdio(false);    cin.tie(0);    cout.tie(0);
	//freopen("in.txt", "r", stdin);
		
	srand(time(0));
	const int N = 10;  //序列最大长度
	const int M = 20;  //序列元素最大值 
	int n = rand() % N + 1;
	cout << "序列长度：" << n << endl;
	for (int i = 1; i <= n; ++ i) a[i] = b[i] = rand() % M + 1;
	
	Sort<int> sam1(a, 1, n, cmp_min);
	Sort<int> sam2(b, 1, n, cmp_max); 
	
	cout << "升序\n";
	for (int i = 0; i < 3; ++ i) {
		if (i == 0) cout << "以中间值为基准数\n";
		else if (i == 1) cout << "以最小值为基准数\n";
		else if (i == 2) cout << "以最大值为基准数\n";
		
		cout << sam1 << endl;
		sam1.work(i);
		cout << sam1 << endl;
		sam1.show();
		
		cout << "=======分割线=======\n";
	}
	
	
	cout << "降序\n";
	for (int i = 0; i < 3; ++ i) {
		if (i == 0) cout << "以中间值为基准数\n";
		else if (i == 1) cout << "以最小值为基准数\n";
		else if (i == 2) cout << "以最大值为基准数\n";
		
		cout << sam2 << endl;
		sam2.work(i);
		cout << sam2 << endl;
		sam2.show();
		
		cout << "=======分割线=======\n";
	}
	
	
	
	return 0;
}
