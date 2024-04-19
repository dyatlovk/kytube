#pragma once
#include <fstream>

#include <json/single_include/nlohmann/json.hpp>

#include "../Config.hpp"
#include "../youtube/YoutubeApi.h"
#include "mtTestSuite.h"

namespace TestYoutubeQt
{
  using json = nlohmann::json;

  TEST_CASE(ParseApiVideo, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/api.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    f.close();

    const auto ApiService = new youtube::api;
    ApiService->MakeFromString(buf.str());

    const auto parsed = ApiService->getParsedData();

    ASSERT_EQ_CHAR(parsed->kind.c_str(), "youtube#searchListResponse", "Read Kind field");
    ASSERT_EQ_CHAR(parsed->etag.c_str(), "testEtag", "Read etag field");
    ASSERT_EQ_CHAR(parsed->nextPageToken.c_str(), "123", "Read nextPageToken field");
    ASSERT_EQ_CHAR(parsed->prevPageToken.c_str(), "456", "Read nextPageToken field");
    ASSERT_EQ_CHAR(parsed->regionCode.c_str(), "UA", "Read region code field");
    ASSERT_EQ_INT(parsed->pageInfo->resultsPerPage, 3, "Read results per page field");
    ASSERT_EQ_INT(parsed->pageInfo->totalResults, 1000000, "Read total results field");
    ASSERT_EQ_INT(parsed->items.size(), 3, "Items size");
    ASSERT_EQ_CHAR(parsed->items.at(0).id->kind.c_str(), "youtube#video", "Read item kind");
    ASSERT_EQ_CHAR(parsed->items.at(0).id->videoId->c_str(), "123", "Read item videoId");
    ASSERT_EQ_CHAR(parsed->items.at(0).snippet->title.c_str(), "Title", "Read item title");
    ASSERT_EQ_CHAR(parsed->items.at(0).snippet->description.c_str(), "description", "Read item description");
    ASSERT_EQ_CHAR(parsed->items.at(0).snippet->channelTitle.c_str(), "ChannelTitle", "Read item channel title");
    ASSERT_EQ_CHAR(parsed->items.at(0).snippet->leaveBroadcastContent.c_str(), "none", "Read item live");

    delete ApiService;
  })

  TEST_CASE(ParseApiPlayList, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/api.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    f.close();

    const auto ApiService = new youtube::api;
    ApiService->MakeFromString(buf.str());

    const auto parsed = ApiService->getParsedData();

    ASSERT_EQ_CHAR(parsed->kind.c_str(), "youtube#searchListResponse", "Read Kind field");
    ASSERT_EQ_CHAR(parsed->etag.c_str(), "testEtag", "Read etag field");
    ASSERT_EQ_CHAR(parsed->nextPageToken.c_str(), "123", "Read nextPageToken field");
    ASSERT_EQ_CHAR(parsed->prevPageToken.c_str(), "456", "Read nextPageToken field");
    ASSERT_EQ_CHAR(parsed->regionCode.c_str(), "UA", "Read region code field");
    ASSERT_EQ_INT(parsed->pageInfo->resultsPerPage, 3, "Read results per page field");
    ASSERT_EQ_INT(parsed->pageInfo->totalResults, 1000000, "Read total results field");
    ASSERT_EQ_INT(parsed->items.size(), 3, "Items size");
    ASSERT_EQ_CHAR(parsed->items.at(1).id->kind.c_str(), "youtube#playlist", "Read item kind");
    ASSERT_EQ_CHAR(parsed->items.at(1).id->playlistId->c_str(), "123", "Read item playlist id");
    ASSERT_EQ_CHAR(parsed->items.at(1).snippet->title.c_str(), "Title", "Read item title");
    ASSERT_EQ_CHAR(parsed->items.at(1).snippet->description.c_str(), "description", "Read item description");
    ASSERT_EQ_CHAR(parsed->items.at(1).snippet->channelTitle.c_str(), "ChannelTitle", "Read item channel title");
    ASSERT_EQ_CHAR(parsed->items.at(1).snippet->leaveBroadcastContent.c_str(), "none", "Read item live");

    delete ApiService;
  })
} // namespace TestYoutubeQt
