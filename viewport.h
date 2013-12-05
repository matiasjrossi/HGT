#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>

class QImage;

class Viewport : public QWidget
{
    Q_OBJECT
public:
    Viewport(QWidget *parent=NULL);
    void setImage(QImage *image);
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private:
    QImage *image;
    bool shrink;
signals:
    void resized(QSize newSize);
    void shrinkPolicyChanged(bool s);
};

#endif // VIEWPORT_H
