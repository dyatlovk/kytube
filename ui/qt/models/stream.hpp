#pragma once

#include <future>
#include <qobject.h>
#include <string>

#include "core/providers/piped/Stream.h"

namespace models
{
  class stream : public QObject
  {
    Q_OBJECT
  public:
    explicit stream(QObject *parent = nullptr);

    ~stream();

    auto Search(const std::string &url) -> void;

    auto SearchAsync(const std::string &url) -> void;

    auto GetParsed() -> piped::stream::Response { return response_; }

  signals:
    void searchComplete();

  private:
    piped::stream::Response response_;
    std::future<void> asyncResult;
  };
} // namespace models
