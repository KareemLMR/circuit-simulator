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
    bool m_waitingToDrop;
    bool m_startWiringPosDetermined;
    std::map<QGraphicsPixmapItem*, QVector<QGraphicsLineItem*>> m_componentsWires;
    std::pair<std::shared_ptr<Node>, QPointF> m_currentWireStartPoint;
    std::pair<std::shared_ptr<Node>, QPointF> m_currentWireEndPoint;
    QPointF m_deltaDistByComponent;
    CircuitManager& cm;
    Orchestrator& oc;
    InventoryManager& im;
    std::map<QGraphicsPixmapItem*, std::map<std::shared_ptr<Node>, QPointF>> m_componentsNodesMap;
};
