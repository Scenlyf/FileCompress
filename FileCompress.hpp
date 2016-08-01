#pragma once

#include"HuffmanTree.hpp"
#include<fstream>
#include<string>
#include<stdio.h>
#define _DEBUG_//文件压缩，大文件出错：文件没有读完，压缩的文件中（随意组成）存在控制字符

typedef long long LongType;
struct CharInfo
{
	unsigned char _ch; //字符
	LongType _count;  //字符出现的次数
	string _code;     //huffman code

	CharInfo(const int& count = 0)//不用初始化string
		:_ch(0)
		, _count(count)
	{}
	bool operator < (const CharInfo& info)const
	{
		return _count < info._count;
	}
	bool operator != (const CharInfo& info)const
	{
		return _count != info._count;
	}
	CharInfo operator + (const CharInfo& info)const
	{
		return  CharInfo(_count + info._count);
	}
};
class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}
	void Compress(const char* filename)//压缩文件
	{
		//读取文件，统计文件字符出现的次数
		FILE* fOut = fopen(filename, "rb");//对于大文件，考虑二进制读写文件
		assert(fOut);
		char ch = fgetc(fOut);//不用unsigned
		while (!feof(fOut))//FEOF：检查文件标志位（返回-1结束）一定可以读到文件的末尾，在文件中做了文件标志位
		{
			_infos[(unsigned char)ch]._count++;//转化为unsigned
			ch = fgetc(fOut);
		}
		
		//构造Huffman Tree
		CharInfo invalid(0);//设置非法值
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		
		//生成Huffman Code
		string cd;
		GenerateHuffmanCode(tree.GetRootNode(), *this, cd);

		//配置文件：存放存在的字符及对应的huffman编码，还有进行压缩的字符总个数
		string CompressConfig = filename;
		CompressConfig += ".Config";
		FILE* fInConfig = fopen(CompressConfig.c_str(), "wb");
		assert(fInConfig);

		//总字符数为树的根结点的权重
		string str; //使用str来保存出现的字符
		char buffer[128] = { 0 };
		for (size_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count > 0)
			{
				str += _infos[i]._ch;
				str += ',';
				//第一个参数是要转换的数字，第二个参数是要写入转换结果的目标字符串，第三个参数是转移数字时所用的基数
				_itoa(_infos[i]._count, buffer, 10);//表示10进制的int类型转换成char类型
				str += buffer;
				str += '\n';
				fputs(str.c_str(), fInConfig);
				str.clear();//每次对str进行清除，就能够保存下一个出现的字符
			}
		}

		//压缩文件
		string uncompress = filename;
		uncompress += ".uncompress";
		FILE* fIn = fopen(uncompress.c_str(), "wb");
		//读取文件，找出对应字符的Huffman编码，转化成为位
		fseek(fOut, 0, SEEK_SET);//函数fseek()给出的流设置位置数据,SEEK_SET为从文件的开始处开始搜索.fseek()成功时返回0, 失败时返回非零.
		ch = fgetc(fOut);
		int value = 0;
		int pos = 0;
		while (!feof(fOut))//!feof(fOut)(针对大文件)
		{
			string code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')//Huffman编码中有1的，将对应位设为1
				{
					value |= 1;
				}
				if (++pos == 8)
				{
					fputc(value, fIn);//写入一个字符后pos和value置0
					pos = 0; value = 0;
				}
			}
			ch = fgetc(fOut);
		}
		if (pos != 0)
		{
			fputc(value << (8 - pos), fIn);//不足一个字节的后面补0
		}
		fclose(fOut);
		fclose(fIn);
		fclose(fInConfig);
	}
	void GenerateHuffmanCode(HuffmanNode<CharInfo>* root, FileCompress& file, string code)//递归实现Huffman Code
	{//左0右1
		if (root == NULL)
			return;
		if (root->_left == NULL && root->_right == NULL)//叶子结点
		{
			_infos[root->_weight._ch]._code = code;
#ifdef _DEBUG_
			cout << root->_weight._ch << ": " << code << endl;//调试信息
#endif _DEBUG_
		}
		//code.push_back('0');
		//GenerateHuffmanCode(root->_left, file, code);//string中可以+
		//code.pop_back();

		//code.push_back('1');
		//GenerateHuffmanCode(root->_right, file, code);
		//code.pop_back();

		if (root->_left)
		{
			GenerateHuffmanCode(root->_left, file, code + '0');//string中可以+
		}
		if (root->_right)
		{
			GenerateHuffmanCode(root->_right, file, code + '1');
		}
	}

	void UnCompress(const char* filename)//解压缩文件
	{
		//从配置文件中读取文件的字符及对应字符出现的次数，得到_infos[256]
		FILE* fOutConfig = fopen("Input.BIG.config", "rb");
		assert(fOutConfig);

		//修改_count,注意\n，有可能代表字符，有可能是行结束标志
		//原型 char *  fgets(char * s, int n,FILE *stream);
		//参数：
	    //s : 字符型指针，指向存储读入数据的缓冲区的地址。
	    //n : 从流中读入n - 1个字符
        //stream ： 指向读取的流。
		//返回值：
		//1. 当n <= 0 时返回NULL，即空指针。
		//2. 当n = 1 时，返回空串"".
		//3. 如果读入成功，则返回缓冲区的地址。
		//4. 如果读入错误或遇到文件结尾(EOF)，则返回NULL.

		//fgets(...)读入文本行的两种情况
		//1、如果n大于一行的字符串长度，那么当读到字符串末尾的换行符时，fgets(..)会返回。
		//并且在s的最后插入字符串结束标志'\0'。 而s缓冲区剩余的位置不会再填充。
		//2、如果n小于等于一行的字符串的长度，那么读入n-1个字符，此时并没有读入\n因为并没有到行尾 ，同样在最后会插入'\0'.
		
		//fgets(...)读入整个文件内容
		//通常用while()循环来使fges()读入文本全部内容，并按行读入
		//当然如果n小于每行的字符个数，也可以读，只不过读的次数要多。n++

		//fgets(...)从标准设备读数据。
		//用fgets(...)还也读入标准输入设备(一般为键盘)的信息
		//原型  ：  fgets(s, n, stdin);
		//假设在控制台下，我们可以用fgets(...)替代gets(), 读入键盘输入的信息，fgets()是安全的，因为不会像gets()有溢出的可能。。

		unsigned char ch = fgetc(fOutConfig);
		char buff[128] = { 0 };
		while (!feof(fOutConfig))
		{
			fgetc(fOutConfig);//读取了逗号
			fgets(buff, 128, fOutConfig);//fgets(...)读入文本行
			_infos[ch]._count = (LongType)atoi(buff);
			ch = fgetc(fOutConfig);//读取下一行首字符
		}

		//读取压缩文件
		FILE* fOut = fopen(filename, "rb");
		assert(fOut);

		//解压后的文件
		string compare = filename;
		compare += ".compare";
		FILE* fIn = fopen(compare.c_str(), "wb");
		assert(fIn);

		//建立哈夫曼树
		CharInfo invalid(0);//设置非法值
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		HuffmanNode<CharInfo>* root = tree.GetRootNode();
		HuffmanNode<CharInfo>* cur = root;
		ch = fgetc(fOut);
		int count = root->_weight._count;//总字符数为树的根结点的权重
		int pos = 7;
		while (count > 0)//读取总共count个字符并写入压缩后的字符
		{
			while (pos >= 0)
			{
				if (ch & (1 << pos))//
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL && cur->_right == NULL)
				{
					fputc(cur->_weight._ch, fIn);
					if (--count == 0)//将剩余没有写入的字符总次数减1
						break; 
					cur = root;
				}
				pos--;
			}
			pos = 7;
			ch = fgetc(fOut);
		}
		fclose(fOut);
		fclose(fIn);
		fclose(fOutConfig);
	}
private:
	CharInfo _infos[256];
}; 
