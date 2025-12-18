#include "MainWindow.h"
#include "inventory.h"
#include "qdebug.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);  // Loads the UI from .ui file

    connect(ui->inventoryButton, &QPushButton::clicked, [this]() {
        Inventory *dialog = new Inventory(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

//         Connect dialog's signal to MainWindow's slot
        connect(dialog, &Inventory::componentSelected,
                this, &MainWindow::onComponentSelected);

        dialog->show();
    });
}

void MainWindow::onComponentSelected(const QString &componentName)
{
    qDebug() << "Component selected:" << componentName;
}

MainWindow::~MainWindow() {
    delete ui;
}
