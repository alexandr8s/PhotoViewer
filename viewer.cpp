#include "viewer.h"
#include "ui_viewer.h"
#include <QList>
#include <QFileDialog>
#include <QScrollBar>
#include <QAbstractItemView>


Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);

    connect(ui->list->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(loadInspector()));
    connect(this, SIGNAL(loadTransmitter(QString, QSize, QListWidgetItem*)), &iProcessor, SLOT(load(QString, QSize, QListWidgetItem*)));
    connect(&iProcessor, SIGNAL(ready(QImage, QListWidgetItem*)), this, SLOT(loadReceiver(QImage, QListWidgetItem*)));

    cThread.start();
    iProcessor.moveToThread(&cThread);
}

Viewer::~Viewer()
{
    cThread.quit();
    cThread.wait();
    delete ui;
}

void Viewer::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (ui->list->currentItem())
        drawImage(ui->list->currentItem());

    manager.clear();
    loadInspector();
}

void Viewer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case (Qt::Key_Left):
            ui->previous->animateClick();
        break;
        case (Qt::Key_Right):
            ui->next->animateClick();
        break;
        case (Qt::Key_Space):
            ui->select->animateClick();
        break;
    }
}

QImage & Viewer::drawImage(QListWidgetItem *current)
{
    QImage & currImage = manager.getItem(workDir + "/" + current->text(), ui->label->size());
    ui->label->setPixmap(QPixmap::fromImage(currImage, Qt::AutoColor));

    return currImage;
}

void Viewer::drawThumbnail(QListWidgetItem *current, QImage & currImage)
{
    QImage scaledImage = currImage.scaled(160, 120, Qt::KeepAspectRatio);

    QLabel * currentLabel = new QLabel();
    currentLabel->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));

    currentLabel->setContentsMargins(5, 7, 5, 18);
    currentLabel->setAlignment(Qt::AlignCenter);

    current->setSizeHint(currentLabel->sizeHint());
    current->listWidget()->setItemWidget(current, currentLabel);
}

void Viewer::on_list_currentItemChanged(QListWidgetItem *current)
{
    if (current)
    {
        QImage & currImage = drawImage(current);
        drawThumbnail(current, currImage);

        this->setWindowTitle(current->text());
    }
}

void Viewer::on_previous_clicked()
{
    int curr = ui->list->currentRow();
    if (curr != 0)
        ui->list->setCurrentRow(--curr);

    ui->list->scrollToItem(ui->list->currentItem(), QAbstractItemView::PositionAtCenter);
}

void Viewer::on_next_clicked()
{
    int curr = ui->list->currentRow();
    if (curr != ui->list->count() - 1 )
        ui->list->setCurrentRow(++curr);

    ui->list->scrollToItem(ui->list->currentItem(), QAbstractItemView::PositionAtCenter);
}

void Viewer::on_select_clicked()
{
    if (ui->list->currentItem())
    {
        QDir dir(workDir);
        dir.mkdir("Trash");
        bool success = dir.rename(ui->list->currentItem()->text(), "Trash/" + ui->list->currentItem()->text());
        if (success)
            delete ui->list->takeItem(ui->list->currentRow());

        loadInspector();
    }
}

void Viewer::on_tool_clicked()
{
    QString tempDir = QFileDialog::getExistingDirectory(this, "Open Directory", workDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (tempDir.isEmpty()) return;

    workDir = tempDir;
    ui->tool->setText(workDir);
    QDir dir(workDir);

    if (dir.isReadable())
    {
        ui->list->clear();
        manager.clear();

        for (auto file : dir.entryList(QDir::Files, QDir::Name))
        {
            QListWidgetItem * thumbnailItem = new QListWidgetItem();
            thumbnailItem->setText(file);
            thumbnailItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
            thumbnailItem->setSizeHint(QSize(160, 120));
            ui->list->addItem(thumbnailItem);
        }

        ui->list->setCurrentRow(0);
        loadInspector();
    }
}

void Viewer::loadReceiver(QImage item, QListWidgetItem * current)
{
    QImage & currImage = manager.addItem(workDir + "/" + current->text(), ui->label->size(), item);
    drawThumbnail(current, currImage);

    loadInspector();
}

void Viewer::loadInspector(void)
{
    disconnect(ui->list->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(loadInspector()));
    QRect listRect = ui->list->contentsRect();

    for (int visibleIndex = ui->list->indexAt(QPoint(1,0)).row(); visibleIndex < ui->list->count(); visibleIndex++)
    {
        QListWidgetItem * current = ui->list->item(visibleIndex);

        QRect itemRect = ui->list->visualItemRect(current);
        if (listRect.intersects(itemRect))
        {
            if (manager.checkItem(workDir + "/" + current->text(), ui->label->size()))
                continue;
            else
                emit loadTransmitter(workDir + "/" + current->text(), ui->label->size(), current);
            return;
        }
        break;
     }
    connect(ui->list->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(loadInspector()));
}
