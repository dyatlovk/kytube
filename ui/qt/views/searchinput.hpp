#pragma once

#include <QKeyEvent>
#include <QLineEdit>

class SearchInput : public QLineEdit
{
  Q_OBJECT

public:
  SearchInput(QWidget *parent = nullptr)
      : QLineEdit(parent)
  {
  }
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

signals:
  void queryEnter();
};
