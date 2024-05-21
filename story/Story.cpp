#include "Story.hpp"
#include "../utility/Utils.hpp"
#include <fstream>

void Story::parseStory(const char *file_path)
{
  std::ifstream file(file_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
      StorySegmentType type = line[0] == 'N' ? StorySegmentType::NARRATIVE : StorySegmentType::ERROR;
      vector<string> parts = Utils::splitString(line, ":");
      story_.push_back(StorySegment(parts[0], parts[1], type));
    }
    file.close();
  }
}

StorySegment Story::getStorySegment(string key)
{
  for (auto &segment : story_)
  {
    if (segment.getKey() == key) { return segment; }
  }
  return StorySegment("", "Could not find story segment with key: " + key, StorySegmentType::ERROR);
}