#include <lib/hotkey-setter.h>

#include <Windows.h>

enum HotKeyId : int {
  /// Some random ID to check a WM_HOTKEY event.
  kBringUp = 0xE3,
};

struct HotkeySetter::Data {
  HWND win_id;
  bool key_set;
};

HotkeySetter::HotkeySetter(const QWidget *widget)
    : data_(new Data{reinterpret_cast<HWND>(widget->winId()), false}) {}

HotkeySetter::~HotkeySetter() {
  if (data_->key_set) {
    UnsetHotkey();
  }
}

void HotkeySetter::SetHotkey() {
  RegisterHotKey(data_->win_id, HotKeyId::kBringUp, MOD_ALT | MOD_NOREPEAT,
                 VK_F3);
  data_->key_set = true;
}

void HotkeySetter::UnsetHotkey() {
  UnregisterHotKey(data_->win_id, HotKeyId::kBringUp);
  data_->key_set = true;
}

bool HotkeySetter::CheckHotkey(void *native_event_message) {
  MSG *msg = reinterpret_cast<MSG *>(native_event_message);
  return msg->message == WM_HOTKEY && msg->wParam == HotKeyId::kBringUp;
}
