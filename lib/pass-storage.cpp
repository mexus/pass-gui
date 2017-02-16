#include "pass-storage.h"

#include <vector>

#include <QDir>
#include <QDirIterator>

static const QString kGpgSuffix(".gpg");

static std::vector<QString> LoadDirectory(const QString &path) {
  std::vector<QString> result;
  QDirIterator it(path, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString name = it.next();
    // We only list files.
    if (!it.fileInfo().isFile()) {
      continue;
    }
    // Ignore non-gpg files.
    if (!name.endsWith(kGpgSuffix)) {
      continue;
    }
    // Strip the path
    name = name.remove(0, path.size());
    // Ignore "hidden" files.
    if (name.startsWith('.')) {
      continue;
    }
    // Strip the ".gpg" suffix.
    name = name.left(name.size() - kGpgSuffix.size());
    result.push_back(std::move(name));
  }
  return result;
}

static QString NormalizePath(const QString &path) {
  // Replace all windows path delimiters with unix ones and add a slash to the
  // end.
  if (!QDir(path).exists()) {
    throw std::runtime_error(
        QString("Path [" + path + "] doesn't exist!").toStdString());
  }
  return QDir(path).canonicalPath().replace('\\', '/') + "/";
}

PassStorage::PassStorage(const QString &path)
    : starting_path_(NormalizePath(path)),
      all_items_(LoadDirectory(starting_path_)) {}

bool PassStorage::FuzzyMatch(const QString &str, const QString &filter) {
  int current_str = 0;
  for (QChar c : filter) {
    if (current_str >= str.size()) {
      return false;
    }
    int index = str.indexOf(
        c, current_str, c.isLower() ? Qt::CaseInsensitive : Qt::CaseSensitive);
    if (index == -1) {
      return false;
    }
    current_str = index + 1;
  }
  return true;
}

std::vector<QString> PassStorage::GetItems(const QString &filter) const {
  std::vector<QString> result;
  for (const QString &str : all_items_) {
    if (FuzzyMatch(str, filter)) {
      result.push_back(str);
    }
  }
  return result;
}

QString PassStorage::GetFileName(const QString &item) const {
  return starting_path_ + item + kGpgSuffix;
}

bool PassStorage::IsEmpty() const { return all_items_.empty(); }
