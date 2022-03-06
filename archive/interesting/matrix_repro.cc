//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Interesting benchmark, showing true cost of abstraction
//  and inline capabilities of compiler
//  Based on homework of Oleg Suhodolov.
//
//  g++ matrix_repro.cc -O2
//  ./a.out < test.dat
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <vector>

template <typename T> class Matrix {
  int lines_, columns_;
  std::vector<T> matrix_;

public:
  Matrix(int lines = 0, int columns = 0)
      : lines_(lines), columns_(columns), matrix_(lines * columns) {}
  T &at(int x, int y) { return matrix_[x * columns_ + y]; }
  const T &at(int x, int y) const { return matrix_[x * columns_ + y]; }
  T *data() { return matrix_.data(); }
  const T *cdata() const { return matrix_.data(); }
  int x() const { return lines_; }
  int y() const { return columns_; }

  void readMatrix() {
    int res, i, j;
    std::cin.exceptions(std::istream::failbit);
    std::cin >> lines_ >> columns_;
    matrix_.resize(lines_ * columns_);
    for (i = 0; i < lines_; ++i)
      for (j = 0; j < columns_; ++j)
        std::cin >> matrix_[i * columns_ + j];
  }
  void resizeto(int lines, int columns) {
    lines_ = lines;
    columns_ = columns;
    matrix_.resize(lines_ * columns_);
  }

  auto begin() { return matrix_.begin(); }
  auto cbegin() { return matrix_.begin(); }
  auto end() { return matrix_.end(); }
  auto cend() { return matrix_.end(); }
};

template <typename T>
void c_transpose_mult(const T *A, const T *B, T *C, int AX, int AY, int BY) {
  assert(A != NULL && B != NULL && C != NULL);
  assert(AX > 0 && AY > 0 && BY > 0);
  std::vector<T> tmp(BY * AY);

  for (int i = 0; i < AY; i++)
    for (int j = 0; j < BY; j++)
      tmp[j * AY + i] = B[i * BY + j];

  for (int i = 0; i < AX; i++)
    for (int j = 0; j < BY; j++) {
      C[i * BY + j] = 0;
      for (int k = 0; k < AY; k++)
        C[i * BY + j] += A[i * AY + k] * tmp[j * AY + k];
    }
}

template <typename T>
void cpp_transpose_mult(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  int AX = a.x(), AY = a.y(), BY = b.y();
  assert(AX == b.x());
  Matrix<T> tmp(BY, AY);
  for (int i = 0; i < AY; ++i)
    for (int j = 0; j < BY; ++j)
      tmp.at(j, i) = b.at(i, j);

  for (int i = 0; i < AX; ++i)
    for (int j = 0; j < BY; ++j) {
      c.at(i, j) = 0;
      for (int k = 0; k < AY; ++k)
        c.at(i, j) += a.at(i, k) * tmp.at(j, k);
    }
}

int main() {
  time_t start, fin;
  long elapsed;
  Matrix<int> A, B, C, D;
  A.readMatrix();
  B.readMatrix();
  C.resizeto(A.x(), A.y());
  D.resizeto(A.x(), A.y());

  start = clock();
  c_transpose_mult(A.cdata(), B.cdata(), C.data(), A.x(), A.y(), B.y());
  fin = clock();
  elapsed = fin - start;
  std::cout << "C with transpose: " << elapsed << "\n";

  start = clock();
  cpp_transpose_mult(A, B, D);
  fin = clock();
  elapsed = fin - start;
  std::cout << "C++ with transpose: " << elapsed << "\n";

  for (int i = 0; i < A.x(); ++i)
    for (int j = 0; j < A.y(); ++j)
      if (C.at(i, j) != D.at(i, j))
        std::cerr << "Divergence at: " << i << ", " << j << std::endl;
}
