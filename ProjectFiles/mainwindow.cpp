#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->translateXSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setDX(int)));
    connect(ui->translateYSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setDY(int)));
    connect(ui->translateZSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setDZ(int)));

    connect(ui->scaleXSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setSX(int)));
    connect(ui->scaleYSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setSY(int)));
    connect(ui->scaleZSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setSZ(int)));

    connect(ui->rotateXSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setAngleX(int)));
    connect(ui->rotateYSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setAngleY(int)));
    connect(ui->rotateZSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setAngleZ(int)));

    connect(ui->shearXSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setSHX(int)));
    connect(ui->shearYSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setSHY(int)));

    //connect(ui->reset,SIGNAL(clicked()),this,SLOT(reset()));

    ui->translateXSlider->setRange(-250, 250);
    ui->translateYSlider->setRange(-250, 250);
    ui->translateZSlider->setRange(-250, 250);

    ui->scaleXSlider->setRange(0, 200);
    ui->scaleYSlider->setRange(0, 200);
    ui->scaleZSlider->setRange(0, 200);
    ui->scaleXSlider->setValue(100);
    ui->scaleYSlider->setValue(100);
    ui->scaleZSlider->setValue(100);

    ui->rotateXSlider->setRange(0, 360);
    ui->rotateYSlider->setRange(0, 360);
    ui->rotateZSlider->setRange(0, 360);

    ui->shearXSlider->setRange(-100, 100);
    ui->shearYSlider->setRange(-100, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

