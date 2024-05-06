#include "StorySegment.hpp"
#include "../utility/Utils.hpp"
#include <fstream>

vector<StorySegment> StorySegment::loadStory(const char *story_path)
{
  vector<StorySegment> story_segments;
  std::ifstream file(story_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
      StorySegmentType type = line[0] == 'N' ? StorySegmentType::NARRATIVE : StorySegmentType::ERROR;
      vector<string> parts = Utils::splitString(line, ":");
      story_segments.push_back(StorySegment(parts[0], parts[1], type));
    }
    file.close();
  }
  return story_segments;
}

std::ostream &operator<<(std::ostream &os, const StorySegment &story_segment)
{
  vector<string> lines = Utils::splitString(story_segment.text_, ";");
  string text;
  bool isFirst = true;
  for (auto &line : lines)
  {
    if (line != "")
    {
      string symbol = story_segment.type_ == StorySegmentType::NARRATIVE ? "**" : "!!";
      if (isFirst)
      {
        text += symbol + " " + line;
        isFirst = false;
      } else {
        text += "\n" + symbol + " " + line;
      }
    }
    else
    {
      text += "\n";
    }
  }
  os << text;
  return os;
}