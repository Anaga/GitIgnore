#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tariffViewForm(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

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

