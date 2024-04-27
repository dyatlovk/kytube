#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include <json/single_include/nlohmann/json.hpp>

#include "PipedException.h"

namespace piped
{
  class search
  {
  public:
    struct Response;
    struct Item;

  public:
    search()
        : response_()
    {
    }

    ~search()
    {
      delete response_.suggestion;
      for (const auto &it : response_.items)
      {
        delete it.uploadedDate;
        delete it.shortDescription;
      }
    }

    auto getParsedData() -> const Response { return response_; };

    auto MakeFromString(const std::string &buffer) -> void
    {
      json j;

      try
      {
        j = json::parse(buffer);
      }
      catch (json::parse_error &ex)
      {
        std::cout << "parse error at byte " << ex.byte << std::endl;
        return;
      }

      // Items
      std::vector<Item> items{};
      const auto _items = j["items"].get<json::array_t>();
      for (const auto &it : _items)
      {
        Item i;
        i.url = it["url"].get<std::string>();
        i.type = it["type"].get<std::string>();
        i.title = it["title"].get<std::string>();
        i.thumbnail = it["thumbnail"].get<std::string>();
        i.uploaderName = it["uploaderName"].get<std::string>();
        i.uploaderUrl = it["uploaderUrl"].get<std::string>();
        i.uploaderAvatar = it["uploaderAvatar"].get<std::string>();
        if (it["uploadedDate"] != nullptr)
        {
          i.uploadedDate = new std::string(it["uploadedDate"].get<std::string>());
        }
        if (it["shortDescription"] != nullptr)
        {
          i.shortDescription = new std::string(it["shortDescription"].get<std::string>());
        }
        i.duration = it["duration"].get<int>();
        i.views = it["views"].get<uint>();
        i.uploaded = it["uploaded"].get<int64_t>();
        i.uploaderVerified = it["uploaderVerified"].get<bool>();
        i.isShort = it["isShort"].get<bool>();
        items.push_back(i);
      }
      response_.items = items;

      response_.corrected = j["corrected"].get<bool>();
      if (j["suggestion"] != nullptr)
      {
        response_.suggestion = new std::string(j["suggestion"].get<std::string>());
      }
      response_.nextpage = j["nextpage"].get<std::string>();
    }

  public:
    struct Response
    {
      std::vector<Item> items;
      std::string nextpage;
      std::string *suggestion = nullptr;
      bool corrected;
    };

    struct Item
    {
      std::string url;
      std::string type;
      std::string title;
      std::string thumbnail;
      std::string uploaderName;
      std::string uploaderUrl;
      std::string uploaderAvatar;
      std::string *uploadedDate = nullptr;
      std::string *shortDescription = nullptr;
      int duration;
      uint views;
      int64_t uploaded;
      bool uploaderVerified;
      bool isShort;
    };

  private:
    using json = nlohmann::json;
    Error error;
    Response response_;
  };
} // namespace piped
