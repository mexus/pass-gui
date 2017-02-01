#ifndef WAITFORGPGDIALOGUE_H
#define WAITFORGPGDIALOGUE_H

#include <QDialog>

class WaitDialog : public QDialog {
  Q_OBJECT
 public:
  WaitDialog(QWidget *parent, const QString &text);
};

#endif  // WAITFORGPGDIALOGUE_H
