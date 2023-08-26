#include "Log.h"

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

auto Log::Add(const LogFields &field) -> void
{
  m_logs.push_back(field);
}
