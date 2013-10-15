#include "TemplateDataDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

TemplateDataDialog::TemplateDataDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->textEditValue->setFocus();
}

void TemplateDataDialog::accept()
{
    if (this->lineEditName->text().isEmpty())
        return;

    QDialog::accept();
}

const QString TemplateDataDialog::getName() const
{
    return this->lineEditName->text();
}

const QString TemplateDataDialog::getValue() const
{
    return this->textEditValue->toPlainText();
}

void TemplateDataDialog::setName(const QString& name)
{
    this->lineEditName->setText(name);
}

void TemplateDataDialog::setValue(const QString& value)
{
    this->setWindowTitle("Edit Template Data");
    this->textEditValue->setPlainText(value);
}
