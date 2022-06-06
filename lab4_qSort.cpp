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

//������������ 
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

//���رȽϵĿ����������� 
template<typename T>
void sort(T a[], int l, int r, bool (*cmp)(T a, T b)) {  //cmpΪ��������<����� 
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
	std::vector<T> vec;     //����ԭ���У��������ܲ���
	T* a;                   //ԭ���е�ָ�룬�����޸� 
	int st, ed;             //���е���ĩ�������䣩 
	bool (*cmp)(T a, T b);  //�ȽϺ�����Ĭ��std::less������ 
	std::vector<std::vector<std::pair<int, int> > > res;  //�洢��������е���Ϣ��pair�Ķ�ά����first��������ֵ��second����ģʽ
														  //ģʽ�� 
	int num_cmp, num_swap;  //����Ƚϴ����ͽ������� 
	std::set<std::pair<int, int> > vis;  //���ڡ���׼����<�������>/<������С>�ᵼ�µݹ������޷���С���������������������� 
public :
	Sort(T aa[], int ll, int rr, bool (*cmp_)(T a, T b)) : a(aa), st(ll), ed(rr), cmp(cmp_) {  //���ݳ�Ա��ʼ��
		vec.resize(ed + 1); for (int i = st; i <= ed; ++ i) vec[i] = a[i];  //����ԭ����
		num_cmp = num_swap = 0;
	}
	Sort(T aa[], int ll, int rr) : Sort(aa, ll, rr, std::less<T>() ) {}  //Ĭ�ϳ�ʼ��
	 
	friend std::ostream& operator << (std::ostream& out, const Sort& t) {  //���������������ǰ��������е�״̬ 
		out << "["; for (int i = t.st; i <= t.ed; ++ i) out << t.a[i] << " ]"[i == t.ed];
		return out;
	}
private: 
void dfs(int l, int r, int k, int op) {  //�㷨���ģ�l��r��������k��op�Ǹ���������k��ʾ�ݹ������op��ʾ����ģʽ
	while (k + 1 > res.size()) res.push_back(std::vector<std::pair<int, int> >(ed + 1, {0, 0}));  //��̬��չ�µĴ洢�������
	if (l >= r) {  //�ݹ�߽� 
		return ;
	}
	if (vis.count({l, r})) return ;
	vis.insert({l, r});   //�����ռ�ļ�¼�������ظ�����ͬһ����
	
	//�㷨���� 
	int i = l - 1, j = r + 1;
	T mid = a[l + r >> 1];
	
	if (op == 0) ;  //Ĭ��ģʽ��ѡ�м��
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
	std::cout << "�Ƚϴ�����" << num_cmp << "�� " << "���������� " << num_swap << std::endl;
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
	const int N = 10;  //������󳤶�
	const int M = 20;  //����Ԫ�����ֵ 
	int n = rand() % N + 1;
	cout << "���г��ȣ�" << n << endl;
	for (int i = 1; i <= n; ++ i) a[i] = b[i] = rand() % M + 1;
	
	Sort<int> sam1(a, 1, n, cmp_min);
	Sort<int> sam2(b, 1, n, cmp_max); 
	
	cout << "����\n";
	for (int i = 0; i < 3; ++ i) {
		if (i == 0) cout << "���м�ֵΪ��׼��\n";
		else if (i == 1) cout << "����СֵΪ��׼��\n";
		else if (i == 2) cout << "�����ֵΪ��׼��\n";
		
		cout << sam1 << endl;
		sam1.work(i);
		cout << sam1 << endl;
		sam1.show();
		
		cout << "=======�ָ���=======\n";
	}
	
	
	cout << "����\n";
	for (int i = 0; i < 3; ++ i) {
		if (i == 0) cout << "���м�ֵΪ��׼��\n";
		else if (i == 1) cout << "����СֵΪ��׼��\n";
		else if (i == 2) cout << "�����ֵΪ��׼��\n";
		
		cout << sam2 << endl;
		sam2.work(i);
		cout << sam2 << endl;
		sam2.show();
		
		cout << "=======�ָ���=======\n";
	}
	
	
	
	return 0;
}
