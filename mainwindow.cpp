#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCloseEvent>
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tariffViewForm(nullptr)
    , settings(new QSettings("Zippo", "GitIgnore", this))
    , itemsTableMod(nullptr)
{
    ui->setupUi(this);
    loadSettings();
    openDatabase();
}

MainWindow::~MainWindow()
{
    if (itemsTableMod) {
        delete itemsTableMod;
    }

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    qDebug() << "Settings saved, app will be close.";
    DatabaseManager::instance().closeDatabase();
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

    // Check file existing.
    QFileInfo fileInfo(db_path);
    if (!fileInfo.exists()) {
        settings->setValue("is_db_file_ok", false);
        qWarning() << "DB file not exist!";
        QMessageBox::critical(this, "Error", "The database file missing.");
        return;
    }

    if (fileInfo.size() == 0 || !fileInfo.isReadable()) {
        settings->setValue("is_db_file_ok", false);
        qWarning() << "DB file is empty, or not readable!";
        QMessageBox::critical(this, "Error", "The database file is empty or can't be read.");
        return;
    }
    settings->setValue("is_db_file_ok", true);

    // Load other settings as needed
}

void MainWindow::saveSettings()
{
    settings->setValue("pos", pos());
    settings->setValue("size", size());
    QString db_path = ui->label_DbPath->text();
    settings->setValue("db_path", db_path);
    bool is_db_file_ok = DatabaseManager::instance().openDatabase(db_path);
    settings->setValue("is_db_file_ok", is_db_file_ok);
}

void MainWindow::openDatabase()
{
    QString db_path = ui->label_DbPath->text();
    qDebug() << "Try to open DB " << db_path;
    if (!DatabaseManager::instance().openDatabase(db_path)) {
        // Handle database connection error
        qDebug() << "Failed to open database";
        return;
    }
    qDebug() << "openDatabase: setupDatabaseModel";
    setupDatabaseModel();
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
    qDebug() << "setupDatabaseModel: A";
    if (itemsTableMod) {
        delete itemsTableMod;
        itemsTableMod = nullptr;
    }

    qDebug() << "setupDatabaseModel: B";

    QSqlDatabase& db = DatabaseManager::instance().database();

    qDebug() << "setupDatabaseModel: C";
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return;
    }

    qDebug() << "setupDatabaseModel: D";

    itemsTableMod = new QSqlRelationalTableModel(this, db);
    itemsTableMod->setTable("equipment_items");

    if (!itemsTableMod->select()) {
        qDebug() << "Error selecting data from table:";
        qDebug() << "Error details:" << itemsTableMod->lastError().text();
        QMessageBox::warning(this, "Предупреждение", "Не удалось загрузить данные из таблицы: " + itemsTableMod->lastError().text());
        return;
    }

    qDebug() << "setupDatabaseModel: E";
    ui->tableView_Items->setModel(itemsTableMod);
    ui->tableView_Items->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_Items));
    ui->tableView_Items->resizeColumnsToContents();
    ui->tableView_Items->horizontalHeader()->setStretchLastSection(true);

    qDebug() << "Model setup successfully";
}

