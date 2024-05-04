#pragma once

#include <string>
#include <vector>

#include <json/single_include/nlohmann/json.hpp>

#include "PipedException.h"

namespace piped
{
  class suggestions
  {
  public:
    struct Response;

  public:
    suggestions()
        : response_()
    {
    }

    ~suggestions() {}

    auto getParsedData() -> const Response { return response_; };

    auto Parse(const std::string &buffer) -> void
    {
      json j;

      try
      {
        j = json::parse(buffer);
      }
      catch (json::parse_error &ex)
      {
        return;
      }

      if (j.size() < 2)
      {
        return;
      }

      response_.query = j.at(0);

      const auto sug_ = j.at(1);

      for (const auto &item : sug_)
      {
        const auto str = item.get<std::string>();
        response_.suggestions.push_back(str);
      }
    }

  public:
    struct Response
    {
      std::string query;
      std::vector<std::string> suggestions;
    };

  private:
    using json = nlohmann::json;
    Response response_;
  };
} // namespace piped
