#include "KeyValueDialog.h"

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

KeyValueDialog::KeyValueDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    this->textEditValue->installEventFilter(this);
}

bool KeyValueDialog::eventFilter(QObject* target, QEvent* event)
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

void KeyValueDialog::accept()
{
    if (this->lineEditKey->text().isEmpty())
        return;

    QDialog::accept();
}

const QString KeyValueDialog::getKey() const
{
    return this->lineEditKey->text();
}

const QString KeyValueDialog::getValue() const
{
    return this->textEditValue->toPlainText();
}

void KeyValueDialog::setKey(const QString& key)
{
    this->lineEditKey->setText(key);

    this->textEditValue->setFocus();
}

void KeyValueDialog::setValue(const QString& value)
{
    this->textEditValue->setPlainText(value);
    this->textEditValue->selectAll();
}

void KeyValueDialog::setTitle(const QString& title)
{
    this->setWindowTitle(title);
}
