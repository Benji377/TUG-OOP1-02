#ifndef STORYSEGMENT_HPP
#define STORYSEGMENT_HPP

#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

enum class StorySegmentType
{
  NARRATIVE,
  ERROR
};

class StorySegment
{
  private:
    string key_;
    string text_;
    StorySegmentType type_;

  public:
    StorySegment(string key, string text, StorySegmentType type) : key_(key), text_(text), type_(type) {}
    friend std::ostream& operator<<(std::ostream& os, const StorySegment& story_segment);
};

std::ostream& operator<<(std::ostream& os, const StorySegment& story_segment)
{
  os << story_segment.text_;
  return os;
}

#endif // STORYSEGMENT_HPP