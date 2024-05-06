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
#include <memory>
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
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the StorySegment class. Initializes the key, text and type of the StorySegment.
    /// @param key The key of the StorySegment.
    /// @param text The text of the StorySegment.
    /// @param type The type of the StorySegment.
    //
    StorySegment(string key, string text, StorySegmentType type) : key_(key), text_(text), type_(type) {}

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Creates a vector of StorySegments from the given file.
    /// @param file_path The path to the file.
    /// @return A vector of StorySegments.
    //
    static vector<StorySegment> loadStory(const char* file_path);

    friend std::ostream& operator<<(std::ostream& os, const StorySegment& story_segment);


};

///---------------------------------------------------------------------------------------------------------------------
///
/// Overloaded operator for the StorySegment class. Prints the text of the StorySegment.
/// @param os The output stream.
/// @param story_segment The StorySegment that should be printed.
/// @return The output stream.
//
std::ostream &operator<<(std::ostream &os, const StorySegment &story_segment);

#endif // STORYSEGMENT_HPP