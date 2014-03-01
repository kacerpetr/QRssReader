#ifndef RSS20PARSER_H
#define RSS20PARSER_H

#include "AbstractParser.h"

class Rss20Parser : public AbstractParser{
    public:
        Rss20Parser();
        bool parseRss(const QString& xml, TRssData& rssData);
};

#endif // RSS20PARSER_H
