#ifndef WINDOWSKEYBOARD_H
#define WINDOWSKEYBOARD_H

#include <lib/keyboard-interface.h>

class WindowsKeyboard : public KeyboardInterface {
 public:
  void TypeString(const QString &string) override;
};

#endif  // WINDOWSKEYBOARD_H
