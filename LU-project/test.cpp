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
		A = MatrixFactory::createRandomRealMatrix(n);
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
		A = MatrixFactory::createRandomRealMatrix(n);
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

TEST_CASE("Multiplication Speedtest") {
	std::clock_t  begin;
	std::clock_t  end;
	bool isCorrect = true;
	std::cout << "Multiplication Speedtest:" << std::endl;
	for (int n = 32; n <= 512; n *= 2) {
		Matrix A = MatrixFactory::createRandomRealMatrix(n);
		Matrix B = MatrixFactory::createRandomRealMatrix(n);

		

		std::cout << "\tN = " << n << std::endl;
		
		begin = clock();

		Matrix multiplied = MatrixFactory::fastMultiplication(A, B);

		end = clock();
		std::cout << "\tTime: " << end - begin << "ms" << std::endl << std::endl;
		isCorrect = multiplied == A * B;
		CHECK(isCorrect);

	}

}

TEST_CASE("LU Speedtest") {
	std::clock_t  begin;
	std::clock_t  end;
	bool isCorrect = true;
	std::cout << "LU Speedtest:" << std::endl;
	for (int n = 32; n <= 1024; n *= 2) {
		Matrix A = MatrixFactory::createRandomRealMatrix(n);
		//DETERMINENT == 0 ???
		Matrix E = MatrixFactory::identityMatrix(n);
		std::cout << "\tN = " << n << std::endl;
		begin = clock();

		Matrix Inverse = A.inverseLU();

		end = clock();

		isCorrect = Inverse * A == E;

		CHECK(isCorrect);
		std::cout << "\tTime: " << end - begin << "ms" << std::endl << std::endl;
	}

}