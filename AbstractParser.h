#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include "globaldef.h"

/**
 * @brief The abstract rss parser class
 */
class AbstractParser{
    public:
        virtual void parseRss(const QString& xml, QList<TRssItem>& items) = 0;
        static ERssVersion parseVersion(const QString& xml);
};

#endif // ABSTRACTPARSER_H
