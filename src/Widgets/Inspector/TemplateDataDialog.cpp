#include "TemplateDataDialog.h"

#include "DatabaseManager.h"

#include <QtCore/QDebug>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAbstractButton>
#include <QCloseEvent>
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>
#endif

TemplateDataDialog::TemplateDataDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->textEditValue->setFocus();

    this->textEditValue->installEventFilter(this);
}

bool TemplateDataDialog::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter)
        {
            QDialog::accept();
            return true;
        }
    }

    return QObject::eventFilter(target, event);
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
    this->textEditValue->selectAll();
}
