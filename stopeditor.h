#ifndef STOPEDITOR_H
#define STOPEDITOR_H

#include <QWidget>

namespace Ui {
    class StopEditor;
}

class Stop;

class StopEditor : public QWidget
{
    Q_OBJECT
public:
    StopEditor(Stop *stop, QWidget *parent = 0);
    ~StopEditor();
signals:
    void deleteMe(StopEditor*);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StopEditor *ui;
    Stop *stop;

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_deleteButton_clicked();
    void on_colorButton_clicked();
    void deleteThis();
};

#endif // STOPEDITOR_H
