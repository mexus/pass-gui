#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <encryptiongpg.h>
#include <keyboard-interface.h>
#include <pass-storage.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private:
  /// Qt-generated UI objects.
  Ui::MainWindow *ui;

  /// The password storage.
  PassStorage pass_storage_;

  /// An interface to type a text.
  std::unique_ptr<KeyboardInterface> keyboard_;

  /// QT system tray icon. Simple as that.
  QSystemTrayIcon *systray_icon_;

  /// A "quit" action.
  QAction *quit_action_;

  /// Path of a passwords storage.
  QString passwords_path_;

  /// Apply the current filter string to the password tree.
  void UpdateFiltering(const QString &filter);

  /// Bring the window to the front on the tray click. See also Activate().
  void ActivateOnTray(QSystemTrayIcon::ActivationReason reason);

  /// Bring the window to the front.
  void Activate();

  /// Handle minimize event.
  void changeEvent(QEvent *e) override;

  /// Filtering events. See also FilterLineKeyEventFilter(),
  /// ResultsListKeyEventFilter().
  bool eventFilter(QObject *obj, QEvent *event) override;

  /// Handling the hotkey press event on Windows.
  bool nativeEvent(const QByteArray &eventType, void *message,
                   long *result) override;

  /// You sure to exit?.. :)
  void closeEvent(QCloseEvent *event) override;

  /// Filter out keyboard events on the "filter line".
  bool FilterLineKeyEventFilter(QKeyEvent *event);

  /// Filter out keyboard event on the "results list".
  bool ResultsListKeyEventFilter(QKeyEvent *event);

  /// Type the current password.
  void TypeCurrentPassword();

  /// Initializes systray icon context menu.
  void InitTrayIconContextMenu();

  /// A dialogue "sure to exit?".
  bool SureToExit();

  /// Reloads passwords tree. In case of failure the loaded tree remains the
  /// same.
  void ReloadTree();

  /// Asks a user to give a new password storage path.
  void ChangeStoragePath();
};

#endif  // MAINWINDOW_H
