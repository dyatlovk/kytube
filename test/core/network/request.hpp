#pragma once

#include <core/network/request.hpp>

#include "../../mtTestSuite.h"

namespace TestNetwork
{
  inline TEST_CASE(Get, {
    auto request = new network::request();
    auto result = request->Get("https://httpbin.org/base64/b2s=");
    ASSERT_EQ_CHAR(result.c_str(), "ok", "Get");
    delete request;
  })

  inline TEST_CASE(UrlEncode, {
    auto request = new network::request();
    auto url = network::request::UrlEncode("test");
    ASSERT_EQ_CHAR(url.c_str(), "test", "Encode");
    delete request;
  })

  inline TEST_CASE(UrlDecode, {
    auto url = network::request::UrlDecode("test");
    ASSERT_EQ_CHAR(url.c_str(), "test", "Decode");
  })

  inline TEST_CASE(UrlEncodeCyrrylic, {
    auto request = new network::request();
    auto url = network::request::UrlEncode("тест");
    ASSERT_EQ_CHAR(url.c_str(), "%D1%82%D0%B5%D1%81%D1%82", "Encode Cyryllic");
    delete request;
  })

  inline TEST_CASE(UrlDecodeCyrrylic, {
    auto request = new network::request();
    auto url = network::request::UrlDecode("%D1%82%D0%B5%D1%81%D1%82");
    ASSERT_EQ_CHAR(url.c_str(), "тест", "Decode Cyryllic");
    delete request;
  })
} // namespace TestNetwork
