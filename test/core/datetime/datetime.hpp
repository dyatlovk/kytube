#pragma once

#include <core/datetime/datetime.hpp>

#include "../../mtTestSuite.h"

namespace TestDateTime
{
  TEST_CASE(Format, {
    auto service = new core::datetime;
    auto now = service->Now("%Y-%m-%d %H:%M:%S");
    ASSERT_TRUE(!now.empty(), "Format");
    delete service;
  });
} // namespace TestDateTime
