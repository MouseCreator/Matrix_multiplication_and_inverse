#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
class Matrix {

private:
	std::vector<std::vector<double>> table;
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
		std::vector<double> newRow;
		while (iss >> toAdd) {
			newRow.push_back(toAdd);
		}
		this->table.push_back(newRow);
		n++;
	}

	std::size_t maxRowLength() {
		if (n == 0) {
			return 0;
		}
		std::size_t result = this->table[0].size();
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
				this->table[i].push_back(0);
			}
		}
		if (dimention > n) {
			std::vector<double> emptyRow;
			for (int i = 0; i < dimention; i++) {
				emptyRow.push_back(0);
			}
			for (; n < dimention; n++) {
				this->table.push_back(emptyRow);
			}
		}
	}
	std::vector<double> createRow(int length) {
		std::vector<double> row;
		for (int j = 0; j < length; j++) {
			row.push_back(0);
		}
		return row;
	}

public:
	Matrix(int size) {
		this->n = size;
		for (int i = 0; i < size; i++) {
			table.push_back(createRow(size));
		}
	}
	Matrix() {
		n = 0;
	}

	

	void setSize(int size) {
		if (n < size) {
			for (int i = 0; i < n; i++) {
				for (int j = n; j < size; j++) {
					table[i].push_back(0);
				}
			}
			for (int i = n; i < size; i++) {
				table.push_back(createRow(size));
			}
		}
		else {
			for (int i = 0; i < size; i++) {
				for (int j = size; j < n; j++) {
					table[i].pop_back();
				}
			}
			for (int i = size; i < n; i++) {
				table.pop_back();
			}
		}
		n = size;
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
	Matrix multiplyBy(Matrix &other) {
		if (this->n != other.n) {
			std::cout << "Matrixes has different dimentions!" << std::endl;
			return Matrix(n);
		}
		Matrix result;
		for (int i = 0; i < n; i++) {
			std::vector<double> newRow;
			result.n++;
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					newRow.push_back(this->table[i][k] + other.table[k][j]);
				}
			}
			result.table.push_back(newRow);
		}
		return result;
	}

};



int main() {
	Matrix matrix;
	Matrix matrix2;
	matrix.loadFromFile("Sample.txt");
	matrix2.loadFromFile("Sample2.txt");
	Matrix result = matrix.multiplyBy(matrix2);
	result.print();
}