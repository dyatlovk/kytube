#pragma once

#include <QtWidgets>
#include <iostream>
#include <thread>

namespace TestQt
{
  class Worker : public QObject
  {
    Q_OBJECT

  public:
    Worker();

    void startAsyncTask()
    {
      // Запуск асинхронной операции с std::async
      auto futureResult = std::async(std::launch::async, &Worker::doAsyncTask, this);
      asyncResult = std::move(futureResult);
      std::cout << "start async" << std::endl;
    }

  signals:
    void textReady(const QString &text);

  private:
    std::future<void> asyncResult;

    void doAsyncTask()
    {
      // Симуляция долгой операции
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::string result = "Async task completed!";
      QString qResult = QString::fromStdString(result);
      emit textReady(qResult);
    }
  };
} // namespace TestQt
