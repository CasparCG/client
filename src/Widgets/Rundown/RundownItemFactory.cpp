#include "RundownItemFactory.h"

#include "RundownBlendModeWidget.h"
#include "RundownBrightnessWidget.h"
#include "RundownCommitWidget.h"
#include "RundownContrastWidget.h"
#include "RundownClipWidget.h"
#include "RundownDeckLinkInputWidget.h"
#include "RundownFileRecorderWidget.h"
#include "RundownImageScrollerWidget.h"
#include "RundownFillWidget.h"
#include "RundownGpiOutputWidget.h"
#include "RundownGridWidget.h"
#include "RundownGroupWidget.h"
#include "RundownKeyerWidget.h"
#include "RundownLevelsWidget.h"
#include "RundownMovieWidget.h"
#include "RundownOpacityWidget.h"
#include "RundownSaturationWidget.h"
#include "RundownTemplateWidget.h"
#include "RundownVolumeWidget.h"
#include "RundownSeparatorWidget.h"
#include "RundownPrintWidget.h"
#include "RundownClearOutputWidget.h"
#include "RundownSolidColorWidget.h"
#include "RundownAudioWidget.h"
#include "RundownStillWidget.h"
#include "RundownCustomCommandWidget.h"
#include "RundownPlayoutCommandWidget.h"
#include "RundownChromaWidget.h"
#include "RundownFadeToBlackWidget.h"
#include "RundownOscOutputWidget.h"
#include "RundownPerspectiveWidget.h"
#include "RundownRotationWidget.h"
#include "RundownAnchorWidget.h"
#include "RundownCropWidget.h"
#include "RundownHttpGetWidget.h"
#include "RundownResetWidget.h"
#include "RundownHtmlWidget.h"
#include "RundownRouteChannelWidget.h"
#include "RundownRouteVideolayerWidget.h"
#include "RundownHttpPostWidget.h"

Q_GLOBAL_STATIC(RundownItemFactory, rundownItemFactory)

RundownItemFactory& RundownItemFactory::getInstance()
{
    return *rundownItemFactory();
}

AbstractRundownWidget* RundownItemFactory::createWidget(const LibraryModel& model)
{
    if (model.getType() == Rundown::BLENDMODE) return new RundownBlendModeWidget(model, this);
    else if (model.getType() == Rundown::BRIGHTNESS) return new RundownBrightnessWidget(model, this);
    else if (model.getType() == Rundown::CONTRAST) return new RundownContrastWidget(model, this);
    else if (model.getType() == Rundown::CLIP) return new RundownClipWidget(model, this);
    else if (model.getType() == Rundown::CHROMAKEY) return new RundownChromaWidget(model, this);
    else if (model.getType() == Rundown::DECKLINKINPUT) return new RundownDeckLinkInputWidget(model, this);
    else if (model.getType() == Rundown::FILERECORDER) return new RundownFileRecorderWidget(model, this);
    else if (model.getType() == Rundown::FILL) return new RundownFillWidget(model, this);
    else if (model.getType() == Rundown::GRID) return new RundownGridWidget(model, this);
    else if (model.getType() == Rundown::GPIOUTPUT) return new RundownGpiOutputWidget(model, this);
    else if (model.getType() == Rundown::KEYER) return new RundownKeyerWidget(model, this);
    else if (model.getType() == Rundown::LEVELS) return new RundownLevelsWidget(model, this);
    else if (model.getType() == Rundown::AUDIO) return new RundownAudioWidget(model, this);
    else if (model.getType() == Rundown::MOVIE) return new RundownMovieWidget(model, this);
    else if (model.getType() == Rundown::STILL) return new RundownStillWidget(model, this);
    else if (model.getType() == Rundown::OPACITY) return new RundownOpacityWidget(model, this);
    else if (model.getType() == Rundown::SATURATION) return new RundownSaturationWidget(model, this);
    else if (model.getType() == Rundown::TEMPLATE) return new RundownTemplateWidget(model, this);
    else if (model.getType() == Rundown::VOLUME) return new RundownVolumeWidget(model, this);
    else if (model.getType() == Rundown::COMMIT) return new RundownCommitWidget(model, this);
    else if (model.getType() == Rundown::IMAGESCROLLER) return new RundownImageScrollerWidget(model, this);
    else if (model.getType() == Rundown::SEPARATOR) return new RundownSeparatorWidget(model, this);
    else if (model.getType() == Rundown::PRINT) return new RundownPrintWidget(model, this);
    else if (model.getType() == Rundown::CLEAROUTPUT) return new RundownClearOutputWidget(model, this);
    else if (model.getType() == Rundown::SOLIDCOLOR) return new RundownSolidColorWidget(model, this);
    else if (model.getType() == Rundown::CUSTOMCOMMAND) return new RundownCustomCommandWidget(model, this);
    else if (model.getType() == Rundown::OSCOUTPUT) return new RundownOscOutputWidget(model, this);
    else if (model.getType() == Rundown::PLAYOUTCOMMAND) return new RundownPlayoutCommandWidget(model, this);
    else if (model.getType() == Rundown::FADETOBLACK) return new RundownFadeToBlackWidget(model, this);
    else if (model.getType() == Rundown::PERSPECTIVE) return new RundownPerspectiveWidget(model, this);
    else if (model.getType() == Rundown::ROTATION) return new RundownRotationWidget(model, this);
    else if (model.getType() == Rundown::ANCHOR) return new RundownAnchorWidget(model, this);
    else if (model.getType() == Rundown::CROP) return new RundownCropWidget(model, this);
    else if (model.getType() == Rundown::HTTPGET) return new RundownHttpGetWidget(model, this);
    else if (model.getType() == Rundown::HTTPPOST) return new RundownHttpPostWidget(model, this);
    else if (model.getType() == Rundown::RESET) return new RundownResetWidget(model, this);
    else if (model.getType() == Rundown::HTML) return new RundownHtmlWidget(model, this);
    else if (model.getType() == Rundown::ROUTECHANNEL) return new RundownRouteChannelWidget(model, this);
    else if (model.getType() == Rundown::ROUTEVIDEOLAYER) return new RundownRouteVideolayerWidget(model, this);

    return NULL;
}
