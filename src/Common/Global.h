#pragma once

#pragma GCC diagnostic ignored "-Wunused-variable"

#define MAJOR_VERSION 2
#define MINOR_VERSION 0
#define REVISION_VERSION 2
#define DATABASE_VERSION "202"

#include <stdexcept>

#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QList>

namespace Enum
{
    struct EventType
    {
        enum Type
        {
            StatusbarMessage = QEvent::User + 11,
            LibraryItemSelected = QEvent::User + 12,
            RundownItemSelected = QEvent::User + 13,
            MediaChanged = QEvent::User + 14,
            TemplateChanged = QEvent::User + 15,
            DataChanged = QEvent::User + 16,
            RefreshLibrary = QEvent::User + 17,
            AutoRefreshLibrary = QEvent::User + 18,
            AddRudnownItem = QEvent::User + 19,
            Preview = QEvent::User + 20,
            ConnectionStateChanged = QEvent::User + 21,
            EmptyRundown = QEvent::User + 22,
            OpenRundown = QEvent::User + 23,
            SaveRundown = QEvent::User + 24,
            ToggleCompactView = QEvent::User + 25,
            ActiveRundownChanged = QEvent::User + 26,
            TargetChanged = QEvent::User + 27,
            DeviceChanged = QEvent::User + 28,
            LabelChanged = QEvent::User + 29,
            NewRundown = QEvent::User + 30,
            DeleteRundown = QEvent::User + 31,
            NewRundownMenu = QEvent::User + 32,
            ChannelChanged = QEvent::User + 33,
            PresetChanged = QEvent::User + 34,
            AddPresetItem = QEvent::User + 35,
            ExportPreset = QEvent::User + 36,
            ImportPreset = QEvent::User + 37
        };
    };
}

namespace Xml
{
    QString encode(const QString& data);
    QString decode(const QString& data);
}

namespace Playout
{
    struct PlayoutType
    {
        enum Type
        {
            Stop = QEvent::User + 110,
            Play = QEvent::User + 120,
            Pause = QEvent::User + 130,
            Load = QEvent::User + 140,
            Next = QEvent::User + 150,
            Update = QEvent::User + 160,
            Invoke = QEvent::User + 170,
            Clear = QEvent::User + 180,
            ClearVideolayer = QEvent::User + 190,
            ClearChannel = QEvent::User + 200
        };
    };

    PlayoutType::Type fromString(const QString& value);
    QString toString(PlayoutType::Type value);
    const QList<PlayoutType::Type>& enumConstants();
}

namespace Define
{
    static const int GROUP_XPOS_OFFSET = -65;
    static const int DEFAULT_ICON_WIDTH = 32;
    static const int DEFAULT_ICON_HEIGHT = 32;
    static const int COMPACT_ICON_WIDTH = 16;
    static const int COMPACT_ICON_HEIGHT = 16;
    static const int DEFAULT_ITEM_HEIGHT = 36;
    static const int COMPACT_ITEM_HEIGHT = 21;
    static const int DEFAULT_THUMBNAIL_WIDTH = 57;
    static const int DEFAULT_THUMBNAIL_HEIGHT = 32;
    static const int COMPACT_THUMBNAIL_WIDTH = 28;
    static const int COMPACT_THUMBNAIL_HEIGHT = 16;
}

namespace Gpi
{
    static const int DEFAULT_GPO_PORT = 0;
}

namespace Mixer
{
    static const float DEFAULT_GRID = 2;
    static const float DEFAULT_BRIGHTNESS = 1;
    static const float DEFAULT_CONTRAST = 1;
    static const float DEFAULT_CROP_LEFT = 0;
    static const float DEFAULT_CROP_RIGHT = 0;
    static const float DEFAULT_CROP_TOP = 1;
    static const float DEFAULT_CROP_BOTTOM = 1;
    static const float DEFAULT_GEOMETRY_XPOS = 0;
    static const float DEFAULT_GEOMETRY_YPOS = 0;
    static const float DEFAULT_GEOMETRY_XSCALE = 1;
    static const float DEFAULT_GEOMETRY_YSCALE = 1;
    static const float DEFAULT_LEVELS_MIN_IN = 0;
    static const float DEFAULT_LEVELS_MAX_IN = 1;
    static const float DEFAULT_LEVELS_MIN_OUT = 0;
    static const float DEFAULT_LEVELS_MAX_OUT = 1;
    static const float DEFAULT_LEVELS_GAMMA = 1;
    static const float DEFAULT_OPACITY = 1;
    static const float DEFAULT_SATURATION = 1;
    static const float DEFAULT_VOLUME = 1;
    static const int DEFAULT_DURATION = 1;
    static const bool DEFAULT_DEFER = false;
    static const QString DEFAULT_BLENDMODE = "Normal";
    static const QString DEFAULT_TWEEN = "Linear";
    static const QString DEFAULT_DIRECTION = "RIGHT";
    static const QString DEFAULT_TRANSITION = "CUT";
}

namespace Color
{
    static const bool DEFAULT_USE_AUTO = false;
    static const QString DEFAULT_COLOR_NAME = "";
    static const QString DEFAULT_CONSUMER_COLOR = "DarkSlateGray";
    static const QString DEFAULT_GPI_COLOR = "Chocolate";
    static const QString DEFAULT_GROUP_COLOR = "SteelBlue";
    static const QString DEFAULT_AUDIO_COLOR = "SlateGray";
    static const QString DEFAULT_STILL_COLOR = "Goldenrod";
    static const QString DEFAULT_MOVIE_COLOR = "DarkKhaki";
    static const QString DEFAULT_PRINT_COLOR = "Goldenrod";
    static const QString DEFAULT_CLEAR_OUTPUT_COLOR = "Goldenrod";
    static const QString DEFAULT_COLOR_PRODUCER_COLOR = "Goldenrod";
    static const QString DEFAULT_MIXER_COLOR = "Sienna";
    static const QString DEFAULT_PRODUCER_COLOR = "SeaGreen";
    static const QString DEFAULT_TEMPLATE_COLOR = "OliveDrab";
    static const QString DEFAULT_SEPARATOR_COLOR = "Maroon";
    static const QString DEFAULT_STORED_DATA_COLOR = "Chocolate";
    static const QString DEFAULT_TRANSPARENT_COLOR = "Transparent";
    static const QString SIENNA_RGBA_COLOR = "136, 45, 23, 128";
    static const QString OLIVEDRAB_RGBA_COLOR = "107, 142, 35, 128";
    static const QString SEAGREEN_RGBA_COLOR = "46, 139, 87, 128";
    static const QString CHOCOLATE_RGBA_COLOR = "123, 63, 0, 128";
    static const QString DARKSLATEGRAY_RGBA_COLOR = "47, 79, 79, 128";
    static const QString STEELBLUE_RGBA_COLOR = "70, 130, 179, 128";
    static const QString MAROON_RGBA_COLOR = "128, 0, 0, 255";
    static const QString MAROONLIGHT_RGBA_COLOR = "128, 0, 0, 64";
    static const QString DARKKHAKI_RGBA_COLOR = "189, 183, 107, 64";
    static const QString ROYALBLUE_RGBA_COLOR = "65, 105, 225, 64";
}

namespace Stylesheet
{
    static const QString DEFAULT_BORDER_COLOR = "42, 42, 42, 255";
}

namespace TemplateData
{
    static const QString DEFAULT_COMPONENT_DATA_XML = "<componentData id=\\\"#KEY\\\"><data id=\\\"text\\\" value=\\\"#VALUE\\\"/></componentData>";
}

namespace Output
{
    static const int DEFAULT_CHANNEL = 1;
    static const int DEFAULT_VIDEOLAYER = 10;
    static const int DEFAULT_AUDIO_VIDEOLAYER = 30;
    static const int DEFAULT_FLASH_VIDEOLAYER = 20;
    static const int DEFAULT_DELAY = 0;
    static const bool DEFAULT_ALLOW_GPI = false;
}

namespace Audio
{
    static const QString DEFAULT_NAME = "";
    static const bool DEFAULT_LOOP = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_USE_AUTO = false;
}

namespace Video
{
    static const QString DEFAULT_NAME = "";
    static const int DEFAULT_SEEK = 0;
    static const int DEFAULT_LENGTH = 0;
    static const bool DEFAULT_LOOP = false;
    static const bool DEFAULT_FREEZE_ON_LOAD = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_USE_AUTO = false;
}

namespace Image
{
    static const QString DEFAULT_NAME = "";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_USE_AUTO = false;
}

namespace ImageScroller
{
    static const QString DEFAULT_NAME = "";
    static const int DEFAULT_BLUR = 0;
    static const int DEFAULT_SPEED = 8;
    static const bool DEFAULT_PREMULTIPLY = false;
    static const bool DEFAULT_PROGRESSIVE = false;
}

namespace Template
{
    static const int DEFAULT_FLASHLAYER = 1;
    static const QString DEFAULT_INVOKE = "";
    static const QString DEFAULT_TEMPLATENAME = "";
}

namespace DeckLinkInput
{
    static const int DEFAULT_DEVICE = 2;
    static const QString DEFAULT_FORMAT = "PAL";
}

namespace FileRecorder
{
    static const QString DEFAULT_OUTPUT = "Output.mov";
    static const QString DEFAULT_CODEC = "dnxhd";
    static const QString DEFAULT_PRESET = "";
    static const QString DEFAULT_TUNE = "";
    static const bool DEFAULT_WITH_ALPHA = false;
}

namespace SolidColor
{
    static const QString DEFAULT_NAME = "#00000000";
}

namespace Library
{
    static const int PRESET_PAGE_INDEX = 0;
    static const int AUDIO_PAGE_INDEX = 1;
    static const int STILL_PAGE_INDEX = 2;
    static const int TEMPLATE_PAGE_INDEX = 3;
    static const int MOVIE_PAGE_INDEX = 4;
    static const int DATA_PAGE_INDEX = 5;
}

namespace Print
{
    static const QString DEFAULT_OUTPUT = "Snapshot";
}

namespace ClearOutput
{
    static const bool DEFAULT_CLEAR_CHANNEL = true;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Group
{
    static const QString DEFAULT_NOTE = "";
    static const bool DEFAULT_AUTO_STEP = false;
}

namespace Rundown
{
    static const QString BLENDMODE = "BLENDMODE";
    static const QString BRIGHTNESS = "BRIGHTNESS";
    static const QString CONTRAST = "CONTRAST";
    static const QString CROP = "CROP";
    static const QString IMAGESCROLLER = "IMAGESCROLLER";
    static const QString DECKLINKINPUT = "DECKLINKINPUT";
    static const QString PRINT = "CHANNELSNAPSHOT";
    static const QString CLEAROUTPUT = "CLEAROUTPUT";
    static const QString GEOMETRY = "TRANSFORMATION";
    static const QString GPIOUTPUT = "GPIOUTPUT";
    static const QString FILERECORDER = "FILERECORDER";
    static const QString SEPARATOR = "SEPARATOR";
    static const QString GRID = "GRID";
    static const QString SOLIDCOLOR = "SOLIDCOLOR";
    static const QString KEYER = "MASK";
    static const QString LEVELS = "LEVELS";
    static const QString OPACITY = "OPACITY";
    static const QString SATURATION = "SATURATION";
    static const QString VOLUME = "VOLUME";
    static const QString COMMIT = "COMMIT";
    static const QString AUDIO = "AUDIO";
    static const QString IMAGE = "STILL";
    static const QString TEMPLATE = "TEMPLATE";
    static const QString VIDEO = "MOVIE";
    static const QString CUSTOMCOMMAND = "CUSTOMCOMMAND";
    static const int MAX_NUMBER_OF_RUNDONWS = 5;
    static const QString DEFAULT_NAME = "New Rundown";
}
