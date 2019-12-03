#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;
#define MAX_TREE 256

// to map each character its huffman value 
// Anh xa ki tu vao gia tri cay huffman
map<char, string> codes;

// to store the tansouency of character of the input data
// Luu tru tan so xuat hien
map<char, int> tanso;
map<char, int> tanso2;

// A Huffman tree node 
// Cay huffman
struct MinHeapNode
{
	char data;			 // ki tu 
	int tanso;			 // tan so xuat hien 
	MinHeapNode* left, * right; // con trai va con phai

	//khoi tao co ban
	MinHeapNode(char c, int tanso)
	{
		left = right = NULL;
		this->data = c;
		this->tanso = tanso;
	}
};

// utility function for the priority queue
// Chuc nang cho hang doi uu tien
// toan tu so sanh struct nap chong toan tu () de ho tro cho cai priority
struct compare
{
	bool operator()(MinHeapNode* l, MinHeapNode* r)
	{
		return (l->tanso > r->tanso);
	}
};

// STL priority queue to store heap tree, with respect 
// to their heap root node value 
//Hang doi uu tien luu tru cay Heap, lien quan den gia tri nut goc
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap2;

// utility function to print characters along with 
// there huffman value 
//in cac ki tu cua cay huffman
void printCodes(struct MinHeapNode* root, string str)
{
	if (!root)
		return;
	if (root->data != '$')
		cout << root->data << ": " << str << "\n";
	printCodes(root->left, str + "0");
	printCodes(root->right, str + "1");
}

// utility function to store characters along with 
// there huffman value in a hash table, here we 
// have C++ STL map 
//luu tru ki tu voi gia tri cay huffman trong bang bam
void storeCodes(struct MinHeapNode* root, string str)
{
	if (root == NULL)
		return;
	if (root->data != '$')
		codes[root->data] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}

// function to build the Huffman tree and store it 
// in minHeap 
//ham xay dung cay huffman va luu tru, in minheap
void HuffmanCodes(int size)
{
	MinHeapNode* left, * right, * top;
	// i chi mot cap gia tri trong map, no tro den first, second la hai gia tri
	for (map<char, int>::iterator i = tanso.begin(); i != tanso.end(); i++)
		minHeap.push(new MinHeapNode(i->first, i->second));
	while (minHeap.size() != 1)
	{
		left = minHeap.top();
		minHeap.pop();
		right = minHeap.top();
		minHeap.pop();
		top = new MinHeapNode('$', left->tanso + right->tanso);
		top->left = left;
		top->right = right;
		minHeap.push(top);
	}
	storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its 
// tansouency in input string 
//Luu tru map voi moi ki tu voi tan suat
void calctanso(string str)
{
	for (int i = 0; i < str.size(); i++)
		tanso[str[i]]++;
}

// function iterates through the encoded string s
// Ham lap qua chuoi s duoc ma hoa
// if s[i]=='1' then move to node->right 
// if s[i]=='0' then move to node->left 
// if leaf node append the node->data to our output string 
// node la noi them data vao chuoi string
string decode_file(struct MinHeapNode* root, string s)
{
	string ans = "";
	struct MinHeapNode* curr = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		// reached leaf node 
		//dat den nut la
		if (curr->left == NULL and curr->right == NULL)
		{
			ans += curr->data;
			curr = root;
		}
	}
	// cout<<ans<<endl; 
	return ans + '\0';
}

//chuyen doi tu nhi prhan sang thap phan cua 8 bit
int BinToDec(const string& Bin)
{
	int Dec = 0;
	//string Tam;
	for (int i = 0; i < 8; i++)
	{
		Dec += ((int)Bin[i]-48) * pow(2, 7-i);
	}
	//Tam = to_string(Dec);
	//return Tam;
	return Dec;
}

//Kiem tra co du 8 bit hay khong tinh tu vi tri vt den cuoi chuoi
bool Kiemtradu8bit(const string& a, int vt)
{
	if (vt + 7 < a.size())
		return true;
	return false;
}

//Lay 8 bit tu chuoi string ke tu vi tri vt
string Lay8bit(const string& a, int vt)
{
	string tam;
	for (int i = vt; i < vt + 8; i++)
	{
		tam += a[i];
	}
	tam = tam + '\0';
	return tam;
}


//Ham them cho du 4 ki tu trong chuoi nhi phan
string ThemDu8Bit(string bin)
{
	int max = 8 - bin.length();
	for (int i = 0; i < max; i++)
		bin = "0" + bin;
	return bin;
}
string ChuyenSangNhiPhan(int n)
{
	if (n == 0)
		return n + "";

	if (n % 2 == 0)
		return ChuyenSangNhiPhan(n / 2) + "0";
	else
		return ChuyenSangNhiPhan(n / 2) + "1";
}
//int StringToInt(string s)
//{
//	int kq = 0;
//	for (int i = 0; i < s.size(); i++)
//	{
//		kq += ((int)s[i] - 48) * pow(10, s.size() - i - 1);
//	}
//	return kq;
//}

int StringToInt(string s)
{
	int kq = 0;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			kq += ((int)s[i] - 48) * pow(10, s.size() - i - 1);
		else continue;
	}
	return kq;
}

//gan tan so tu chuoi s da doc duoc vao freq
void ThemTanSo(map<char, int> &freq, string s)
{
	//map<char, int>::iterator i;
	string tam;
	for (int i = 2; i < s.size(); i++)
	{
		tam += s[i];
	}
	tam += '\0';
	//cout << tam;
	freq.insert(pair<char, int>(s[0], StringToInt(tam)/100));
}
void LNR(MinHeapNode* root)
{
	if (root != NULL)
	{
		LNR(root->left);
		cout << root->data;
		LNR(root->right);
	}
}
int main()
{

//=================================Nen nha====================================================
	//string Data;
	//string Line;
	//fstream infile;
	//infile.open("in.txt", ios::in);
	//while (!infile.eof())
	//{
	//	getline(infile, Line);
	//	Data += Line;
	//	Data += "\n";
	//}
	//infile.close();
	//Data.pop_back();
	////cout << Data;
	////cout << Data;
	////string str = "PhamVanThat";
	//string encodedString, decodedString;
	////calctanso(str);
	////calctanso(Data);
	////HuffmanCodes(str.length());
	////HuffmanCodes(Data.length());
	////cout << "Character With there frequencies:\n";
	//// kieu auto: tu dong nhan biet kieu du lieu cua minh
	////for (auto i = codes.begin(); i != codes.end(); i++)
	//	//cout << i->first << ' ' << i->second << endl;

	////for (auto i : str)
	////for (auto i : Data)
	//	//encodedString += codes[i];
	////cout << encodedString.size();
	////cout << encodedString.size();
	////cout << "\nEncoded Huffman data:\n" << encodedString << endl;

	////decodedString = decode_file(minHeap.top(), encodedString);
	////cout << "\nDecoded Huffman Data:\n" << decodedString << endl;
	////LNR(minHeap.top());

	//string Encode;
	//char m;
	//for (int i = 0; i < encodedString.size();i++)
	//{
	//	if (Kiemtradu8bit(encodedString, i))
	//	{
	//		m = BinToDec(Lay8bit(encodedString, i));
	//		Encode += m;
	//		m = '\0';
	//		//Encode += ",";
	//		i += 7;
	//	}
	//	else
	//	{
	//		Encode += "!";
	//		Encode += encodedString[i];
	//	}
	//}

	////cout << Encode;

	//ofstream outfile;
	//outfile.open("filenen.txt");
	////char a;
	//////outfile << encodedString;
	////char z;
	////for (int i = 0; i < encodedString.size(); i++)
	////{
	////	if (Kiemtradu8bit(encodedString, i))
	////	{
	////		z = BinToDec(Lay8bit(encodedString, i));
	////		outfile << z;
	////		//outfile << ",";
	////		i += 7;
	////	}
	////	else outfile << encodedString[i];
	////}
	//outfile << tanso.size();
	//outfile << "\n";
	//for (map<char, int>::iterator i = tanso.begin(); i != tanso.end(); i++)
	//{
	//	outfile << i->first;
	//	outfile << " ";
	//	outfile << i->second;
	//	outfile << "\n";
	//}
	//outfile << Encode;
	////outfile << encodedString;
	//outfile.close();





//=================================Giai nen ne========================================
	string Data2;
	string Line2;
	fstream infile2;
	infile2.open("filenen.txt", ios::in);
	string n;
	getline(infile2, n);
	int sodong = StringToInt(n);
	//cout << sodong;
	for (int i = 0; i < sodong; i++)
	{
		string dong;
		getline(infile2, dong);
		dong += '\0';
		//cout << dong << endl;
		ThemTanSo(tanso2, dong);
	}
	for (auto i = tanso2.begin(); i != tanso2.end(); i++)
		cout << i->first << ' ' << i->second << endl;
	/*while (!infile2.eof())
	{
		getline(infile2, Line2);
		Data2 += Line2;
	}
	infile2.close();*/
	
	////cout << Data;

	//string kq;
	//for (int i = 0; i < Data2.size(); i++)
	//{
	//	kq += ThemDu8Bit(ChuyenSangNhiPhan((int)Data2[i]));
	//}

	//cout<<decode_file(minHeap.top(), Data2);

	system("pause");
	return 0;
}