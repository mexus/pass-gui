#include <lib/encryptiongpg.h>

#include <QProcess>

#include <Windows.h>

static void OverwriteQString(QString *str) {
  int size = str->size();
  SecureZeroMemory(reinterpret_cast<void *>(str->data()), size * sizeof(QChar));
}

GpgDecryptionResult::GpgDecryptionResult(const QString &name) : name_(name) {}

QString GpgDecryptionResult::GetName() const { return name_; }

GpgDecryptionError::GpgDecryptionError(const QString &name,
                                       const QString &error)
    : GpgDecryptionResult(name), error_(error) {}

QString GpgDecryptionError::GetError() const { return error_; }

GpgDecryptionSuccess::GpgDecryptionSuccess(const QString &name,
                                           QString &&result)
    : GpgDecryptionResult(name), result_(std::move(result)) {}

const QString &GpgDecryptionSuccess::GetResult() const { return result_; }

void GpgDecryptionSuccess::ClearResult() { OverwriteQString(&result_); }

std::shared_ptr<GpgDecryptionResult> GpgDecryptFileFirstLine(
    const QString &name, const QString &path) {
  QProcess gpg;
  gpg.start("gpg2", QStringList() << "--decrypt"
                                  << "-q" << path);
  if (!gpg.waitForStarted()) {
    return std::make_shared<GpgDecryptionError>(name, "Failed to launch gpg!");
  }

  if (!gpg.waitForFinished()) {
    return std::make_shared<GpgDecryptionError>(name, "gpg2 has failed");
  }

  int exit_code = gpg.exitCode();
  if (exit_code != 0) {
    return std::make_shared<GpgDecryptionError>(
        name, "GPG2 exited with a non-zero exit code (" +
                  QString::number(exit_code) + "), error message:\n" +
                  QString::fromUtf8(gpg.readAllStandardError()));
  }
  QString output = gpg.readLine();
  return std::make_shared<GpgDecryptionSuccess>(name, std::move(output));
}
