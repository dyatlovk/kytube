#pragma once

#include <QKeyEvent>
#include <QLineEdit>
#include <future>
#include <qabstractitemview.h>
#include <qcompleter.h>
#include <qstringlistmodel.h>

class SearchInputTest : public QLineEdit
{
  Q_OBJECT

private:
  const int AUTOCOMPLETE_SYMB_COUNT_DEFAULT_TRIGGER = 3;

public:
  SearchInputTest(QWidget *parent = nullptr)
      : QLineEdit(parent)
      , completer(new QCompleter(this))
      , autocompleteSymbolsCountTrigger(AUTOCOMPLETE_SYMB_COUNT_DEFAULT_TRIGGER)
  {
    completer->setCompletionMode(QCompleter::PopupCompletion);
    connect(this, &QLineEdit::textChanged, this, &SearchInputTest::textChanged);
    connect(this, &SearchInputTest::searchFinish, this,
        [this](const QStringList &completions)
        {
          completer->setModel(new QStringListModel(completions, completer));
          setCompleter(completer);
          completer->complete();
        });
  }

  ~SearchInputTest() { delete completer; }

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
    QStringList wordList;
    wordList << "apple" << "banana" << "cherry" << "date" << "elderberry" << "fig" << "grape" << "cherries";
    QStringList completions = wordList.filter(query, Qt::CaseInsensitive);
    emit searchFinish(completions);
  }

  auto searchAsync(const QString &query) -> void
  {
    auto futureResult = std::async(std::launch::async, &SearchInputTest::search, this, query);
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
