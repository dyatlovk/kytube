#include "../Config.hpp"
#include "JsonTest.h"
#include "YoutubeApiTest.h"

int main()
{
  TestYoutubeQt::ParseJsonSimple();
  TestYoutubeQt::ParseApiVideo();
  TestYoutubeQt::ParseApiPlayList();
  return 0;
}
