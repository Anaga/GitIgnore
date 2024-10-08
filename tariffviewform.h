#ifndef TARIFFVIEWFORM_H
#define TARIFFVIEWFORM_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class TariffViewForm;
}

class TariffViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit TariffViewForm(QWidget *parent = nullptr);
    ~TariffViewForm();

private slots:
    void on_pushButton_Exit_clicked();

private:
    Ui::TariffViewForm *ui;
};

#endif // TARIFFVIEWFORM_H
