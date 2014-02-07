#include "PresetDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

PresetDialog::PresetDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->lineEditName->setStyleSheet("border-color: firebrick;");
}

void PresetDialog::accept()
{
    if (this->lineEditName->text().isEmpty())
        return;

    QDialog::accept();
}

const QString PresetDialog::getName() const
{
    return this->lineEditName->text();
}

void PresetDialog::nameChanged(QString name)
{
    if (this->lineEditName->text().isEmpty())
        this->lineEditName->setStyleSheet("border-color: firebrick;");
    else
        this->lineEditName->setStyleSheet("");

}
