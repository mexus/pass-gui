#include "windows-keyboard.h"

#include <exception>
#include <memory>
#include <thread>
#include <vector>

#include <Windows.h>

#include <QTextCodec>

static INPUT InputFromSymbol(DWORD symbol) {
  INPUT input = {};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = 0;
  input.ki.wScan = symbol;
  input.ki.dwFlags = KEYEVENTF_UNICODE;
  input.ki.time = 0;
  input.ki.dwExtraInfo = 0;

  return input;
}

static void TypeCharacter(DWORD symbol, DWORD additional_flags = 0) {
  INPUT input = InputFromSymbol(symbol);
  input.ki.dwFlags |= additional_flags;
  if (SendInput(1, &input, sizeof(INPUT)) == 0) {
    DWORD error = GetLastError();
    throw std::runtime_error("Can't sent an input, error number is #" +
                             std::to_string(error));
  }
}

static std::vector<DWORD> StringToUtf16(const QString &string) {
  QTextCodec *codec = QTextCodec::codecForName("UTF-16");
  if (codec == nullptr) {
    throw std::runtime_error("Can't get access to a UTF-16 codec");
  }
  std::unique_ptr<QTextEncoder> encoder(
      codec->makeEncoder(QTextCodec::IgnoreHeader));
  if (encoder == nullptr) {
    throw std::runtime_error("Can't create an encoder");
  }
  QByteArray utf16_array = encoder->fromUnicode(string);

  std::vector<DWORD> result;
  for (int i = 0; i < utf16_array.size() / 2; ++i) {
    DWORD first = utf16_array[2 * i];
    DWORD second = utf16_array[2 * i + 1];
    result.push_back((second << 8) + first);
  }
  return result;
}

void WindowsKeyboard::TypeString(const QString &string) {
  std::vector<DWORD> utf16_string = StringToUtf16(string);

  for (DWORD symbol : utf16_string) {
    TypeCharacter(symbol);
    TypeCharacter(symbol, KEYEVENTF_KEYUP);
  }
}
