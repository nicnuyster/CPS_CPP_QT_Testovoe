#include "mainwindow.h"
#include "./ui_mainwindow.h"

// MainWindow constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800,600);
    setWindowTitle("MyQtApp");
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// functions