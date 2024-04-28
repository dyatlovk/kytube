#include "log.hpp"

namespace ui
{
  Log::Log(QDialog *parent)
      : QDialog(parent)
      , ui(new Ui::Log)
  {
    ui->setupUi(this);
  }

  Log::~Log()
  {
    delete ui;
  }

  auto Log::hideEvent(QHideEvent *event) -> void
  {
    onClose();
  }

  auto Log::OnClose(std::function<void()> callback) -> void
  {
    onClose = callback;
  }

  auto Log::Close() -> void
  {
    close();
  }
} // namespace ui
