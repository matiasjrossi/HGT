#ifndef STOP_H
#define STOP_H

#include <QObject>
#include <QColor>
#define GRADIENT_RANGE 100000

class Stop : public QObject
{
    Q_OBJECT
public:
    Stop();
    Stop(int,QColor);
    Stop(Stop &other);
    bool operator<(const Stop &other) const;
    int getPosition() const;
    QColor getColor() const;
public slots:
    void deleteStop();
    void setPosition(int);
    void setColor(QColor);
signals:
    void deleted(Stop*);
    void deleted();
    void changed();
    void changedPosition(int);
private:
    int position;
    QColor color;
};

#endif // STOP_H
