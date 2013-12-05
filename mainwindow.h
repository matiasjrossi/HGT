#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
    class MainWindow;
}

class Viewport;
class Gradient;
class GradientViewer;
class ElevationModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    typedef enum {ORIGINAL, ADJUSTED} GradientSelection;
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void useGradient(GradientSelection);

protected slots:
    void scrollArea_setWidgetResizable(bool);
    void setGradient(Gradient*);

private:
    Ui::MainWindow *ui;
    Viewport *vp;
    Gradient *gOrig, *gAdjusted, *g;
    GradientViewer *gv;
    ElevationModel *em;
    QString lastDir;
    GradientSelection current;

private slots:
    void on_actionAutoPalette_toggled(bool );
    void on_actionSave_palette_triggered();
    void on_actionLoad_palette_triggered();
    void on_actionEdit_triggered();
    void on_actionDEMOpen_triggered();
};

#endif // MAINWINDOW_H
