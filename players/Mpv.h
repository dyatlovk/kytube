#pragma once

#include <string>

namespace Players
{
  class Mpv
  {
  public:
    Mpv()
        : url("")
        , _fs(false)
        , vol(50)
        , _quite(true)
        , _cmd("mpv")
    {
    }

    ~Mpv() = default;

    auto SetUrl(const char *_url) -> Mpv *
    {
      this->url = _url;
      return this;
    };

    auto SetFullScreen(bool val = false) -> Mpv *
    {
      this->_fs = val;

      return this;
    }

    auto SetVolume(int val = 50) -> Mpv *
    {
      this->vol = val;

      return this;
    }

    auto SetQuiet(bool val = true) -> Mpv *
    {
      this->_quite = val;

      return this;
    }

    auto BuildCmd() -> void
    {
      _cmd.append(" " + std::string(url));
      if (_fs)
        _cmd.append(" --fullscreen");

      if (_quite)
        _cmd.append(" --no-terminal");

      _cmd.append(" --volume=" + std::to_string(vol));
      _cmd.append(" &!");
    }

    auto Run() -> bool
    {
      if (_cmd.length() == 0)
      {
        return false;
      }

      int result = system(_cmd.c_str());

      return true;
    }

    auto Handler(const char *_url) -> void
    {
      SetUrl(_url);
      BuildCmd();
      Run();
    }

  private:
    // youtube url
    const char *url;

    // run in full screen
    bool _fs;

    int vol;

    // Make console output less verbose
    bool _quite;

    std::string _cmd;
  };
} // namespace Players
