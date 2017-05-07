#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  opengl = new GLWidget();
  ui->setupUi(this);
  ui->glScrollArea->setWidgetResizable(true);
  ui->glScrollArea->setWidget(opengl);
  opengl->setFocus();

  QObject::connect(ui->arcButton, SIGNAL(clicked()), this,
                   SLOT(resetOpenGLView()));
}

void MainWindow::resetOpenGLView() { opengl->resetArcballAndZoom(); }

MainWindow::~MainWindow() {
  delete opengl;
  delete ui;
}
