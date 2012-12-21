#include "InspectorImageScrollerWidget.h"

#include "Global.h"

#include "Events/RundownItemSelectedEvent.h"

InspectorImageScrollerWidget::InspectorImageScrollerWidget(QWidget* parent)
    : QWidget(parent),
      preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorImageScrollerWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<ImageScrollerCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<ImageScrollerCommand*>(rundownItemSelectedEvent->getCommand());

            // This will also set the slider value.
            // TODO: Why QString -> float, see InspectorVolumeWidget.cpp
            this->spinBoxBlur->setValue(this->command->getBlur());
            this->spinBoxSpeed->setValue(this->command->getSpeed());
            this->checkBoxPremultiply->setChecked(this->command->getPremultiply());
            this->checkBoxProgressive->setChecked(this->command->getProgressive());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
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
    this->spinBoxSpeed->setValue(Media::DEFAULT_SPEED);
    this->command->setSpeed(this->spinBoxSpeed->value());
}

void InspectorImageScrollerWidget::resetBlur(QString blur)
{
    this->spinBoxBlur->setValue(Media::DEFAULT_BLUR);
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
