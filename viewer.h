#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include <QListWidget>
#include "cachemanager.h"
#include "imageprocessor.h"

namespace Ui {
class Viewer;
}

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    CacheManager manager;
    QThread cThread;
    ImageProcessor iProcessor;
    explicit Viewer(QWidget * parent = 0);
    void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent *event);
    ~Viewer();

private:
    QString workDir = "/home";
    QImage & drawImage(QListWidgetItem *current);
    void drawThumbnail(QListWidgetItem *current, QImage & currImage);
    Ui::Viewer *ui;

signals:
    void loadTransmitter(QString path, QSize size, QListWidgetItem * current);

public slots:
    void loadInspector(void);
    void loadReceiver(QImage currImage, QListWidgetItem * current);

private slots:
    void on_list_currentItemChanged(QListWidgetItem *current);
    void on_previous_clicked();
    void on_next_clicked();
    void on_tool_clicked();
    void on_select_clicked();


};

#endif // VIEWER_H
