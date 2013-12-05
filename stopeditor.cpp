#include "stopeditor.h"
#include "ui_stopeditor.h"
#include <QColorDialog>
#include "stop.h"

StopEditor::StopEditor(Stop *stop, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopEditor),
    stop(stop)
{
    ui->setupUi(this);
    ui->horizontalSlider->setValue(stop->getPosition());
    connect(stop,SIGNAL(changedPosition(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),stop,SLOT(setPosition(int)));
    connect(stop, SIGNAL(deleted()), this, SLOT(deleteThis()));
    ui->colorButton->setAutoFillBackground(true);
    ui->colorButton->setPalette(QPalette(stop->getColor()));
}

StopEditor::~StopEditor()
{
    delete ui;
}

void StopEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void StopEditor::on_deleteButton_clicked()
{
    stop->deleteStop();
}

void StopEditor::on_colorButton_clicked()
{
    QColor color;
    if((color = QColorDialog::getColor(stop->getColor(), this)).isValid())
    {
        stop->setColor(color);
        ui->colorButton->setPalette(QPalette(color));
    }
}

void StopEditor::deleteThis()
{
    emit deleteMe(this);
}

void StopEditor::on_horizontalSlider_valueChanged(int value)
{
    ui->doubleSpinBox->setValue(double(value)/1000);
}
