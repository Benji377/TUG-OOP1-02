//----------------------------------------------------------------------------------------------------------------------
//
// Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam nec purus feugiat, molestie ipsum et, ultricies
//
// Group: 086
//
//
// Author: Elija Innerkofler, 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef STORYSEGMENT_HPP
#define STORYSEGMENT_HPP

#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

// Enum for the type of the StorySegment
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

///---------------------------------------------------------------------------------------------------------------------
///
/// Overloads the << operator to print the text of the StorySegment.
///
/// @param os The output stream.
/// @param story_segment The StorySegment to print.
/// @return The output stream.
//
std::ostream &operator<<(std::ostream &os, const StorySegment &story_segment);

#endif // STORYSEGMENT_HPP