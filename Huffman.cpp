#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
using namespace std;

struct NODE
{
	unsigned char c;	//Ki tu
	int tanso;	// So lan xuat hien
	bool done;	// Danh dau node da xu li 
	int Left;	// Chi so nut con ben trai
	int Right;	// Chi so nut con ben phai
};

struct MABIT
{
	char* bit;
	int soBit;
};

//const int MAX_NODE = 256 * 9;
//const int MAX_BIT_LEN = 10000;
#define MAX_NODE 256*9
#define MAX_BIT_LEN 10000

NODE TreeHuffman[MAX_NODE];
MABIT bangMaBit[256];


void KhoiTao()
{
	for (int i = 0; i < MAX_NODE; i++)
	{
		TreeHuffman[i].c = i;
		TreeHuffman[i].tanso = 0;
		TreeHuffman[i].done = false;
		TreeHuffman[i].Left = -1;
		TreeHuffman[i].Right = -1;
	}
}

void ThongKeTanSoXuatHien(char* tenFile)
{
	FILE* fi = fopen(tenFile, "rt");
	unsigned char c;

	while (1)
	{
		fscanf(fi, "%c", &c);
		if(feof(fi))
		{
			break;
		}
		TreeHuffman[c].tanso++; // Tang tan so len		
	}

	fclose(fi);
}

void XuatBangThongKe()
{
	printf("Bang thong ke tan suat: \n");
	for (int i = 0; i < 256; i++) {
		if (TreeHuffman[i].tanso > 0) {// Ghi chú: tại sao cần kiểm tra điều kiện này?
			printf("%c: %d\n", i, TreeHuffman[i].tanso);
		}
	}
}

bool Tim2PhanTuMin(int& i, int& j, int nNode) {
	i = -1;
	j = -1;

	// tim 2 phan tu co trong so nho nhat
	for (int k = 0; k < nNode; k++)
		if (TreeHuffman[k].done == false && TreeHuffman[k].tanso > 0) { // Ghi chú: Tại sao cần kiểm tra 2 điều kiện này
			if (i == -1) {
				i = k;
			}
			else if (j == -1) {
				j = k;
			}
			else {
				if (TreeHuffman[i].tanso > TreeHuffman[j].tanso) {
					if (TreeHuffman[k].tanso < TreeHuffman[i].tanso) {
						i = k;
					}
				}
				else {
					if (TreeHuffman[k].tanso < TreeHuffman[j].tanso) {
						j = k;
					}
				}
			}
		}
	// sap xep lai 2 phan tu de co i: phan tu co trong so nho nhat, j: phan tu co trong so nho nhi	
	// co 2 truong hop can doi i,j:
			// TreeHuffman[i].tanso > TreeHuffman[j].tanso
			// TreeHuffman[i].tanso == TreeHuffman[j].tanso va (TreeHuffman[i].c > TreeHuffman[j].c)
	if (i != -1 && j != -1) {
		if ((TreeHuffman[i].tanso > TreeHuffman[j].tanso) || ((TreeHuffman[i].tanso > TreeHuffman[j].tanso) && (TreeHuffman[i].c > TreeHuffman[j].c))) {
			int t = i;
			i = j;
			j = t;
		}
		return true;
	}
	else {
		return false;
	}
}


int TaoCayHuffman() {
	int nNode = 256;
	int i, j;
	bool timThay = false;
	while (true) {

		// Ghi chú: bước này để làm gì
		timThay = Tim2PhanTuMin(i, j, nNode);
		if (!timThay) {
			break;
		}

		// Ghi chú: bước này để làm gì
		TreeHuffman[nNode].c = (TreeHuffman[i].c < TreeHuffman[j].c) ? TreeHuffman[i].c : TreeHuffman[j].c;
		TreeHuffman[nNode].tanso = TreeHuffman[i].tanso + TreeHuffman[j].tanso;
		TreeHuffman[nNode].Left = i;
		TreeHuffman[nNode].Right = j;

		// Ghi chú: bước này để làm gì
		TreeHuffman[i].done = true;
		TreeHuffman[j].done = true;

		// Ghi chú: bước này để làm gì
		TreeHuffman[nNode].done = false;
		nNode++;

	}
	return nNode - 1; // Ghi chú: ý nghĩa của giá trị trả về?

}

void XuatCayHuffman(int node, int tab) {
	if (node == -1) {
		return;
	}
	for (int i = 0; i < tab; i++) {
		printf("\t");
	}
	if (TreeHuffman[node].Left == -1 && TreeHuffman[node].Right == -1) {
		printf("%c\n", TreeHuffman[node].c);
	}
	else {
		printf("%c..\n", TreeHuffman[node].c);
		XuatCayHuffman(TreeHuffman[node].Left, tab + 1);
		XuatCayHuffman(TreeHuffman[node].Right, tab + 1);
	}
}

void DuyetCayHuffman(int node, char maBit[], int nMaBit) {
	if (node == -1) {
		return;
	}
	if (TreeHuffman[node].Left == -1 && TreeHuffman[node].Right == -1) {	//Ghi chú: ý nghĩa của điều kiện kiểm tra là gì?	
		bangMaBit[node].soBit = nMaBit;
		bangMaBit[node].bit = new char[nMaBit];
		for (int i = 0; i < nMaBit; i++) {
			bangMaBit[node].bit[i] = maBit[i];
		}
		return;
	}
	else {
		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 0;
		DuyetCayHuffman(TreeHuffman[node].Left, maBit, nMaBit + 1);

		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 1;
		DuyetCayHuffman(TreeHuffman[node].Right, maBit, nMaBit + 1);
	}
}

void PhatSinhMaBit(int nRoot) { // Ghi chú: ý nghĩa của tham số nRoot?
	for (int i = 0; i < 256; i++) {
		bangMaBit[i].soBit = 0;
		bangMaBit[i].bit = NULL;
	};
	char maBit[MAX_BIT_LEN / 8];
	int nMaBit = 0;

	DuyetCayHuffman(nRoot, maBit, nMaBit);
}

void XuatBangMaBit() {
	for (int i = 0; i < 256; i++)
		if (bangMaBit[i].soBit > 0) {
			printf("%c: ", i);
			for (int j = 0; j < bangMaBit[i].soBit; j++)
				printf("%d", bangMaBit[i].bit[j]);
			printf("\n");
		}
}

void XuatByte(unsigned char out, int soBitCoNghia) {
	for (int i = 7; i >= 7 - soBitCoNghia + 1; i--) {
		if ((out >> i) & 1) {// Ghi chú: Thao tác này để làm gì?
			printf("1");
		}
		else {
			printf("0");
		}
	}
	printf(" ");
}

void MaHoa1KyTu(unsigned char c, unsigned char& out, unsigned char& viTriBit) { //Ghi chú: ý nghĩa của các tham số truyền vào?

	for (int i = 0; i < bangMaBit[c].soBit; i++) {
		if (bangMaBit[c].bit[i] == 1) {
			out = out | (1 << viTriBit); // Ghi chú: Thao tác này để làm gì?
		}
		if (viTriBit == 0) { // da du 1 byte, ghi byte do ra file
			viTriBit = 7;
			XuatByte(out, 8);
			out = 0;
		}
		else {
			viTriBit--;
		}
	}
}

void NenHuffman(char* inputFile) {
	// BUOC 1: thong ke tan suat xuat hien cua cac ki tu
	KhoiTao();

	ThongKeTanSoXuatHien(inputFile);

	//	XuatBangThongKe();

	// BUOC 2: tao cay Huffman

	int nRoot = TaoCayHuffman();

	//	XuatCayHuffman(nRoot, 0);

	// BUOC 3: phat sinh ma bit

	PhatSinhMaBit(nRoot);

	//	XuatBangMaBit();

	// BUOC 4: thay the cac ky tu bang ma bit tuong ung
	unsigned char out = 0;				// ky tu se xuat ra
	unsigned char soBitCoNghia = 0;		// byte cuoi co the ko su dung het cac bit nen can luu so bit co nghia cua byte cuoi

	unsigned char c;
	unsigned char viTriBit = 7;			//Ghi chú: ý nghĩa của biến viTriBit?
	FILE* fi = fopen(inputFile, "rt");
	while (true) {
		fscanf(fi, "%c", &c);
		if (feof(fi)) {
			break;
		}
		MaHoa1KyTu(c, out, viTriBit);
	}
	soBitCoNghia = 7 - viTriBit;
	if (soBitCoNghia == 0) {
		soBitCoNghia = 8;
	}
	else {
		XuatByte(out, soBitCoNghia);
	}

	fclose(fi);
}

void main() {
	// nen
	char in[] = "input.txt";
	NenHuffman(in);
}