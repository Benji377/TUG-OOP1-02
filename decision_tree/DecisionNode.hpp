#ifndef DECISION_NODE_HPP
#define DECISION_NODE_HPP

#include <functional>
#include <memory>
#include <iostream>
#include <string>

enum NodeType {
  CONDITION,
  ACTION
};

class DecisionNode {
public:
  std::string task_name_;
  std::function<bool(Game*, std::shared_ptr<Player>)> condition_;
  std::function<Action(Game*, std::shared_ptr<Player>)> action_;
  NodeType node_type_;
  std::shared_ptr<DecisionNode> true_branch;
  std::shared_ptr<DecisionNode> false_branch;

  DecisionNode(std::function<bool(Game*, std::shared_ptr<Player>)> condition, std::string task_name)
    : task_name_(task_name), condition_(condition), node_type_(CONDITION) {}

  DecisionNode(std::function<Action(Game*, std::shared_ptr<Player>)> action, std::string task_name)
    : task_name_(task_name), action_(action), node_type_(ACTION) {}

  bool isConditionNode() const {
    return node_type_ == CONDITION;
  }

  bool evaluateCondition(Game* game, std::shared_ptr<Player> player) const {
    return condition_(game, player);
  }

  Action reviewAction(Game* game, std::shared_ptr<Player> player) const {
    return action_(game, player);
  }

  void toDot(std::ostream& out, int& node_id) const {
    int current_id = node_id++;
    out << "  node" << current_id << " [label=\"";
    out << task_name_;
    out << "\"];\n";
    if (node_type_ == CONDITION) {
      int true_id = node_id;
      true_branch->toDot(out, node_id);
      out << "  node" << current_id << " -> node" << true_id << " [label=\"true\"];\n";
      int false_id = node_id;
      false_branch->toDot(out, node_id);
      out << "  node" << current_id << " -> node" << false_id << " [label=\"false\"];\n";
    }
  }
};

#endif // DECISION_NODE_HPP
