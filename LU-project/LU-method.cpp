#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

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
		numbers.push_back(value);
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

public:
	Row operator [](std::size_t i) const {
		return table[i];
	}
	Row& operator [](std::size_t i) {
		return table[i];
	}
	Matrix operator *(Matrix other) {
			if (this->n != other.n) {
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
		return (x < n && y < n) ? this->table[x][y] : 0;
	}

	void clear() {
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
			for (int i = 0; i < size; i++) {
				this->table.push_back(Row(size));
			}
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

};



int main() {
	Matrix matrix;
	matrix.loadFromFile("Sample.txt");
	matrix.inverseLU();
	matrix.print();
}