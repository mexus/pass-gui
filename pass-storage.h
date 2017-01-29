#ifndef PASS_STORAGE_H
#define PASS_STORAGE_H

#include <vector>

#include <QString>

/// A class to store all the password files.
class PassStorage {
 public:
  /// Initializes an empty storage.
  PassStorage() = default;

  /// Initializes a tree from a given path on a file system.
  PassStorage(const QString &path);

  /// Copying the object is not permited.
  PassStorage(const PassStorage &other) = delete;

  /// Default move-constructor.
  PassStorage(PassStorage &&other) = default;

  /// Default move-assignment operator.
  PassStorage &operator=(PassStorage &&other) = default;

  /// Returns a filtered list of the password files.
  std::vector<QString> GetItems(const QString &filter) const;

  QString GetFileName(const QString &item) const;

  static bool FuzzyMatch(const QString &str, const QString &filter);

 private:
  QString starting_path_;
  std::vector<QString> all_items_;
};

#endif  // PASS_STORAGE_H
