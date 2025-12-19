#include "inventory.h"
#include "InventoryManager.h"
#include "qdebug.h"
#include "ui_inventory.h"

Inventory::Inventory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inventory)
{
    ui->setupUi(this);
    InventoryManager& im = InventoryManager::getInstance();

    if (im.init("../devices/"))
    {
        for (auto& device : im.getSupportedDevices())
        {
            ui->inventoryList->addItem(QString::fromStdString(device));
        }
    }
    else
    {
        qDebug() << "Failed to initialize the InventoryManager!";
    }

    // Connect item click to emit signal
    connect(ui->inventoryList, &QListWidget::itemDoubleClicked,
            this, &Inventory::on_listWidget_itemDoubleClicked);

    // Or if you have an OK button
//    connect(ui->pushButton_ok, &QPushButton::clicked,
//            this, &InventoryDialog::on_pushButton_ok_clicked);
}

void Inventory::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // Emit the signal with the component name
    emit componentSelected(item->text());

    // Optional: close dialog after selection
    accept();  // Closes dialog with QDialog::Accepted
}

//void InventoryDialog::on_pushButton_ok_clicked()
//{
//    QListWidgetItem *currentItem = ui->listWidget->currentItem();
//    if (currentItem) {
//        // Emit the signal with selected component
//        emit componentSelected(currentItem->text());
//    }
//    accept();
//}

Inventory::~Inventory()
{
    delete ui;
}
