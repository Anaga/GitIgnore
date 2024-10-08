#include "tariffviewform.h"
#include "ui_tariffviewform.h"

TariffViewForm::TariffViewForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TariffViewForm)
{
    ui->setupUi(this);
    qDebug() << "TariffViewForm open";
}

TariffViewForm::~TariffViewForm()
{
    delete ui;
}

void TariffViewForm::on_pushButton_Exit_clicked()
{
    qDebug() << "TariffViewForm close";
    // Close the window
    this->close();

    // Schedule this object for deletion
    this->deleteLater();
}

