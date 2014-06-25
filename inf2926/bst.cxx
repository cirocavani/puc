#include <iostream>
#include <queue>

struct N {
  std::string label;
  double w;
  int level;

  N* parent;
  N* right;
  N* left;

  N(std::string _l, double _w, N* _p = nullptr)
    : label(_l), w(_w), level(_p == nullptr ? 0 : _p->level + 1), parent(_p), right(nullptr), left(nullptr) { }

  N* new_left(std::string label, double w) {
    return left = new N(label, w, this);
  }

  N* new_right(std::string label, double w) {
    return right = new N(label, w, this);
  }
};

double weighted_path_length(N* root) {
  double len = 1.;
  std::queue<N*> nodes;
  nodes.push(root);
  while (!nodes.empty()) {
    N* n = nodes.front();
    nodes.pop();
    if (n->left != nullptr) nodes.push(n->left);
    if (n->right != nullptr) nodes.push(n->right);
    len += n->level * n->w;
  }
  return len;
}

int main() {
  {
    std::cout << "Stop-If-Do Tree" << std::endl;
    auto n_stop = new N("stop", 0.05);
    n_stop->new_right("stop-right", 0.05);
    auto n_if = n_stop->new_left("if", 0.1);
    n_if->new_left("if-stop", 0.05);
    auto n_do = n_if->new_right("do", 0.5);
    n_do->new_right("do-if", 0.1);
    n_do->new_left("do-left", 0.15);
    auto root = n_stop;
    auto len = weighted_path_length(root);
    std::cout << "Length: " << len << std::endl;
  }
  {
    std::cout << "Do-If-Stop Tree" << std::endl;
    auto n_do = new N("do", 0.5);
    n_do->new_left("do-left", 0.15);
    auto n_if = n_do->new_right("if", 0.1);
    n_if->new_left("do-if", 0.1);
    auto n_stop = n_if->new_right("stop", 0.05);
    n_stop->new_right("stop-right", 0.05);
    n_stop->new_left("if-stop", 0.05);
    auto root = n_do;
    auto len = weighted_path_length(root);
    std::cout << "Length: " << len << std::endl;
  }
}
