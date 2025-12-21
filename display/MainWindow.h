#pragma once
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

QVector<QPointF> analyzeComponentImage(const QImage& image);
QVector<QPointF> getComponentTerminalsInScene(QGraphicsPixmapItem* component);
void onComponentSelected(const QString &componentName);

private:
    Ui::MainWindow* ui;
};
