#pragma once
#include "Matrix.h"
class MatrixFactory {
private:
	static bool isPowerOfTwo(int n)
	{
		return (ceil(log2(n)) == floor(log2(n)));
	}
public:
	static Matrix identityMatrix(int n) {
		Matrix identity;
		for (int i = 0; i < n; i++) {
			Row rowToAdd(n);
			rowToAdd[i] = 1;
			identity.appendRow(rowToAdd);
		}
		return identity;
	}
	static Matrix Strassen(Matrix& A, Matrix& B) {
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
			P2 = Strassen(S2, vB[3]);
			P3 = Strassen(S3, vB[0]);
			P4 = Strassen(vA[3], S4);
			P5 = Strassen(S5, S6);
			P6 = Strassen(S7, S8);
			P7 = Strassen(S9, S10);
			Matrix Ñ11;
			Matrix Ñ12;
			Matrix Ñ21;
			Matrix Ñ22;
			Ñ11 = P5 + P4 - P2 + P6;
			Ñ12 = P1 + P2;
			Ñ21 = P3 + P4;
			Ñ22 = P5 + P1 - P3 - P7;
			Matrix res(size);
			std::vector<double> v;
			Row row;
			for (int i = 0; i < size / 2; i++) {
				for (int j = 0; j < size / 2; j++) {
					res[i][j] = Ñ11[i][j];
					res[i][j + size / 2] = Ñ12[i][j];
					res[i + size / 2][j] = Ñ21[i][j];
					res[i + size / 2][j + size / 2] = Ñ22[i][j];
				}
			}
			return res;
		}
		else {

			return A * B;
		}

	}
	static Matrix fastMultiplication(Matrix A, Matrix B) {
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
	static Row createRandomRow(int size) {
		Row row;
		for (int i = 0; i < size; i++) {
			row.add(rand() % size);
		}
		return row;
	}
	static Matrix createRandomMatrix(int size) {
		Matrix res;

		for (int i = 0; i < size; i++) {

			res.addRow(createRandomRow(size));
		}

		return res;
	}

	static Row createSparsedRow(int size) {
		Row row;
		for (int i = 0; i < size; i++) {
			row.add((rand() % 2) ? rand() % size : 0);
		}
		return row;
	}
	static Matrix createRandomSparsedMatrix(int size) {
		Matrix res;

		for (int i = 0; i < size; i++) {

			res.addRow(createSparsedRow(size));
		}

		return res;
	}

};