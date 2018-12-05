#pragma once

#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

namespace Atem
{
    static const int DEFAULT_SPEED = 12;
    static const QString DEFAULT_MACRO = "0";
    static const QString DEFAULT_TRANSITION = "0";
    static const QString DEFAULT_INPUT = "0";
    static const QString DEFAULT_SWITCHER = "prev";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
    static const QString DEFAULT_KEYER = "0";
    static const bool DEFAULT_KEYER_STATE = false;
    static const QString DEFAULT_VIDEO_FORMAT = "4";
    static const QString DEFAULT_AUDIO_INPUT = "1";
    static const QString DEFAULT_AUDIO_INPUT_STATE = "0";
    static const float DEFAULT_AUDIO_INPUT_BALANCE = 0.0f;
    static const float DEFAULT_AUDIO_GAIN = 0.0f;
    static const QString DEFAULT_MIXER_STEP = "0";
}

namespace Panasonic
{
    static const QString DEFAULT_ADDRESS = "";
    static const int DEFAULT_PRESET = 1;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Sony
{
    static const QString DEFAULT_ADDRESS = "";
    static const int DEFAULT_PRESET = 1;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Spyder
{
    static const QString DEFAULT_ADDRESS = "";
    static const int DEFAULT_PRESET = 1;
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Http
{
    static const QString DEFAULT_URL = "";
    static const bool DEFAULT_TRIGGER_ON_NEXT = false;
}

namespace Stream
{
    static const int DEFAULT_PORT = 9250;
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
    static const int DEFAULT_WEBSOCKET_PORT = 4250;
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
    static const QString FILERECORDER_FRAME_FILTER = "#IPADDRESS#/channel/#CHANNEL#/output/port/200/frame";
    static const QString FILERECORDER_FPS_FILTER = "#IPADDRESS#/channel/#CHANNEL#/output/port/200/fps";
    static const QString FILERECORDER_PATH_FILTER = "#IPADDRESS#/channel/#CHANNEL#/output/port/200/path";
    static const QString VIDEOLAYER_TIME_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/foreground/file/time";
    static const QString VIDEOLAYER_CLIP_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/foreground/file/clip";
    static const QString VIDEOLAYER_FPS_FILTER = "#IPADDRESS#/channel/#CHANNEL#/framerate";
    static const QString VIDEOLAYER_NAME_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/foreground/file/name";
    static const QString VIDEOLAYER_PAUSED_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/foreground/paused";
    static const QString VIDEOLAYER_LOOP_FILTER = "#IPADDRESS#/channel/#CHANNEL#/stage/layer/#VIDEOLAYER#/foreground/loop";
    static const QString AUDIOCHANNEL_FILTER = "#IPADDRESS#/channel/#CHANNEL#/mixer/audio/#AUDIOCHANNEL#/dBFS";
    static const QString ITEM_CONTROL_STOP_FILTER = "/control/#UID#/stop";
    static const QString ITEM_CONTROL_PLAY_FILTER = "/control/#UID#/play";
    static const QString ITEM_CONTROL_PLAYNOW_FILTER = "/control/#UID#/playnow";
    static const QString ITEM_CONTROL_LOAD_FILTER = "/control/#UID#/load";
    static const QString ITEM_CONTROL_PAUSE_FILTER = "/control/#UID#/pause";
    static const QString ITEM_CONTROL_NEXT_FILTER = "/control/#UID#/next";
    static const QString ITEM_CONTROL_UPDATE_FILTER = "/control/#UID#/update";
    static const QString ITEM_CONTROL_INVOKE_FILTER = "/control/#UID#/invoke";
    static const QString ITEM_CONTROL_PREVIEW_FILTER = "/control/#UID#/preview";
    static const QString ITEM_CONTROL_CLEAR_FILTER = "/control/#UID#/clear";
    static const QString ITEM_CONTROL_CLEARVIDEOLAYER_FILTER = "/control/#UID#/clearvideolayer";
    static const QString ITEM_CONTROL_CLEARCHANNEL_FILTER = "/control/#UID#/clearchannel";
    static const QString RUNDOWN_CONTROL_STOP_FILTER = "/control/stop";
    static const QString RUNDOWN_CONTROL_PLAY_FILTER = "/control/play";
    static const QString RUNDOWN_CONTROL_PLAYNOW_FILTER = "/control/playnow";
    static const QString RUNDOWN_CONTROL_LOAD_FILTER = "/control/load";
    static const QString RUNDOWN_CONTROL_PAUSE_FILTER = "/control/pause";
    static const QString RUNDOWN_CONTROL_NEXT_FILTER = "/control/next";
    static const QString RUNDOWN_CONTROL_UPDATE_FILTER = "/control/update";
    static const QString RUNDOWN_CONTROL_INVOKE_FILTER = "/control/invoke";
    static const QString RUNDOWN_CONTROL_PREVIEW_FILTER = "/control/preview";
    static const QString RUNDOWN_CONTROL_CLEAR_FILTER = "/control/clear";
    static const QString RUNDOWN_CONTROL_CLEARVIDEOLAYER_FILTER = "/control/clearvideolayer";
    static const QString RUNDOWN_CONTROL_CLEARCHANNELFILTER = "/control/clearchannel";
    static const QString RUNDOWN_CONTROL_DOWN_FILTER = "/control/down";
    static const QString RUNDOWN_CONTROL_UP_FILTER = "/control/up";
    static const QString RUNDOWN_CONTROL_PLAYNOWIFCHANNEL_FILTER = "/control/playnowifchannel";
    static const QString RUNDOWN_CONTROL_PLAYANDAUTOSTEP_FILTER = "/control/playandautostep";
    static const QString RUNDOWN_CONTROL_PLAYNOWANDAUTOSTEP_FILTER = "/control/playnowandautostep";
}

namespace Route
{
    static const int DEFAULT_FROM_CHANNEL = 1;
    static const int DEFAULT_FROM_VIDEOLAYER = 1;
    static const int DEFAULT_OUTPUT_DELAY = 0;
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
    static const int DEFAULT_DURATION = 0;
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
    static const bool DEFAULT_SEND_AS_JSON = false;
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
    static const QString ATEMFADETOBLACK = "ATEMFADETOBLACK";
    static const QString ATEMMACRO = "ATEMMACRO";
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
    static const QString SONYPRESET = "SONYPRESET";
    static const QString SPYDERPRESET = "SPYDERPRESET";
    static const QString PERSPECTIVE = "PERSPECTIVE";
    static const QString ROTATION = "ROTATION";
    static const QString RESET = "RESET";
    static const QString ANCHOR = "ANCHOR";
    static const QString ROUTECHANNEL = "ROUTECHANNEL";
    static const QString ROUTEVIDEOLAYER = "ROUTEVIDEOLAYER";
    static const int MAX_NUMBER_OF_RUNDONWS = 10;
    static const QString DEFAULT_NAME = "New Rundown";
    static const QString DEFAULT_AUDIO_NAME = "Audio";
    static const QString DEFAULT_STILL_NAME = "Image";
    static const QString DEFAULT_IMAGESCROLLER_NAME = "Image Scroller";
    static const QString DEFAULT_TEMPLATE_NAME = "Template";
    static const QString DEFAULT_MOVIE_NAME = "Video";
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
    static const int COMPACT_PREVIEW_HEIGHT = 25;
    static const int COMPACT_LIVE_HEIGHT = 25;
    static const int COMPACT_AUDIOLEVELS_HEIGHT = 25;
}

namespace Action
{
    enum class ActionType
    {
        KeyPress,
        GpiPulse
    };
}

namespace XmlFormatting
{
    static const bool ENABLE_FORMATTING = true;
    static const int NUMBER_OF_SPACES = 4;
}

namespace Utils
{
    static const Qt::TimerType DEFAULT_TIMER_TYPE = Qt::PreciseTimer;
}
