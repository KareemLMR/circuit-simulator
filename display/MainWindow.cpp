#include "MainWindow.h"
#include "inventory.h"
#include "qdebug.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);  // Loads the UI from .ui file

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);

    qDebug() << "GraphicsView created:" << ui->graphicsView;

    connect(ui->inventoryButton, &QPushButton::clicked, [this]() {
        Inventory *dialog = new Inventory(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

//         Connect dialog's signal to MainWindow's slot
        connect(dialog, &Inventory::componentSelected,
                this, &MainWindow::onComponentSelected);

        dialog->show();
    });

    connect(ui->wiringButton, &QPushButton::toggled,
            this, &MainWindow::on_wiringButton_toggled);
}

QVector<QPointF> MainWindow::analyzeComponentImage(const QString& imagePath)
{
    QImage image(imagePath);
//    if (image.isNull()) return;

    QVector<QPointF> terminals;
    int width = image.width();
    int height = image.height();

    // Simple clockwise scan
    // 1. Top edge (left to right)
    for (int x = 0; x < width; x++)
    {
        if (qGray(image.pixel(x, 0)) < 128)
        {
            terminals.append(QPointF(x, 0));
            break;  // Found first terminal
        }
    }

    // 2. Right edge (top to bottom)
    for (int y = 0; y < height; y++)
    {
        if (qGray(image.pixel(width-1, y)) < 128)
        {
            terminals.append(QPointF(width-1, y));
            break;
        }
    }

    // 3. Bottom edge (right to left)
    for (int x = width-1; x >= 0; x--)
    {
        if (qGray(image.pixel(x, height-1)) < 128)
        {
            terminals.append(QPointF(x, height-1));
            break;
        }
    }

    // 4. Left edge (bottom to top)
    for (int y = height-1; y >= 0; y--)
    {
        if (qGray(image.pixel(0, y)) < 128)
        {
            terminals.append(QPointF(0, y));
            break;
        }
    }

    qDebug() << "Terminals found at positions:" << terminals;

    // Use for wiring
    for (int i = 0; i < terminals.size(); i++)
    {
        qDebug() << "Terminal" << i << "at" << terminals[i];
    }
    return terminals;
}

void MainWindow::on_wiringButton_toggled(bool checked)
{
    m_wiringMode = checked;
    m_waitingForSecondClick = false;

    if (checked) {
        ui->graphicsView->setCursor(Qt::CrossCursor);
        qDebug() << "Wiring mode ON";
    } else {
        ui->graphicsView->setCursor(Qt::ArrowCursor);
        qDebug() << "Wiring mode OFF";
    }
}

// Handle mouse clicks
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // Only handle clicks on the graphics view
    if (watched != ui->graphicsView->viewport()) {
        return QMainWindow::eventFilter(watched, event);
    }

    // Only handle mouse press events
    if (event->type() != QEvent::MouseButtonPress) {
        return QMainWindow::eventFilter(watched, event);
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    // Only handle left clicks in wiring mode
    if (mouseEvent->button() != Qt::LeftButton || !m_wiringMode) {
        return QMainWindow::eventFilter(watched, event);
    }

    // Get click position in scene coordinates
    QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

    // Find nearest terminal
    QPointF nearestTerminal = findNearestTerminal(scenePos, 30);

    if (nearestTerminal.isNull()) {
        qDebug() << "Click on a component terminal";
        return true;  // Event handled
    }

    // First or second click?
    if (!m_waitingForSecondClick) {
        // First click - store terminal
        m_firstTerminal = nearestTerminal;
        m_waitingForSecondClick = true;
        qDebug() << "First terminal selected:" << nearestTerminal;
    } else {
        // Second click - draw wire
        drawWire(m_firstTerminal, nearestTerminal);
        m_waitingForSecondClick = false;
        qDebug() << "Wire drawn from" << m_firstTerminal << "to" << nearestTerminal;
    }

    return true;  // Event handled
}

// Find nearest terminal from any component
QPointF MainWindow::findNearestTerminal(const QPointF &scenePos, qreal maxDistance)
{
    QPointF nearestTerminal;
    qreal minDistance = maxDistance;

    // Get all items in scene
    QList<QGraphicsItem*> allItems = ui->graphicsView->scene()->items();

    for (QGraphicsItem *item : allItems) {
        // Check if it's a component (pixmap item)
        if (QGraphicsPixmapItem *component = qgraphicsitem_cast<QGraphicsPixmapItem*>(item)) {
            // Get terminals for this component
            QVector<QPointF> terminals = getComponentTerminals(component);

            // Check each terminal
            for (const QPointF &terminal : terminals) {
                qreal distance = QLineF(scenePos, terminal).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestTerminal = terminal;
                }
            }
        }
    }

    // Return terminal only if within maxDistance
    return (minDistance < maxDistance) ? nearestTerminal : QPointF();
}

// Get terminals for a component (simplified version)
QVector<QPointF> MainWindow::getComponentTerminals(QGraphicsPixmapItem *component)
{
    QVector<QPointF> terminals;

    // For now, just use component bounding box edges
    // You'll replace this with your edge detection code
    QRectF bounds = component->boundingRect();

    // Left center (terminal 1)
    terminals.append(component->mapToScene(bounds.left(), bounds.center().y()));

    // Right center (terminal 2)
    terminals.append(component->mapToScene(bounds.right(), bounds.center().y()));

    return terminals;
}

// Draw a wire between two points
void MainWindow::drawWire(const QPointF &from, const QPointF &to)
{
    QGraphicsLineItem *wire = new QGraphicsLineItem(QLineF(from, to));
    wire->setPen(QPen(Qt::black, 2));
    ui->graphicsView->scene()->addItem(wire);
}

void MainWindow::onComponentSelected(const QString &componentName)
{
    qDebug() << "Component selected:" << componentName;
    QGraphicsPixmapItem* component = new QGraphicsPixmapItem();

    QString imagePath = "../devices/" + componentName + "/" + componentName + ".png";
    analyzeComponentImage(imagePath);
    component->setPixmap(QPixmap(imagePath));

    component->setPos(100, 100);

    component->setFlag(QGraphicsItem::ItemIsMovable, true);

    ui->graphicsView->scene()->addItem(component);
}

MainWindow::~MainWindow() {
    delete ui;
}
