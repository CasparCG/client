#include "AddRundownDialog.h"

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

AddRundownDialog::AddRundownDialog(QWidget* parent)
    : QDialog(parent),
      editMode(false)
{
    setupUi(this);

    this->nameAnimation = new BorderAnimation(this->lineEditName);
    this->nameAnimation->start();
}

void AddRundownDialog::setName(const QString& name)
{
    this->lineEditName->setText(name);
}

void AddRundownDialog::setEditMode(bool editMode)
{
    this->editMode = editMode;

    this->setWindowTitle("Edit Rundown");
}

const QString AddRundownDialog::getName() const
{
    return this->lineEditName->text();
}

void AddRundownDialog::accept()
{
    if (this->lineEditName->text().isEmpty())
        return;

    QDialog::accept();
}

void AddRundownDialog::nameChanged(QString name)
{
    if (!this->lineEditName->text().isEmpty())
        this->nameAnimation->stop();
    else
        this->nameAnimation->start();
}
