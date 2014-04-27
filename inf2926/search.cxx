#include <iostream>
#include <vector>
#include <string>

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

int search_first(const std::vector<int>& v, const int c) {
  for (int i = 0; i < v.size(); ++i)
    if (v[i] == c)
      return i;
  return -1;
}

int search_last(const std::vector<int>& v, const int c) {
  for (int i = v.size() - 1; i >= 0; --i)
    if (v[i] == c)
      return i;
  return -1;
}

int bsearch_first(const std::vector<int>& v, const int c, const int i, const int j) {
  if (i == j)
    return c == v[i] ? i : -1;

  int n = i + j,
    m = (n - n % 2) / 2;

  if (c > v[m])
    return bsearch_first(v, c, m + 1, j);
  else
    return bsearch_first(v, c, i, m);
}

int bsearch_first(const std::vector<int>& v, const int c) {
  return bsearch_first(v, c, 0, v.size() - 1);
}

int bsearch_last(const std::vector<int>& v, const int c, const int i, const int j) {
  if (i == j)
    return c == v[i] ? i : -1;

  int n = i + j,
    m = (n + n % 2) / 2;

  if (c < v[m])
    return bsearch_last(v, c, i, m - 1);
  else
    return bsearch_last(v, c, m, j);
}

int bsearch_last(const std::vector<int>& v, const int c) {
  return bsearch_last(v, c, 0, v.size() - 1);
}

int main() {
  std::cout << "Search" << std::endl;
 
  const std::vector<int> v1 { 5, 4, 3, 2, 1 };
  std::cout << "1: " << v1 << std::endl;

  const std::vector<int> v2 { 10 };
  std::cout << "2: " << v2 << std::endl;

  const std::vector<int> v3 { 10, 3, 30, 3, 7, 3 };
  std::cout << "3: " << v3 << std::endl;

  const std::vector<int> v4 { 13, 19, 9, 5, 12, 8, 7, 4, 11, 2, 6, 21 };
  std::cout << "4: " << v4 << std::endl; 

  auto test = [](std::string name,
		 int (*search)(const std::vector<int>& v, const int c),
		 const std::vector<int>& v, const int c) {
    int i = search(v, c);
    std::cout << name << ": " << i << std::endl;
  };

  test("seq first V1",  search_first, v1, 4);
  test("seq first V1,-1",  search_first, v1, 10);
  test("seq last V1",  search_last, v1, 4);
  test("seq last V1,-1",  search_last, v1, 10);

  test("seq first V2", search_first, v2, 10);
  test("seq first V2,-1", search_first, v2, 4);
  test("seq last V2", search_last, v2, 10);
  test("seq last V2,-1", search_last, v2, 4);

  test("seq first V3", search_first, v3, 3);
  test("seq first V3,-1", search_first, v3, 5);
  test("seq last V3", search_last, v3, 3);
  test("seq last V3,-1", search_last, v3, 5);

  test("seq first V4", search_first, v4, 11);
  test("seq first V4,-1", search_first, v4, 15);
  test("seq last V4", search_first, v4, 11);
  test("seq last V4,-1", search_first, v4, 15);

  const std::vector<int> v5 { 5, 5, 5, 5, 5 };
  std::cout << "5: " << v5 << std::endl;

  std::cout << "seq first V5:     " << search_first(v5, 5) << std::endl;
  std::cout << "seq last V5:      " << search_last(v5, 5) << std::endl;
  std::cout << "bserach first V5: " << bsearch_first(v5, 5) << std::endl;
  std::cout << "bsearch last V5:  " << bsearch_last(v5, 5) << std::endl;

  const std::vector<int> v6 { 11, 13, 17, 17, 17, 17, 17, 17, 17, 17, 19, 23 };
  std::cout << "6: " << v6 << std::endl;

  std::cout << "serach first:  " << search_first(v6, 17) << std::endl;
  std::cout << "search last:   " << search_last(v6, 17) << std::endl;
  std::cout << "bserach first: " << bsearch_first(v6, 17) << std::endl;
  std::cout << "bsearch last:  " << bsearch_last(v6, 17) << std::endl;
}
