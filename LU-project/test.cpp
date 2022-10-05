#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "MatrixFactory.h"
TEST_CASE("testing LU extension") {
	int n = 5;
	Matrix A;
	Matrix L;
	Matrix U;
	srand(time(NULL));
	int testCases = 100;
	for (int i = 0; i < testCases; i++) {
		A = MatrixFactory::createRandomMatrix(n);
		bool isValid = true;
		if (!A.LUextension(L, U)) {
			isValid = L * U == A;
		}
		CHECK(isValid);
	}
	for (int i = 0; i < testCases; i++) {
		A = MatrixFactory::createRandomSparsedMatrix(n);
		bool isValid = true;
		if (!A.LUextension(L, U)) {
			isValid = L * U == A;
		}
		CHECK(isValid);
	}
}
TEST_CASE("Test fast multiplication") {
	Matrix A;
	Matrix B;
	Matrix C;
	Matrix D;
	int n = 60;
	A = MatrixFactory::createRandomMatrix(n);
	B = MatrixFactory::createRandomMatrix(n);


	C = MatrixFactory::fastMultiplication(A, B);
	D = A * B;

	bool isPassed = C == D;
	CHECK(isPassed);
	if (isPassed) {
		std::cout << "Peremoga" << std::endl;
	}
}

TEST_CASE("Test inverse matrix using LU extension") {
	int n = 3;
	Matrix A;
	srand(time(NULL));
	int testCases = 100;
	Matrix E = MatrixFactory::identityMatrix(n);
	for (int i = 0; i < testCases; i++) {
		A = MatrixFactory::createRandomMatrix(n);
		Matrix B = A.inverseLU();
		bool isValid = true;
		if (B.getSize() > 0) {
			isValid = B * A == E;
		}
		CHECK(isValid);
	}
	for (int i = 0; i < testCases; i++) {
		A = MatrixFactory::createRandomSparsedMatrix(n);
		Matrix B = A.inverseLU();
		bool isValid = true;
		if (B.getSize() > 0) {
			isValid = B * A == E;
		}
		CHECK(isValid);
	}
}

TEST_CASE("Speedtest") {
	std::clock_t  begin;
	std::clock_t  end;
	bool isCorrect = true;
	std::cout << "Speedtest:" << std::endl;
	for (int n = 32; n <= 512; n *= 2) {
		Matrix A = MatrixFactory::createRandomMatrix(n);
		Matrix B = MatrixFactory::createRandomMatrix(n);

		//DETERMINENT == 0 ???

		Matrix E = MatrixFactory::identityMatrix(n);
		std::cout << "N = " << n << std::endl;
		
		begin = clock();

		Matrix multiplied = MatrixFactory::fastMultiplication(A, B);

		end = clock();
		std::cout << "\tFast Matrix Multiplication: " << end - begin  << "ms" << std::endl;
		isCorrect = multiplied == A * B;
		CHECK(isCorrect);

		begin = clock();

		A.inverseLU();

		end = clock();
		std::cout << "\tInverse matrix (LU extension): " << end - begin << "ms" << std::endl;
	}

}