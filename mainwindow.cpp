#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tariffViewForm(nullptr)
    , settings(new QSettings("Zippo", "GitIgnore", this))
{
    ui->setupUi(this);
    loadSettings();

    // Инициализация соединения с базой данных
    db = QSqlDatabase::addDatabase("QSQLITE");
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    qDebug() << "Settings saved, app will be close.";
    event->accept();
}

void MainWindow::loadSettings()
{
    QString db_path = settings->value("db_path", "No file").toString();
    QPoint pos = settings->value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings->value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
    ui->label_DbPath->setText(db_path);
    // Load other settings as needed
}

void MainWindow::saveSettings()
{
    settings->setValue("pos", pos());
    settings->setValue("size", size());
    QString db_path = ui->label_DbPath->text();
    settings->setValue("db_path", db_path);
}

void MainWindow::on_pushButton_EditTariff_clicked()
{
    qDebug() << "edit button click";

    // Create a new instance each time
    TariffViewForm *newTariffViewForm = new TariffViewForm();

    // Set window flags to make it a separate window
    newTariffViewForm->setWindowFlags(Qt::Window);

    // Show the window
    newTariffViewForm->show();

    // Set up the window to delete itself when closed
    connect(newTariffViewForm, &QWidget::destroyed, [=]() {
        qDebug() << "Done!";
    });
}


void MainWindow::on_actionExit_triggered()
{
    // Close the main window, which will typically exit the application
    this->close();
}


void MainWindow::on_actionSelect_DB_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open Database File"),
        QString(),
        tr("Database Files (*.db *.db3 *.s3db *.sqlite);;All Files (*)")
        );

    if (!filePath.isEmpty()) {
        // User selected a file
        // Add your code here to open and work with the database file
        QMessageBox::information(this, tr("File Selected"), tr("You selected: %1").arg(filePath));
        ui->label_DbPath->setText(filePath);
    } else {
        // User canceled the dialog
        QMessageBox::warning(this, tr("No File Selected"), tr("No database file was selected."));
    }
}

void MainWindow::setupDatabaseModel()
{
    // Удаляем старую модель, если она существует
    if (itemsTableMod) {
        delete itemsTableMod;
        itemsTableMod = nullptr;
    }

    itemsTableMod = new QSqlRelationalTableModel(this, db);
    itemsTableMod->setTable("equipment_items");
}

