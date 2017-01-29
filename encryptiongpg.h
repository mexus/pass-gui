#ifndef ENCRYPTIONGPG_H
#define ENCRYPTIONGPG_H

#include <exception>

#include <QString>

class GpgDecryptionFailed : public std::runtime_error {
 public:
  GpgDecryptionFailed(int exit_code, const QByteArray &error_output);
};

QString GpgDecryptFileFirstLine(const QString &path);

#endif  // ENCRYPTIONGPG_H
