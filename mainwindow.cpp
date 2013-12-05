#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewport.h"
#include "imagegenerator.h"
#include "elevationmodel.h"
#include "gradientviewer.h"
#include "gradient.h"
#include "gradienteditor.h"
#include "stop.h"
#include "gradientadjuster.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QScrollArea>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vp(new Viewport()),
    gOrig(new Gradient(":/palettes/default.palette")),
    gAdjusted(NULL),
    g(gOrig),
    gv(new GradientViewer(this,g,GradientViewer::VERTICAL)),
    em(NULL),
    lastDir("./Examples"),
    current(ORIGINAL)

{
    ui->setupUi(this);
    ui->scrollArea->setWidget(vp);
    connect(vp,SIGNAL(shrinkPolicyChanged(bool)),this,SLOT(scrollArea_setWidgetResizable(bool)));
    ui->dockWidget->setWidget(gv);
    gv->setScale(-32768,65536,QString("%1 m.a.s.l."));
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionLoad_palette);
    menu->addAction(ui->actionSave_palette);
    menu->addAction(ui->actionEdit);
    ui->actionPalette_Editor->setMenu(menu);
    connect(ui->actionPalette_Editor,SIGNAL(triggered()),ui->actionEdit,SIGNAL(triggered()));
    ui->actionAutoPalette->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::scrollArea_setWidgetResizable(bool s)
{
    ui->scrollArea->setWidgetResizable(s);
}

void MainWindow::setGradient(Gradient *g)
{
    this->g = NULL;
    if (this->gOrig != NULL)
        delete this->gOrig;
    this->gOrig = g;

    if (this->gAdjusted != NULL) {
        delete this->gAdjusted;
        this->gAdjusted = NULL;
    }

    if (em != NULL)
        this->gAdjusted = GradientAdjuster::getAdjusted(gOrig, em);
    this->useGradient(this->current);
}

void MainWindow::useGradient(GradientSelection selection)
{
    this->current = selection;
    if (selection == ORIGINAL)
        this->g = this->gOrig;
    else
        this->g = this->gAdjusted;
    gv->setGradient(this->g);
    if (em != NULL)
        vp->setImage(ImageGenerator::generate(this->em,this->g));
    else
        vp->setImage(new QImage());
}

void MainWindow::on_actionDEMOpen_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, QString("Select the file"), lastDir, "SRTM DEMs (*.hgt *.M01)");
    if (filepath == NULL) return;
    lastDir = filepath.left(filepath.lastIndexOf(QChar('/')));

    if (em != NULL)
        delete em;
    em = NULL;
    if (gAdjusted != NULL)
        delete gAdjusted;
    gAdjusted = NULL;

    try {
        em = new ElevationModel(filepath);
        gAdjusted = GradientAdjuster::getAdjusted(gOrig, em);
        useGradient(current);
        ui->actionAutoPalette->setEnabled(true);
    } catch(HGTFormatException e) {
        QMessageBox::critical(this, QString("Error opening file"), QString("Error opening file: %1:\n%2").arg(filepath).arg(e.getError()));
        ui->actionAutoPalette->setChecked(false);
        ui->actionAutoPalette->setEnabled(false);
        this->useGradient(ORIGINAL);
    }
}

void MainWindow::on_actionEdit_triggered()
{
    GradientEditor *ge = new GradientEditor(new Gradient(*gOrig), this);
    connect(ge,SIGNAL(accepted(Gradient*)),this,SLOT(setGradient(Gradient*)));
    ge->show();
}

void MainWindow::on_actionLoad_palette_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, QString("Select the palette to load"), lastDir, "Palettes (*.palette)");
    if (filepath == NULL) return;
    lastDir = filepath.left(filepath.lastIndexOf(QChar('/')));
    this->current = ORIGINAL;
    ui->actionAutoPalette->setChecked(false);
    setGradient(new Gradient(filepath));
}

void MainWindow::on_actionSave_palette_triggered()
{
    QString filepath = QFileDialog::getSaveFileName(this, QString("Save current palette as..."), lastDir+QString("/myPaletteFile"), "Palettes (*.palette)");
    if (filepath == NULL) return;
    lastDir = filepath.left(filepath.lastIndexOf(QChar('/')));
    if (!filepath.endsWith(".palette", Qt::CaseInsensitive))
        filepath = filepath.append(".palette");
    gOrig->save(filepath);
}

void MainWindow::on_actionAutoPalette_toggled(bool state)
{
    if (state == true)
        useGradient(ADJUSTED);
    else
        useGradient(ORIGINAL);
}
