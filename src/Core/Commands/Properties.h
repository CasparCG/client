#pragma once

#include "AbstractProperty.h"

#define DEFINE_PROPERTY(TYPE, PROP, XML_NAME)                           \
struct PROP { };                                                        \
template<> struct PropertyTraits<PROP>                                  \
{                                                                       \
    using Type = TYPE;                                                  \
    static constexpr const wchar_t* name = XML_NAME;                    \
};                                                                      \
class PROP ## Property : public QObject, public AbstractProperty<PROP>  \
{                                                                       \
    Q_OBJECT                                                            \
public:                                                                 \
    explicit PROP ## Property(const Type& def_value, CommandProperties* owner = nullptr, QObject* parent = nullptr) : \
        QObject(parent),                                                \
        AbstractProperty(def_value, owner)                              \
    { }                                                                 \
    using AbstractProperty<PROP>::get;                                  \
    using AbstractProperty<PROP>::set;                                  \
    Q_SIGNAL void changed(const TYPE&) const;                           \
private:                                                                \
    void emit_changed(const Type& value) const override { emit changed(value); } \
}                                                                       \

DEFINE_PROPERTY(   bool, AllowGpi             , L"allowgpi");
DEFINE_PROPERTY(   bool, AllowRemoteTriggering, L"allowremotetriggering");
DEFINE_PROPERTY(    int, Channel              , L"channel");
DEFINE_PROPERTY(    int, Delay                , L"delay");
DEFINE_PROPERTY(    int, Duration             , L"duration");
DEFINE_PROPERTY(QString, RemoteTriggerId      , L"remotetriggerid");
DEFINE_PROPERTY(QString, StoryId              , L"storyid");
DEFINE_PROPERTY(    int, Videolayer           , L"videolayer");
