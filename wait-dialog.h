#ifndef WAITFORGPGDIALOGUE_H
#define WAITFORGPGDIALOGUE_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

class WaitDialog : public QDialog {
  Q_OBJECT
 public:
  WaitDialog(QWidget *parent);
  void ShowDialog(const QString &text);

 private:
  QLabel *text_label_;
  QLabel *animation_label_;
  QTimer *timer_;

  void closeEvent(QCloseEvent *event) override;

  void Animate();
};

#endif  // WAITFORGPGDIALOGUE_H
