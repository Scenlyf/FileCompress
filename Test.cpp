#define _CRT_SECURE_NO_WARNINGS 1
//#include"Heap.hpp"
//#include"HuffmanTree.hpp"
#include"FileCompress.hpp"
#include<ctime>
//void Test()
//{
//	int a[10] = { 1, 3, 5, 0, 4, 2, 6, 7, 8, 9 };
//	//int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//	//Heap<int,Less> h(a, 10);
//	HuffmanTree<int> hf(a, 10, 0);
//}

void TestCompress()
{
	cout << "压缩时间：";
	clock_t start, finish;
	start = clock();

	FileCompress fc;
	//fc.Compress("lyf.com");
	fc.Compress("Input.BIG");

	finish = clock();
	cout << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
}
void TestUnCompress()
{
	cout << "解压时间：";
	clock_t start, finish;
    start = clock();

	FileCompress fc;
	//fc.UnCompress("lyf.com.uncompress");
	fc.UnCompress("Input.BIG.uncompress");

	finish = clock();
    cout << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
}
int main()
{
	//Test();
	//TestCompress();
	TestUnCompress();
	system("pause");
	return 0;
}
