#pragma once
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <map>
#include "InventoryManager.h"
#include "Node.h"
#include "CircuitManager.h"
#include "Orchestrator.h"
#include <QString>
#include <utility>

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
bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow* ui;
    bool m_wiringMode;
    int m_wiringClicksCounter;
    std::map<QGraphicsPixmapItem*, QVector<QGraphicsLineItem*>> m_componentsWires;
    QPointF currentWireStartPoint;
    QPointF currentWireEndPoint;
    CircuitManager& cm;
    Orchestrator& oc;
    InventoryManager& im;
    std::map<std::pair<double, double>, std::shared_ptr<Node>> m_nodesPointMap;
};
