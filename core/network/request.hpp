#pragma once

#include <cstddef>
#include <string>

namespace network
{
  class request
  {
  public:
    const char *DEFAULT_UA = "KTube/1.0";

  public:
    request();

    ~request();

    auto Get(const std::string &url) -> const std::string;

    auto Post(const std::string &url) -> const std::string;

    auto GetUa() -> const std::string { return ua_; }

    auto SetUa(const std::string &ua) -> void { this->ua_ = ua; }

    static auto UrlEncode(const std::string &input) -> const std::string;

    static auto UrlDecode(const std::string &input) -> const std::string;

  private:
    std::string result_;
    std::string ua_;
    static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
  };
} // namespace network
