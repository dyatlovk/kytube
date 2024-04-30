#pragma once

#include <string>

namespace players
{
  class mpv
  {
  public:
    mpv();

    ~mpv();

    auto SetUrl(const char *_url) -> mpv *;

    auto SetFullScreen(bool val = false) -> mpv *;

    auto SetVolume(int val = 50) -> mpv *;

    auto SetQuiet(bool val = true) -> mpv *
    {
      this->_quite = val;

      return this;
    }

    auto BuildCmd() -> void;

    auto Run() -> bool;

    auto Handler(const char *_url) -> void;

  private:
    // video url
    const char *url;

    // run in full screen
    bool _fs;

    int vol;

    // Make console output less verbose
    bool _quite;

    std::string _cmd;
  };
} // namespace players
