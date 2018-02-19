#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QImage>
#include <QListWidget>
#include "cachemanager.h"


#define MAX_CACHE_PRELOAD 10

namespace Ui {
class Viewer;
}

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    CacheManager manager = CacheManager(MAX_CACHE_PRELOAD);
    explicit Viewer(QWidget * parent = 0);
    void resizeEvent(QResizeEvent * event);
    ~Viewer();

private slots:
    void on_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_previous_clicked();
    void on_next_clicked();
    void on_tool_clicked();

    void on_select_clicked();

private:
    QString workDir = "/home";
    QImage & drawImage(QListWidgetItem *current);
    void drawThumbnail(QListWidgetItem *current, QImage & currImage);
    Ui::Viewer *ui;
};

#endif // VIEWER_H
