#include "cachemanager.h"
#include <thread>

CacheManager::CacheManager(int cache_size): cache_max_size(cache_size)
{
}
CacheManager::~CacheManager()
{
    clear();
}

void CacheManager::clear()
{
    cache.clear();
}

QImage & CacheManager::getItem(QString path, QSize size)
{
    for (auto & c : cache)
    {
        if (c.name == path && c.size == size)
            return c.image;
    }
    return processItem(path, size);
}

QImage & CacheManager::processItem(QString path, QSize size)
{
    if (cache.size() > cache_max_size )
        cache.pop_front();
    cache.append(CacheItem(path, size, processImage(path, size)));

    return cache.back().image;
}

QImage CacheManager::processImage(QString path, QSize size)
{
    std::string fullpath = path.toStdString();
    cv::Mat cvimage = cv::imread(fullpath, CV_LOAD_IMAGE_COLOR);

    QImage img = QImage((const unsigned char*)(cvimage.data), cvimage.cols, cvimage.rows, QImage::Format_RGB888).rgbSwapped();
    return img.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
}
