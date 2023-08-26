#include "AboutDialog.h"

AboutDialog::AboutDialog(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::About)
{
  ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
  delete ui;
}

auto AboutDialog::Close() -> void
{
  this->close();
}

auto AboutDialog::Show() -> void
{
  this->show();
}
