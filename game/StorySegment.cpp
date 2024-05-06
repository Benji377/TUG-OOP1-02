#include "StorySegment.hpp"

std::ostream &operator<<(std::ostream &os, const StorySegment &story_segment)
{
  os << story_segment.text_;
  return os;
}