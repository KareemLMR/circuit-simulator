#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);  // Loads the UI from .ui file
}

MainWindow::~MainWindow() {
    delete ui;
}
