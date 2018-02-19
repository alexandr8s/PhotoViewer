#include "viewer.h"
#include "ui_viewer.h"
#include <QFileDialog>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);
}

Viewer::~Viewer()
{
    delete ui;
}
void Viewer::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (ui->list->currentItem())
        drawImage(ui->list->currentItem());
}

QImage & Viewer::drawImage(QListWidgetItem *current)
{
    QImage & currImage = manager.getItem(workDir + "/" + current->text(), ui->label->size());
    ui->label->setPixmap(QPixmap::fromImage(currImage, Qt::AutoColor));
    ui->label->show();

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
    ui->list->setItemWidget(current, currentLabel);
}

void Viewer::on_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
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
    if (curr == 0)
        curr = ui->list->count();
    else
        curr--;
    ui->list->setCurrentRow(curr);
}

void Viewer::on_next_clicked()
{
    int curr = ui->list->currentRow();
    if (curr == ui->list->count())
        curr = 0;
    else
        curr++;
    ui->list->setCurrentRow(curr);
}

void Viewer::on_select_clicked()
{
    if (ui->list->currentItem())
        if (ui->list->currentItem()->background() == Qt::NoBrush)
            ui->list->currentItem()->setBackground(Qt::Dense4Pattern);
        else
            ui->list->currentItem()->setBackground(Qt::NoBrush);
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
            QListWidgetItem * thumbnailItem= new QListWidgetItem();
            thumbnailItem->setText(file);
            thumbnailItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
            ui->list->addItem(thumbnailItem);
        }

        ui->list->setCurrentRow(0);
    }
}


