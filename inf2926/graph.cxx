#include <iostream>
#include <vector>
#include <map>

using uint = unsigned int;

struct G;
struct E;

struct V {
  const G* g;
  const uint id;
  std::vector<E*> e;

  V(const G* _g, const uint _id)
    : g(_g), id(_id), e() { }
};

struct E {
  const G* g;
  const uint id;
  const V* i;
  const V* j;

  E(const G* _g, const uint _id, const V* _i, const V* _j)
    : g(_g), id(_id), i(_i), j(_j) { }

  const V* next(const V* v) const {
    if (v != i && v != j)
      return nullptr;
    return v == i ? j : i;
  }
};

struct G {
  std::vector<V*> v;
  std::vector<E*> e;
  
  G() : v(), e() { }

  V* new_vertex(uint id) {
    if (vertex(id) != nullptr)
      return nullptr;

    auto _v = new V(this, id);
    v.push_back(_v);
    return _v;
  }
  E* new_edge(uint id, V* i, V* j) {
    if (edge(id) != nullptr)
      return nullptr;

    auto _e = new E(this, id, i, j);
    e.push_back(_e);
    i->e.push_back(_e);
    j->e.push_back(_e);
    return _e;
  }
  V* vertex(uint id) {
    for (auto _v : v) {
      if (_v->id == id)
	return _v;
    }
    return nullptr;
  }
  E* edge(uint id) {
    for (auto _e : e) {
      if (_e->id == id)
	return _e;
    }
    return nullptr;
  }
};

std::ostream& operator<<(std::ostream& out, const G& g) {
  bool first = true;
  for (auto e : g.e) {
    if (first)
      first = false;
    else
      out << "\n";
    out << e->id << ": ";
    out << "(" << e->i->id << ")--(" << e->j->id << ")";
  }
  return out;
}

void depth(std::map<uint, bool>& mark, const V* v) {
  mark[v->id] = true;
  for (auto e : v->e) {
    auto n = e->next(v);
    if (mark[n->id])
      continue;
    depth(mark, n);
  }
  std::cout << v->id << std::endl;
}

void depth(const G& g) {
  std::cout << "Depth-First Search\n" << std::endl;
  std::map<uint, bool> mark {};
  depth(mark, g.v[0]);
}

int main() {
  std::cout << "Graph\n" << std::endl;
  G g;

  V* v1 = g.new_vertex(1);
  V* v2 = g.new_vertex(2);
  E* e1 = g.new_edge(1, v1, v2);

  //std::cout << g << std::endl;
  depth(g);
}
