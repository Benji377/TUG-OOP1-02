#ifndef STORY_HPP
#define STORY_HPP

#include <vector>
#include "StorySegment.hpp"

using std::vector;

class Story
{
  private:
    vector<StorySegment> story_;

  public:
    void parseStory(const char *file_path);
    StorySegment getStorySegment(string key);
};

#endif //STORY_HPP