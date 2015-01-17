#include <iostream>
#include <vector>
#include <random>

using uint = unsigned int;

template<uint N>
struct matrix {
  std::vector<int> data;
  matrix(int v = 0) : data(std::vector<int>(N * N, v)) { }
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
  friend bool operator==(const matrix<N>& m1, const matrix<N>& m2) {
    for (uint i = 0; i < N; ++i)
      for (uint j = 0; j < N; ++j)
	if (m1(i, j) != m2(i, j))
	  return false;
    return true;
  }
};


// Matrix Multiplication Big-Theta (n3)
template<uint N>
matrix<N> mult_n3(const matrix<N>& m1, const matrix<N>& m2) {
  matrix<N> m;
  for (uint i = 0; i < N; ++i)
    for (uint j = 0; j < N; ++j)
      for (uint k = 0; k < N; ++k)
	m(i, j) += m1(i, k) * m2(k, j);
  return m;
}


// Divide-and-conquer, Big-Theta (n3)
template<uint N>
struct matrix_view {
  matrix<N>& m;
  uint i_offset, j_offset, n;
  matrix_view(matrix<N>& _m,
	      uint _i = 0,
	      uint _j = 0,
	      uint _n = N)
    : m(_m), i_offset(_i), j_offset(_j), n(_n) { }
  int& operator()(uint i, uint j) {
    return m(i + i_offset, j + j_offset);
  } 
  int operator()(uint i, uint j) const {
    return m(i + i_offset, j + j_offset);
  } 
  int size() const {
    return n;
  }
  matrix_view<N> view_1_1() const {
    return matrix_view<N>(m, i_offset, j_offset, n / 2);
  }
  matrix_view<N> view_1_2() const {
    return matrix_view<N>(m, i_offset, n / 2 + j_offset, n / 2);
  }
  matrix_view<N> view_2_1() const {
    return matrix_view<N>(m, n / 2 + i_offset, j_offset, n / 2);
  }
  matrix_view<N> view_2_2() const {
    return matrix_view<N>(m, n / 2 + i_offset, n / 2 + j_offset, n / 2);
  }
};

template<uint N>
void mult_dc(matrix_view<N> m, const matrix_view<N> m1, const matrix_view<N> m2) {
  if (m.size() == 1) {
    m(0, 0) += m1(0, 0) * m2(0, 0);
    return;
  }

  matrix_view<N> m_11 = m.view_1_1(),
    m_12 = m.view_1_2(),
    m_21 = m.view_2_1(),
    m_22 = m.view_2_2();
  matrix_view<N> m1_11 = m1.view_1_1(),
    m1_12 = m1.view_1_2(),
    m1_21 = m1.view_2_1(),
    m1_22 = m1.view_2_2();
  matrix_view<N> m2_11 = m2.view_1_1(),
    m2_12 = m2.view_1_2(),
    m2_21 = m2.view_2_1(),
    m2_22 = m2.view_2_2();
  
  mult_dc(m_11, m1_11, m2_11); 
  mult_dc(m_11, m1_12, m2_21); 
  mult_dc(m_12, m1_11, m2_12); 
  mult_dc(m_12, m1_12, m2_22); 
  mult_dc(m_21, m1_21, m2_11); 
  mult_dc(m_21, m1_22, m2_21); 
  mult_dc(m_22, m1_21, m2_12); 
  mult_dc(m_22, m1_22, m2_22); 
}

template<uint N>
matrix<N> mult_dc(matrix<N>& m1, matrix<N>& m2) {
  matrix<N> m;
  matrix_view<N> mv(m);
  const matrix_view<N> m1v(m1);
  const matrix_view<N> m2v(m2);
  mult_dc(mv, m1v, m2v);
  return m;
}

template<uint N>
matrix<N> eye() {
  matrix<N> m;
  for (uint i = 0; i < N; ++i)
    m(i, i) = 1;
  return m;
}

template<uint N>
matrix<N> rnd() {
  std::random_device rnd;
  std::uniform_int_distribution<int> rndn(-10,10);
  matrix<N> m;
  for (uint i = 0; i < N * N; ++i)
    m(i / N, i % N) = rndn(rnd);
  return m;
}

int main() {
  std::cout << "Matrix" << std::endl;
  matrix<4> a(3);
  matrix<4> b = eye<4>();
  matrix<4> c = mult_n3(a, b);
  std::cout << "A\n" << a << std::endl;
  std::cout << "B\n" << b << std::endl;
  std::cout << "C = A x B\n" << c << std::endl;

  matrix<4> d(1);
  matrix<4> e = mult_n3(c, d);
  std::cout << "D\n" << d << std::endl;
  std::cout << "E = C x D\n" << e << std::endl;

  matrix<4> f = mult_dc(c, d);
  std::cout << "F = C x D (Divide-and-Conquer)\n" << f << std::endl;

  std::cout << "E == F? " << (e == f ? "true" : "false") << std::endl;

  auto g = rnd<256>();
  auto h = rnd<256>();
  auto i = mult_n3(g, h);
  auto j = mult_dc(g, h);
  std::cout << "I == J? " << (i == j ? "true" : "false") << std::endl;
}
