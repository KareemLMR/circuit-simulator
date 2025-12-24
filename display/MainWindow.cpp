#include "MainWindow.h"
#include "inventory.h"
#include "qdebug.h"
#include "ui_MainWindow.h"
#include <queue>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cm(CircuitManager::getInstance()), oc(Orchestrator::getInstance()), im(InventoryManager::getInstance()) {
    ui->setupUi(this);  // Loads the UI from .ui file

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->viewport()->installEventFilter(this);

    qDebug() << "GraphicsView created:" << ui->graphicsView;

    connect(ui->inventoryButton, &QPushButton::clicked, [this]() {
        Inventory *dialog = new Inventory(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

//         Connect dialog's signal to MainWindow's slot
        connect(dialog, &Inventory::componentSelected,
                this, &MainWindow::onComponentSelected);

        dialog->show();
    });

    connect(ui->wiringButton, &QPushButton::clicked, [this]() {
        m_wiringMode = !m_wiringMode;
    });

    connect(ui->simulation, &QPushButton::clicked, [this]() {
        oc.init(1000.0, 1000000.0, &cm);
        oc.start();
    });


    m_wiringMode = false;
    m_waitingToDrop = false;
    m_startWiringPosDetermined = false;
//    connect(ui->wiringButton, &QPushButton::clicked, [this]() {
//        QList<QGraphicsItem*> allItems = ui->graphicsView->scene()->items();
//        QVector<QPointF> terminals1 = getComponentTerminalsInScene((allItems[0]));
//        QVector<QPointF> terminals2 = getComponentTerminalsInScene(qgraphicsitem_cast<QGraphicsPixmapItem*>((allItems[1])));

//        QVector<QPointF> sceneTerminals1;

//        for (const QPointF& imageTerminal : terminals[0]) {
//            QPointF sceneTerminal = allItems[0]->mapToScene(imageTerminal);
//            sceneTerminals1.append(sceneTerminal);
//        }

//        QVector<QPointF> sceneTerminals2;

//        for (const QPointF& imageTerminal : terminals[1]) {
//            QPointF sceneTerminal = allItems[1]->mapToScene(imageTerminal);
//            sceneTerminals2.append(sceneTerminal);
//        }

//        QGraphicsScene* scene = ui->graphicsView->scene();
//        if (!scene) {
//            qDebug() << "No scene!";
//            return;
//        }

//        // Draw line directly in the scene
//        QGraphicsLineItem* wire = new QGraphicsLineItem(sceneTerminals1[0].x(), sceneTerminals1[0].y(), sceneTerminals2[1].x(), sceneTerminals2[1].y());
//        wire->setPen(QPen(Qt::black, 2));
//        scene->addItem(wire);

//        qDebug() << "Line drawn from (50,50) to (200,200)";
//    });
}

//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == ui->graphicsView->viewport() &&
//        event->type() == QEvent::MouseButtonPress)
//    {
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//        QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

//        qDebug() << "Scene position:" << scenePos.x() << "," << scenePos.y();

//        // Create a simple circle at click position
//        if (!ui->graphicsView->scene()) {
//            ui->graphicsView->setScene(new QGraphicsScene(this));
//        }
////        ui->graphicsView->scene()->addEllipse(
////            scenePos.x() - 5, scenePos.y() - 5, 10, 10,
////            QPen(Qt::red), QBrush(Qt::blue));
//        if (m_wiringMode)
//        {
//            m_wiringClicksCounter++;
//            uint64_t smallest_d = std::numeric_limits<uint64_t>::max();;
//            QPointF nearestNode;

//            QList<QGraphicsItem*> allItems = ui->graphicsView->scene()->items();

//            // We need to match terminals with their corresponding items
//            // Assuming terminals[i] corresponds to the i-th component item
//            int componentIndex = 0;

//            for (int i = 0; i < allItems.size() && componentIndex < terminals.size(); i++)
//            {
//                QGraphicsItem* item = allItems[i];

//                // Skip wire items
//                if (QGraphicsLineItem* wire = dynamic_cast<QGraphicsLineItem*>(item)) {
//                    continue; // Skip wires
//                }

//                // Only process if we have terminals for this component
//                if (componentIndex < terminals.size()) {
//                    auto terminal = terminals[componentIndex];

//                    for (int j = 0; j < terminal.size(); j++)
//                    {
//                        auto imageTerminal = terminal[j];
//                        QPointF sceneTerminal = item->mapToScene(imageTerminal);
//                        qDebug() << "Processing terminal" << j << "of component" << componentIndex << ":" << sceneTerminal;

//                        uint64_t d = (sceneTerminal.x() - scenePos.x()) * (sceneTerminal.x() - scenePos.x()) +
//                                (sceneTerminal.y() - scenePos.y()) * (sceneTerminal.y() - scenePos.y());
//                        if (d < smallest_d)
//                        {
//                            smallest_d = d;
//                            nearestNode = sceneTerminal;
//                        }
//                    }
//                    componentIndex++;
//                }
//            }
//            QGraphicsScene* scene = ui->graphicsView->scene();
//            if (!scene) {
//                qDebug() << "No scene!";
//            }
//            if (m_wiringClicksCounter % 2)
//            {
//                qDebug() << "First click detected " << nearestNode.x() << ", " << nearestNode.y();
//                currentWireStartPoint = nearestNode;
//            }
//            else
//            {
//                qDebug() << "Second click detected " << nearestNode.x() << ", " << nearestNode.y();
//                currentWireEndPoint = nearestNode;
//                QGraphicsLineItem* wire = new QGraphicsLineItem(currentWireStartPoint.x(), currentWireStartPoint.y(), currentWireEndPoint.x(), currentWireEndPoint.y());
//                wire->setPen(QPen(Qt::black, 2));
//                scene->addItem(wire);
//                cm.connect(m_nodesPointMap[std::make_pair(currentWireStartPoint.x(), currentWireStartPoint.y())], m_nodesPointMap[std::make_pair(currentWireEndPoint.x(), currentWireEndPoint.y())]);
//            }
//        }
//    }



//    return QMainWindow::eventFilter(watched, event);
//}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->graphicsView->viewport())
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

            // Get the top-most item at the click position
            QGraphicsItem* clickedItem = ui->graphicsView->scene()->itemAt(scenePos,
                                                                           ui->graphicsView->transform());
            if (clickedItem)
            {
                qDebug() << "Clicked item type:" << typeid(*clickedItem).name();
                qDebug() << "Clicked item position:" << clickedItem->pos();
                QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(clickedItem);
                if (pixmapItem && !m_wiringMode)
                {
                    m_waitingToDrop = true;
                    m_deltaDistByComponent = pixmapItem->pos();
                    qDebug() << "This is a pixmap item (component)";

                }
                else if (m_wiringMode)
                {
                    uint64_t distanceToNearestNode = std::numeric_limits<uint64_t>::max();
                    for (auto& node : m_componentsNodesMap[pixmapItem])
                    {
                        uint64_t d = (mouseEvent->pos().x() - node.second.x()) * (mouseEvent->pos().x() - node.second.x()) + (mouseEvent->pos().y() - node.second.y()) * (mouseEvent->pos().y() - node.second.y());
                        if (d  < distanceToNearestNode)
                        {
                            distanceToNearestNode = d;
                            if (!m_startWiringPosDetermined)
                            {
                                m_currentWireStartPoint = node;
                            }
                            else
                            {
                                m_currentWireEndPoint = node;
                            }
                        }

                    }
                    if (m_startWiringPosDetermined)
                    {
                        qDebug() << "Wiring...";
                        QGraphicsLineItem* wire = new QGraphicsLineItem(m_currentWireStartPoint.second.x(), m_currentWireStartPoint.second.y(), m_currentWireEndPoint.second.x(), m_currentWireEndPoint.second.y());
                        wire->setPen(QPen(Qt::black, 2));
                        ui->graphicsView->scene()->addItem(wire);
                        cm.connect(m_currentWireStartPoint.first, m_currentWireEndPoint.first);
                    }
                    m_startWiringPosDetermined = !m_startWiringPosDetermined;
                }
            }
            qDebug() << "Mouse PRESSED at:" << mouseEvent->pos();
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

            // Get the top-most item at the click position
            QGraphicsItem* releasedItem = ui->graphicsView->scene()->itemAt(scenePos,
                                                                           ui->graphicsView->transform());
            if (releasedItem && m_waitingToDrop)
            {
                qDebug() << "Clicked item type:" << typeid(*releasedItem).name();
                qDebug() << "Clicked item position:" << releasedItem->pos();
                if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(releasedItem))
                {
                    m_deltaDistByComponent -= pixmapItem->pos();
                    for (auto& node : m_componentsNodesMap[pixmapItem])
                    {
                        node.second -= m_deltaDistByComponent;
                    }
                    qDebug() << "Component moved " << m_deltaDistByComponent.x() << ", " << m_deltaDistByComponent.y();
                }
                m_waitingToDrop = false;
            }
            qDebug() << "Mouse RELEASED at:" << mouseEvent->pos();
            // This is what you asked for - when user releases mouse button
            if (mouseEvent->button() == Qt::LeftButton) {
//                handleMouseRelease(mouseEvent);
            }
            break;
        }

        case QEvent::MouseButtonDblClick:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

            // Get the top-most item at the click position
            QGraphicsItem* clickedItem = ui->graphicsView->scene()->itemAt(scenePos,
                                                                           ui->graphicsView->transform());
            if (clickedItem)
            {
                qDebug() << "Clicked item type:" << typeid(*clickedItem).name();
                qDebug() << "Clicked item position:" << clickedItem->pos();
                if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(clickedItem))
                {
                    m_waitingToDrop = true;
                    m_deltaDistByComponent = pixmapItem->pos();
                    qDebug() << "This is a pixmap item (component)";

                }
            }
            qDebug() << "Mouse DOUBLE CLICK at:" << mouseEvent->pos();
            break;
        }

        case QEvent::MouseMove:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            // Check if left button is pressed during movement
            if (mouseEvent->buttons() & Qt::LeftButton) {
                qDebug() << "Mouse MOVING while pressed at:" << mouseEvent->pos();
//                handleMouseDrag(mouseEvent);
            }
            break;
        }

        case QEvent::Wheel:
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            qDebug() << "Mouse WHEEL delta:" << wheelEvent->angleDelta().y();
            break;
        }

        default:
            break;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

QVector<QPointF> MainWindow::getComponentTerminalsInScene(QGraphicsPixmapItem* component)
{
    // 1. Get terminals in image coordinates (pixels)
    QVector<QPointF> imageTerminals = analyzeComponentImage(component->pixmap().toImage());

    // 2. Convert each to scene coordinates
    QVector<QPointF> sceneTerminals;

    for (const QPointF& imageTerminal : imageTerminals) {
        QPointF sceneTerminal = component->mapToScene(imageTerminal);
        sceneTerminals.append(sceneTerminal);
    }

    return sceneTerminals;
}

QVector<QPointF> MainWindow::analyzeComponentImage(const QImage& image)
{
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
            qDebug() << "First terminal is at the top";
            break;  // Found first terminal
        }
    }

    // 2. Right edge (top to bottom)
    for (int y = 0; y < height; y++)
    {
        if (qGray(image.pixel(width-1, y)) < 128)
        {
            terminals.append(QPointF(width-1, y));
            qDebug() << "First terminal is at the right";
            break;
        }
    }

    // 3. Bottom edge (right to left)
    for (int x = width-1; x >= 0; x--)
    {
        if (qGray(image.pixel(x, height-1)) < 128)
        {
            terminals.append(QPointF(x, height-1));
            qDebug() << "First terminal is at the bottom";
            break;
        }
    }

    // 4. Left edge (bottom to top)
    for (int y = height-1; y >= 0; y--)
    {
        if (qGray(image.pixel(0, y)) < 128)
        {
            terminals.append(QPointF(0, y));
            qDebug() << "First terminal is at the left";
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

void MainWindow::onComponentSelected(const QString &componentName)
{
    qDebug() << "Component selected:" << componentName;

    QString imagePath = "../devices/" + componentName + "/" + componentName + ".png";
    QImage image(imagePath);

    // Convert to transparent format
    image = image.convertToFormat(QImage::Format_ARGB32);

    // Remove white background (if any)
    for (int y = 0; y < image.height(); ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = line[x];
            int r = qRed(pixel), g = qGreen(pixel), b = qBlue(pixel);

            // If pixel is white (or nearly white), make transparent
            if (r > 240 && g > 240 && b > 240) {
                line[x] = qRgba(r, g, b, 0);
            }
        }
    }

    // Create component with transparent image
    QGraphicsPixmapItem* component = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    component->setPos(100, 100);
    component->setFlag(QGraphicsItem::ItemIsMovable, true);
    ui->graphicsView->scene()->addItem(component);

    // Optional: Set stacking order
    component->setZValue(0);

    component->setPos(100, 100);

    component->setFlag(QGraphicsItem::ItemIsMovable, true);

    ui->graphicsView->scene()->addItem(component);

    std::string deviceName = componentName.toStdString() + std::to_string(im.getSupportedDevices()[componentName.toStdString()]);

    qDebug() << "Device " << QString::fromStdString(deviceName) << " created";
    std::shared_ptr<Device> dev = cm.createDevice(componentName.toStdString(), std::make_pair<std::string, std::vector<double>>(std::move(deviceName), {1e3}));
    if (dev != nullptr)
    {
        for (int i = 0 ; i < dev->getPins().size() ; i++)
        {
            QPointF sceneTerminal = component->mapToScene(analyzeComponentImage(image)[i]);
            m_componentsNodesMap[component][dev->getPins()[i]] = sceneTerminal;
            qDebug() << "Saving node " << QString::fromStdString(dev->getPins()[i]->getName()) << " into " << sceneTerminal.x() << sceneTerminal.y();
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
