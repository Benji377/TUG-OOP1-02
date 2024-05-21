//---------------------------------------------------------------------------------------------------------------------
//
// The Story class represents a story in the game world. It contains a vector of StorySegments that represent the story.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef STORY_HPP
#define STORY_HPP

#include <vector>
#include "StorySegment.hpp"

class Story
{
  private:
    std::vector<StorySegment> story_;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Function to parse a story from a file
    ///
    /// @param file_path the path to the file
    //
    void parseStory(const char *file_path);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns a StorySegment by key
    ///
    /// @param key the key of the StorySegment
    ///
    /// @return the StorySegment with the given key
    //
    StorySegment getStorySegment(string key);
};

#endif //STORY_HPP