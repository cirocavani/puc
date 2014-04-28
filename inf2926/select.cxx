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

void insertion_sort(std::vector<int>& v, int start, int end) {
  for (int j = start + 1; j <= end; ++j) {
    int k = v[j], i = j - 1;
    for (;i >= start && v[i] > k; --i) {
      v[i + 1] = v[i];
    }
    v[i + 1] = k;
  }
}

int select_median(std::vector<int>& v, const int start, const int end) {
  if (start == end)
    return v[start];

  for (int i = start, j = start + 4; i < end; i += 5, j += 5)
    insertion_sort(v, i, j <= end ? j : end);

  int n = start;
  for (int i = start,
	 j = start + 4,
	 len = end - start + 1,
  	 r = len % 5 > 0 ? len % 5 - 1 : 0,
	 mr = (r - r % 2) / 2;
       i < end; i += 5, j += 5, ++n)
    swap(v, n, i + (j <= end ? 2 : mr));

  return select_median(v, start, n - 1);
}

int select_median(std::vector<int>& v, const int k, const int start, const int end) {
  if (start >= end)
    return v[start];

  int pivot = select_median(v, start, end),
    i = start - 1,
    j = end + 1;
  for (;;) {
    while (v[++i] < pivot);
    while (v[--j] > pivot);
    if (i >= j) break;
    swap(v, i, j);
  }
  if (k == i)
    return pivot;
  else if (k < i)
    return select_median(v, k, start, i - 1);
  else
    return select_median(v, k, j + 1, end);
}
    
int select_median(std::vector<int>& v, const int k) {
  return select_median(v, k, 0, v.size() - 1);
}

int main() {
  std::vector<int> v { 6, 8, 7, 10, 9, 16, 18, 17, 20, 19, 1, 3, 2, 5, 4, 11, 13, 12, 15, 14, 21, 23, 22, 25, 24 };
  std::cout << v << std::endl;
  auto v2 = v; // copy
  std::cout << "18th: " << select(v2, 17) << std::endl;
  auto v3 = v; // copy
  std::cout << "18th: " << select_median(v3, 17) << std::endl;
}
