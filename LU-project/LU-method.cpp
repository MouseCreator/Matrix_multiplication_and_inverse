#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
std::vector<double> slice(std::vector<double> const& v, int m, int n)
{
	auto first = v.begin() + m;
	auto last = v.begin() + n ;

	std::vector<double> vec(first, last);
	return vec;
}
bool isPowerOfTwo(int n)
{
	return (ceil(log2(n)) == floor(log2(n)));
}
class Row {
private:
	std::vector<double> numbers;
public:
	Row() {}
	Row(std::size_t size) {
		for (int i = 0; i < size; i++) {
			numbers.push_back(0);
		}
	}
	double operator [](std::size_t i) const {
		return numbers[i];
	}
	double& operator [](std::size_t i) {
		return numbers[i];
	}
	Row operator +(Row& other) {
		Row result;
		std::size_t minSize = std::min(this->numbers.size(), other.numbers.size());

		for (std::size_t i = 0; i < minSize; i++) {
			result.numbers.push_back(this->get(i) + other.get(i));
		}

		for (std::size_t i = minSize; i < this->numbers.size(); i++) {
			result.numbers.push_back(this->get(i));
		}

		for (std::size_t i = minSize; i < other.numbers.size(); i++) {
			result.numbers.push_back(other.get(i));
		}

		return result;
	}
	Row operator -(Row& other) {
		Row result;
		std::size_t minSize = std::min(this->numbers.size(), other.numbers.size());

		for (std::size_t i = 0; i < minSize; i++) {
			result.numbers.push_back(this->get(i) - other[i]);
		}

		for (std::size_t i = minSize; i < this->numbers.size(); i++) {
			result.numbers.push_back(this->get(i));
		}

		for (std::size_t i = minSize; i < other.numbers.size(); i++) {
			result.numbers.push_back(other.get(i));
		}

		return result;
	}
	Row& operator -=(const Row& other) {

		for (std::size_t i = 0; i < size(); i++) {
			this->numbers[i] = (this->get(i) - other[i]);
		}

		return *this;
	}
	Row operator *(double& by) {
		Row result;
		result.numbers = this->numbers;
		for (std::size_t i = 0; i < numbers.size(); i++) {
			result.numbers[i] *= by;
		}
		return result;
	}
	Row operator /(double& by) {
		Row result;
		result.numbers = this->numbers;
		for (std::size_t i = 0; i < numbers.size(); i++) {
			result.numbers[i] /= by;
		}
		return result;
	}
	void add(double value) {
	
		this->numbers.push_back(value);
	}
	void set(double value, std::size_t index) {
		numbers[index] = value;
	}
	double get(std::size_t index) {
		return numbers[index];
	}
	std::size_t size() {
		return numbers.size();
	}
	Row getSlice(int beg,int end) {
		Row row(end-beg);
		if (end - beg > this->numbers.size()) {
			std::cout << "Incorrect data\n";
		}
		else {
			row.numbers = slice(this->numbers, beg, end);
		}
		return row;
	}
	void print() {
		for (int i = 0; i < numbers.size(); i++) {
			std::cout << numbers[i] << std::endl;
		}
	}
	bool operator ==(Row other) {
		if (this->numbers.size() != other.numbers.size()) {
			return false;
		}
		else {
			for (int i = 0; i < this->numbers.size(); i++) {
				if (this->numbers[i] != other.numbers[i]) {
					return false;
				}
			}
		}
	}
	bool operator !=(Row other) {
		Row row = *this;
		return !(row == other);
	}
};

class Matrix {

private:
	std::vector<Row> table;
	int n;

	int extendRow(std::vector<double>& row) {
		for (int i = row.size(); i <= n; i++) {
			row.push_back(0);
		}
		return row.size();
	}


	void addRowFromLine(std::string line) {
		std::istringstream iss(line);
		double toAdd;
		Row newRow;
		while (iss >> toAdd) {
			newRow.add(toAdd);
		}
		this->table.push_back(newRow);
		n++;
	}

	std::size_t maxRowLength() {
		if (n == 0) {
			return 0;
		}
		std::size_t result = table[0].size();
		for (int i = 1; i < n; i++) {
			std::size_t currentSize = this->table[i].size();
			if (currentSize > result)
				result = currentSize;
		}
		return result;
	}
	void toSquareMatrix() {
		std::size_t dimention = maxRowLength();
		for (int i = 0; i < n; i++) {
			for (int j = this->table[i].size(); j < dimention; j++) {
				this->table[i].add(0);
			}
		}
		if (dimention > n) {
			Row emptyRow(dimention);
			for (; n < dimention; n++) {
				this->table.push_back(emptyRow);
			}
		}
	}
	void setZero() {
		for (int i = 0; i < n; i++) {
			this->table[i]-=this->table[i];
		}
	}
public:
	Row operator [](std::size_t i) const {
		return table[i];
	}
	Row& operator [](std::size_t i) {
		return table[i];
	}
	Matrix operator *(Matrix other) {
			if (this->n != other.n) 
			{
				std::cout << this->n << " " << other.n << std::endl;
				std::cout << "Matrixes has different dimentions!" << std::endl;
				return Matrix(n);
			}
			Matrix result;
			for (int i = 0; i < n; i++) {
				Row newRow;
				result.n++;
				for (int j = 0; j < n; j++) {
					double toAdd = 0.0;
					for (int k = 0; k < n; k++) {
						toAdd += this->table[i][k] * other.table[k][j];
					}
					newRow.add(toAdd);
				}
				result.table.push_back(newRow);
			}
			return result;
	}
	
	Matrix operator -(Matrix other) {
		Matrix result;
		for (int i = 0; i < n; i++) {
			result.table.push_back(this->table[i]-other.table[i]);
		}
		result.n = this->n;
		return result;
	}
	Matrix operator +(Matrix other) {
	
		Matrix result;
		for (int i = 0; i < n; i++) {
			result.table.push_back(this->table[i] + other.table[i]);
		}
		result.n = this->n;
		return result;
	}
	bool operator ==(Matrix other) {
		if (this->n != other.n) {
			return false;
		}
		else {
			for (int i = 0; i < this->n; i++) {
				if (this->table[i] != other.table[i]) {
					return false;
				}
			}
		}
		return true;
	}
	Matrix(int size) {
		this->n = size;
		for (int i = 0; i <size; i++) {
			table.push_back(Row(size));
		}
	}
	Matrix() {
		n = 0;
	}


	int getSize() {
		return this->n;
	}
	

	double get(int x, int y) {
		x = abs(x);
		y = abs(y);
		return (x < n && y < n) ? this->table[x][y] : 0;
	}

	void clear() {
		this->n = 0;
		this->table.clear();
	}

	void print() {
		std::cout << "Matrix:" << std::endl;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << table[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}


	bool loadFromFile(std::string filename) {
		std::ifstream infile;
		infile.open(filename);
		if (infile.fail()) {
			std::cout << "Can't open file" << std::endl;
			return true;
		}
		else {
			this->clear();
			std::string line;
			while (std::getline(infile, line))
			{
				if (!line.empty())
					this->addRowFromLine(line);
			}
			this->toSquareMatrix();
		}
	}
	
	void addColumn(std::vector<double> column) {
		if (this->n == 0) {
			std::size_t size = column.size();
			
				this->table.resize(size);
			
			this->n = size;
		}
		for (int i = 0; i < n; i++) {
			this->table[i].add(column[i]);
		}
	}
	
	void inverseLU() {
		Matrix L(this->n);
		Matrix U = (*this);

		for (std::size_t i = 0; i < n; i++) {
			double divideBy = U[i][i];
			L[i][i] = 1;
			for (std::size_t j = i + 1; j < n; j++) {
				double coeficeint = U[j][i] / divideBy;
				U[j] -= U[i] * coeficeint;
				L[j][i] = coeficeint;
			}
		}
		L.print();
		U.print();
	}
	void inverseLowerTriangle() {
		Matrix E(this->n);
		for (int i = 0; i < n; i++) {
			E[i][i] = 1;
		}

	}
	std::vector<Matrix> decompose() {
		/*std::vector<Matrix> v;
		Row row(n/2);
		Matrix matr;
		
		std::vector<double> vec;
		for (std::size_t i = 0; i < this->n/2; i++) {
			for (std::size_t j = 0; j < this->n/2; j++) {
				vec.push_back(this->get(j, i));
			}
		
			matr.addColumn(vec);
			vec.clear();
		}
		v.push_back(matr);
		matr.clear();
		vec.clear();
		for (std::size_t i = this->n/2; i < this->n; i++) {
			for (std::size_t j = 0; j < this->n/2; j++) {
				vec.push_back(this->get(j, i));
			}
			matr.addColumn(vec);
			
			vec.clear();
		}
		v.push_back(matr);
		matr.clear();
		vec.clear();
		for (std::size_t i = 0; i < this->n/2; i++) {
			for (std::size_t j = this->n / 2; j < this->n; j++) {
				vec.push_back(this->get(j, i));
			}
			matr.addColumn(vec);
			vec.clear();
		}
		v.push_back(matr);
		matr.clear();
		vec.clear();
		for (std::size_t i = this->n / 2; i < this->n; i++) {
			for (std::size_t j = this->n / 2; j < this->n; j++) {
				vec.push_back(this->get(j, i));
			}
			
			matr.addColumn(vec);
			vec.clear();
		}
		v.push_back(matr);
		matr.clear();
		vec.clear();
	
		return v;*/
		Row row;
		Matrix A11;
		Matrix A12;
		Matrix A21;
		Matrix A22;
		for (int i = 0; i < this->n / 2; i++) {
			A11.addRow(this->table[i].getSlice(0, this->n / 2));
		}
		for (int i = 0; i < this->n / 2; i++) {
			A12.addRow(this->table[i].getSlice(this->n / 2, this->n));
		}
		for (int i = this->n / 2; i < this->n; i++) {
			A21.addRow(this->table[i].getSlice(0, this->n/2));
		}
		for (int i = this->n / 2; i < this->n; i++) {
			A22.addRow(this->table[i].getSlice(this->n / 2, this->n ));
		}
		std::vector<Matrix> vec;
		vec.push_back(A11);
		vec.push_back(A12);
		vec.push_back(A21);
		vec.push_back(A22);
		return vec;
	}
	void addRow(Row row) {
	
		this->table.push_back(row);
	
		n++;
	}
	void toBlockMatrix() {
		int a = log(this->n) / log(2);
		a += 1;
		
		Row row(n);
		a = pow(2, a);
		int x = a - this->n;
		
		std::vector<double> vec;
		for (std::size_t i = 0; i < a; i++) {
			vec.push_back(0);
		}
		for (std::size_t i = 0; i < x; i++) {
			//std::cout << i<<std::endl;
			this->addRow(row);
		}
		std::cout << this->n << std::endl;
		
		for (std::size_t i = 0; i < x; i++) {
			std::cout << i << std::endl;
			this->addColumn(vec);
		}
		this->n = a;
	}
};
Matrix Strassen(Matrix& A,Matrix& B) {
	int size = A.getSize();
	int size1 = B.getSize();
	
	if (size > 8) {
		std::vector<Matrix> vA = A.decompose();
		std::vector<Matrix> vB = B.decompose();
		Matrix S1;
		Matrix S2;
		Matrix S3;
		Matrix S4;
		Matrix S5;
		Matrix S6;
		Matrix S7;
		Matrix S8;
		Matrix S9;
		Matrix S10;
		S1 = vB[1] - vB[3];
		S2 = vA[0] + vA[1];
		S3 = vA[2] + vA[3];
		S4 = vB[2] - vB[0];
		S5 = vA[0] + vA[3];
		S6 = vB[0] + vB[3];
		S7 = vA[1] - vA[3];
		S8 = vB[2] + vB[3];
		S9 = vA[0] - vA[2];
		S10 = vB[0] + vB[1];
		Matrix P1;
		Matrix P2;
		Matrix P3;
		Matrix P4;
		Matrix P5;
		Matrix P6;
		Matrix P7;
		P1 = Strassen(vA[0], S1);
		P2 = Strassen(S2,vB[3]);
		P3 = Strassen(S3, vB[0]);
		P4 = Strassen(vA[3], S4);
		P5 = Strassen(S5, S6);
		P6 = Strassen(S7, S8);
		P7 = Strassen(S9, S10);
		Matrix С11;
		Matrix С12;
		Matrix С21;
		Matrix С22;
		С11 = P5 + P4 - P2 + P6;
		С12 = P1 + P2;
		С21 = P3 + P4;
		С22 = P5 + P1 - P3 - P7;
		Matrix res(size);
		std::vector<double> v;
		/*for (std::size_t i = 0; i < size / 2; i++) {
			for (std::size_t j = 0; j < size / 2; j++) {
				v.push_back(С11[j][i]);
			}
			for (std::size_t j = 0; j < size/2; j++) {
				v.push_back(С21[j][i]);
			}
			res.addColumn(v);
			v.clear();
		}
		for (std::size_t i = 0; i < size/2; i++) {
			for (std::size_t j = 0; j < size / 2; j++) {
				v.push_back(С12[j][i]);
			}
			for (std::size_t j = 0; j < size/2; j++) {
				v.push_back(С22[j][i]);
			}
			res.addColumn(v);
			v.clear();
		}*/
		Row row;
		for (int i = 0; i < size / 2; i++) {
			for (int j = 0; j < size / 2; j++) {
				res[i][j] = С11[i][j];
				res[i][j + size / 2] = С12[i][j];
				res[i + size / 2][j] = С21[i][j];
				res[i + size / 2][j + size / 2] = С22[i][j];
			}
		}
		return res;
	}
	else {
		
		return A * B;
	}
	
}
Matrix fastMultiplication(Matrix A, Matrix B) {
	int size = A.getSize();
	if (!isPowerOfTwo(A.getSize())) {
		A.toBlockMatrix();
	}
	if (!isPowerOfTwo(B.getSize())) {
		B.toBlockMatrix();
	}

	Matrix C = Strassen(A, B);
	Matrix D(size);
	if (C.getSize() != size) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				D[i][j] = C[i][j];
			}
		}
		return D;
	}
	return C;
}
Row createRandomRow(int size) {
	Row row;
	for (int i = 0; i < size; i++) {
		row.add(rand() % size);
	}
	return row;
}
Matrix createRandomMatrix(int size) {
	Matrix res;
	
	for (int i = 0; i < size; i++) {
		
		res.addRow(createRandomRow(size));
	}
	
	return res;
}

int main() {
	srand(time(NULL));
	Matrix A;
	Matrix B;
	Matrix C;
	Matrix D;
	int n = 60;
	A = createRandomMatrix(n);
	B = createRandomMatrix(n);
	//std::vector<Matrix> v;
	

	C=fastMultiplication(A, B);
	//C.print();
	D = A * B;
	if (C == D) {
		std::cout << "Peremoga" << std::endl;
	}
	//matrix.inverseLU();
	//matrix.print();
}