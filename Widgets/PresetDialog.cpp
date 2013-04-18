#include "PresetDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

PresetDialog::PresetDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->nameAnimation = new BorderAnimation(this->lineEditName);
    this->nameAnimation->start();
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
    if (!this->lineEditName->text().isEmpty())
        this->nameAnimation->stop();
    else
        this->nameAnimation->start();
}
