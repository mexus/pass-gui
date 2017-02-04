#ifndef KEYBOARDINTERFACE_H
#define KEYBOARDINTERFACE_H

#include <QString>

class KeyboardInterface {
 public:
  virtual ~KeyboardInterface() = default;
  virtual void TypeString(const QString &string) = 0;
};

#endif  // KEYBOARDINTERFACE_H
