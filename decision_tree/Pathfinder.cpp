#include "Pathfinder.hpp"
#include <queue>
#include <cmath>
#include <unordered_map>
#include <algorithm>

Node::Node(int x, int y, int g, int h, std::shared_ptr<Node> parent)
  : x_(x), y_(y), g_(g), h_(h), parent_(parent) {}

int Node::get_f() const {
  return g_ + h_;
}

bool Node::operator==(const Node& other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool CompareNode::operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
  return a->get_f() > b->get_f();
}

int Pathfinder::heuristic(int x1, int y1, int x2, int y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool Pathfinder::is_valid(int x, int y, const std::vector<std::vector<int>>& map) {
  return x >= 0 && y >= 0 && static_cast<size_t>(x) < map.size() && static_cast<size_t>(y) < map[0].size() && map[x][y] == 0;
}

std::vector<std::shared_ptr<Node>> Pathfinder::get_neighbors(std::shared_ptr<Node> current, const std::vector<std::vector<int>>& map) {
  std::vector<std::shared_ptr<Node>> neighbors;
  int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
  int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};

  for (int i = 0; i < 8; ++i) {
    int nx = current->x_ + dx[i];
    int ny = current->y_ + dy[i];

    if (is_valid(nx, ny, map)) {
      neighbors.push_back(std::make_shared<Node>(nx, ny, current->g_ + 1, 0, current));
    }
  }

  return neighbors;
}

std::vector<std::shared_ptr<Node>> Pathfinder::reconstruct_path(std::shared_ptr<Node> end) {
  std::vector<std::shared_ptr<Node>> path;
  auto current = end;

  while (current) {
    path.push_back(current);
    current = current->parent_;
  }

  std::reverse(path.begin(), path.end());
  return path;
}

bool Pathfinder::find_path(const std::vector<std::vector<int>>& map, const std::pair<int, int> start, const std::pair<int, int>& end, std::pair<int, int>& move_position, int& distance) {
  auto start_node = std::make_shared<Node>(start.first - 1, start.second - 1, 0, 0);
  auto end_node = std::make_shared<Node>(end.first - 1, end.second - 1, 0, 0);

  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> open_list;
  std::unordered_map<int, std::shared_ptr<Node>> closed_list;

  start_node->h_ = heuristic(start_node->x_, start_node->y_, end_node->x_, end_node->y_);
  open_list.push(start_node);

  while (!open_list.empty()) {
    auto current = open_list.top();
    open_list.pop();

    if (*current == *end_node) {
      std::vector<std::shared_ptr<Node>> path = reconstruct_path(current);
      distance = current->g_;
      if (path.size() > 1) {
        move_position = { path[1]->x_ + 1, path[1]->y_ + 1 };
        return true;
      } else {
        return false;
      }
    }

    closed_list[current->x_ * map[0].size() + current->y_] = current;

    for (auto neighbor : get_neighbors(current, map)) {
      neighbor->h_ = heuristic(neighbor->x_, neighbor->y_, end_node->x_, end_node->y_);

      if (closed_list.find(neighbor->x_ * map[0].size() + neighbor->y_) != closed_list.end()) {
        continue;
      }

      open_list.push(neighbor);
    }
  }

  distance = -1;
  return false;
}
