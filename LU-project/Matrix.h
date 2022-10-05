#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
class Row {
private:
	std::vector<double> numbers;

	std::vector<double> slice(std::vector<double> const& v, int m, int n)
	{
		auto first = v.begin() + m;
		auto last = v.begin() + n;

		std::vector<double> vec(first, last);
		return vec;
	}

public:
	Row() {}
	Row(std::size_t size) {
		numbers = std::vector<double>(size, 0);
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
	Row getSlice(int beg, int end) {
		Row row(end - beg);
		if (end - beg > this->numbers.size()) {
			std::cout << "Incorrect data\n";
		}
		else {
			row.numbers = slice(this->numbers, beg, end);
		}
		return row;
	}
	void print() const {
		for (int i = 0; i < numbers.size(); i++) {
			std::cout << numbers[i] << std::endl;
		}
	}
	bool operator ==(Row other) const{
		double delta = 0.00001;
		if (this->numbers.size() != other.numbers.size()) {
			return false;
		}
		else {
			for (int i = 0; i < this->numbers.size(); i++) {
				if (abs(this->numbers[i] - other.numbers[i]) > delta) {
					return false;
				}
			}
		}
		return true;
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


	std::vector<double> solveForL(std::vector<double> b) {
		std::vector<double> result;
		for (int i = 0; i < n; i++) {
			double sumOfOthers = 0;
			for (int j = 0; j < i; j++) {
				sumOfOthers += result[j] * this->table[i][j];
			}
			result.push_back(b[i] - sumOfOthers); 
		}
		return result;
	}
	std::vector<double> solveForU(std::vector<double> b) {
		std::vector<double> result(n, 0);
		for (int i = n - 1; i >= 0; i--) {
			double sumOfOthers = 0;
			for (int j = i + 1; j < n; j++) {
				sumOfOthers += result[j] * this->table[i][j];
			}
			result[i] = (b[i] - sumOfOthers)/ this->table[i][i];
		}
		return result;
	}

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
	void appendColumn(std::vector<double> column) {
		for (int i = 0; i < n; i++) {
			this->table[i].add(column[i]);
		}
	}

	void setZero() {
		for (int i = 0; i < n; i++) {
			this->table[i] -= this->table[i];
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
			result.table.push_back(this->table[i] - other.table[i]);
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
		for (int i = 0; i < size; i++) {
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
		return (x < n&& y < n) ? this->table[x][y] : 0;
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
	void appendRow(Row row) {
		this->table.push_back(row);
		n++;
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


	bool LUextensionUsingSum(Matrix& L, Matrix& U);
	bool LUextension(Matrix& L, Matrix& U);
	Matrix inverseLU();
	std::vector<Matrix> decompose() {
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
			A21.addRow(this->table[i].getSlice(0, this->n / 2));
		}
		for (int i = this->n / 2; i < this->n; i++) {
			A22.addRow(this->table[i].getSlice(this->n / 2, this->n));
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
			this->addRow(row);
		}
		//std::cout << this->n << std::endl;

		for (std::size_t i = 0; i < x; i++) {
			//std::cout << i << std::endl;
			this->addColumn(vec);
		}
		this->n = a;
	}


	
};