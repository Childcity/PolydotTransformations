#include "GaussJordanElimination.h"

#include <QLoggingCategory>

#include <sstream>
#include <string>

namespace gauss_jordan_elimination {

namespace {

Q_LOGGING_CATEGORY(gauss_jordan_elimination, "utils.gauss_jordan_elimination", QtInfoMsg);

using Array = std::vector<float>;

void writeLine(std::string_view line)
{
	qCDebug(gauss_jordan_elimination) << line.data();
}

// Reduce matrix to reduced row echelon form.
int performOperation(Matrix &a)
{
	int i{}, flag{};
	const auto n = a.size();

	// Performing elementary operations
	for (i = 0; i < n; i++) {
		int k{};
		if (qFuzzyIsNull(a[i][i])) {
			int c = 1;
			while ((i + c) < n && qFuzzyIsNull(a[i + c][i])) {
				c++;
				writeLine(std::format("i: {} c: {} i+c: {}", i, c, i + c));
			}
			if ((i + c) == n) {
				flag = 1;
				break;
			}
			for (int j = i, k = 0; k <= n; k++) {
				const float temp = a[j][k];
				a[j][k] = a[j + c][k];
				a[j + c][k] = temp;
			}
		}

		for (int j = 0; j < n; j++) {
			// Excluding all i == j
			if (i != j) {
				// Converting Matrix to reduced row
				// echelon form(diagonal matrix)
				const float p = a[j][i] / a[i][i];
				for (k = 0; k <= n; k++) {
					a[j][k] = a[j][k] - (a[i][k]) * p;
				}
			}
		}
	}
	return flag;
}

Array getSolution(const Matrix &a, int flag)
{
	Array solution(a.size());
	std::string line;

	// Getting the solution by dividing constants by
	// their respective diagonal elements
	for (int i = 0; i < a.size(); i++) {
		solution[i] = a[i][a.size()] / a[i][i];
		line += std::format("{:>.5} ", solution[i]);
	}

	writeLine("Solution is:");
	writeLine(line);
	return solution;
}

// To check whether infinite solutions
// exists or no solution exists
int checkConsistency(Matrix a, int flag)
{
	float sum{};

	// flag == 2 for infinite solution
	// flag == 3 for No solution
	flag = 3;
	for (int i = 0; i < a.size(); i++) {
		sum = 0;

		int j = 0;
		for (; j < a.size(); j++) {
			sum += a[i][j];
		}
		if (qFuzzyCompare(sum, a[i][j])) {
			flag = 2;
		}
	}
	return flag;
}

void printMatrix(const Matrix &a)
{
	for (int i = 0; i < a.size(); i++) {
		std::string line;
		for (int j = 0; j <= a.size(); j++) {
			line += std::format("{:>5.2} ", a[i][j]);
		}
		writeLine(line);
	}
}

} // namespace

Array SolveSystem(const Matrix &inA)
{
	Matrix a = inA;

	writeLine("Solving system: ");
	printMatrix(a);

	// Performing Matrix transformation
	int flag = performOperation(a);

	if (flag == 1) {
		flag = checkConsistency(a, flag);
	}

	if (flag == 2) {
		throw InfiniteSolutionError("InfiniteSolution");
	} else if (flag == 3) {
		throw NoSolutionError("NoSolutionError");
	}

	writeLine("Final Augmented Matrix is: ");
	printMatrix(a);

	// Return Solutions(if exist)
	return getSolution(a, flag);
}

} // namespace gauss_jordan_elimination
