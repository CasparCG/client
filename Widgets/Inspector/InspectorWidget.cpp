#include "InspectorWidget.h"
#include "Inspector/InspectorBlendModeWidget.h"
#include "Inspector/InspectorBrightnessWidget.h"
#include "Inspector/InspectorContrastWidget.h"
#include "Inspector/InspectorCropWidget.h"
#include "Inspector/InspectorFileRecorderWidget.h"
#include "Inspector/InspectorGeometryWidget.h"
#include "Inspector/InspectorGpiOutputWidget.h"
#include "Inspector/InspectorGridWidget.h"
#include "Inspector/InspectorSaturationWidget.h"
#include "Inspector/InspectorLevelsWidget.h"
#include "Inspector/InspectorMediaWidget.h"
#include "Inspector/InspectorMetadataWidget.h"
#include "Inspector/InspectorOpacityWidget.h"
#include "Inspector/InspectorOutputWidget.h"
#include "Inspector/InspectorTemplateWidget.h"
#include "Inspector/InspectorVolumeWidget.h"
#include "Inspector/InspectorDeckLinkInputWidget.h"
#include "Inspector/InspectorImageScrollerWidget.h"

#include "Global.h"

#include "Commands/ICommand.h"
#include "Commands/BlendModeCommand.h"
#include "Commands/BrightnessCommand.h"
#include "Commands/CommitCommand.h"
#include "Commands/ContrastCommand.h"
#include "Commands/CropCommand.h"
#include "Commands/GeometryCommand.h"
#include "Commands/GpiOutputCommand.h"
#include "Commands/GridCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/KeyerCommand.h"
#include "Commands/LevelsCommand.h"
#include "Commands/MediaCommand.h"
#include "Commands/OpacityCommand.h"
#include "Commands/SaturationCommand.h"
#include "Commands/TemplateCommand.h"
#include "Commands/VolumeCommand.h"
#include "Commands/DeckLinkInputCommand.h"
#include "Commands/ImageScrollerCommand.h"
#include "Events/RundownItemSelectedEvent.h"

InspectorWidget::InspectorWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(0)), 0, new InspectorMetadataWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(1)), 0, new InspectorOutputWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(2)), 0, new InspectorTemplateWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(3)), 0, new InspectorMediaWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(4)), 0, new InspectorBlendModeWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(5)), 0, new InspectorBrightnessWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(6)), 0, new InspectorContrastWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(7)), 0, new InspectorCropWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(8)), 0, new InspectorGeometryWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(9)), 0, new InspectorGridWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(10)), 0, new InspectorLevelsWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(11)), 0, new InspectorOpacityWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(12)), 0, new InspectorSaturationWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(13)), 0, new InspectorVolumeWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(14)), 0, new InspectorDeckLinkInputWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(15)), 0, new InspectorGpiOutputWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(16)), 0, new InspectorImageScrollerWidget(this));
    this->treeWidgetInspector->setItemWidget(new QTreeWidgetItem(this->treeWidgetInspector->topLevelItem(17)), 0, new InspectorFileRecorderWidget(this));

    this->treeWidgetInspector->topLevelItem(1)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(2)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(3)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(4)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(5)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(6)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(7)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(8)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(9)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(10)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(11)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(12)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(13)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(14)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(15)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(16)->setHidden(true);
    this->treeWidgetInspector->topLevelItem(17)->setHidden(true);

    this->treeWidgetInspector->expandAll();
    this->treeWidgetInspector->doItemsLayout();

    qApp->installEventFilter(this);
}

bool InspectorWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        this->treeWidgetInspector->topLevelItem(1)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(2)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(3)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(4)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(5)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(6)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(7)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(8)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(9)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(10)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(11)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(12)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(13)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(14)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(15)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(16)->setHidden(true);
        this->treeWidgetInspector->topLevelItem(17)->setHidden(true);

        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<TemplateCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(2)->setHidden(false);
        }
        else if (dynamic_cast<MediaCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(3)->setHidden(false);
        }
        else if (dynamic_cast<BlendModeCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(4)->setHidden(false);
        }
        else if (dynamic_cast<BrightnessCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);   
            this->treeWidgetInspector->topLevelItem(5)->setHidden(false);
        }
        else if (dynamic_cast<ContrastCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(6)->setHidden(false);
        }
        else if (dynamic_cast<CropCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(7)->setHidden(false);
        }
        else if (dynamic_cast<GeometryCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(8)->setHidden(false);
        }
        else if (dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(9)->setHidden(false);
        }
        else if (dynamic_cast<KeyerCommand*>(rundownItemSelectedEvent->getCommand()) ||
                 dynamic_cast<CommitCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
        }
        else if (dynamic_cast<LevelsCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(10)->setHidden(false);
        }
        else if (dynamic_cast<OpacityCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(11)->setHidden(false);
        }
        else if (dynamic_cast<SaturationCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(12)->setHidden(false);
        }
        else if (dynamic_cast<VolumeCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(13)->setHidden(false);
        }
        else if (dynamic_cast<GpiOutputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(15)->setHidden(false);
        }
        else if (dynamic_cast<DeckLinkInputCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(14)->setHidden(false);
        }
        else if (dynamic_cast<ImageScrollerCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(16)->setHidden(false);
        }
        else if (dynamic_cast<FileRecorderCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(17)->setHidden(false);
        }
        else if (dynamic_cast<GroupCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->treeWidgetInspector->topLevelItem(1)->setHidden(false);
            this->treeWidgetInspector->topLevelItem(2)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(3)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(4)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(5)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(6)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(7)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(8)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(9)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(10)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(11)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(12)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(13)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(14)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(15)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(16)->setHidden(true);
            this->treeWidgetInspector->topLevelItem(17)->setHidden(true);
            this->treeWidgetInspector->expandAll();
        }

        this->treeWidgetInspector->expandAll();
    }

    return QObject::eventFilter(target, event);
}

void InspectorWidget::toggleExpandItem(QTreeWidgetItem* item, int index)
{
    item->setExpanded(!item->isExpanded());
}
