#pragma once

#include "Shared.h"

#include <functional>

#include <QString>

/**
 * Predicate that always returns true.
 */
template<typename E>
struct Any : public std::unary_function<E, bool>
{
    bool operator()(E elem) const
    {
        return true;
    }
};

/**
 * Predicate that always returns false.
 */
template<typename E>
struct None : public std::unary_function<E, bool>
{
    bool operator()(E elem) const
    {
        return false;
    }
};

/**
 * Predicate using QString::startsWith() to determine element inclusion.
 */
class OSC_EXPORT StartsWith : public std::unary_function<const QString&, bool>
{
    public:
        StartsWith(const QString& start);
        bool operator()(const QString& elem) const;
    private:
        QString start;
};

/**
 * Predicate using the == operator to determine element inclusion.
 *
 * Use the Equals() free function for automatic template argument deduction.
 */
template<typename E>
class EqualsPredicate : public std::unary_function<E, bool>
{
    public:
        EqualsPredicate(E equals)
            : equals(equals)
        {
        }

        bool operator()(E elem) const
        {
            return elem == equals;
        }
    private:
        E equals;
};

/**
 * Provides automatic template argument deduction for EqualsPredicate.
 */
template<typename E>
EqualsPredicate<E> Equals(E equals)
{
    return EqualsPredicate<E>(equals);
}

/**
 * Predicate using QString::contains() to determine element inclusion.
 */
class OSC_EXPORT Contains : public std::unary_function<const QString&, bool>
{
    public:
        Contains(const QString& contains);
        bool operator()(const QString& elem) const;
    private:
        QString contains;
};

/**
 * Composes two predicates evauluating to true if either of the predicates
 * evaluates to true.
 *
 * Use the Or() free function for automatic template argument deduction.
 */
template<typename P1, typename P2>
class OrPredicate : public std::unary_function<typename P1::argument_type, bool>
{
    public:
        OrPredicate(const P1& lhs, const P2& rhs)
            : lhs(lhs), rhs(rhs)
        {
        }

        bool operator()(typename P1::argument_type elem) const
        {
            return lhs(elem) || rhs(elem);
        }
    private:
        P1 lhs;
        P2 rhs;
};

/**
 * Composes two predicates evauluating to true only if both of the predicates
 * evaluates to true.
 *
 * Use the And() free function for automatic template argument deduction.
 */
template<typename P1, typename P2>
class AndPredicate : public std::unary_function<typename P1::argument_type, bool>
{
    public:
        AndPredicate(const P1& lhs, const P2& rhs)
            : lhs(lhs), rhs(rhs)
        {
        }

        bool operator()(typename P1::argument_type elem) const
        {
            return lhs(elem) && rhs(elem);
        }
    private:
        P1 lhs;
        P2 rhs;
};

/**
 * Provides automatic template argument deduction for OrPredicate.
 */
template<typename Pred1, typename Pred2>
OrPredicate<Pred1, Pred2> Or(const Pred1& lhs, const Pred2& rhs)
{
    return OrPredicate<Pred1, Pred2>(lhs, rhs);
}

/**
 * Provides automatic template argument deduction for AndPredicate.
 */
template<typename Pred1, typename Pred2>
AndPredicate<Pred1, Pred2> And(const Pred1& lhs, const Pred2& rhs)
{
    return AndPredicate<Pred1, Pred2>(lhs, rhs);
}
