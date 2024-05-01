#include "core/providers/piped/Stream.h"

#include "core/network/request.hpp"
#include "stream.hpp"

namespace models
{
  stream::stream(QObject *parent)
      : QObject(parent)
  {
  }

  stream::~stream() = default;

  auto stream::Search(const std::string &url) -> void
  {
    const auto request = new network::request;
    auto response = request->Get(url);
    delete request;

    const auto streamProvider = new piped::stream();
    streamProvider->Parse(response);
    response_ = streamProvider->GetParsedData();
    delete streamProvider;
    emit searchComplete();
  }

  auto stream::SearchAsync(const std::string &url) -> void
  {
    auto futureResult = std::async(std::launch::async, &stream::Search, this, url);
    asyncResult = std::move(futureResult);
  }
} // namespace models
