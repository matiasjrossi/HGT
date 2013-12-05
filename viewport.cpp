#include "viewport.h"
#include <QPainter>
#include <QImage>
#include "logger.h"

Viewport::Viewport(QWidget *parent) :
        QWidget(parent),
        image(NULL),
        shrink(true)
{
    setCursor(Qt::PointingHandCursor);
}

void Viewport::setImage(QImage *image)
{
    if (this->image != NULL) {
        delete this->image;
    }
    this->image = image;
    update();
}

void Viewport::paintEvent(QPaintEvent *)
{
    if (image != NULL) {
        QPainter painter(this);
        if (shrink){
            QRect area(geometry());
            if (width() > height())
            {
                area.setLeft((width() - height())/2);
                area.setRight(area.left()+height());
            }
            else
            {
                area.setTop((height() - width())/2);
                area.setBottom(area.top()+width());
            }
            painter.drawImage(area,*image);
        }
        else
        {
            resize(image->size());
            painter.drawImage(0,0,*image);
        }
    }
}

void Viewport::resizeEvent(QResizeEvent *)
{
    emit resized(this->size());
}

void Viewport::mouseReleaseEvent(QMouseEvent *)
{
    shrink = !shrink;
    emit shrinkPolicyChanged(shrink);
    update();
}
