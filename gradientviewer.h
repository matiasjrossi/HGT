#ifndef GRADIENTVIEWER_H
#define GRADIENTVIEWER_H

#include <QWidget>

class Gradient;

class GradientViewer : public QWidget
{
    Q_OBJECT
public:
    typedef enum { HORIZONTAL = 0, VERTICAL, DIAGONAL } Orientation;
    GradientViewer(QWidget *parent=NULL, Gradient *gradient=NULL, Orientation orientation = HORIZONTAL);
    void setGradient(Gradient*);
    Gradient *getGradient();
    void setScale(int offset, int range, QString tooltip);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool event(QEvent *);
private:
    Gradient *gradient;
    Orientation orientation;
    int range;
    int offset;
    QString tooltip;
    QPoint mouseLastPos;
};

#endif // GRADIENTVIEWER_H
