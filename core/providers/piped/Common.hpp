#pragma once

#include <string>

namespace piped
{
  inline auto FindVideoId(const std::string &url) -> std::string
  {
    if (url.empty())
      return "";

    const std::string url_prefix = "/watch?v=";
    size_t pos = url.find(url_prefix);
    std::string result = "";
    if (pos != std::string::npos)
    {
      result = url.substr(pos + url_prefix.size());
    }

    return result;
  }
} // namespace piped
