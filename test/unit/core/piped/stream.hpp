#pragma once

#include <fstream>

#include <json/single_include/nlohmann/json.hpp>

#include "../../mtTestSuite.h"
#include "../Config.hpp"
#include "core/providers/piped/Stream.h"

namespace TestPipedStream
{
  using json = nlohmann::json;

  inline TEST_CASE(ParseFromDump, {
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/piped/stream.json");
    if (!f.is_open())
    {
      ASSERT_TRUE(false, "File not found");
      f.close();
      return;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    f.close();

    const auto ApiService = new piped::stream;
    ApiService->Parse(buf.str());
    const auto parsed = ApiService->GetParsedData();
    delete ApiService;
  });
} // namespace TestPipedSearch
