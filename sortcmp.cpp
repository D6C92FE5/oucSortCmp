#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// 关键字比较次数
long long count_compare;
// 关键字移动次数
long long count_move;

// 关键字类型
class Item {
public:
	Item() {
		this->v = 0;
	}
	Item(int v) {
		this->v = v;
	}

	Item& operator=(Item &t) {
		count_move++;
		v = t.v;
		return *this;
	}

	bool operator==(Item &t) {
		count_compare++;
		return v == t.v;
	}
	bool operator!=(Item &t) {
		count_compare++;
		return v != t.v;
	}
	bool operator>(Item &t) {
		count_compare++;
		return v > t.v;
	}
	bool operator>=(Item &t) {
		count_compare++;
		return v >= t.v;
	}
	bool operator<(Item &t) {
		count_compare++;
		return v < t.v;
	}
	bool operator<=(Item &t) {
		count_compare++;
		return v <= t.v;
	}

	int v;
};

// 待排序数据量
const int N = 1000;

// 产生的测试数据
Item *testdata;
// 用于排序的数据
Item *sortdata;

// 交换两个关键字
void swap(Item &a, Item &b) {
	Item t = a;
	a = b;
	b = t;
}


// 冒泡排序
void sort_bubble(Item a[]) {
	for (int k = 1; k < N; k++) {
		bool swapped = false;
		for (int i = 0; i < N-k; i++) {
			if (a[i+1] < a[i]) {
				swap(a[i], a[i+1]);
				swapped = true;
			}
		}
		if (!swapped) {
			break;
		}
	}
}

// 插入排序
void sort_insertion(Item a[]) {
	for (int k = 1; k <= N; k++) {
		Item t = a[k];
		int i = k;
		while (i > 0 && a[i-1] > t) {
			a[i] = a[i-1];
			i--;
		}
		a[i] = t;
	}
}

// 选择排序
void sort_selection(Item a[]) {
	for (int k = 1; k < N; k++) {
		int t = k - 1;
		for (int i = k; i < N; i++) {
			if (a[i] < a[t]) {
				t = i;
			}
		}
		swap(a[k-1], a[t]);
	}
}

// 快速排序
void sort_quick(Item a[], int l, int r) {
	if (l >= r) {
		return;
	}
	int i = l;
	int j = r;
	Item x = a[l];
	while (i < j) {
		while (i < j && a[j] > x) {
			j--;
		}
		if (i < j) {
			a[i] = a[j];
			i++;
		}
		while (i < j && a[i] < x) {
			i++;
		}
		if (i < j) {
			a[j] = a[i];
			j--;
		}
	}
	a[i] = x;
	sort_quick(a, l, i - 1);
	sort_quick(a, i + 1, r);
}
// 快速排序 紧凑版
void sort_quick_compact(Item a[], int l, int r) {
	if (l >= r) return;
	int i = l, j = r;
	Item x = a[l];
	while (i < j) {
		while (i < j && a[j] > x) j--;
		if (i < j) a[i++] = a[j];
		while (i < j && a[i] < x) i++;
		if (i < j) a[j--] = a[i];
	}
	a[i] = x;
	sort_quick_compact(a, l, i - 1);
	sort_quick_compact(a, i + 1, r);
}
// 快速排序 开始
void sort_quick(Item a[]) {
	sort_quick(a, 0, N);
}

// 希尔排序
void sort_shell(Item a[]) {
	const int dn = 3;
	int da[dn] = { 5, 3, 1 };  // 步长序列
	for (int k = 0; k < dn; k++) {
		int d = da[k];
		for (int j = d; j <= N; j+=d) {
			Item t = a[j];
			int i = j;
			while (i > 0 && a[i-d] > t) {
				a[i] = a[i-d];
				i-=d;
			}
			a[i] = t;
		}
	}
}

// 堆排序 堆调整
void sort_heap_sift(Item a[], int i, int n) {
	int child;
	Item t;
	for (t = a[i]; n >= 2*i; i = child) {
		child = 2 * i;
		if (child != n && a[child+1] > a[child]) {
			child++;
		}
		if (t < a[child]) {
			a[i] = a[child];
		} else {
			break;
		}
	}
	a[i] = t;
}
// 堆排序
void sort_heap(Item a[]) {
	int i;
	for (i = N/2; i >= 1; i--) {
		sort_heap_sift(a, i, N);
	}
	for (i = N; i >= 2; i--) {
		swap(a[1], a[i]);
		sort_heap_sift(a, 1, i-1);
	}
}


// 生成随机的数据
void generate_data_random() {
	for (int i = 0; i < N; i++) {
		testdata[i].v = (rand() << 15) + rand();
	}
}
// 生成接近排好序的数据
void generate_data_nearly_sorted() {
	testdata[0].v = rand();
	for (int i = 1; i < N; i++) {
		testdata[i].v = testdata[i-1].v + (rand() & 7) - 1;
	}
}
// 生成已排好序的数据
void generate_data_sorted() {
	testdata[0].v = rand();
	for (int i = 1; i < N; i++) {
		testdata[i].v = testdata[i-1].v + (rand() & 15);
	}
}
// 生成逆序的数据
void generate_data_reversed() {
	testdata[0].v = 0x7FFFFFFF - rand();
	for (int i = 1; i < N; i++) {
		testdata[i].v = testdata[i-1].v - (rand() & 15);
	}
}
// 生成重复项很多的数据
void generate_data_few_unique() {
	for (int i = 0; i < N; i++) {
		testdata[i].v = rand() & 15;
	}
}

// 准备待排序数据
void prepare_data() {
	for (int i = 0; i < N; i++) {
		sortdata[i].v = testdata[i].v;
	}
	count_compare = 0;
	count_move = 0;
}
// 检查是否真的排好序了
void test_sorted() {
	for (int i = 1; i < N; i++) {
		if (sortdata[i-1] > sortdata[i]) {
			cout << " 排序结果有误" << endl << flush;
			throw new exception("排序结果有误");
		}
	}
}
// 输出关键字比较次数和关键字移动次数
void print_count() {
	cout << "比较 " << count_compare << " 次，";
	cout << "移动 " << count_move << " 次" ;
}

// 测试一个排序算法
void test(const char *name, void (*sort)(Item a[])) {
	prepare_data();
	sort(sortdata);
	cout << name;
	print_count();
	test_sorted();
	cout << endl;
}

// 测试一组数据
void test_all() {
	//test("    冒泡排序，", sort_bubble);
	//test("    插入排序，", sort_insertion);
	//test("    选择排序，", sort_selection);
	//test("    快速排序，", sort_quick);
	test("    希尔排序，", sort_shell);
	//test("    　堆排序，", sort_heap);
}

int main() {
	testdata = new Item[N];
	sortdata = new Item[N];
	srand(time(0));

	cout << "随机的数据" << endl;
	generate_data_random();
	test_all();
	cout << endl;

	cout << "接近已排序的数据" << endl;
	generate_data_nearly_sorted();
	test_all();
	cout << endl;

	cout << "已排好序的数据" << endl;
	generate_data_sorted();
	test_all();
	cout << endl;

	cout << "倒序的数据" << endl;
	generate_data_reversed();
	test_all();
	cout << endl;

	cout << "含有大量相同项的数据" << endl;
	generate_data_few_unique();
	test_all();
	cout << endl;

	return 0;
}
