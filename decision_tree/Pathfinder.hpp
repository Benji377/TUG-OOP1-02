#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <memory>
#include <utility>

class Node {
public:
  int x_, y_;
  int g_, h_;
  std::shared_ptr<Node> parent_;

  Node(int x, int y, int g, int h, std::shared_ptr<Node> parent = nullptr);

  int get_f() const;

  bool operator==(const Node& other) const;
};

// Idea from ChatGPT
struct CompareNode {
  bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const;
};

class Pathfinder {
  private:
    static int heuristic(int x1, int y1, int x2, int y2);
    static bool is_valid(int x, int y, const std::vector<std::vector<int>>& map);
    static std::vector<std::shared_ptr<Node>> get_neighbors(std::shared_ptr<Node> current, const std::vector<std::vector<int>>& map);
    static std::vector<std::shared_ptr<Node>> reconstruct_path(std::shared_ptr<Node> end);
  public:
    static bool find_path(const std::vector<std::vector<int>>& map, const std::pair<int, int> start,
      const std::pair<int, int>& end, std::pair<int, int>& move_position, int& distance);
};

#endif // PATHFINDER_HPP
