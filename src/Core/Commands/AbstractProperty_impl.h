#pragma once

#include "AbstractProperty.h"
#include "AbstractProperties.h"

#include <QtCore/QVariant>

#include <functional> // std::bind

template<typename Tag>
AbstractProperty<Tag>::AbstractProperty(const Type& def_value, AbstractProperties* reg) :
    value(def_value), def_value(def_value)
{
    reg->registerReader(std::bind(&AbstractProperty<Tag>::read<Tag>, this, std::placeholders::_1));
    reg->registerWriter(std::bind(&AbstractProperty<Tag>::write, this, std::placeholders::_1));
}

template<typename Tag>
AbstractProperty<Tag>::~AbstractProperty() { }

// generic version for types acceptable by boost::property_tree::wptree::get
template<typename Tag>
template<typename U>
typename std::enable_if< !std::is_same<QString, typename PropertyTraits<U>::Type>::value >::type
AbstractProperty<Tag>::read(boost::property_tree::wptree& pt)
{
    set(pt.get(PropertyTraits<Tag>::name, def_value));
}

// overload for QString
template<typename Tag>
template<typename U>
typename std::enable_if< std::is_same<QString, typename PropertyTraits<U>::Type>::value >::type
AbstractProperty<Tag>::read(boost::property_tree::wptree& pt)
{
    set(QString::fromStdWString(pt.get(PropertyTraits<Tag>::name, def_value.toStdWString())));
}

template<typename Tag>
void AbstractProperty<Tag>::write(QXmlStreamWriter* writer)
{
    writer->writeTextElement(QString::fromStdWString(PropertyTraits<Tag>::name), QVariant::fromValue(get()).toString());
}
