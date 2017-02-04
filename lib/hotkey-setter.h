#ifndef HOTKEYSETTER_H
#define HOTKEYSETTER_H

#include <memory>

#include <QWidget>

class HotkeySetter {
 public:
  HotkeySetter(const QWidget *widget);
  ~HotkeySetter();

  void SetHotkey();
  void UnsetHotkey();

  bool CheckHotkey(void *native_event_message);

 private:
  struct Data;
  std::unique_ptr<Data> data_;
};

#endif  // HOTKEYSETTER_H
