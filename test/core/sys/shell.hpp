#pragma once

#include <core/sys/shell.hpp>

#include "../../mtTestSuite.h"

namespace TestCore
{
  inline TEST_CASE(getShellCmd, {
    const auto result = core::sys::getShellCmd();
    ASSERT_TRUE(result.size() > 0, "Shell found");
  });

  inline TEST_CASE(getShell, {
    const auto bash = core::sys::getShell("/bin/bash");
    ASSERT_EQ_INT((int)bash, 2, "Bash");

    const auto zsh = core::sys::getShell("-zsh");
    ASSERT_EQ_INT((int)zsh, 1, "Zsh");

    const auto fish = core::sys::getShell("/usr/bin/fish");
    ASSERT_EQ_INT((int)fish, 3, "Fish");

    const auto sh = core::sys::getShell("/bin/sh");
    ASSERT_EQ_INT((int)sh, 0, "Sh");

    const auto common = core::sys::getShell("undefined cmd");
    ASSERT_EQ_INT((int)common, 0, "Sh default");
  });

  inline auto Run() -> void
  {
    getShellCmd();
    getShell();
  }
} // namespace TestCore
