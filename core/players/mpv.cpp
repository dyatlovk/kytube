#include "mpv.hpp"

#include "../sys/shell.hpp"

namespace players
{
  mpv::mpv()
      : url("")
      , _fs(false)
      , vol(50)
      , _quite(true)
      , _cmd("mpv")
  {
  }

  mpv::~mpv() = default;

  auto mpv::SetUrl(const char *_url) -> mpv *
  {
    this->url = _url;
    return this;
  };

  auto mpv::SetFullScreen(bool val) -> mpv *
  {
    this->_fs = val;
    return this;
  }

  auto mpv::SetVolume(int val) -> mpv *
  {
    this->vol = val;
    return this;
  }

  auto mpv::BuildCmd() -> void
  {
    _cmd.append(" " + std::string(url));
    if (_fs)
      _cmd.append(" --fullscreen");

    if (_quite)
      _cmd.append(" --no-terminal");

    _cmd.append(" --volume=" + std::to_string(vol));
    const auto shellCmd = core::sys::getShellCmd();
    const auto shell = core::sys::getShell(shellCmd);
    const auto detachSymb = core::sys::getShellDetachSymb(shell);
    _cmd.append(" " + detachSymb);
  }

  auto mpv::Run() -> bool
  {
    if (_cmd.length() == 0)
    {
      return false;
    }

    int result = system(_cmd.c_str());

    return true;
  }

  auto mpv::Handler(const char *_url) -> void
  {
    SetUrl(_url);
    BuildCmd();
    Run();
  }
} // namespace players
