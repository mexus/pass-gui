#include "wait-dialog.h"

#include <QLabel>
#include <QVBoxLayout>

WaitDialog::WaitDialog(QWidget *parent, const QString &text) : QDialog(parent) {
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  auto layout = new QVBoxLayout();
  setLayout(layout);

  auto label = new QLabel(text);
  layout->addWidget(label);
  layout->setContentsMargins(10, 10, 10, 10);
}
