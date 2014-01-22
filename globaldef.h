#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include <QString>
#include <QColor>

typedef struct{
    QString name;
    QString url;
    QString description;
    QColor bkgColor;
    QColor textColor;
    bool enabled;
}FeedItem;

bool operator==(const FeedItem& item1, const FeedItem& item2){
    if(item1.name == item2.name) return true;
    if(item1.url == item2.url) return true;
    return false;
}

#endif // GLOBALDEF_H
