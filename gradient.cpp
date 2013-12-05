#include "gradient.h"
#include "stop.h"
#include <QtAlgorithms>
#include <QFile>
#include <QDebug>

Gradient::Gradient()
{
}

Gradient::Gradient(Gradient &other) :
        QObject()
{
    for (int i=0; i<other.getStops().size(); i++)
        this->addStop(new Stop(*other.getStops().at(i)));
}

Gradient::Gradient(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line;
        while (!file.atEnd())
        {
            line = file.readLine();
            if (line.startsWith("stop:",Qt::CaseInsensitive))
            {
                int colonPos, slashPos, fCommmaPos, sCommaPos;
                colonPos = line.indexOf(":");
                slashPos = line.indexOf("/");
                fCommmaPos = line.indexOf(",");
                sCommaPos = line.lastIndexOf(",");
                QString position, red, green, blue;
                position = line.mid(colonPos + 1, slashPos - colonPos - 1);
                red = line.mid(slashPos + 1, fCommmaPos - slashPos - 1);
                green = line.mid(fCommmaPos + 1, sCommaPos - fCommmaPos - 1);
                blue = line.mid(sCommaPos + 1, -1);
                addStop(new Stop(position.toInt(),QColor(red.toInt(),green.toInt(),blue.toInt())));
            }
        }
        file.close();
    }
}

void Gradient::save(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for (int i=0; i<stops.size(); i++)
        {
            Stop* s = stops.at(i);
            file.write(QString("stop:%1/%2,%3,%4\n").
                       arg(s->getPosition()).
                       arg(s->getColor().red()).
                       arg(s->getColor().green()).
                       arg(s->getColor().blue()).
                       toAscii());
        }
    }
}

QColor Gradient::getColor(short position) const
{
    return getColorF(float(position)/GRADIENT_RANGE);
}

QColor Gradient::getColorF(float position) const
{
    if (stops.empty())
        return QColor(0,0,0);
    short i = 0, previous, next = -1;
    do
    {
        previous = next;
        next = i;
    }
    while (i<stops.size() && position > (float(stops[i++]->getPosition())/GRADIENT_RANGE));
    if (next == stops.size())
        return stops[next-1]->getColor();
    if (previous == -1)
        return stops[0]->getColor();
    Stop pStop = *(stops[previous]);
    Stop nStop = *(stops[next]);
    float prevRelDistance = (GRADIENT_RANGE*position-pStop.getPosition())/
                            (nStop.getPosition()-pStop.getPosition());
    return QColor(
            (1.0 - prevRelDistance)*pStop.getColor().red() + prevRelDistance*nStop.getColor().red(),
            (1.0 - prevRelDistance)*pStop.getColor().green() + prevRelDistance*nStop.getColor().green(),
            (1.0 - prevRelDistance)*pStop.getColor().blue() + prevRelDistance*nStop.getColor().blue());
}

void Gradient::addStop(Stop *stop)
{
    stops.push_back(stop);
    connect(stop, SIGNAL(changed()), this, SLOT(update()));
    connect(stop,SIGNAL(deleted(Stop*)),this, SLOT(deleteStop(Stop*)));
    update();
}

const QVector<Stop*> &Gradient::getStops() const
{
    return stops;
}

void Gradient::deleteStop(Stop *stop)
{
    stops.remove(stops.indexOf(stop,0));
}

bool stopsLessThan(const Stop *s1, const Stop *s2)
{
    return s1->operator <(*s2);
}

void Gradient::update()
{
    qSort(stops.begin(), stops.end(), stopsLessThan);
    emit changed();
}
