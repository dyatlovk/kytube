#pragma once

#include <string>

namespace core::sys
{
  enum class Shell
  {
    Sh,
    Zsh,
    Bash,
    Fish,
  };

  static auto getShellCmd() -> std::string
  {
    FILE *stream = popen("echo $SHELL", "r");
    std::string result = "";
    char buffer[128];
    while (fgets(buffer, sizeof buffer, stream) != NULL)
    {
      result += buffer;
    }
    pclose(stream);
    return result;
  }

  static auto getShell(const std::string &shell) -> Shell
  {
    if (shell.find("zsh") != std::string::npos)
    {
      return Shell::Zsh;
    }

    if (shell.find("bash") != std::string::npos)
    {
      return Shell::Bash;
    }

    if (shell.find("fish") != std::string::npos)
    {
      return Shell::Fish;
    }

    if (shell.find("sh") != std::string::npos)
    {
      return Shell::Sh;
    }

    return Shell::Sh;
  }

  static auto getShellDetachSymb(const Shell shell) -> std::string
  {
    switch (shell)
    {
    case Shell::Sh:
      return "&";
    case Shell::Zsh:
      return "&!";
    case Shell::Bash:
      return "&";
    case Shell::Fish:
      return "&";
    default:
      return "&";
    }
  }
} // namespace core::sys
