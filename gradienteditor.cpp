#include "gradienteditor.h"
#include "ui_gradienteditor.h"
#include "stopeditor.h"
#include "gradientviewer.h"
#include "gradient.h"
#include "stop.h"

GradientEditor::GradientEditor(Gradient *gradient, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientEditor),
    gradient(gradient)
{
    ui->setupUi(this);
    GradientViewer *gv = new GradientViewer(this,this->gradient, GradientViewer::HORIZONTAL);
    ui->frame->layout()->addWidget(gv);
    connect(this, SIGNAL(updateMe()), gv, SLOT(update()));
    QVector<Stop*> stops = gradient->getStops();
    for (int i=0; i<stops.size(); i++)
        addStopEditor(new StopEditor(stops[i]));
}

GradientEditor::~GradientEditor()
{
    delete ui;
}

Gradient *GradientEditor::getGradient()
{
    return gradient;
}

void GradientEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GradientEditor::addStopEditor(StopEditor *se)
{
    ui->verticalLayout->addWidget(se);
    connect(se, SIGNAL(deleteMe(StopEditor*)), this, SLOT(deleteStopEditor(StopEditor*)));
}

void GradientEditor::on_addStopButton_clicked()
{
    Stop *s = new Stop();
    StopEditor *se = new StopEditor(s,this);
    gradient->addStop(s);
    addStopEditor(se);
}

void GradientEditor::on_buttonBox_accepted()
{
    emit accepted(gradient);
}

void GradientEditor::deleteStopEditor(StopEditor *se)
{
    ui->verticalLayout->removeWidget(se);
    delete se;
    emit updateMe();
}
