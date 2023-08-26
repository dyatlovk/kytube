#pragma once

#include <iostream>
#include <vector>

#include <json/single_include/nlohmann/json.hpp>

#include "YoutubeException.h"

namespace youtube
{
  class api
  {
  public:
    struct Main;
    struct Page;
    struct Item;
    struct ItemId;
    struct Snippet;

  public:
    api() = default;
    ~api()
    {
      for (const auto &it : main.items)
      {
        delete it.snippet;
        if(it.id->videoId) delete it.id->videoId;
        if(it.id->channelId) delete it.id->channelId;
        if(it.id->playlistId) delete it.id->playlistId;
        delete it.id;
      }
      if(main.nextPageToken) delete main.nextPageToken;
      if(main.prevPageToken) delete main.prevPageToken;
      main.items.clear();
    };

    auto getParsedData() -> const Main * { return &main; };

    auto MakeFromString(const std::string &buffer) -> void
    {
      const auto j = json::parse(buffer);

      if (j.contains("error"))
      {
        error.code = j["error"]["code"].get<int>();
        error.message = j["error"]["message"].get<std::string>();
        error.reason = j["error"]["reason"].get<std::string>();
        throw Exception(error.message.c_str(), error.reason.c_str(), error.code);
      }
      main.kind = j["kind"].get<std::string>();
      main.etag = j["etag"].get<std::string>();
      if (j.contains("nextPageToken"))
      {
        main.nextPageToken = new std::string(j["nextPageToken"].get<std::string>());
      }
      if (j.contains("prevPageToken"))
      {
        main.prevPageToken = new std::string(j["prevPageToken"].get<std::string>());
      }
      main.regionCode = j["regionCode"].get<std::string>();

      // Page info
      page.totalResults = j["pageInfo"]["totalResults"].get<int>();
      page.resultsPerPage = j["pageInfo"]["resultsPerPage"].get<int>();
      main.pageInfo = &page;

      // Items
      std::vector<Item> items{};
      const auto _items = j["items"].get<json::array_t>();
      for (const auto &it : _items)
      {
        Item i;
        i.id = new ItemId;
        i.kind = it["kind"].get<std::string>();
        i.etag = it["etag"].get<std::string>();

        i.id->kind = it["id"]["kind"].get<std::string>();
        if (it["id"].contains("videoId"))
        {
          i.id->videoId = new std::string(it["id"]["videoId"].get<std::string>());
        }

        if (it["id"].contains("playlistId"))
        {
          i.id->playlistId = new std::string(it["id"]["playlistId"].get<std::string>());
        }

        if (it["id"].contains("channelId"))
        {
          i.id->channelId = new std::string(it["id"]["channelId"].get<std::string>());
        }

        i.snippet->publishedAt = it["snippet"]["publishedAt"].get<std::string>();
        i.snippet->channelId = it["snippet"]["channelId"].get<std::string>();
        i.snippet->title = it["snippet"]["title"].get<std::string>();
        i.snippet->description = it["snippet"]["description"].get<std::string>();
        i.snippet->channelTitle = it["snippet"]["channelTitle"].get<std::string>();
        i.snippet->leaveBroadcastContent = it["snippet"]["liveBroadcastContent"].get<std::string>();

        items.push_back(i);
      }

      main.items = items;
    };

  public:
    struct Main
    {
      std::string kind;
      std::string etag;
      std::string *nextPageToken = nullptr;
      std::string *prevPageToken = nullptr;
      std::string regionCode;
      Page *pageInfo = nullptr;
      std::vector<Item> items;
    } main;

    struct Page
    {
      int totalResults;
      int resultsPerPage;
    } page{};

    struct Item
    {
      std::string kind;
      std::string etag;
      ItemId *id = nullptr;
      Snippet *snippet = new Snippet;
    };

    struct ItemId
    {
      std::string kind;
      std::string *videoId = nullptr;
      std::string *playlistId = nullptr;
      std::string *channelId = nullptr;
    };

    struct Snippet
    {
      std::string publishedAt;
      std::string channelId;
      std::string title;
      std::string description;
      std::string channelTitle;
      std::string leaveBroadcastContent;
    };

  private:
    using json = nlohmann::json;
    Error error;
  };
} // namespace youtube
