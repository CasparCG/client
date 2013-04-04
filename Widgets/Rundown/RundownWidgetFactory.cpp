#include "RundownWidgetFactory.h"

#include "RundownBlendModeWidget.h"
#include "RundownBrightnessWidget.h"
#include "RundownCommitWidget.h"
#include "RundownContrastWidget.h"
#include "RundownCropWidget.h"
#include "RundownDeckLinkInputWidget.h"
#include "RundownFileRecorderWidget.h"
#include "RundownImageScrollerWidget.h"
#include "RundownGeometryWidget.h"
#include "RundownGpiOutputWidget.h"
#include "RundownGridWidget.h"
#include "RundownGroupWidget.h"
#include "RundownKeyerWidget.h"
#include "RundownLevelsWidget.h"
#include "RundownVideoWidget.h"
#include "RundownOpacityWidget.h"
#include "RundownSaturationWidget.h"
#include "RundownTemplateWidget.h"
#include "RundownVolumeWidget.h"
#include "RundownSeparatorWidget.h"
#include "RundownPrintWidget.h"
#include "RundownClearOutputWidget.h"
#include "RundownSolidColorWidget.h"
#include "RundownAudioWidget.h"
#include "RundownImageWidget.h"

Q_GLOBAL_STATIC(RundownWidgetFactory, rundownWidgetFactory)

RundownWidgetFactory& RundownWidgetFactory::getInstance()
{
    return *rundownWidgetFactory();
}

AbstractRundownWidget* RundownWidgetFactory::createWidget(const LibraryModel& model)
{
    if (model.getType() == "BLENDMODE") return new RundownBlendModeWidget(model, this);
    else if (model.getType() == "BRIGHTNESS") return new RundownBrightnessWidget(model, this);
    else if (model.getType() == "CONTRAST") return new RundownContrastWidget(model, this);
    else if (model.getType() == "CROP") return new RundownCropWidget(model, this);
    else if (model.getType() == "DECKLINKINPUT") return new RundownDeckLinkInputWidget(model, this);
    else if (model.getType() == "FILERECORDER") return new RundownFileRecorderWidget(model, this);
    else if (model.getType() == "TRANSFORMATION") return new RundownGeometryWidget(model, this);
    else if (model.getType() == "GRID") return new RundownGridWidget(model, this);
    else if (model.getType() == "GPIOUTPUT") return new RundownGpiOutputWidget(model, this);
    else if (model.getType() == "MASK") return new RundownKeyerWidget(model, this);
    else if (model.getType() == "LEVELS") return new RundownLevelsWidget(model, this);
    else if (model.getType() == "AUDIO") return new RundownAudioWidget(model, this);
    else if (model.getType() == "MOVIE") return new RundownVideoWidget(model, this);
    else if (model.getType() == "STILL") return new RundownImageWidget(model, this);
    else if (model.getType() == "OPACITY") return new RundownOpacityWidget(model, this);
    else if (model.getType() == "SATURATION") return new RundownSaturationWidget(model, this);
    else if (model.getType() == "TEMPLATE") return new RundownTemplateWidget(model, this);
    else if (model.getType() == "VOLUME") return new RundownVolumeWidget(model, this);
    else if (model.getType() == "COMMIT") return new RundownCommitWidget(model, this);
    else if (model.getType() == "IMAGESCROLLER") return new RundownImageScrollerWidget(model, this);
    else if (model.getType() == "SEPARATOR") return new RundownSeparatorWidget(model, this);
    else if (model.getType() == "CHANNELSNAPSHOT") return new RundownPrintWidget(model, this);
    else if (model.getType() == "CLEAROUTPUT") return new RundownClearOutputWidget(model, this);
    else if (model.getType() == "SOLIDCOLOR") return new RundownSolidColorWidget(model, this);

    return NULL;
}
