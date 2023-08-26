#pragma once

#include <fstream>

#include <json/single_include/nlohmann/json.hpp>

#include "mtTestSuite.h"

namespace TestYoutubeQt
{
  using json = nlohmann::json;

  TEST_CASE(ParseJsonSimple, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/simple.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    json data = json::parse(f);
    f.close();

    ASSERT_EQ_FLOAT(data["pi"].get<float>(), 3.141, "Read json float");
    ASSERT_TRUE(data["happy"].get<bool>(), "Read json boolean");
  })
} // namespace TestYoutubeQt
