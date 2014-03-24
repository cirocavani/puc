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

void swap(std::vector<int>& v, int i, int j) {
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
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

  int len = end - start + 1;
  int middle = start + (len - len % 2) / 2 - 1;
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

  int pivot = v[end],
    i = start - 1,
    j = end + 1;
  for (;;) {
    while (v[++i] < pivot);
    while (v[--j] > pivot);
    if (i >= j) break;
    swap(v, i, j);
  }

  quick_sort(v, start, i - 1);
  quick_sort(v, j + 1, end);
}

void quick_sort(std::vector<int>& v) {
  quick_sort(v, 0, v.size() - 1);
}

// Quick-sort w/ Pivot selection Big-Theta(n lg n)
int select_median(std::vector<int>& v, int start, int end) {
  if (start == end) {
    return v[start];
  }

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

int select_k(std::vector<int>& v, int start, int end, int k) {
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
  if (k == i) return pivot;
  else if (k < i) return select_k(v, start, i - 1, k);
  else return select_k(v, j + 1, end, k - i);
}

void quick_sort2(std::vector<int>& v, int start, int end) {
  if (start >= end)
    return;

  int len = end - start + 1,
    m = start + (len - len % 2) - 1,
    pivot = select_k(v, start, end, m),
    i = start - 1,
    j = end + 1;
  for (;;) {
    while (v[++i] < pivot);
    while (v[--j] > pivot);
    if (i >= j) break;
    swap(v, i, j);
  }

  quick_sort2(v, start, i - 1);
  quick_sort2(v, j + 1, end);
}

void quick_sort2(std::vector<int>& v) {
  quick_sort2(v, 0, v.size() - 1);
}

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

  const std::vector<int> v6 { 13, 19, 9, 5, 12, 8, 7, 4, 11, 2, 6, 21 };
  std::cout << v5 << std::endl;

  auto test_order = [](const std::vector<int>& v) -> std::string {
    if (v.empty())
      return "pass";
    for (int i = 0, j = v.size() - 1; i < j; ++i) {
      if (v[i] > v[j])
	return "fail";
    }
    return "pass";
  };

  auto test_sort = [&test_order](void (*sort)(std::vector<int>&), const std::vector<int>& v) {
    auto mv = v; // copy mutable
    sort(mv);
    auto ok = test_order(mv);
    std::cout << ok << "... " << v << " -> " << mv << std::endl;
  };

  auto test = [&](std::string label, void (*sort)(std::vector<int>&)) {
    std::cout << label << std::endl;
    auto test = [&](const std::vector<int>& v) { test_sort(sort, v); };
    test(v1);
    test(v2);
    test(v3);
    test(v4);
    test(v5);
    test(v6);
  };

  test("Insertion-sort, Big-Theta(n2)", insertion_sort);
  test("Merge-sort, Big-Theta(n lg n)", merge_sort);
  test("Quick-sort, Big-Theta(n lg n)", quick_sort);
  test("Quick-sort (Pivot), Big-Theta(n lg n)", quick_sort2);
}
