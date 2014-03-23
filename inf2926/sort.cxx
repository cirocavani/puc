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


// Insertion-sort Big-Theta(n2)
void insertion_sort(std::vector<int>& v, int start, int end) {
  for (int j = start + 1; j <= end; ++j) {
    int k = v[j], i = j - 1;
    for (;i >= start && v[i] > k; --i) {
      v[i + 1] = v[i];
    }
    v[i + 1] = k;
  }
}

void insertion_sort(std::vector<int>& v) {
  insertion_sort(v, 0, v.size() - 1);
}

// Merge-sort Big-Theta(n lg n)
void merge_sort(std::vector<int>& v, int start, int end) {
  if (start >= end)
    return;

  int len = start + end + 1;
  int middle = (len - len % 2) / 2 - 1;
  merge_sort(v, start, middle);
  merge_sort(v, middle + 1, end);

  std::vector<int> t(len);
  for (int i = 0; i < len; ++i)
    t[i] = v[start + i];

  for (int k = start,
	 i = 0, maxi = middle - start,
	 j = middle - start + 1, maxj = end - start;
       k <= end; ++k)
    if (i > maxi)
      v[k] = t[j++];
    else if (j > maxj)
      v[k] = t[i++];
    else
      v[k] = t[i] < t[j] ? t[i++] : t[j++];
}

void merge_sort(std::vector<int>& v) {
  merge_sort(v, 0, v.size() - 1);
}

// Quick-sort Big-Theta(n lg n)
void quick_sort(std::vector<int>& v, int start, int end) {
  if (start >= end)
    return;

  int pivot = v[end], i = start - 1, j = end + 1;
  for (;;) {
    while (v[++i] < pivot);
    while (v[--j] > pivot);
    if (i >= j) break;

    int t = v[i];
    v[i] = v[j];
    v[j] = t;
  }

  quick_sort(v, start, i - 1);
  quick_sort(v, j + 1, end);
}

void quick_sort(std::vector<int>& v) {
  quick_sort(v, 0, v.size() - 1);
}

// Quick-sort w/ Pivot selection Big-Theta(n lg n)
//void quick_sort2(std::vector<int>& v) {
//}

int main() {
  std::cout << "Sort" << std::endl;

  const std::vector<int> v1 { 4, 3, 2, 1 };
  std::cout << v1 << std::endl;

  const std::vector<int> v2 { 5, 5, 5, 5 };
  std::cout << v2 << std::endl;

  const std::vector<int> v3 { 1, 2, 3, 4 };
  std::cout << v3 << std::endl;

  const std::vector<int> v4 { 10 };
  std::cout << v4 << std::endl;

  const std::vector<int> v5 { 10, 3, 30, 3, 7, 3 };
  std::cout << v5 << std::endl;

  auto test_sort = [](void (*sort)(std::vector<int>&), const std::vector<int>& v) {
    auto mv = v; // copy mutable
    sort(mv);
    std::cout << v << " -> " << mv << std::endl;
  };

  auto test = [&](std::string label, void (*sort)(std::vector<int>&)) {
    std::cout << label << std::endl;
    auto test = [&](const std::vector<int>& v) { test_sort(sort, v); };
    test(v1);
    test(v2);
    test(v3);
    test(v4);
    test(v5);
  };

  test("Insertion-sort, Big-Theta(n2)", insertion_sort);
  test("Merge-sort, Big-Theta(n lg n)", merge_sort);
  test("Quick-sort, Big-Theta(n lg n)", quick_sort);
  //test("Quick-sort (Pivot), Big-Theta(n lg n)", quick_sort2);
}
