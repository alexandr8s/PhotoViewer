#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QListWidgetItem>
#include <opencv2/highgui/highgui.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT

private:
    std::mutex mx;

public:
    explicit ImageProcessor(QObject *parent = nullptr);

signals:
    void ready(QImage item, QListWidgetItem * current);

public slots:
    void load(QString path, QSize size, QListWidgetItem * current);
};

#endif // IMAGEPROCESSOR_H
