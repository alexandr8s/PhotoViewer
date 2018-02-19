#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QImage>
#include <opencv2/highgui/highgui.hpp>

struct CacheItem
{
    QString name;
    QSize size;
    QImage image;

    CacheItem(QString n, QSize s, QImage i): name(n), size(s), image(i) {}
};

class CacheManager
{

private:
    int cache_max_size;
public:
    QList<CacheItem> cache;
    CacheManager(int cache_size = 10);
    ~CacheManager();
    QImage & getItem(QString path, QSize size);
    QImage & processItem(QString path, QSize size);
    QImage processImage(QString path, QSize size);
    void clear();
};

#endif // CACHEMANAGER_H
