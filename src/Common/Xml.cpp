#include "Xml.h"

#include <QtCore/QString>

QString Xml::encode(const QString& data)
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

QString Xml::decode(const QString& data)
{
    QString temp(data);

    temp.replace("&amp;", "&");
    temp.replace("&apos;", "'");
    temp.replace("&quot;", "\"");
    temp.replace("&lt;", "<");
    temp.replace("&gt;", ">");

    return temp;
}
