#include "about.hpp"

namespace ui
{
  About::About(QDialog *parent)
      : QDialog(parent)
      , ui(new Ui::About)
  {
    ui->setupUi(this);
  }

  About::~About()
  {
    delete ui;
  }

  auto About::Close() -> void
  {
    this->close();
  }

  auto About::Show() -> void
  {
    this->show();
  }
} // namespace ui
