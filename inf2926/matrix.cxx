#include <iostream>
#include <array>
#include <random>
#include <limits>

using uint = unsigned int;

template<uint N>
struct matrix {
  std::array<int, N * N> data;
  matrix(int v = 0) { data.fill(v); }
  int& operator()(uint i, uint j) {
    return data[i * N + j];
  } 
  int operator()(uint i, uint j) const {
    return data[i * N + j];
  }
  friend std::ostream& operator<<(std::ostream& out, const matrix<N>& m) {
    for (uint i = 0; i < N; ++i) {
      for (uint j = 0; j < N; ++j) {
	out << m(i, j);
	if (j + 1 != N)
	  out << "\t";
      }
      if (i + 1 != N)
	out << "\n";
    }
    return out;
  }
};

template<uint N>
matrix<N> mult(const matrix<N>& m1, const matrix<N>& m2) {
  matrix<N> m;
  for (uint i = 0; i < N; ++i)
    for (uint j = 0; j < N; ++j)
      for (uint k = 0; k < N; ++k)
	m(i, j) += m1(i, k) * m2(k, j);
  return m;
}

template<uint N>
matrix<N> eye() {
  matrix<N> m;
  for (uint i = 0; i < N; ++i)
    m(i, i) = 1;
  return m;
}
 
int main() {
  std::cout << "Matrix" << std::endl;
  matrix<4> a(3);
  matrix<4> b = eye<4>();
  matrix<4> c = mult(a, b);
  std::cout << "A\n" << a << std::endl;
  std::cout << "B\n" << b << std::endl;
  std::cout << "C = A x B\n" << c << std::endl;

  matrix<4> d(1);
  matrix<4> e = mult(c, d);
  std::cout << "D\n" << d << std::endl;
  std::cout << "E = C x D\n" << e << std::endl;
}
