#pragma once

#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdexcept>

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

namespace Atem
{
    static const int DEFAULT_SPEED = 12;
    static const QString DEFAULT_TRANSITION = "0";
    static const QString DEFAULT_INPUT = "1";
    static const QString DEFAULT_SWITCHER = "prev";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const QString DEFAULT_KEYER = "0";
    static const bool DEFAULT_KEYER_STATE = false;
    static const QString DEFAULT_VIDEO_FORMAT = "4";
    static const QString DEFAULT_AUDIO_INPUT = "1";
    static const QString DEFAULT_AUDIO_INPUT_STATE = "0";
    static const float DEFAULT_AUDIO_INPUT_BALANCE = 0.0f;
    static const float DEFAULT_AUDIO_GAIN = 0.0f;
}

namespace Panasonic
{
    static const QString DEFAULT_ADDRESS = "";
    static const QString DEFAULT_PRESET = "";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Http
{
    static const QString DEFAULT_URL = "";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Stream
{
    static const int COMPACT_WIDTH = 288;
    static const int COMPACT_HEIGHT = 162;
}

namespace TriCaster
{
    static const int DEFAULT_PORT = 5950;
    static const QString DEFAULT_SPEED = "";
    static const QString DEFAULT_TRANSITION = "";
    static const QString DEFAULT_PRESET = "0";
    static const QString DEFAULT_SOURCE = "audiomixer";
    static const QString DEFAULT_INPUT = "ddr";
    static const QString DEFAULT_SWITCHER = "prev";
    static const QString DEFAULT_NETWORK_SOURCE = "";
    static const QString DEFAULT_NETWORK_TARGET = "net";
    static const QString DEFAULT_MACRO = "";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Repository
{
    static const int DEFAULT_PORT = 8250;
}

namespace Osc
{
    static const bool DEFAULT_USE_BUNDLE = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const int DEFAULT_PORT = 6250;
    static const QString DEFAULT_OUTPUT = "";
    static const QString DEFAULT_TYPE = "String";
    static const int DEFAULT_OUTPUT_PORT = 7250;
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
    static const QString DEFAULT_TIME = "00:00:00:00";
    static const QString DEFAULT_TIME_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/time";
    static const QString DEFAULT_FRAME_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/frame";
    static const QString DEFAULT_FPS_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/fps";
    static const QString DEFAULT_PATH_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/file/path";
    static const QString DEFAULT_PAUSED_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/paused";
    static const QString DEFAULT_LOOP_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/loop";
    static const QString DEFAULT_AUDIO_FILTER = "#IPADDRESS#/channel/#CHANNEL#/mixer/audio/#AUDIOCHANNEL#/dBFS";
    static const QString DEFAULT_STOP_CONTROL_FILTER = "/control/#UID#/stop";
    static const QString DEFAULT_PLAY_CONTROL_FILTER = "/control/#UID#/play";
    static const QString DEFAULT_PLAYNOW_CONTROL_FILTER = "/control/#UID#/playnow";
    static const QString DEFAULT_LOAD_CONTROL_FILTER = "/control/#UID#/load";
    static const QString DEFAULT_PAUSE_CONTROL_FILTER = "/control/#UID#/pause";
    static const QString DEFAULT_NEXT_CONTROL_FILTER = "/control/#UID#/next";
    static const QString DEFAULT_UPDATE_CONTROL_FILTER = "/control/#UID#/update";
    static const QString DEFAULT_INVOKE_CONTROL_FILTER = "/control/#UID#/invoke";
    static const QString DEFAULT_PREVIEW_CONTROL_FILTER = "/control/#UID#/preview";
    static const QString DEFAULT_CLEAR_CONTROL_FILTER = "/control/#UID#/clear";
    static const QString DEFAULT_CLEAR_VIDEOLAYER_CONTROL_FILTER = "/control/#UID#/clearvideolayer";
    static const QString DEFAULT_CLEAR_CHANNEL_CONTROL_FILTER = "/control/#UID#/clearchannel";
    static const QString DEFAULT_STOP_RUNDOWN_CONTROL_FILTER = "/control/stop";
    static const QString DEFAULT_PLAY_RUNDOWN_CONTROL_FILTER = "/control/play";
    static const QString DEFAULT_PLAYNOW_RUNDOWN_CONTROL_FILTER = "/control/playnow";
    static const QString DEFAULT_LOAD_RUNDOWN_CONTROL_FILTER = "/control/load";
    static const QString DEFAULT_PAUSE_RUNDOWN_CONTROL_FILTER = "/control/pause";
    static const QString DEFAULT_NEXT_RUNDOWN_CONTROL_FILTER = "/control/next";
    static const QString DEFAULT_UPDATE_RUNDOWN_CONTROL_FILTER = "/control/update";
    static const QString DEFAULT_INVOKE_RUNDOWN_CONTROL_FILTER = "/control/invoke";
    static const QString DEFAULT_PREVIEW_RUNDOWN_CONTROL_FILTER = "/control/preview";
    static const QString DEFAULT_CLEAR_RUNDOWN_CONTROL_FILTER = "/control/clear";
    static const QString DEFAULT_CLEAR_VIDEOLAYER_RUNDOWN_CONTROL_FILTER = "/control/clearvideolayer";
    static const QString DEFAULT_CLEAR_CHANNEL_RUNDOWN_CONTROL_FILTER = "/control/clearchannel";
    static const QString DEFAULT_DOWN_RUNDOWN_CONTROL_FILTER = "/control/down";
    static const QString DEFAULT_UP_RUNDOWN_CONTROL_FILTER = "/control/up";
}

namespace Route
{
    static const int DEFAULT_FROM_CHANNEL = 1;
    static const int DEFAULT_FROM_VIDEOLAYER = 1;
}

namespace GpiOutput
{
    static const int DEFAULT_PORT = 0;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Mixer
{
    static const float DEFAULT_ANCHOR_XPOS = 0.0f;
    static const float DEFAULT_ANCHOR_YPOS = 0.0f;
    static const float DEFAULT_GRID = 2.0f;
    static const float DEFAULT_ROTATION = 0.0f;
    static const float DEFAULT_BRIGHTNESS = 1.0f;
    static const float DEFAULT_CONTRAST = 1.0f;
    static const float DEFAULT_CLIP_LEFT = 0.0f;
    static const float DEFAULT_CLIP_WIDTH = 1.0f;
    static const float DEFAULT_CLIP_TOP = 0.0f;
    static const float DEFAULT_CLIP_HEIGHT = 1.0f;
    static const float DEFAULT_FILL_XPOS = 0.0f;
    static const float DEFAULT_FILL_YPOS = 0.0f;
    static const float DEFAULT_FILL_XSCALE = 1.0f;
    static const float DEFAULT_FILL_YSCALE = 1.0f;
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
    static const float DEFAULT_PERSPECTIVE_UPPERLEFT_X = 0.0f;
    static const float DEFAULT_PERSPECTIVE_UPPERLEFT_Y = 0.0f;
    static const float DEFAULT_PERSPECTIVE_UPPERRIGHT_X = 1.0f;
    static const float DEFAULT_PERSPECTIVE_UPPERRIGHT_Y = 0.0f;
    static const float DEFAULT_PERSPECTIVE_LOWERRIGHT_X = 1.0f;
    static const float DEFAULT_PERSPECTIVE_LOWERRIGHT_Y = 1.0f;
    static const float DEFAULT_PERSPECTIVE_LOWERLEFT_X = 0.0f;
    static const float DEFAULT_PERSPECTIVE_LOWERLEFT_Y = 1.0f;
    static const float DEFAULT_CROP_LEFT = 0.0f;
    static const float DEFAULT_CROP_TOP = 0.0f;
    static const float DEFAULT_CROP_RIGHT = 1.0f;
    static const float DEFAULT_CROP_BOTTOM = 1.0f;
    static const int DEFAULT_DURATION = 1;
    static const bool DEFAULT_DEFER = false;
    static const bool DEFAULT_MIPMAP = false;
    static const QString DEFAULT_BLENDMODE = "Normal";
    static const QString DEFAULT_TWEEN = "Linear";
    static const QString DEFAULT_DIRECTION = "RIGHT";
    static const QString DEFAULT_TRANSITION = "CUT";
    static const QString DEFAULT_CHROMAKEY = "None";
}

namespace Appearance
{
    static const QString CURVE_THEME = "Curve";
}

namespace Color
{
    static const QString DEFAULT_ACTIVE_COLOR = "rgba(0, 255, 0, 255)";
    static const QString DEFAULT_CONSUMER_COLOR = "DarkSlateGray";
    static const QString DEFAULT_GPI_COLOR = "Chocolate";
    static const QString DEFAULT_GROUP_COLOR = "rgba(70, 115, 195, 255)";
    static const QString DEFAULT_AUDIO_COLOR = "SlateGray";
    static const QString DEFAULT_STILL_COLOR = "Goldenrod";
    static const QString DEFAULT_MOVIE_COLOR = "DarkKhaki";
    static const QString DEFAULT_PRINT_COLOR = "Goldenrod";
    static const QString DEFAULT_CLEAR_OUTPUT_COLOR = "Goldenrod";
    static const QString DEFAULT_COLOR_PRODUCER_COLOR = "Goldenrod";
    static const QString DEFAULT_MIXER_COLOR = "Sienna";
    static const QString DEFAULT_TRICASTER_COLOR = "DarkOliveGreen";
    static const QString DEFAULT_ATEM_COLOR = "DarkOliveGreen";
    static const QString DEFAULT_PANASONIC_COLOR = "DarkOliveGreen";
    static const QString DEFAULT_PRODUCER_COLOR = "SeaGreen";
    static const QString DEFAULT_TEMPLATE_COLOR = "OliveDrab";
    static const QString DEFAULT_SEPARATOR_COLOR = "Maroon";
    static const QString DEFAULT_STORED_DATA_COLOR = "Chocolate";
    static const QString DEFAULT_TRANSPARENT_COLOR = "Transparent";
    static const QString SIENNA_COLOR = "rgba(136, 45, 23, 128)";
    static const QString OLIVEDRAB_COLOR = "rgba(107, 142, 35, 128)";
    static const QString SEAGREEN_COLOR = "rgba(46, 139, 87, 128)";
    static const QString CHOCOLATE_COLOR = "rgba(123, 63, 0, 128)";
    static const QString DARKSLATEGRAY_COLOR = "rgba(47, 79, 79, 128)";
    static const QString STEELBLUE_COLOR = "rgba(70, 130, 179, 128)";
    static const QString MAROON_COLOR = "rgba(128, 0, 0, 192)";
    static const QString MAROONLIGHT_COLOR = "rgba(128, 0, 0, 64)";
    static const QString DARKKHAKI_COLOR = "rgba(189, 183, 107, 64)";
    static const QString ROYALBLUE_COLOR = "rgba(65, 105, 225, 64)";
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
    static const int DEFAULT_DURATION = 0;
    static const bool DEFAULT_ALLOW_GPI = false;
    static const bool DEFAULT_ALLOW_REMOTE_TRIGGERING = false;
    static const QString DEFAULT_REMOTE_TRIGGER_ID = "";
    static const QString DEFAULT_DELAY_IN_FRAMES = "Frames";
    static const QString DEFAULT_DELAY_IN_MILLISECONDS = "Milliseconds";
    static const QString DEFAULT_PLAYOUT_COMMAND = "Play";
}

namespace Audio
{
    static const QString DEFAULT_NAME = "";
    static const bool DEFAULT_LOOP = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_USE_AUTO = false;
}

namespace Movie
{
    static const QString DEFAULT_NAME = "";
    static const int DEFAULT_SEEK = 0;
    static const int DEFAULT_LENGTH = 0;
    static const bool DEFAULT_LOOP = false;
    static const bool DEFAULT_FREEZE_ON_LOAD = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_AUTO_PLAY = false;
}

namespace Html
{
    static const QString DEFAULT_URL = "";
    static const bool DEFAULT_FREEZE_ON_LOAD = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const bool DEFAULT_USE_AUTO = false;
}


namespace Still
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

namespace Fill
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Custom
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Rotation
{
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Anchor
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
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
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
    static const QString DEFAULT_COLOR = "#FF000000";
}

namespace FadeToBlack
{
    static const bool DEFAULT_USE_AUTO = false;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const QString DEFAULT_COLOR = "#00000000";
    static const int DEFAULT_DURATION = 12;
    static const QString DEFAULT_TRANSITION = "MIX";
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
    static const QString CLIP = "CLIP";
    static const QString CROP = "CROP";
    static const QString IMAGESCROLLER = "IMAGESCROLLER";
    static const QString DECKLINKINPUT = "DECKLINKINPUT";
    static const QString PRINT = "PRINT";
    static const QString CLEAROUTPUT = "CLEAROUTPUT";
    static const QString FILL = "FILL";
    static const QString GPIOUTPUT = "GPIOUTPUT";
    static const QString HTTPGET = "HTTPGET";
    static const QString HTTPPOST = "HTTPPOST";
    static const QString FILERECORDER = "FILERECORDER";
    static const QString SEPARATOR = "SEPARATOR";
    static const QString GRID = "GRID";
    static const QString SOLIDCOLOR = "SOLIDCOLOR";
    static const QString KEYER = "KEYER";
    static const QString LEVELS = "LEVELS";
    static const QString OPACITY = "OPACITY";
    static const QString SATURATION = "SATURATION";
    static const QString VOLUME = "VOLUME";
    static const QString HTML = "HTML";
    static const QString COMMIT = "COMMIT";
    static const QString AUDIO = "AUDIO";
    static const QString STILL = "STILL";
    static const QString TEMPLATE = "TEMPLATE";
    static const QString MOVIE = "MOVIE";
    static const QString CUSTOMCOMMAND = "CUSTOMCOMMAND";
    static const QString PLAYOUTCOMMAND = "PLAYOUTCOMMAND";
    static const QString FADETOBLACK = "FADETOBLACK";
    static const QString CHROMAKEY = "CHROMAKEY";
    static const QString TRICASTERAUTO = "TRICASTERAUTO";
    static const QString TRICASTERTAKE = "TRICASTERTAKE";
    static const QString TRICASTERINPUT = "TRICASTERINPUT";
    static const QString TRICASTERPRESET = "TRICASTERPRESET";
    static const QString TRICASTERNETSOURCE = "TRICASTERNETSOURCE";
    static const QString TRICASTERMACRO = "TRICASTERMACRO";
    static const QString ATEMAUTO = "ATEMAUTO";
    static const QString ATEMINPUT = "ATEMINPUT";
    static const QString ATEMKEYERSTATE = "ATEMKEYERSTATE";
    static const QString ATEMCUT = "ATEMCUT";
    static const QString ATEMVIDEOFORMAT = "ATEMVIDEOFORMAT";
    static const QString ATEMAUDIOINPUTSTATE = "ATEMAUDIOINPUTSTATE";
    static const QString ATEMAUDIOGAIN = "ATEMAUDIOGAIN";
    static const QString ATEMAUDIOINPUTBALANCE = "ATEMAUDIOINPUTBALANCE";
    static const QString OSCOUTPUT = "OSCOUTPUT";
    static const QString PANASONICPRESET = "PANASONICPRESET";
    static const QString PERSPECTIVE = "PERSPECTIVE";
    static const QString ROTATION = "ROTATION";
    static const QString RESET = "RESET";
    static const QString ANCHOR = "ANCHOR";
    static const QString ROUTECHANNEL = "ROUTECHANNEL";
    static const QString ROUTEVIDEOLAYER = "ROUTEVIDEOLAYER";
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

namespace Panel
{
    static const int DEFAULT_PREVIEW_HEIGHT = 188;
    static const int DEFAULT_LIVE_HEIGHT = 188;
    static const int DEFAULT_AUDIOLEVELS_HEIGHT = 147;
#if defined(Q_OS_WIN)
    static const int COMPACT_PREVIEW_HEIGHT = 25;
    static const int COMPACT_LIVE_HEIGHT = 25;
    static const int COMPACT_AUDIOLEVELS_HEIGHT = 25;
#else
    static const int COMPACT_PREVIEW_HEIGHT = 27;
    static const int COMPACT_LIVE_HEIGHT = 27;
    static const int COMPACT_AUDIOLEVELS_HEIGHT = 27;
#endif
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

namespace Action
{
    struct ActionType
    {
        enum Type
        {
            KeyPress,
            GpiPulse
        };
    };
}

namespace Playout
{
    struct PlayoutType
    {
        enum Type
        {
            Stop = QEvent::User + 110,
            Play = QEvent::User + 120,
            PlayNow = QEvent::User + 130,
            PauseResume = QEvent::User + 140,
            Load = QEvent::User + 150,
            Next = QEvent::User + 160,
            Update = QEvent::User + 170,
            Invoke = QEvent::User + 180,
            Preview = QEvent::User + 190,
            Clear = QEvent::User + 200,
            ClearVideoLayer = QEvent::User + 210,
            ClearChannel = QEvent::User + 220
        };
    };

    static PlayoutType::Type fromString(const QString& value)
    {
        if (value == "Stop")
            return PlayoutType::Stop;
        else if (value == "Play")
            return PlayoutType::Play;
        else if (value == "Play Now")
            return PlayoutType::PlayNow;
        else if (value == "Pause / Resume")
            return PlayoutType::PauseResume;
        else if (value == "Load")
            return PlayoutType::Load;
        else if (value == "Next")
            return PlayoutType::Next;
        else if (value == "Update")
            return PlayoutType::Update;
        else if (value == "Invoke")
            return PlayoutType::Invoke;
        else if (value == "Preview")
            return PlayoutType::Preview;
        else if (value == "Clear")
            return PlayoutType::Clear;
        else if (value == "Clear Video Layer")
            return PlayoutType::ClearVideoLayer;
        else if (value == "Clear Channel")
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
        case PlayoutType::PlayNow:
            return "Play Now";
        case PlayoutType::PauseResume:
            return "Pause / Resume";
        case PlayoutType::Load:
            return "Load";
        case PlayoutType::Next:
            return "Next";
        case PlayoutType::Update:
            return "Update";
        case PlayoutType::Invoke:
            return "Invoke";
        case PlayoutType::Preview:
            return "Preview";
        case PlayoutType::Clear:
            return "Clear";
        case PlayoutType::ClearVideoLayer:
            return "Clear Video Layer";
        case PlayoutType::ClearChannel:
            return "Clear Channel";
        default:
            throw std::invalid_argument("Invalid enum constant");
        }
    }

    static QList<PlayoutType::Type> createConstantList()
    {
        QList<PlayoutType::Type> result;

        result.push_back(PlayoutType::Stop);
        result.push_back(PlayoutType::Play);
        result.push_back(PlayoutType::PlayNow);
        result.push_back(PlayoutType::PauseResume);
        result.push_back(PlayoutType::Load);
        result.push_back(PlayoutType::Next);
        result.push_back(PlayoutType::Update);
        result.push_back(PlayoutType::Invoke);
        result.push_back(PlayoutType::Preview);
        result.push_back(PlayoutType::Clear);
        result.push_back(PlayoutType::ClearVideoLayer);
        result.push_back(PlayoutType::ClearChannel);

        return result;
    }

    static const QList<PlayoutType::Type>& enumConstants()
    {
        static QList<PlayoutType::Type> result(createConstantList());

        return result;
    }
}
