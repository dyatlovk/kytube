#pragma once

#include <QKeyEvent>
#include <QLineEdit>
#include <future>
#include <iostream>
#include <qabstractitemview.h>
#include <qcompleter.h>
#include <qsettings.h>
#include <qstringlistmodel.h>

#include <core/network/request.hpp>

#include "core/providers/piped/Suggestions.h"

class SearchInput : public QLineEdit
{
  Q_OBJECT

private:
  const int AUTOCOMPLETE_SYMB_COUNT_DEFAULT_TRIGGER = 3;

public:
  SearchInput(QWidget *parent = nullptr)
      : QLineEdit(parent)
      , completer(new QCompleter(this))
      , autocompleteSymbolsCountTrigger(AUTOCOMPLETE_SYMB_COUNT_DEFAULT_TRIGGER)
  {
    completer->setCompletionMode(QCompleter::PopupCompletion);
    connect(this, &QLineEdit::textChanged, this, &SearchInput::textChanged);
    connect(this, &SearchInput::searchFinish, this,
        [this](const QStringList &completions)
        {
          completer->setModel(new QStringListModel(completions, completer));
          setCompleter(completer);
          completer->complete();
        });
  }

  ~SearchInput() { delete completer; }

  void keyPressEvent(QKeyEvent *event) override
  {
    if (!hasFocus())
      return;
    if (event->key() == Qt::Key_Return)
    {
      emit queryEnter();
    }
    QLineEdit::keyPressEvent(event);
  }

private slots:
  void textChanged(const QString &text)
  {
    if (completer->popup()->isVisible())
      return;

    const auto symbolCount = this->text().length();

    if (symbolCount >= 3)
    {
      this->searchAsync(this->text());
    }
    else
    {
      completer->setModel(nullptr);
    }
  }

private:
  auto search(const QString &query) -> void
  {
    const auto q = network::request::UrlEncode(query.toStdString());
    const auto request = new network::request();
    const auto searchProvider = new piped::suggestions;
    const auto settings = new QSettings(DOMAIN_NAME, CONFIG_NAME);
    std::string url = settings->value("Piped/apiUrl").toString().toStdString() + "/opensearch/suggestions?query=";
    url.append(q);
    const auto response = request->Get(url);

    try
    {
      searchProvider->Parse(response);
    }
    catch (piped::Exception &e)
    {
      std::cout << e.what() << std::endl;
      return;
    }

    const auto suggestions = searchProvider->getParsedData().suggestions;

    QStringList wordList;
    for (const auto &item : suggestions)
    {
      wordList << item.c_str();
    }
    QStringList completions = wordList.filter(query, Qt::CaseInsensitive);
    emit searchFinish(completions);

    delete settings;
    delete searchProvider;
    delete request;
  }

  auto searchAsync(const QString &query) -> void
  {
    auto futureResult = std::async(std::launch::async, &SearchInput::search, this, query);
    asyncResultSearch = std::move(futureResult);
  }

private:
  QCompleter *completer;
  std::future<void> asyncResultSearch;
  int autocompleteSymbolsCountTrigger;

signals:
  void queryEnter();
  void searchFinish(const QStringList &completions);
};
