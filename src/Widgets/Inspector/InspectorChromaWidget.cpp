#include "InspectorChromaWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PreviewEvent.h"
#include "Models/ChromaModel.h"

#include <QtGui/QApplication>

InspectorChromaWidget::InspectorChromaWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));

    loadChroma();
}

void InspectorChromaWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<ChromaCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<ChromaCommand*>(event.getCommand());

        this->comboBoxKey->setCurrentIndex(this->comboBoxKey->findText(this->command->getKey()));
        this->checkBoxShowMask->setChecked(this->command->getShowMask());
        this->spinBoxThreshold->setValue(QString("%1").arg(this->command->getThreshold() * 100).toFloat());
        this->sliderThreshold->setValue(QString("%1").arg(this->command->getThreshold() * 100).toFloat());
        this->spinBoxSoftness->setValue(QString("%1").arg(this->command->getSpread() * 100).toFloat());
        this->sliderSoftness->setValue(QString("%1").arg(this->command->getSpread() * 100).toFloat());
        this->spinBoxBlur->setValue(QString("%1").arg(this->command->getBlur() * 100).toFloat());
        this->sliderBlur->setValue(QString("%1").arg(this->command->getBlur() * 100).toFloat());
        this->spinBoxSpill->setValue(QString("%1").arg(this->command->getSpill() * 100).toFloat());
        this->sliderSpill->setValue(QString("%1").arg(this->command->getSpill() * 100).toFloat());
    }

    blockAllSignals(false);
}

void InspectorChromaWidget::blockAllSignals(bool block)
{
    this->comboBoxKey->blockSignals(block);
    this->checkBoxShowMask->blockSignals(block);
    this->spinBoxThreshold->blockSignals(block);
    this->spinBoxSoftness->blockSignals(block);
    this->spinBoxBlur->blockSignals(block);
    this->spinBoxSpill->blockSignals(block);
}

void InspectorChromaWidget::loadChroma()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxKey->blockSignals(true);

    QList<ChromaModel> models = DatabaseManager::getInstance().getChroma();
    foreach (ChromaModel model, models)
        this->comboBoxKey->addItem(model.getKey());

    this->comboBoxKey->blockSignals(false);
}

void InspectorChromaWidget::blurChanged(int blur)
{
    this->command->setBlur(static_cast<float>(blur) / 100.0);

    this->spinBoxBlur->setValue(blur);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorChromaWidget::keyChanged(QString key)
{
    this->command->setKey(key);

    this->comboBoxKey->setCurrentIndex(this->comboBoxKey->findText(key));

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorChromaWidget::showMaskChanged(bool show)
{
    this->command->setShowMask(show);

    this->checkBoxShowMask->setChecked(show);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorChromaWidget::softnessChanged(int softness)
{
    this->command->setSpread(static_cast<float>(softness) / 100.0);

    this->spinBoxSoftness->setValue(softness);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorChromaWidget::spillChanged(int spill)
{
    this->command->setSpill(static_cast<float>(spill) / 100.0);

    this->spinBoxSpill->setValue(spill);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}

void InspectorChromaWidget::thresholdChanged(int threshold)
{
    this->command->setThreshold(static_cast<float>(threshold) / 100.0);

    this->spinBoxThreshold->setValue(threshold);

    EventManager::getInstance().firePreviewEvent(PreviewEvent());
}
