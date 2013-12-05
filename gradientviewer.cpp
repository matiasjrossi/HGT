#include "gradientviewer.h"
#include "gradient.h"
#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>
//#include "logger.h"


GradientViewer::GradientViewer(QWidget *parent, Gradient *gradient, Orientation orientation) :
        QWidget(parent),
        gradient(gradient),
        orientation(orientation),
        range(0),
        offset(0),
        tooltip(""),
        mouseLastPos(QPoint())
{
    connect(gradient,SIGNAL(changed()),this,SLOT(update()));
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
}

void GradientViewer::setGradient(Gradient *gradient)
{
    this->gradient = gradient;
    update();
}

Gradient *GradientViewer::getGradient()
{
    return gradient;
}

void GradientViewer::setScale(int offset, int range, QString tooltip)
{
    this->offset = offset;
    this->range = range;
    this->tooltip = tooltip;
}

void GradientViewer::paintEvent(QPaintEvent *)
{
    QImage image(this->size(), QImage::Format_RGB32);
    switch(orientation)
    {
    case HORIZONTAL:
        for (int i=0; i<image.width(); i++)
            for (int j=0; j<image.height(); j++)
                image.setPixel(i, j, gradient->getColorF(float(i)/image.width()).rgb());
        break;
    case VERTICAL:
        for (int i=0; i<image.width(); i++)
            for (int j=0; j<image.height(); j++)
                image.setPixel(i, j, gradient->getColorF(float(image.height()-j-1)/image.height()).rgb());
        break;
    case DIAGONAL:
        for (int i=0; i<image.width(); i++)
            for (int j=0; j<image.height(); j++)
                image.setPixel(i, j, gradient->getColorF(float(i+j)/(image.height()+image.width())).rgb());
        break;
    }
    QPainter painter(this);
    painter.drawImage(QPoint(0,0),image);
}

void GradientViewer::mouseMoveEvent(QMouseEvent *event)
{
    mouseLastPos = event->pos();
}

bool GradientViewer::event(QEvent *event)
{
    if (range != 0 && event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        int value = 0;
        switch (orientation)
        {
        case VERTICAL:
            value = (float(height()-mouseLastPos.y()-1)/height())*range+offset;
            break;
        case HORIZONTAL:
            value = (float(mouseLastPos.x())/width())*range+offset;
            break;
        default:
            return QWidget::event(event);
        }
        QToolTip::showText(helpEvent->globalPos(), tooltip.arg(value));
    }
    return QWidget::event(event);
}
