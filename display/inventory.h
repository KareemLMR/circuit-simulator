#ifndef INVENTORY_H
#define INVENTORY_H

#include <QDialog>
#include "qlistwidget.h"

namespace Ui {
class Inventory;
}

class Inventory : public QDialog
{
    Q_OBJECT

public:
    explicit Inventory(QWidget *parent = nullptr);
    ~Inventory();

signals:
    void componentSelected(const QString &componentName);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
//    void on_pushButton_ok_clicked();

private:
    Ui::Inventory *ui;
};

#endif // INVENTORY_H
