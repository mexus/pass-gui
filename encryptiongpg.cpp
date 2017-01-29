#include "encryptiongpg.h"

#include <QProcess>

GpgDecryptionFailed::GpgDecryptionFailed(int exit_code,
                                         const QByteArray &error_output)
    : std::runtime_error("GPG2 exited with a non-zero exit code (" +
                         std::to_string(exit_code) + "), error message:\n" +
                         error_output.constData()) {}

QString GpgDecryptFileFirstLine(const QString &path) {
  QProcess gpg;
  gpg.start("gpg2", QStringList() << "--decrypt"
                                  << "-q" << path);
  if (!gpg.waitForStarted()) {
    throw std::runtime_error("Failed to launch gpg!");
  }

  if (!gpg.waitForFinished()) {
    throw std::runtime_error("gpg2 has failed");
  }

  int exit_code = gpg.exitCode();
  if (exit_code != 0) {
    throw GpgDecryptionFailed(exit_code, gpg.readAllStandardError());
  }
  QByteArray output = gpg.readLine();
  return QString::fromUtf8(output);
}
