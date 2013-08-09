#include "ActionWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/BlendModeModel.h"

#include <QtGui/QApplication>
#include <QtGui/QListWidgetItem>
#include <QtGui/QPushButton>

ActionWidget::ActionWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);




    QListWidgetItem* item1 = new QListWidgetItem();
    item1->setText("asdfasdf");

    QListWidgetItem* item2 = new QListWidgetItem();
    item2->setText("23452345");

    QListWidgetItem* item3 = new QListWidgetItem();
    item3->setText("asdfasdf");

    QListWidgetItem* item4 = new QListWidgetItem();
    item4->setText("23452345");


    QListWidgetItem* item5 = new QListWidgetItem();
    item5->setText("asdfasdf");

    QListWidgetItem* item6 = new QListWidgetItem();
    item6->setText("23452345");


    QListWidgetItem* item7 = new QListWidgetItem();
    item7->setText("asdfasdf");

    QListWidgetItem* item8 = new QListWidgetItem();
    item8->setText("asdfasdf");

    QListWidgetItem* item9 = new QListWidgetItem();
    item9->setText("23452345");

    QListWidgetItem* item10 = new QListWidgetItem();
    item10->setText("asdfasdf");

    QListWidgetItem* item11 = new QListWidgetItem();
    item11->setText("23452345");

    QListWidgetItem* item12 = new QListWidgetItem();
    item12->setText("asdfasdf");

    QListWidgetItem* item13 = new QListWidgetItem();
    item13->setText("23452345");


    this->listWidgetAction->addItem(item1);
    this->listWidgetAction->addItem(item2);

    this->listWidgetAction->addItem(item3);
    this->listWidgetAction->addItem(item4);
    this->listWidgetAction->addItem(item5);
    this->listWidgetAction->addItem(item6);
    this->listWidgetAction->addItem(item7);
    this->listWidgetAction->addItem(item8);
    this->listWidgetAction->addItem(item9);
    this->listWidgetAction->addItem(item10);

    this->listWidgetAction->addItem(item11);
    this->listWidgetAction->addItem(item12);
    this->listWidgetAction->addItem(item13);



    qApp->installEventFilter(this);
}

bool ActionWidget::eventFilter(QObject* target, QEvent* event)
{
    return QObject::eventFilter(target, event);
}
