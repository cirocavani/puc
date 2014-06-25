#include <iostream>
#include <vector>
#include <algorithm>

struct T {
  int i, begin, end, value;
  T(int _i, int _b, int _e, int _v)
    : i(_i), begin(_b), end(_e), value(_v) { }
};

std::ostream& operator<<(std::ostream& out, const T& t) {
  return out << t.i << ": (" << t.begin << ", " << t.end << ") ~ " << t.value;
}

std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  for (auto& t : v)
    out << t << std::endl;
  return out;
}

std::vector<T> schedule(const std::vector<T>& tasks) {
  std::vector<T> tasks_begin = tasks; // copy
  std::sort(tasks_begin.begin(), tasks_begin.end(), [](const T& i, const T& j) { return i.begin < j.begin; });
  std::cout << "Tasks sorted by begin:" << std::endl;
  std::cout << tasks_begin << std::endl;

  std::vector<T> tasks_end = tasks; // copy
  std::sort(tasks_end.begin(), tasks_end.end(), [](const T& i, const T& j) { return i.end < j.end; });
  std::cout << "Tasks sorted by end:" << std::endl;
  std::cout << tasks_end << std::endl;

  std::vector<int> prior(tasks.size() + 1, -1);
  int p = -1;
  auto tend = tasks_end.begin();
  for (auto& tbegin : tasks_begin) {
    for (auto iend = tend; tbegin.begin >= iend->end; ++iend) {
      p = iend->i;
      tend = iend; // copy
    }
    prior[tbegin.i] = p;
  }
  
  std::cout << "Prior Tasks:" << std::endl;
  for (int i = 1; i < prior.size(); ++i) {
    const T& t = tasks[i - 1];
    int p = prior[i];
    std::cout << t << " prior ";
    if (p == -1)
      std::cout << "none";
    else
      std::cout << tasks[p - 1];
    std::cout << std::endl;
  }

  std::vector<int> v(tasks.size() + 1, 0);
  std::vector<int> v_last(tasks.size() + 1, -1);
  int last = -1;
  for (auto& t : tasks_end) {
    int w = t.value;
    int p = prior[t.i];
    if (p != -1)
      w += v[p];
    int wlast = last == -1 ? 0 : v[last];
    last = w > wlast ? t.i : last;
    v[t.i] = std::max(w, wlast);
    v_last[t.i] = last;
  }

  std::vector<T> result;
  while (last != -1) {
    const T& t = tasks[last - 1];
    result.push_back(t);
    int p = prior[t.i];
    last = p == -1 ? -1 : v_last[p];
  }
  return result;
}

void execute(const std::vector<T>& tasks) {
  std::cout << "Tasks:" << std::endl;
  std::cout << tasks << std::endl;

  std::vector<T> result = schedule(tasks);
  
  std::cout << "\nSchedule:" << std::endl;
  std::cout << result << std::endl;
}

int main() {
  std::cout << "Weighted Interval Scheduling\n" << std::endl;

  {
    std::cout << "Case 1\n" << std::endl;
    int i = 1;
    std::vector<T> tasks {
      T(i, 10, 30, 700),
	T(++i, 10, 20, 300),
	T(++i, 50, 60, 400),
	T(++i, 20, 30, 100),
	T(++i, 50, 80, 300),
	T(++i, 40, 70, 200),
	};
    execute(tasks);
  }

  {
    std::cout << "Case 2\n" << std::endl;
    int i = 1;
    std::vector<T> tasks {
      T(i, 10, 20, 6),
	T(++i, 15, 25, 5),
	T(++i, 25, 30, 10),
	};
    execute(tasks);
  }

  {
    std::cout << "Case 3\n" << std::endl;
    int i = 1;
    std::vector<T> tasks {
      T(i, 10, 20, 2),
	T(++i, 20, 30, 3),
	T(++i, 30, 40, 5),
	};
    execute(tasks);
  }

  {
    std::cout << "Case 4\n" << std::endl;
    int i = 1;
    std::vector<T> tasks {
      T(i, 10, 20, 5),
	T(++i, 15, 25, 6),
	T(++i, 20, 30, 10),
	};
    execute(tasks);
  }
}
