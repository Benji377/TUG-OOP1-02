//----------------------------------------------------------------------------------------------------------------------
//
// The StorySegment class represents a segment of a story in the game world. It contains a key, text and type.
// The key is used to identify the StorySegment, the text contains the actual content of the StorySegment and the type
// indicates whether the StorySegment is a narrative or an error.
//
// Group: 086
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
using std::ostream;

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
    /// Friend function to overload the << operator for the StorySegment class.
    ///
    /// @param os The output stream.
    /// @param story_segment The StorySegment that should be printed.
    ///
    /// @return The output stream.
    //
    friend ostream& operator<<(ostream& os, const StorySegment& story_segment);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the key of the StorySegment.
    ///
    /// @return The key of the StorySegment.
    //
    string getKey() const { return key_; }
};

#endif // STORYSEGMENT_HPP