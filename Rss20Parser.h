#ifndef RSS20PARSER_H
#define RSS20PARSER_H

#include "AbstractParser.h"

class Rss20Parser : public AbstractParser{
    public:
        Rss20Parser();
        void parseRss(const QString& xml, QList<TRssItem>& items);
        QDateTime parseDateTime(const QString& str) const;
};

#endif // RSS20PARSER_H
