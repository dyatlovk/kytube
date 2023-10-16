#include "ViewHistory.h"

ViewHistory::ViewHistory(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::Views)
{
  ui->setupUi(this);
}

ViewHistory::~ViewHistory()
{
  delete ui;
}
