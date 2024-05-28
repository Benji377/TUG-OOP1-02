#include "Helper.hpp"

bool is_valid_move(State state, int x, int y) {return true;}  // Just for testing

// Example helper function to check if a move is valid and rewards the AI
double perform_move(State& state, int dx, int dy) {
  // Implement logic to move the AI:
  // 1. Check if the move is within the bounds of the grid.
  // 2. Update the state (AI position).
  // 3. Calculate and return the reward based on the move.
  double reward = -1.0; // Small penalty to encourage efficient movement

  // Example logic:
  if (is_valid_move(state, dx, dy)) {
    state.setCurrentPosition(std::make_pair(dx, dy));
    reward += 0.0; // Adjust reward based on the new state, if necessary
  } else {
    reward -= 5.0; // Penalty for invalid move
  }

  return reward;
}

