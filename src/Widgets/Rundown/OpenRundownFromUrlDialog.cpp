#include "OpenRundownFromUrlDialog.h"

#include "DatabaseManager.h"

#include <iostream>

#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include <QtGui/QAbstractButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMessageBox>

#include <QDebug>

OpenRundownFromUrlDialog::OpenRundownFromUrlDialog(QWidget* parent)
    : QDialog(parent), repositoryUrl("")
{
    setupUi(this);

    this->treeWidgetRundowns->headerItem()->setText(0, "");
    this->treeWidgetRundowns->setColumnWidth(0, 25);
    this->treeWidgetRundowns->setColumnHidden(2, true);
    this->treeWidgetRundowns->setColumnHidden(3, true);

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    this->repositoryUrl = DatabaseManager::getInstance().getConfigurationByName("RundownRepository").getValue();
    this->networkManager->get(QNetworkRequest(QUrl(this->repositoryUrl)));

    qApp->installEventFilter(this);
}

bool OpenRundownFromUrlDialog::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (target == this->treeWidgetRundowns)
        {
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                QDialog::accept();
                return true;
            }
        }
    }

    return QObject::eventFilter(target, event);
}

void OpenRundownFromUrlDialog::accept()
{
    QDialog::accept();
}

void OpenRundownFromUrlDialog::finished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());
    QStringList rundowns = data.split("\n");
    rundowns.removeLast();

    foreach (const QString& rundown, rundowns)
    {
        QStringList pathSplit = QUrl(rundown).path().split("/");

        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this->treeWidgetRundowns);
        treeItem->setIcon(0, QIcon(":/Graphics/Images/Rundown.png"));
        treeItem->setText(1, pathSplit.last());
        treeItem->setTextAlignment(1, Qt::AlignTop);
        treeItem->setText(2, "");
        treeItem->setText(3, rundown);
    }
}

QString OpenRundownFromUrlDialog::getPath() const
{
    if (this->treeWidgetRundowns->selectedItems().count() == 0)
        return "";

    return this->treeWidgetRundowns->selectedItems().at(0)->text(3);
}

void OpenRundownFromUrlDialog::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    QDialog::accept();
}
