#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& out, const std::vector<int>& v) {
  if (v.empty())
    return out << "{}";

  out << "{ ";
  bool first = true;
  for (int i : v) {
    if (first)
      first = false;
    else
      out << ", ";
    out << i;
  }
  out << " }";
  return out;
}

void swap(std::vector<int>& v, const int i, const int j) {
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}

int select(std::vector<int>& v, const int k, const int start, const int end) {
  std::cout << "k,s,e=" << k << "," << start << "," << end << std::endl;
  if (start >= end)
    return v[start];
 
  int pivot = v[end],
    i = start - 1,
    j = end;
  std::cout << "pivot=" << pivot << std::endl;
  do {
    do { i++; } while (v[i] < pivot);
    do { j--; } while (v[j] > pivot);
    std::cout << "i,j=" << i << "," << j << std::endl;
    swap(v, i, j);
    std::cout << v << std::endl;
  } while (j >= i);
  int t = v[j];
  v[j] = v[i];
  v[i] = pivot;
  v[end] = t;
  std::cout << v << std::endl;
  if (k == i) return pivot;
  if (k < i) return select(v, k, start, i - 1);
  else return select(v, k, j + 1, end);
}

int select(std::vector<int>& v, const int k) {
  return select(v, k, 0, v.size() - 1);
}
    
int main() {
  std::vector<int> v { 6, 8, 7, 10, 9, 16, 18, 17, 20, 19, 1, 3, 2, 5, 4, 11, 13, 12, 15, 14, 21, 23, 22, 25, 24 };
  std::cout << v << std::endl;
  std::cout << "18th: " << select(v, 17) << std::endl;
}
