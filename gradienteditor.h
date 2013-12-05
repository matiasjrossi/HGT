#ifndef GRADIENTEDITOR_H
#define GRADIENTEDITOR_H

#include <QDialog>

namespace Ui {
    class GradientEditor;
}

class Gradient;
class StopEditor;

class GradientEditor : public QDialog {
    Q_OBJECT
public:
    GradientEditor(Gradient *gradient, QWidget *parent = 0);
    ~GradientEditor();
    Gradient *getGradient();

protected:
    void changeEvent(QEvent *e);

private:
    void addStopEditor(StopEditor*);
    Ui::GradientEditor *ui;
    Gradient *gradient;

private slots:
    void on_buttonBox_accepted();
    void on_addStopButton_clicked();
    void deleteStopEditor(StopEditor*);

signals:
    void accepted(Gradient*);
    void updateMe();
};

#endif // GRADIENTEDITOR_H
