#ifndef RSS10PARSER_H
#define RSS10PARSER_H

#include <QString>
#include <QList>
#include "globaldef.h"
#include "AbstractParser.h"

/**
 * @brief Rss 1.0 parser
 */
class Rss10Parser : public AbstractParser{
    public:
        Rss10Parser();
        void parseRss(const QString& xml, QList<TRssItem>& items);
        QDateTime parseDateTime(const QString& str) const;
};

#endif // RSS10PARSER_H
