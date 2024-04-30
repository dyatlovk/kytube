#include "stream.hpp"

namespace TestQt
{
  StreamDialog::StreamDialog(QDialog *parent)
      : QDialog(parent)
      , ui(new Ui::Stream)
  {
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::released, this, &StreamDialog::Close);
  }

  StreamDialog::~StreamDialog()
  {
    delete ui;
  }

  auto StreamDialog::Close() -> void
  {
    this->close();
  }
} // namespace TestQt
