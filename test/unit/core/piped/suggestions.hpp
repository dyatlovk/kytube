#pragma once

#include <fstream>

#include <json/single_include/nlohmann/json.hpp>

#include "../../mtTestSuite.h"
#include "../Config.hpp"
#include "core/providers/piped/Suggestions.h"

namespace TestPipedSuggestions
{
  using json = nlohmann::json;

  inline TEST_CASE(ParseFromDump, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/piped/suggestions.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    f.close();

    const auto ApiService = new piped::suggestions;
    ApiService->Parse(buf.str());
    const auto parsed = ApiService->getParsedData();
    ASSERT_EQ_CHAR(parsed.query.c_str(), "game", "Query");
    ASSERT_EQ_INT(parsed.suggestions.size(), 14, "Result size");
    delete ApiService;
  });

  inline auto Run() -> void
  {
    ParseFromDump();
  }
} // namespace TestPipedSuggestions
