#include "matrix.h"


Matrix::Matrix() // Khoi tao matrix rong
{
	_a = NULL;
	_dong = _cot = 0;
}

Matrix::~Matrix() // Thu hoi vung nho Matrix
{
	/*if (_a != NULL)
	{
		for (int i = _dong - 1; i >= 0; i--)
			delete[] _a[i];
		delete[]_a;
	}*/
	//_a = NULL;
}

void Matrix::Input() // Tao vung nho va nhap Matrix
{
	cout << "Nhap so dong cua Matrix: ";
	cin >> _dong;
	cout << "Nhap so cot cua Matrix: ";
	cin >> _cot;
	_a = new double* [_dong];
	for (int i = 0; i < _dong; i++)
		* (_a + i) = new double[_cot];
	for (int i = 0; i < _dong; i++)
		for (int j = 0; j < _cot; j++)
		{
			cout << "Nhap a[" << i << "][" << j << "]: ";
			cin >> *(*(_a + i) + j);
		}
}

void Matrix::Output() // Xuat Matrix
{
	if (_a == NULL)
		return;
	cout << "Matrix " << _dong << "x" << _cot << endl;
	for (int i = 0; i < _dong; i++)
	{
		for (int j = 0; j < _cot; j++)
		{
			cout << *(*(_a + i) + j) << " ";
		}
		cout << endl;
	}
}

void Matrix::TruDong(double n, int a, double m, int b) // dong a = n* dong a - m* dong b
{
	for (int i = 0; i < _cot; i++)
		_a[a][i] = n * _a[a][i] - m * _a[b][i];
}

void Matrix::HoanViDong(int a, int b) // Hoan vi dong a va dong b
{
	if (a >= _dong || b >= _dong)
		return;
	double* p;
	p = *(_a + a);
	*(_a + a) = *(_a + b);
	*(_a + b) = p;
}

void Matrix::HoanViCot(int a, int b) // Hoan vi cot a va cot b
{
	if (a >= _cot|| b >= _cot)
		return;
	for (int i = 0; i < _dong; i++)
		swap(_a[i][a], _a[i][b]);
}

Matrix Matrix::CoppyMatrix() // Tao ra 1 matrix giong matrix goc
{
	Matrix temp;
	temp._dong = _dong;
	temp._cot = _cot;
	temp._a = new double* [_dong];
	for (int i = 0; i < _dong; i++)
		temp._a[i] = new double[_cot];
	for (int i = 0; i < _dong; i++)
		for (int j = 0; j < _cot; j++)
			temp._a[i][j] = _a[i][j];
	return temp;
}

Matrix Matrix::EchelonMatrix() // Dua ve matrix bac thang, su dung thuat toan Gauss
{
	Matrix temp;
	temp = CoppyMatrix();
	int i = 0, j = 0;
	while (i < temp._dong -1 && j < temp._cot -1)
	{
		if (temp._a[i][j])
		{
			for (int k = i + 1; k < temp._dong; k++)
			{
				temp.TruDong(temp._a[i][j], k, temp._a[k][j], i);
			}
			i++;
			j++;
		}
		else
			for (int k = i + 1; k < temp._dong; k++)
				if (temp._a[k][j])
				{
					temp.HoanViDong(i, k);
					break;
				}
				else if (k == temp._dong - 1)
					j++;
	}
	return temp;
}

bool Matrix::IsEmptyRow(int n) // Kiem tra hang n trong matrix co rong khong?
{
	if (n >= _dong)
		return false; // Hang khong ton tai
	for (int i = _cot - 1; i >= 0; i--)
		if (_a[n][i])
			return true; // Hang khong rong
	return false; // Hang rong
}

Matrix Matrix::Matrixij(int i, int j)// Matrix matrix bo di dong i va cot j
{
	Matrix m;
	m._a = new double* [_dong-1];
	for (int i = 0; i < _dong-1; i++)
		m._a[i] = new double[_cot-1];
	m._dong = _dong - 1;
	m._cot = _cot - 1;
	for (int a = 0; a < _dong-1; a++)
		for (int b = 0; b < _cot-1; b++)
		{
			if (a < i && b < j)
				m._a[a][b] = _a[a][b];
			if (a < i && b >= j)
				m._a[a][b] = _a[a][b + 1];
			if (a >= i && b < j)
				m._a[a][b] = _a[a + 1][b];
			if (a >= i && b >= j)
				m._a[a][b] = _a[a + 1][b + 1];
		}
	return m;
}

double Matrix::Det() // Tinh dinh thuc cua Matrix
{
	if (_dong != _cot || _dong == 0)
	{
		cout << "Khong ton tai dinh thuc!!" << endl;
		return 0;
	}
	if (_dong == 1)
		return **_a;
	if (_dong == 2)
		return _a[0][0] * _a[1][1] - _a[0][1] * _a[1][0];

	double s = 0;
	for (int i = 0; i < _cot; i++)
		if (i % 2 == 0)
			s+= _a[0][i] * Matrixij(0, i).Det();
		else
			s-=_a[0][i] * Matrixij(0, i).Det();
	return s;
}

Matrix Matrix::NghichDao() // Tim ma tran nghich dao
{
	if (_dong != _cot || _dong == 0 || Det() == 0 || RankMatrix() != _dong)
	{
		cout << "Matrix khong kha nghich!!" << endl;
		return Matrix();
	}

	Matrix p;
	p._a = new double* [_dong];
	for (int i = 0; i < _dong; i++)
		p._a[i] = new double[_dong];
	p._dong = p._cot = _dong;
	double det = Det();

	for (int i = 0; i < _dong; i++)
		for (int j = 0; j < _dong; j++)
			if ((i + j) % 2)
				p._a[j][i] = -Matrixij(i, j).Det() / det;
			else
				p._a[j][i] = Matrixij(i, j).Det() / det;
	return p;
}

int Matrix::RankMatrix() // Tinh rank Matrix
{
	Matrix temp = EchelonMatrix();
	for (int i = _cot - 1; i >= 0; i--)
		if (temp.IsEmptyRow(i) == true) // Hang thu i + 1 khong rong
			return i + 1;
	return 0; // Matrix rong
}

Matrix Matrix::MultiplyMatrix(Matrix m) // Tich 2 Matrix
{
	if (_cot != m._dong)
	{
		cout << "Khong the tich hai Matrix!!" << endl;
		return Matrix();
	}

	Matrix temp;
	temp._dong = _dong;
	temp._cot = m._cot;
	temp._a = new double* [temp._dong];

	for (int i = 0; i < temp._dong; i++)
	{
		temp._a[i] = new double[temp._cot];
		for (int j = 0; j < temp._cot; j++)
		{
			temp._a[i][j] = 0;
			for (int k = 0; k < _cot; k++)
				temp._a[i][j] += _a[i][k] * m._a[k][j];
		}
	}
		

	return temp;
}


void Matrix::GiaiHePhuongTrinhTuyenTinh() // Giai he phuong trinh tuyen tinh voi a[i]*x + b[i]*y + .... = z[i] voi i la so dong
{
	Matrix* p;
	p = &EchelonMatrix();

	cout << 1;

	int rank = RankMatrix();

	cout << rank << endl;

	for (int i = _cot - 2; i >= 0; i--)
	{
		if (_a[rank - 1][i])
			break;
		if (i == 0) // khong co an nhung bang mot so khac 0 (0*x + 0*y +.... = z)
		{
			cout << "Phuong trinh vo nghiem!" << endl;
			return;
		}
	}

	if (rank < _cot - 1) // So luong an nhieu hon so luong dong
	{
		cout << "Phuong trinh vo so nghiem" << endl;
		return;
	}

	_cot--;
	double delta = Det();
	_cot++;
	string ng = "xyztuvwpqrsmno";
	cout << "Phuong trinh co nghiem la:" << endl;
	for (int i = 0; i < _cot - 1; i++)
	{
		HoanViCot(i, _cot - 1);
		_cot--;
		cout << ng[i] << "= " << double(int(Det() * 1000)) / 1000 / delta << endl;
		_cot++;
		HoanViCot(i, _cot - 1);
	}
	return;
}
