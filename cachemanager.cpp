#include "cachemanager.h"
#include <qdebug.h>

CacheManager::CacheManager(QObject *parent) : QObject(parent)
{
}

bool CacheManager::checkItem(QString path, QSize size)
{
    for (auto & c : cache)
        if (c.name == path && c.size == size)
            return true;
    return false;
}

QImage & CacheManager::addItem(QString path, QSize size, QImage & item)
{
    if (cache.size() > cache_max_size)
        cache.pop_front();
    cache.append(CacheItem(path, size, item));
    return cache.back().image;
}

QImage & CacheManager::getItem(QString path, QSize size)
{
    for (auto & c : cache)
        if (c.name == path && c.size == size)
        {
            return c.image;
        }
    return processImage(path, size);
}

QImage & CacheManager::processImage(QString & path, QSize & size)
{
    if (cache.size() > cache_max_size)
        cache.pop_front();

    cv::Mat cvimage = cv::imread(path.toStdString(), CV_LOAD_IMAGE_COLOR);
    QImage img = QImage((const unsigned char*)(cvimage.data), cvimage.cols, cvimage.rows, QImage::Format_RGB888).rgbSwapped();

    cache.append(CacheItem(path, size, img.scaled(size.width(), size.height(), Qt::KeepAspectRatio)));

    return cache.back().image;
}

void CacheManager::clear(void)
{
    cache.clear();
}
