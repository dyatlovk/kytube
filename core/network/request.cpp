#include "request.hpp"

#include <cstring>
#include <curl/curl.h>
#include <string>

namespace network
{
  request::request()
      : result_("")
      , ua_(DEFAULT_UA)
  {
  }

  request::~request() = default;

  auto request::Get(const std::string &url) -> const std::string
  {
    result_.clear();
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl)
      return "";

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, GetUa().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result_);
    struct curl_slist *headers = NULL;
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return result_;
  }

  auto request::Post(const std::string &url) -> const std::string
  {
    result_.clear();
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl)
      return "";

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, GetUa().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result_);
    struct curl_slist *headers = NULL;
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return result_;
  }

  size_t request::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
  {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
  }

  auto request::UrlEncode(const std::string &input) -> const std::string
  {
    const auto encoded_value = curl_easy_escape(nullptr, input.c_str(), static_cast<int>(input.length()));
    std::string result(encoded_value);
    curl_free(encoded_value);
    return result;
  }

  auto request::UrlDecode(const std::string &input) -> const std::string
  {
    int output_length;
    const auto decoded_value =
        curl_easy_unescape(nullptr, input.c_str(), static_cast<int>(input.length()), &output_length);
    std::string result(decoded_value, output_length);
    curl_free(decoded_value);
    return result;
  }

} // namespace network
