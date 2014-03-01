#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include "globaldef.h"

/**
 * @brief The abstract rss parser class
 */
class AbstractParser{
    public:
        virtual bool parseRss(const QString& xml, TRssData& rssData) = 0;
};

#endif // ABSTRACTPARSER_H
