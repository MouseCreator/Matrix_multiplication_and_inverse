#include "MatrixFactory.h"

bool Matrix::LUextensionUsingSum(Matrix& L, Matrix& U) {

	L = MatrixFactory::identityMatrix(n);
	U = Matrix(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double sum = 0;
			for (int k = 0; k < n - 1; k++) {
				sum += L[i][k] * U[k][j];
			}
			if (i <= j) {
				U[i][j] = table[i][j] - sum;
			}
			else {
				if (abs(U[j][j]) < 0.0001)
					return true;
				L[i][j] = (table[i][j] - sum) / U[j][j];
			}
		}
	}
	return false;
}

bool Matrix::LUextension(Matrix& L, Matrix& U) {
	L = (this->n);
	U = (*this);
	for (std::size_t i = 0; i < n; i++) {
		double divideBy = U[i][i];
		if (abs(divideBy) < 0.00001) {
			return true;
		}
		L[i][i] = 1;
		for (std::size_t j = i + 1; j < n; j++) {
			double coefficient = U[j][i] / divideBy;
			U[j] -= U[i] * coefficient;
			L[j][i] = coefficient;
		}
	}
	return false;
}


Matrix Matrix::inverseLU() {
	Matrix L;
	Matrix U;

	if (LUextension(L, U)) {
		return Matrix(0);
	}

	Matrix result;
	result.n = n;
	result.table = std::vector<Row>(n);
	for (int i = 0; i < n; i++) {
		std::vector<double> identityRow(n);
		identityRow[i] = 1;
		std::vector<double> y = L.solveForL(identityRow);
		std::vector<double> x = U.solveForU(y);
		result.appendColumn(x);
	}
	return result;
}