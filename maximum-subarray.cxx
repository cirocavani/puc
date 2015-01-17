#include <iostream>
#include <vector>

// Sigma
struct S {
  const int i, j, sum;
  S(int _i = 0, int _j = 0, int _sum = 0)
    : i(_i), j(_j), sum(_sum) { }
};

bool operator>=(const S& x, const S& y) {
  return x.sum >= y.sum;
}

std::ostream& operator<<(std::ostream& out, const S& m) {
  return out << "S(" << m.i << ", " << m.j << ") = " << m.sum << std::endl;
}

// Brute-force Big-Theta(n3)
S max_n3(const std::vector<int>& v) {
  int mi = 0, mj = 0, msum = 0;
  for (int i = 0; i < v.size(); ++i) {
    for (int j = i; j < v.size(); ++j) {
      int sum = 0;
      for (int k = i; k <= j; ++k)
	sum += v[k];
      if (sum > msum) {
	mi = i;
	mj = j;
	msum = sum;
      }
    }
  }
  return S(mi, mj, msum);
}

// Brute-force Big-Theta(n2)
S max_n2(const std::vector<int>& v) {
  int mi = 0, mj = 0, msum = 0;
  for (int i = 0; i < v.size(); ++i) {
    for (int j = i, sum = 0; j < v.size(); ++j) {
      sum += v[j];
      if (sum > msum) {
	mi = i;
	mj = j;
	msum = sum;
      }
    }
  }
  return S(mi, mj, msum);
}

// Divide-and-Conquer Big-Theta(n lg n)
S max_dac_middle(const std::vector<int>& v, int start, int end, int middle) {
  int start_i = 0, start_sum = 0;
  for (int i = middle, sum = 0; i >= start; --i) {
    sum += v[i];
    if (sum > start_sum) {
      start_sum = sum;
      start_i = i;
    }
  }
  int end_i = 0, end_sum = 0;
  for (int i = middle + 1, sum = 0; i <= end; ++i) {
    sum += v[i];
    if (sum > end_sum) {
      end_sum = sum;
      end_i = i;
    }
  }
  
  return S(start_i, end_i, start_sum + end_sum);
}

S max_dac_rec(const std::vector<int>& v, int start, int end) {
  if (start == end)
    return S(start, end, v[start] > 0 ? v[start] : 0);

  int len = end - start + 1;
  int middle = start + (len - len % 2) / 2 - 1;
  S head_max = max_dac_rec(v, start, middle);
  S tail_max = max_dac_rec(v, middle + 1, end);
  S cross_max = max_dac_middle(v, start, end, middle);

  if (head_max >= tail_max && head_max >= cross_max)
    return head_max;
  else if (tail_max >= head_max && tail_max >= cross_max)
    return tail_max;
  else
    return cross_max;
}

S max_dac(const std::vector<int>& v) {
  return max_dac_rec(v, 0, v.size() - 1);
}

// Dynamic Programming Big-Theta(n)
S max_dp(const std::vector<int>& v) {
  int i = 0, j = 0, max = 0;
  for (int first = 0, last = 0, sum = 0; last < v.size(); ++last) {
    sum += v[last];
    if (sum > max) {
      max = sum;
      i = first;
      j = last;
    }
    else if (sum < 0) {
      first = last + 1;
      sum = 0;
    }
  }

  return S(i, j, max);
}


int main() {
  std::cout << "Maximum Subarray" << std::endl;

  const std::vector<int> v1 { 13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7 };
  
  const std::vector<int> v2 { -13, -3, -25, -20 };

  const std::vector<int> v3 { 13, 3, 25, 20 };

  auto test = [&](std::string label, S (*max)(const std::vector<int>&)) {
    std::cout << label << std::endl;
    std::cout << max(v1);
    std::cout << max(v2);
    std::cout << max(v3);
  };

  test("Big-Theta(n3)", max_n3);
  test("Big-Theta(n2)", max_n2);
  test("Big-Theta(n lg n)", max_dac);
  test("Big-Theta(n)", max_dp);
}
