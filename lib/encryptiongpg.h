#ifndef ENCRYPTIONGPG_H
#define ENCRYPTIONGPG_H

#include <memory>

#include <QString>

class GpgDecryptionResult {
 public:
  virtual ~GpgDecryptionResult() = default;
  GpgDecryptionResult(const QString &name);

  QString GetName() const;

 private:
  const QString name_;
};

class GpgDecryptionError : public GpgDecryptionResult {
 public:
  GpgDecryptionError(const QString &name, const QString &error);
  virtual ~GpgDecryptionError() = default;

  QString GetError() const;

 private:
  const QString error_;
};

class GpgDecryptionSuccess : public GpgDecryptionResult {
 public:
  GpgDecryptionSuccess(const QString &name, const QString &result);

  const QString &GetResult() const;

  void ClearResult();

 private:
  QString result_;
};

std::shared_ptr<GpgDecryptionResult> GpgDecryptFileFirstLine(
    const QString &name, const QString &path);

#endif  // ENCRYPTIONGPG_H
