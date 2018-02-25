#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QObject>
#include <QImage>
#include <opencv2/highgui/highgui.hpp>

#define MAX_CACHE_ITEMS 20

struct CacheItem
{
    QString name;
    QSize size;
    QImage image;

    CacheItem(QString n, QSize s, QImage i): name(n), size(s), image(i) {}
};

class CacheManager : public QObject
{

private:
    const int cache_max_size = MAX_CACHE_ITEMS;
    QImage & processImage(QString & path, QSize & size);
public:
    QList<CacheItem> cache;
    explicit CacheManager(QObject *parent = nullptr);
    bool checkItem(QString path, QSize size);
    QImage & addItem(QString path, QSize size, QImage & item);
    QImage & getItem(QString path, QSize size);
    void clear(void);
};

#endif // CACHEMANAGER_H
