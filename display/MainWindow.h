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

QVector<QPointF> analyzeComponentImage(const QString& imagePath);
void onComponentSelected(const QString &componentName);
QVector<QPointF> getComponentTerminals(QGraphicsPixmapItem *component);
void drawWire(const QPointF &start, const QPointF &end);
QPointF findNearestTerminal(const QPointF &scenePos, qreal maxDistance);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_wiringButton_toggled(bool checked);

private:
bool m_wiringMode = false;
QPointF m_firstTerminal;
bool m_waitingForSecondClick = false;

private:
QVector<QPointF> m_pendingWirePoints;
bool m_isFirstClick = true;

private:
    Ui::MainWindow* ui;
};
