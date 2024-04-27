#include "history.hpp"

namespace ui
{
  History::History(QDialog *parent)
      : QDialog(parent)
      , ui(new Ui::History)
  {
    ui->setupUi(this);
  }

  History::~History()
  {
    delete ui;
  }

  auto History::Close() -> void
  {
    this->close();
  }

  auto History::Show() -> void
  {
    this->show();
  }
} // namespace ui
