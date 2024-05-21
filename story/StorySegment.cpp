#include "StorySegment.hpp"
#include "../utility/Utils.hpp"
#include <fstream>

ostream &operator<<(ostream &os, const StorySegment &story_segment)
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
      }
      else
      {
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