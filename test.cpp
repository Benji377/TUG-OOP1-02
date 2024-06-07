#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <array>
#include <stdexcept>
#include <map>
#include <chrono>

const std::vector<std::string> INITIAL_INPUTS = {"3", "Elija", "w", "Benjo", "b", "Hanno", "r"};
const int MAX_ITERATIONS = 200;
const int MAX_CYCLES = 10;
const std::string FILE_NAME = "output.txt";
const char *const CONFIG_FILE = "configs/dungeon_config_01.txt";
const char *const STORY_FILE = "configs/story_config.txt";

// 17512

void send_input(int write_fd, const std::string& input) {
  write(write_fd, input.c_str(), input.size());
  write(write_fd, "\n", 1);
  fsync(write_fd);
}

std::string read_output(int read_fd) {
  std::array<char, 256> buffer{};
  std::string result;
  ssize_t count;
  while ((count = read(read_fd, buffer.data(), buffer.size() - 1)) > 0) {
    buffer[count] = '\0';  // Null-terminate the buffer
    result.append(buffer.data(), count);
    if (result.find('>') != std::string::npos) {  // Check for the prompt
      break;
    }
  }
  return result;
}

int main() {
  const char* program = "./a2";
  const char* args[] = {program, CONFIG_FILE, STORY_FILE, nullptr};

  int total_failed_cycles = 0;
  int total_completed_cycles = 0;
  int total_qtable_actions = 0;
  int total_random_actions = 0;

  std::ofstream file(FILE_NAME);

  auto start_total_timer = std::chrono::high_resolution_clock::now();  // Start timer

  for (int cycle = 0; cycle < MAX_CYCLES; ++cycle) {
    int to_child_pipe[2];
    int from_child_pipe[2];
    int current_room = 1;

    std::map<std::string, int> commands_dict = {
            {"attack", 0},
            {"loot", 0},
            {"move", 0},
            {"use", 0},
            {"exploration", 0},
            {"exploitation", 0}
    };

    if (pipe(to_child_pipe) == -1 || pipe(from_child_pipe) == -1) {
      std::cerr << "Error creating pipes." << std::endl;
      return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
      std::cerr << "Error forking." << std::endl;
      return 1;
    }

    auto cycle_start_time = std::chrono::high_resolution_clock::now();

    if (pid == 0) { // Child process
      close(to_child_pipe[1]);
      close(from_child_pipe[0]);

      dup2(to_child_pipe[0], STDIN_FILENO);
      dup2(from_child_pipe[1], STDOUT_FILENO);
      dup2(from_child_pipe[1], STDERR_FILENO);

      close(to_child_pipe[0]);
      close(from_child_pipe[1]);

      execvp(program, const_cast<char* const*>(args));
      std::cerr << "Error executing program." << std::endl;
      exit(1);
    }
    else
    { // Parent process
      close(to_child_pipe[0]);
      close(from_child_pipe[1]);

      for (const auto& input : INITIAL_INPUTS) {
        send_input(to_child_pipe[1], input);
        std::string output = read_output(from_child_pipe[0]);
        file << output << std::endl;
      }

      for (int i = 0; i < MAX_ITERATIONS; ++i) {
        // Write the cycle.iteration to the file
        file << cycle + 1 << "." << i << " Iteration" << std::endl;

        send_input(to_child_pipe[1], "play");
        std::string output = read_output(from_child_pipe[0]);
        file << output << std::endl;

        // Check if child process is still running
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result != 0) {
          total_completed_cycles++;
          break;
        }

        size_t pos = output.find("-- ROOM");
        if (pos != std::string::npos) {
          int room = std::stoi(output.substr(pos + 8, 1));
          if (room != current_room) {
            current_room = room;
            std::cout << "Room " << current_room - 1 << " completed in " << i << " iterations" << std::endl;
          }
        }

        if (output.find("Exploration") != std::string::npos) {
          ++commands_dict["exploration"];
        } else if (output.find("Exploitation") != std::string::npos) {
          ++commands_dict["exploitation"];
        }


        for (auto& command : commands_dict) {
          if (output.find(command.first) != std::string::npos) {
            ++command.second;
          }
        }

        if (output.find("!!") != std::string::npos) {
          std::cout << "\033[31m" << "Encountered an error at " << i << ". iteration. Exiting..." << "\033[0m" << std::endl;
          total_failed_cycles++;
          std::cout << "\033[31m" << output << "\033[0m" << std::endl;
          break;
        }
      }

      close(to_child_pipe[1]);
      close(from_child_pipe[0]);

      waitpid(pid, nullptr, 0);
    }
    auto cycle_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = cycle_end_time - cycle_start_time;
    int cycle_seconds = static_cast<int>(elapsed.count());
    int cycle_hours = cycle_seconds / 3600;
    cycle_seconds %= 3600;
    int cycle_minutes = cycle_seconds / 60;
    cycle_seconds %= 60;
    int cycle_seconds_final = cycle_seconds;

    std::cout << "Max room completed: " << --current_room << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "CYCLE " << cycle + 1 << " COMPLETED" << std::endl;
    std::cout << "Time elapsed: " << cycle_hours << "h " << cycle_minutes << "m " << cycle_seconds_final << "s" << std::endl;
    // Write the cycle number to the file
    file << "CYCLE " << cycle + 1 << " COMPLETED" << std::endl;

    // Print command statistics
    for (const auto& command : commands_dict) {
      std::cout << command.first << ": " << command.second << std::endl;
    }
    total_qtable_actions += commands_dict["exploitation"];
    total_random_actions += commands_dict["exploration"];
    std::cout << "-----------------------------------" << std::endl;
  }

  auto end_total_timer = std::chrono::high_resolution_clock::now();  // End timer
  std::chrono::duration<double> elapsed = end_total_timer - start_total_timer;
  int total_seconds = static_cast<int>(elapsed.count());
  int hours = total_seconds / 3600;
  total_seconds %= 3600;
  int minutes = total_seconds / 60;
  total_seconds %= 60;
  int seconds = total_seconds;

  std::cout << "Overall statistics:" << std::endl;
  std::cout << "Total time elapsed: " << hours << "h " << minutes << "m " << seconds << "s" << std::endl;
  std::cout << "Total completed cycles: " << total_completed_cycles << std::endl;
  std::cout << "Total failed cycles: " << total_failed_cycles << std::endl;
  std::cout << "Total Q-table actions: " << total_qtable_actions << std::endl;
  std::cout << "Total random actions: " << total_random_actions << std::endl;


  return 0;
}