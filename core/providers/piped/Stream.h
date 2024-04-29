#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include <json/single_include/nlohmann/json.hpp>

#include "PipedException.h"

namespace piped
{
  class stream
  {
  public:
    struct Response;
    struct AudioStream;
    struct VideoStream;
    struct RelatedStream;
    struct Stream; // audio and video
    struct Subtitle;
    struct PreviewFrame;

  public:
    stream() = default;

    ~stream()
    {
      delete response.dash;
      for (const auto item : response.audioStreams)
      {
        delete item.audioTrackId;
        delete item.audioTrackLocale;
        delete item.audioTrackType;
        delete item.codec;
      }
      for (const auto item : response.videoStreams)
      {
        delete item.audioTrackId;
        delete item.audioTrackLocale;
        delete item.audioTrackType;
        delete item.codec;
      }
      for (const auto item : response.relatedStreams)
      {
        delete item.shortDescription;
      }
    }

    auto GetParsedData() -> const Response { return response; }

    auto Parse(const std::string &buffer) -> void
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

      response.title = j["title"].get<std::string>();
      response.description = j["description"].get<std::string>();
      response.uploadDate = j["uploadDate"].get<std::string>();
      response.uploader = j["uploader"].get<std::string>();
      response.uploaderUrl = j["uploaderUrl"].get<std::string>();
      response.uploaderAvatar = j["uploaderAvatar"].get<std::string>();
      response.thumbnailUrl = j["thumbnailUrl"].get<std::string>();
      response.hls = j["hls"].get<std::string>();
      if (j["dash"] != nullptr)
      {
        response.dash = new std::string(j["dash"].get<std::string>());
      }

      if (j["lbryId"] != nullptr)
      {
        response.lbryId = new std::string(j["lbryId"].get<std::string>());
      }

      response.category = j["category"].get<std::string>();
      response.license = j["license"].get<std::string>();
      response.visibility = j["visibility"].get<std::string>();

      // Tags
      std::vector<std::string> tags{};
      const auto _tags = j["tags"].get<json::array_t>();
      for (const auto &tag : _tags)
      {
        tags.push_back(tag);
      }
      response.tags = tags;
      // End Tags

      // Metainfo
      std::vector<std::string> meta{};
      const auto _meta = j["metaInfo"].get<json::array_t>();
      for (const auto &m : _meta)
      {
        meta.push_back(m);
      }
      response.metaInfo = meta;
      // end Metainfo

      response.uploaderVerified = j["uploaderVerified"].get<bool>();
      response.duration = j["duration"].get<uint>();
      response.duration = j["duration"].get<uint>();
      response.views = j["views"].get<uint>();
      response.likes = j["likes"].get<uint>();
      response.dislikes = j["dislikes"].get<uint>();
      response.uploaderSubscriberCount = j["uploaderSubscriberCount"].get<uint>();

      // Audiostreams ==========================================================
      std::vector<AudioStream> audio_streams_{};
      const auto a_streams_ = j["audioStreams"].get<json::array_t>();
      for (const auto &item : a_streams_)
      {
        AudioStream stream;
        stream.url = item["url"].get<std::string>();
        stream.format = item["format"].get<std::string>();
        stream.quality = item["quality"].get<std::string>();
        stream.mimeType = item["mimeType"].get<std::string>();
        if (item["codec"] != nullptr)
        {
          stream.codec = new std::string(item["codec"].get<std::string>());
        }
        if (item["audioTrackId"] != nullptr)
        {
          stream.audioTrackId = new uint(item["audioTrackId"].get<uint>());
        }
        if (item["audioTrackType"] != nullptr)
        {
          stream.audioTrackType = new std::string(item["audioTrackType"].get<std::string>());
        }
        if (item["audioTrackLocale"] != nullptr)
        {
          stream.audioTrackLocale = new std::string(item["audioTrackLocale"].get<std::string>());
        }
        stream.videoOnly = item["videoOnly"].get<bool>();
        stream.itag = item["itag"].get<uint>();
        stream.bitrate = item["bitrate"].get<uint>();
        stream.initStart = item["initStart"].get<uint>();
        stream.initEnd = item["initEnd"].get<uint>();
        stream.indexStart = item["indexStart"].get<uint>();
        stream.indexEnd = item["indexEnd"].get<uint>();
        stream.width = item["width"].get<uint>();
        stream.height = item["height"].get<uint>();
        stream.fps = item["fps"].get<uint>();
        stream.contentLength = item["contentLength"].get<int>();
        audio_streams_.push_back(stream);
      }
      response.audioStreams = audio_streams_;
      // End audiostreams

      // Videostreams ==========================================================
      std::vector<VideoStream> video_streams_{};
      const auto v_streams_ = j["videoStreams"].get<json::array_t>();
      for (const auto &item : v_streams_)
      {
        VideoStream stream;
        stream.url = item["url"].get<std::string>();
        stream.format = item["format"].get<std::string>();
        stream.quality = item["quality"].get<std::string>();
        stream.mimeType = item["mimeType"].get<std::string>();
        if (item["codec"] != nullptr)
        {
          stream.codec = new std::string(item["codec"].get<std::string>());
        }
        if (item["audioTrackId"] != nullptr)
        {
          stream.audioTrackId = new uint(item["audioTrackId"].get<uint>());
        }
        if (item["audioTrackType"] != nullptr)
        {
          stream.audioTrackType = new std::string(item["audioTrackType"].get<std::string>());
        }
        if (item["audioTrackLocale"] != nullptr)
        {
          stream.audioTrackLocale = new std::string(item["audioTrackLocale"].get<std::string>());
        }
        stream.videoOnly = item["videoOnly"].get<bool>();
        stream.itag = item["itag"].get<uint>();
        stream.bitrate = item["bitrate"].get<uint>();
        stream.initStart = item["initStart"].get<uint>();
        stream.initEnd = item["initEnd"].get<uint>();
        stream.indexStart = item["indexStart"].get<uint>();
        stream.indexEnd = item["indexEnd"].get<uint>();
        stream.width = item["width"].get<uint>();
        stream.height = item["height"].get<uint>();
        stream.fps = item["fps"].get<uint>();
        stream.contentLength = item["contentLength"].get<int>();
        video_streams_.push_back(stream);
      }
      response.videoStreams = video_streams_;
      // End videostreams

      // Related streams =======================================================
      std::vector<RelatedStream> related_streams_{};
      const auto r_streams = j["relatedStreams"].get<json::array_t>();
      for (const auto &item : r_streams)
      {
        RelatedStream stream;
        stream.url = item["url"].get<std::string>();
        stream.type = item["type"].get<std::string>();
        stream.title = item["title"].get<std::string>();
        stream.thumbnail = item["thumbnail"].get<std::string>();
        stream.uploaderName = item["uploaderName"].get<std::string>();
        stream.uploaderUrl = item["uploaderUrl"].get<std::string>();
        stream.uploaderAvatar = item["uploaderAvatar"].get<std::string>();
        stream.uploadedDate = item["uploadedDate"].get<std::string>();
        if (item["shortDescription"] != nullptr)
        {
          stream.shortDescription = new std::string(item["shortDescription"].get<std::string>());
        }
        stream.duration = item["duration"].get<uint>();
        stream.views = item["views"].get<uint>();
        stream.uploaded = item["uploaded"].get<uint>();
        stream.uploaderVerified = item["uploaderVerified"].get<bool>();
        stream.isShort = item["isShort"].get<bool>();
        related_streams_.push_back(stream);
      }
      response.relatedStreams = related_streams_;
      // End Related streams

      // Subtitles =============================================================
      std::vector<Subtitle> subtitles_{};
      const auto subt_ = j["subtitles"].get<json::array_t>();
      for (const auto &item : subt_)
      {
        Subtitle subtitle;
        subtitle.url = item["url"].get<std::string>();
        subtitle.mimeType = item["mimeType"].get<std::string>();
        subtitle.name = item["name"].get<std::string>();
        subtitle.code = item["code"].get<std::string>();
        subtitle.autoGenerated = item["autoGenerated"].get<bool>();
        subtitles_.push_back(subtitle);
      }
      response.subtitles = subtitles_;
      // End Subtitles

      // Preview frames
      std::vector<PreviewFrame> previewFrames{};
      const auto prev_frames_ = j["previewFrames"].get<json::array_t>();
      for (const auto &item : prev_frames_)
      {
        PreviewFrame frame;
        const auto urls_ = item["urls"].get<json::array_t>();
        for (const auto &url : urls_)
        {
          frame.urls.push_back(url);
        }
        frame.frameWidth = item["frameWidth"].get<uint>();
        frame.frameHeight = item["frameHeight"].get<uint>();
        frame.totalCount = item["totalCount"].get<uint>();
        frame.durationPerFrame = item["durationPerFrame"].get<uint>();
        frame.framesPerPageX = item["framesPerPageX"].get<uint>();
        frame.framesPerPageY = item["framesPerPageY"].get<uint>();
        previewFrames.push_back(frame);
      }
      response.previewFrames = previewFrames;
      // End preview frames
    }

  public:
    struct Response
    {
      std::string title;
      std::string description;
      std::string uploadDate;
      std::string uploader;
      std::string uploaderUrl;
      std::string uploaderAvatar;
      std::string thumbnailUrl;
      std::string hls;
      std::string *dash = nullptr;
      std::string *lbryId = nullptr;
      std::string category;
      std::string license;
      std::string visibility;
      std::vector<std::string> tags;
      std::vector<std::string> metaInfo;
      std::string uploaderVerified;
      uint duration;
      uint views;
      uint likes;
      uint dislikes;
      uint uploaderSubscriberCount;
      std::vector<AudioStream> audioStreams;
      std::vector<VideoStream> videoStreams;
      std::vector<RelatedStream> relatedStreams;
      std::vector<Subtitle> subtitles;
      bool livestream;
      std::string proxyUrl;
      std::vector<std::string> chapters;
      std::vector<PreviewFrame> previewFrames;
    } response;

    struct Stream
    {
      std::string url;
      std::string format;
      std::string quality;
      std::string mimeType;
      std::string *codec = nullptr;
      uint *audioTrackId = nullptr;
      std::string *audioTrackName = nullptr;
      std::string *audioTrackType = nullptr;
      std::string *audioTrackLocale = nullptr;
      bool videoOnly;
      uint itag;
      uint bitrate;
      uint initStart;
      uint initEnd;
      uint indexStart;
      uint indexEnd;
      uint width;
      uint height;
      uint fps;
      int contentLength;
    };

    struct AudioStream : public Stream
    {
    };

    struct VideoStream : public Stream
    {
    };

    struct RelatedStream
    {
      std::string url;
      std::string type;
      std::string title;
      std::string thumbnail;
      std::string uploaderName;
      std::string uploaderUrl;
      std::string uploaderAvatar;
      std::string uploadedDate;
      std::string *shortDescription = nullptr;
      uint duration;
      uint views;
      uint uploaded;
      bool uploaderVerified;
      bool isShort;
    };

    struct Subtitle
    {
      std::string url;
      std::string mimeType;
      std::string name;
      std::string code;
      bool autoGenerated;
    };

    struct PreviewFrame
    {
      std::vector<std::string> urls;
      uint frameWidth;
      uint frameHeight;
      uint totalCount;
      uint durationPerFrame;
      uint framesPerPageX;
      uint framesPerPageY;
    };

  private:
    using json = nlohmann::json;
    Error error;
  };
} // namespace piped
