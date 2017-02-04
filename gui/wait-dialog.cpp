#include "wait-dialog.h"

#include <QLabel>
#include <QVBoxLayout>

WaitDialog::WaitDialog(QWidget *parent)
    : QDialog(parent),
      text_label_(new QLabel),
      animation_label_(new QLabel),
      timer_(new QTimer(this)) {
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  auto layout = new QVBoxLayout();
  setLayout(layout);

  connect(timer_, &QTimer::timeout, this, &WaitDialog::Animate);
  timer_->setInterval(150);

  animation_label_->setAlignment(Qt::AlignCenter);

  layout->addWidget(text_label_);
  layout->addWidget(animation_label_);
  layout->setContentsMargins(10, 10, 10, 10);
}

void WaitDialog::ShowDialog(const QString &text) {
  if (timer_->isActive()) {
    timer_->start();
  }
  text_label_->setText(text);
  timer_->start();
  open();
}

void WaitDialog::closeEvent(QCloseEvent *event) {
  if (timer_->isActive()) {
    timer_->start();
  }
  QDialog::closeEvent(event);
}

void WaitDialog::Animate() {
  int kMaxSize = text_label_->text().size();
  int size = animation_label_->text().size();
  if (size >= kMaxSize) {
    animation_label_->setText(".");
    return;
  }
  animation_label_->setText(animation_label_->text() + ".");
}
