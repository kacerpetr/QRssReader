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
        bool parseRss(const QString& xml, TRssData& rssData);
};

#endif // RSS10PARSER_H
