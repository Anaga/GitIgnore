#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tariffviewform.h"
#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_pushButton_EditTariff_clicked();

    void on_actionExit_triggered();

    void on_actionSelect_DB_file_triggered();

private:
    Ui::MainWindow *ui;
    TariffViewForm *tariffViewForm;
    QSettings *settings;
    QSqlDatabase db;
    QSqlRelationalTableModel *itemsTableMod;
    void setupDatabaseModel();
    void loadSettings();
    void saveSettings();
};
#endif // MAINWINDOW_H
