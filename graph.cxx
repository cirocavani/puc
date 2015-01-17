#include <iostream>
#include <vector>
#include <map>

struct G;
struct E;

struct V {
  const G* g;
  const std::string id;
  std::vector<E*> e;

  V(const G* _g, const std::string _id)
    : g(_g), id(_id), e() { }
};

struct E {
  const G* g;
  const std::string id;
  const V* i;
  const V* j;

  E(const G* _g, const std::string _id, const V* _i, const V* _j)
    : g(_g), id(_id), i(_i), j(_j) { }

  const V* next(const V* v) const {
    if (v != i && v != j)
      return nullptr;
    return v == i ? j : i;
  }
};

enum T {
  DIRECTED,
  UNDIRECTED,
};

struct G {
  const T t;
  std::vector<V*> v;
  std::vector<E*> e;

  G(const T _t = UNDIRECTED) : t(_t), v(), e() { }

  V* new_vertex(std::string id) {
    if (vertex(id) != nullptr)
      return nullptr;

    auto _v = new V(this, id);
    v.push_back(_v);
    return _v;
  }
  E* new_edge(std::string id, V* i, V* j) {
    if (edge(id) != nullptr)
      return nullptr;

    auto _e = new E(this, id, i, j);
    e.push_back(_e);
    i->e.push_back(_e);
    if (t == UNDIRECTED)
      j->e.push_back(_e);
    return _e;
  }
  V* vertex(std::string id) {
    for (auto _v : v) {
      if (_v->id == id)
	return _v;
    }
    return nullptr;
  }
  E* edge(std::string id) {
    for (auto _e : e) {
      if (_e->id == id)
	return _e;
    }
    return nullptr;
  }
};

std::ostream& operator<<(std::ostream& out, const E& e) {
  out << e.id << ": ";
  out << "(" << e.i->id;
  out << (e.g->t == DIRECTED ? ")-->(" : ")--(");
  out << e.j->id << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const G& g) {
  bool first = true;
  for (auto e : g.e) {
    if (first)
      first = false;
    else
      out << "\n";
    out << *e;
  }
  return out;
}

struct VertexIndex {
  int tree;
  int index;
  int count;
  VertexIndex(int _t = 0, int _i = 0, int _c = 0)
    : tree(_t), index(_i), count(_c) { }
  int inc(int c) { return count += c; }
};

enum EdgeType {
  TREE = 0,
  FORWARD,
  BACKWARD,
  CROSSING,
};

EdgeType edge_type(const VertexIndex& i, const VertexIndex& j) {
  if (i.index < j.index && j.index <= i.index + i.count)
    return FORWARD;
  if (j.index < i.index && i.index <= j.index + j.count)
    return BACKWARD;
  return CROSSING;
}

EdgeType edge_type(const E* e, const std::map<const V*, VertexIndex*>& vertex) {
    auto i = *vertex.at(e->i);
    auto j = *vertex.at(e->j);
    return edge_type(i, j);
}

void depth_index(std::vector<E*>& tree, std::map<const V*, VertexIndex*>& index, int& n, int t, const V* v) {
  auto vi = new VertexIndex(t, ++n);
  index[v] = vi;
  for (auto e : v->e) {
    auto j = e->j;
    if (index.count(j) != 0)
      continue;
    tree.push_back(e);
    depth_index(tree, index, n, t, j);
    vi->inc(1 + index[j]->count);
  }
}

void edge_type(const G& g) {
  std::cout << "Edge Type\n" << std::endl;
  std::map<E*, EdgeType> edges;

  std::map<const V*, VertexIndex*> index;
  int n = 0;
  int t = 0;
  for (auto v : g.v) {
    if (index.count(v) != 0)
      continue;
    std::vector<E*> tree;
    depth_index(tree, index, n, ++t, v);
    for (auto e : tree)
      edges[e] = TREE;
  }

  for (auto e : g.e) {
    if (edges.count(e) != 0)
      continue;
    edges[e] = edge_type(e, index);
  }

  for (auto& t : edges)
    std::cout << *t.first << " type " << t.second << std::endl;
}

int main() {
  std::cout << "Graph\n" << std::endl;
  G g(DIRECTED);

  V* va = g.new_vertex("A");
  V* vb = g.new_vertex("B");
  V* vc = g.new_vertex("C");
  V* vd = g.new_vertex("D");
  V* ve = g.new_vertex("E");
  V* vf = g.new_vertex("F");
  V* vg = g.new_vertex("G");

  g.new_edge("a->b", va, vb);
  g.new_edge("a->e", va, ve);
  g.new_edge("b->d", vb, vd);
  g.new_edge("c->a", vc, va);
  g.new_edge("c->f", vc, vf);
  g.new_edge("c->g", vc, vg);
  g.new_edge("d->e", vd, ve);
  g.new_edge("d->a", vd, va);
  g.new_edge("e->b", ve, vb);
  g.new_edge("g->f", vg, vf);


  //std::cout << g << std::endl;
  edge_type(g);
}
