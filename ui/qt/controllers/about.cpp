#include "about.hpp"

#include "Config.hpp"

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

  auto About::SetContent() -> void
  {
    const std::string version(YoutubeQt::VERSION);
    const std::string app(CONFIG_NAME);
    const std::string ver = "Version " + version;
    ui->programVersion->setText(QString(ver.c_str()));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("YouTubeQt - About");
  }
} // namespace ui
