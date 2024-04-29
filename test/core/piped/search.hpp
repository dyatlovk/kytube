#pragma once

#include <core/providers/piped/Search.h>
#include <fstream>

#include <core/network/request.hpp>
#include <json/single_include/nlohmann/json.hpp>

#include "../Config.hpp"
#include "../../mtTestSuite.h"

namespace TestPipedSearch
{
  using json = nlohmann::json;

  inline TEST_CASE(ParseFromDump, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/piped/search.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    f.close();

    const auto ApiService = new piped::search;
    ApiService->MakeFromString(buf.str());
    const auto parsed = ApiService->getParsedData();
    ASSERT_EQ_INT(parsed.corrected, false, "Corrected");
    ASSERT_EQ_CHAR(parsed.suggestion->c_str(), "", "Suggestion");
    ASSERT_EQ_CHAR(parsed.nextpage.c_str(), "nextpage_url", "Next page");
    ASSERT_EQ_INT(parsed.items.size(), 2, "Items size");

    auto item = parsed.items.at(0);
    ASSERT_EQ_CHAR(item.url.c_str(), "/watch?v=123", "Item url");
    ASSERT_EQ_CHAR(item.type.c_str(), "stream", "Item stream");
    ASSERT_EQ_CHAR(item.title.c_str(), "Title", "Title");
    ASSERT_EQ_CHAR(item.thumbnail.c_str(), "thumbnail_link", "Thumbnail");
    ASSERT_EQ_CHAR(item.uploaderName.c_str(), "Name", "Name");
    ASSERT_EQ_CHAR(item.uploaderUrl.c_str(), "/channel/id", "Channel url");
    ASSERT_EQ_CHAR(item.uploaderAvatar.c_str(), "avatar_link", "Avatar link");
    ASSERT_EQ_CHAR(item.uploadedDate->c_str(), "2 years ago", "Uploaded date");
    ASSERT_EQ_CHAR(item.shortDescription->c_str(), "description", "Description");
    ASSERT_EQ_INT(item.duration, 3150, "Duration");
    ASSERT_EQ_INT(item.views, 1586334, "Views");
    ASSERT_EQ_LONG(item.uploaded, 1650319200000, "Uploaded");
    ASSERT_EQ_INT(item.uploaderVerified, true, "Uploaded verified");
    ASSERT_EQ_INT(item.isShort, false, "is short");

    ASSERT_TRUE(parsed.items.at(1).uploadedDate == nullptr, "Uploaded is null");
    delete ApiService;
  })

  inline TEST_CASE(ParseRequest, {
    const auto search = new piped::search;
    const auto requestService = new network::request();
    auto result = requestService->Get("https://pipedapi.kavin.rocks/search?q=trending&filter=videos");
    search->MakeFromString(result);
    const auto data = search->getParsedData();
    ASSERT_TRUE(data.items.size() > 0, "Items size");
    delete search;
    delete requestService;
  })
} // namespace TestPipedSearch
