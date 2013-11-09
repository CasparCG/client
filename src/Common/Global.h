#pragma once

#pragma GCC diagnostic ignored "-Wunused-variable"

#define MAJOR_VERSION 2
#define MINOR_VERSION 0
#define REVISION_VERSION 6
#define DATABASE_VERSION "206DH"

#include <stdexcept>

#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QList>

namespace Event
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
            ImportPreset = QEvent::User + 37,
            VideolayerChanged = QEvent::User + 38,
            AddTemplateData = QEvent::User + 39,
            RundownIsChanged = QEvent::User + 40,
            AddActionItem = QEvent::User + 41,
            AutoPlayRundownItem = QEvent::User + 42,
            AutoPlayChanged =  QEvent::User + 43,
            AutoPlayNextRundownItem =  QEvent::User + 44
        };
    };
}

namespace Osc
{
    static const int DEFAULT_PORT = 6250;
    static const int DEFAULT_PROGRESSBAR_X = 64;
    static const int DEFAULT_PROGRESSBAR_Y = 3;
    static const int DEFAULT_PROGRESSBAR_HEIGHT = 24;
    static const int COMPACT_PROGRESSBAR_X = 64;
    static const int COMPACT_PROGRESSBAR_Y = 2;
    static const int COMPACT_PROGRESSBAR_HEIGHT = 11;
    static const int DEFAULT_LOOP_X = 180;
    static const int DEFAULT_LOOP_Y = 15;
    static const int COMPACT_LOOP_X = 180;
    static const int COMPACT_LOOP_Y = 0;
    static const int DEFAULT_PLAY_X = 102;
    static const int DEFAULT_PLAY_Y = 15;
    static const int COMPACT_PLAY_X = 102;
    static const int COMPACT_PLAY_Y = 0;
    static const int DEFAULT_PAUSE_X = 102;
    static const int DEFAULT_PAUSE_Y = 14;
    static const int COMPACT_PAUSE_X = 102;
    static const int COMPACT_PAUSE_Y = 0;
}

namespace GpiOutput
{
    static const int DEFAULT_PORT = 0;
}

namespace Mixer
{
    static const float DEFAULT_GRID = 2.0f;
    static const float DEFAULT_BRIGHTNESS = 1.0f;
    static const float DEFAULT_CONTRAST = 1.0f;
    static const float DEFAULT_CROP_LEFT = 0.0f;
    static const float DEFAULT_CROP_RIGHT = 0.0f;
    static const float DEFAULT_CROP_TOP = 1.0f;
    static const float DEFAULT_CROP_BOTTOM = 1.0f;
    static const float DEFAULT_GEOMETRY_XPOS = 0.0f;
    static const float DEFAULT_GEOMETRY_YPOS = 0.0f;
    static const float DEFAULT_GEOMETRY_XSCALE = 1.0f;
    static const float DEFAULT_GEOMETRY_YSCALE = 1.0f;
    static const float DEFAULT_LEVELS_MIN_IN = 0.0f;
    static const float DEFAULT_LEVELS_MAX_IN = 1.0f;
    static const float DEFAULT_LEVELS_MIN_OUT = 0.0f;
    static const float DEFAULT_LEVELS_MAX_OUT = 1.0f;
    static const float DEFAULT_LEVELS_GAMMA = 1.0f;
    static const float DEFAULT_OPACITY = 1.0f;
    static const float DEFAULT_SATURATION = 1.0f;
    static const float DEFAULT_VOLUME = 1.0f;
    static const float DEFAULT_CHROMABLUR = 0.0f;
    static const float DEFAULT_CHROMABLENDSTART = 0.340f;
    static const float DEFAULT_CHROMABLENDSTOP = 0.440f;
    static const float DEFAULT_CHROMASPILL = 1.0f;
    static const int DEFAULT_DURATION = 1;
    static const bool DEFAULT_DEFER = false;
    static const QString DEFAULT_BLENDMODE = "Normal";
    static const QString DEFAULT_TWEEN = "Linear";
    static const QString DEFAULT_DIRECTION = "RIGHT";
    static const QString DEFAULT_TRANSITION = "CUT";
    static const QString DEFAULT_CHROMAKEY = "None";
}

namespace Color
{
    static const QString DEFAULT_ACTIVE_COLOR = "Lime";
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
    static const bool DEFAULT_AUTO_PLAY = false;
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

namespace Opacity
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Geometry
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Custom
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Template
{
    static const int DEFAULT_FLASHLAYER = 1;
    static const QString DEFAULT_INVOKE = "";
    static const QString DEFAULT_TEMPLATENAME = "";
    static const bool DEFAULT_USE_STORED_DATA = false;
    static const bool DEFAULT_USE_UPPERCASE_DATA = false;
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
    static const bool DEFAULT_USE_AUTO = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const QString DEFAULT_COLOR = "#00000000";
}

namespace Library
{
    static const int TOOLS_PAGE_INDEX = 0;
    static const int AUDIO_PAGE_INDEX = 1;
    static const int STILL_PAGE_INDEX = 2;
    static const int TEMPLATE_PAGE_INDEX = 3;
    static const int MOVIE_PAGE_INDEX = 4;
    static const int DATA_PAGE_INDEX = 5;
    static const int PRESET_PAGE_INDEX = 6;
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
    static const bool DEFAULT_AUTO_PLAY = false;
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
    static const QString CHROMAKEY = "CHROMAKEY";
    static const int MAX_NUMBER_OF_RUNDONWS = 5;
    static const QString DEFAULT_NAME = "New Rundown";
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
    static const int GROUP_INDENTION = 65;
}

namespace Osc
{
    static const QString DEFAULT_TIME = "00:00:00:00";
    static const QString DEFAULT_TIME_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/time.*";
    static const QString DEFAULT_FRAME_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/frame.*";
    static const QString DEFAULT_FPS_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/fps.*";
    static const QString DEFAULT_PATH_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/path.*";
    static const QString DEFAULT_PAUSED_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/paused.*";
    static const QString DEFAULT_LOOP_FILTER = "^#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/loop.*";
}

namespace Xml
{
    static QString encode(const QString& data)
    {
        QString temp;

        for (int index = 0; index < data.size(); index++)
        {
            QChar character(data.at(index));
            switch (character.unicode())
            {
                case '&':
                    temp += "&amp;";
                    break;
                case '\'':
                    temp += "&apos;";
                    break;
                case '"':
                    temp += "&quot;";
                    break;
                case '<':
                    temp += "&lt;";
                    break;
                case '>':
                    temp += "&gt;";
                    break;
                default:
                    temp += character;
                    break;
            }
        }

        return temp;
     }

    static QString decode(const QString& data)
    {
        QString temp(data);

        temp.replace("&amp;", "&");
        temp.replace("&apos;", "'");
        temp.replace("&quot;", "\"");
        temp.replace("&lt;", "<");
        temp.replace("&gt;", ">");

        return temp;
    }
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

    static PlayoutType::Type fromString(const QString& value)
    {
        if (value == "Stop")
            return PlayoutType::Stop;
        else if (value == "Play")
            return PlayoutType::Play;
        else if (value == "Pause")
            return PlayoutType::Pause;
        else if (value == "Load")
            return PlayoutType::Load;
        else if (value == "Next")
            return PlayoutType::Next;
        else if (value == "Update")
            return PlayoutType::Update;
        else if (value == "Invoke")
            return PlayoutType::Invoke;
        else if (value == "Clear")
            return PlayoutType::Clear;
        else if (value == "ClearVideolayer")
            return PlayoutType::ClearVideolayer;
        else if (value == "ClearChannel")
            return PlayoutType::ClearChannel;
        else
            throw std::invalid_argument(value.toStdString());
    }

    static QString toString(PlayoutType::Type value)
    {
        switch (value)
        {
        case PlayoutType::Stop:
            return "Stop";
        case PlayoutType::Play:
            return "Play";
        case PlayoutType::Pause:
            return "Pause";
        case PlayoutType::Load:
            return "Load";
        case PlayoutType::Next:
            return "Next";
        case PlayoutType::Update:
            return "Update";
        case PlayoutType::Invoke:
            return "Invoke";
        case PlayoutType::Clear:
            return "Clear";
        case PlayoutType::ClearVideolayer:
            return "ClearVideolayer";
        case PlayoutType::ClearChannel:
            return "ClearChannel";
        default:
            throw std::invalid_argument("Invalid enum constant");
        }
    }

    static QList<PlayoutType::Type> createConstantList()
    {
        QList<PlayoutType::Type> result;

        result.push_back(PlayoutType::Stop);
        result.push_back(PlayoutType::Play);
        result.push_back(PlayoutType::Pause);
        result.push_back(PlayoutType::Load);
        result.push_back(PlayoutType::Next);
        result.push_back(PlayoutType::Update);
        result.push_back(PlayoutType::Invoke);
        result.push_back(PlayoutType::Clear);
        result.push_back(PlayoutType::ClearVideolayer);
        result.push_back(PlayoutType::ClearChannel);

        return result;
    }

    static const QList<PlayoutType::Type>& enumConstants()
    {
        static QList<PlayoutType::Type> result(createConstantList());

        return result;
    }
}
