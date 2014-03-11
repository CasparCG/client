#include "InspectorImageScrollerWidget.h"

#include "Global.h"

#include "EventManager.h"

InspectorImageScrollerWidget::InspectorImageScrollerWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorImageScrollerWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<ImageScrollerCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<ImageScrollerCommand*>(event.getCommand());

        this->spinBoxBlur->setValue(this->command->getBlur());
        this->spinBoxSpeed->setValue(this->command->getSpeed());
        this->checkBoxPremultiply->setChecked(this->command->getPremultiply());
        this->checkBoxProgressive->setChecked(this->command->getProgressive());
    }

    blockAllSignals(false);
}

void InspectorImageScrollerWidget::blockAllSignals(bool block)
{
    this->spinBoxBlur->blockSignals(block);
    this->spinBoxSpeed->blockSignals(block);
    this->checkBoxPremultiply->blockSignals(block);
    this->checkBoxProgressive->blockSignals(block);
}

void InspectorImageScrollerWidget::blurChanged(int blur)
{
    this->command->setBlur(blur);
}

void InspectorImageScrollerWidget::speedChanged(int speed)
{
    this->command->setSpeed(speed);
}

void InspectorImageScrollerWidget::premultiplyChanged(int state)
{
    this->command->setPremultiply((state == Qt::Checked) ? true : false);
}

void InspectorImageScrollerWidget::progressiveChanged(int state)
{
    this->command->setProgressive((state == Qt::Checked) ? true : false);
}

void InspectorImageScrollerWidget::resetSpeed(QString speed)
{
    this->spinBoxSpeed->setValue(ImageScroller::DEFAULT_SPEED);
    this->command->setSpeed(this->spinBoxSpeed->value());
}

void InspectorImageScrollerWidget::resetBlur(QString blur)
{
    this->spinBoxBlur->setValue(ImageScroller::DEFAULT_BLUR);
    this->command->setBlur(this->spinBoxBlur->value());
}

void InspectorImageScrollerWidget::resetPremultiply(QString premultiply)
{
    this->checkBoxPremultiply->setChecked(false);
    this->command->setPremultiply(this->checkBoxPremultiply->isChecked());
}

void InspectorImageScrollerWidget::resetProgressive(QString progressive)
{
    this->checkBoxProgressive->setChecked(false);
    this->command->setProgressive(this->checkBoxProgressive->isChecked());
}
