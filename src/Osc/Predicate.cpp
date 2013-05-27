#include "Predicate.h"

StartsWith::StartsWith(const QString& start)
    : start(start)
{
}

bool StartsWith::operator()(const QString& elem) const
{
    return elem.startsWith(start);
}

Contains::Contains(const QString& contains)
    : contains(contains)
{
}

bool Contains::operator()(const QString& elem) const
{
    return elem.contains(contains);
}
