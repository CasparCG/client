#pragma once

#include <boost/property_tree/ptree.hpp>

#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

#include <type_traits>
#include <utility> // std::move

class AbstractProperties;

template<typename Tag> struct PropertyTraits { };

template<typename Tag>
class AbstractProperty
{
public:
    using Type = typename PropertyTraits<Tag>::Type;

    AbstractProperty(const Type& def_value, AbstractProperties*);
    virtual ~AbstractProperty();

    const Type& get() const { return value; }
    void set(Type new_value) { value = std::move(new_value); emit_changed(value); }

protected:
    Type value;
    const Type def_value;

private:
    virtual void emit_changed(const Type&) const = 0;

    friend class AbstractProperties;

    // generic version for types acceptable by boost::property_tree::wptree::get
    template<typename U = Tag>
    typename std::enable_if< !std::is_same<QString, typename PropertyTraits<U>::Type>::value >::type
    read(boost::property_tree::wptree&);

    // overload for QString
    template<typename U = Tag>
    typename std::enable_if< std::is_same<QString, typename PropertyTraits<U>::Type>::value >::type
    read(boost::property_tree::wptree&);

    void write(QXmlStreamWriter*);
};

#include "AbstractProperty_impl.h"
