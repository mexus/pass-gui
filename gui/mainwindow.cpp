#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QtConcurrent>

#include <lib/hotkey-setter.h>

static const QString kSettingsFieldPasswordsPath = "passwords_path";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      pass_storage_(),
      systray_icon_(new QSystemTrayIcon(QIcon(":/gui/lock-logo.svg"), this)),
      quit_action_(new QAction("Quit", this)),
      hotkey_setter_(this),
      wait_dialog_(new WaitDialog(this)) {
  ui->setupUi(this);

  QSettings settings;
  passwords_path_ = settings.value(kSettingsFieldPasswordsPath).toString();

  // Removing a "Maximize" button.
  setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint |
                 Qt::WindowCloseButtonHint);

  connect(ui->filter_line, &QLineEdit::textEdited, this,
          &MainWindow::UpdateFiltering);
  connect(ui->results_list, &QListWidget::itemDoubleClicked, this,
          &MainWindow::TypeCurrentPassword);
  connect(systray_icon_, &QSystemTrayIcon::activated, this,
          &MainWindow::ActivateOnTray);
  connect(quit_action_, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionReload, &QAction::triggered, this, &MainWindow::ReloadTree);
  connect(ui->action_path_select, &QAction::triggered, this,
          &MainWindow::ChangeStoragePath);
  connect(&password_future_watcher_,
          &QFutureWatcher<std::shared_ptr<GpgDecryptionResult>>::finished, this,
          &MainWindow::TypePassword);

  // Handle key events for components:
  ui->filter_line->installEventFilter(this);
  ui->results_list->installEventFilter(this);

  systray_icon_->show();

  InitTrayIconContextMenu();

  ReloadTree();

  hotkey_setter_.SetHotkey();
}

void MainWindow::ActivateOnTray(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::DoubleClick:
    // pass
    case QSystemTrayIcon::Trigger:
      Activate();
      break;
    default:
      break;
  }
}

void MainWindow::Activate() {
  if (isHidden()) {
    show();
    setWindowState(Qt::WindowNoState);
    ui->filter_line->clear();
    UpdateFiltering(QString());
  }
  activateWindow();
  raise();
  ui->filter_line->setFocus();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::UpdateFiltering(const QString &filter) {
  if (pass_storage_.IsEmpty()) {
    ui->filter_line->setEnabled(false);
    ui->results_list->setEnabled(false);
    return;
  }
  ui->filter_line->setEnabled(true);
  ui->results_list->clear();
  auto items = pass_storage_.GetItems(filter);
  if (items.empty()) {
    ui->results_list->setEnabled(false);
    return;
  }
  ui->results_list->setEnabled(true);
  for (const QString &item : items) {
    ui->results_list->addItem(item);
  }
  ui->results_list->setCurrentRow(0);
}

void MainWindow::changeEvent(QEvent *e) {
  if (e->type() != QEvent::WindowStateChange) {
    return;
  }
  if (windowState() == Qt::WindowMinimized) {
    hide();
  }
}  //

bool MainWindow::FilterLineKeyEventFilter(QKeyEvent *event) {
  if (ui->results_list->count() == 0) {
    return false;
  }
  switch (event->key()) {
    case Qt::Key_Return:
      TypeCurrentPassword();
      return true;
    case Qt::Key_Down:
      ui->results_list->setFocus();
      QCoreApplication::sendEvent(ui->results_list, event);
      return true;
    default:
      break;
  }
  return false;
}

bool MainWindow::ResultsListKeyEventFilter(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Return:
      TypeCurrentPassword();
      return true;
    case Qt::Key_Down:
    // pass
    case Qt::Key_Up:
      // do nothing;
      break;
    default:
      ui->filter_line->setFocus();
      QCoreApplication::sendEvent(ui->filter_line, event);
      return true;
  }

  return false;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() != QEvent::KeyPress) {
    return false;
  }
  QKeyEvent *key_event = static_cast<QKeyEvent *>(event);

  if (key_event->key() == Qt::Key_Escape) {
    hide();
    return true;
  }

  if (obj == ui->filter_line && FilterLineKeyEventFilter(key_event)) {
    return true;
  }

  if (obj == ui->results_list && ResultsListKeyEventFilter(key_event)) {
    return true;
  }

  return false;
}

bool MainWindow::nativeEvent(const QByteArray & /*eventType*/, void *message,
                             long * /*result*/) {
  if (hotkey_setter_.CheckHotkey(message)) {
    Activate();
  }
  return false;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  bool hidden = isHidden();
  if (hidden) {
    Activate();
  }
  if (SureToExit()) {
    event->accept();
    return;
  }
  event->ignore();
  if (hidden) {
    hide();
  }
}

bool MainWindow::SureToExit() {
  QMessageBox::StandardButton reply = QMessageBox::question(
      this, QApplication::applicationName(), "Are you sure you want to exit?",
      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  return reply == QMessageBox::Yes;
}

int MainWindow::ReloadTree() {
  if (passwords_path_.isEmpty()) {
    QMessageBox::information(this, "No storage path",
                             "Please select a password storage path.");
    UpdateFiltering("");
    return -1;
  }
  try {
    PassStorage new_storage(passwords_path_);
    pass_storage_ = std::move(new_storage);
    UpdateFiltering(ui->filter_line->text());
    return 0;
  } catch (const std::exception &e) {
    QMessageBox::warning(this, "Can't load the passwords tree:\n", e.what());
    UpdateFiltering("");
    return -1;
  }
}

void MainWindow::ChangeStoragePath() {
  QDir new_path = QFileDialog::getExistingDirectory(
      this, "Passwords storage directory", passwords_path_,
      QFileDialog::ShowDirsOnly);
  if (!new_path.exists()) {
    return;
  }
  if (new_path.path() == ".") {
    // Probably "Cancel" has been hit.
    return;
  }
  if (!ReloadTree()) {
    return;
  }
  passwords_path_ = new_path.absolutePath();
  QSettings settings;
  settings.setValue(kSettingsFieldPasswordsPath, passwords_path_);
}

void MainWindow::TypeCurrentPassword() {
  if (ui->results_list->count() == 0) {
    return;
  }
  int row = ui->results_list->currentRow();
  if (row < 0) {
    return;
  }
  QListWidgetItem *item = ui->results_list->item(row);
  if (item == nullptr) {
    return;
  }
  QString name = item->text();
  QString file_name = pass_storage_.GetFileName(name);

  wait_dialog_->ShowDialog("Waiting for GPG");
  password_future_ =
      QtConcurrent::run(&GpgDecryptFileFirstLine, name, file_name);
  password_future_watcher_.setFuture(password_future_);
}

void MainWindow::TypePassword() {
  wait_dialog_->close();
  auto result = password_future_watcher_.result();
  if (!result) {
    QMessageBox::critical(
        this, "Unknown",
        "Please contact the developers: " __FILE__ + QString::number(__LINE__));
    return;
  }
  QString name = result->GetName();
  if (auto error = std::dynamic_pointer_cast<GpgDecryptionError>(result)) {
    QMessageBox::critical(this, "GPG failed for " + name, error->GetError());
    return;
  }
  auto success = std::dynamic_pointer_cast<GpgDecryptionSuccess>(result);
  if (!success) {
    QMessageBox::critical(
        this, "Unknown",
        "Please contact the developers: " __FILE__ + QString::number(__LINE__));
    return;
  }
  hide();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  keyboard_.TypeString(success->GetResult());
  systray_icon_->showMessage("Password typed", name);
  success->ClearResult();
}

void MainWindow::InitTrayIconContextMenu() {
  auto menu = new QMenu(this);
  menu->addAction(quit_action_);
  systray_icon_->setContextMenu(menu);
}
