#ifndef GRADIENT_H
#define GRADIENT_H

#include <QColor>
#include <QObject>
#include <QVector>

class Stop;

class Gradient : public QObject
{
    Q_OBJECT
public:
    //Gradient class prototypes
    Gradient();
    Gradient(Gradient &other);
    Gradient(QString filename);
    void save(QString filename);
    QColor getColor(short) const;
    QColor getColorF(float) const;
    void addStop(Stop *stop);
    const QVector<Stop*> &getStops() const;
public slots:
    void deleteStop(Stop *stop);
signals:
    void changed();
private:
    QVector<Stop*> stops;
private slots:
    void update();
};

#endif // GRADIENT_H
