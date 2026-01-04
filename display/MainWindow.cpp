#include "MainWindow.h"
#include "inventory.h"
#include "qdebug.h"
#include "ui_MainWindow.h"
#include <queue>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cm(CircuitManager::getInstance()), oc(Orchestrator::getInstance()), im(InventoryManager::getInstance()) {
    ui->setupUi(this);  // Loads the UI from .ui file

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);

    ui->graphicsView->viewport()->setMouseTracking(true);

    ui->graphicsView->setMouseTracking(true);

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

        QList<QGraphicsItem*> items = ui->graphicsView->items();
        for (auto& item : items)
        {
            item->setFlag(QGraphicsItem::ItemIsMovable, !m_wiringMode);
        }
    });

    connect(ui->simulation, &QPushButton::clicked, [this]() {
//        oc.init(1.0, 1.0, &cm);
//        oc.start();
        cm.solveCircuit(1.0);
        for (auto& node : cm.queryDeviceVoltages("resistor1").second)
        {
            qDebug() << node->getVolt() ;
        }

        for (auto& path : cm.queryDeviceCurrents("resistor1"))
        {
            qDebug() << "Node " << QString::fromStdString(path.first->getName()) << " current = " << path.second ;
        }
    });


    m_wiringMode = false;
    m_waitingToDrop = false;
    m_startWiringPosDetermined = false;
    m_wiringPivotPointChanged = false;
    m_currentWire1 = nullptr;
    m_currentWire2 = nullptr;
}

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
            QList<QGraphicsItem*> items = ui->graphicsView->scene()->items(scenePos);
            QList<QGraphicsItem*> wires;

            // Filter out wires and temporary items
            items.erase(std::remove_if(items.begin(), items.end(),
                                       [&wires](QGraphicsItem* item) {
                                           // Remove wires
                                           if (item->type() == QGraphicsLineItem::Type)
                                           {
                                               wires.append(item);
                                               return true;
                                           }
                                           return false;
                                       }), items.end());

            if (mouseEvent->button() == Qt::RightButton)
            {
                qDebug() << "Mouse Right Button PRESSED at:" << mouseEvent->pos();
                if (items.size() > 0 && items[0]->type() == QGraphicsPixmapItem::Type)
                {
                    qDebug() << "Clicked item type:" << typeid(items[0]).name();
                    qDebug() << "Clicked item position:" << items[0]->pos();
                    QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(items[0]);
                    qreal currentRotation = pixmapItem[0].rotation();
                    pixmapItem[0].setTransformOriginPoint(pixmapItem->boundingRect().center());
                    pixmapItem[0].setRotation(currentRotation - 90.0);

                    QRectF bounds = pixmapItem[0].boundingRect();
                    QPointF centerLocal = bounds.center();
                    QPointF center = pixmapItem[0].mapToScene(centerLocal);

                    for (auto& node : m_componentsNodesMap[pixmapItem])
                    {
//                        node.second = rotatePoint(node.second, pixmapItem->boundingRect().center(), -90);

                        qDebug() << "Center of rotation = " << center.x() << ", " << center.y();
                        QTransform transform;
                        transform.translate(center.x(), center.y());    // Move to center
                        transform.rotate(-90);                 // Rotate
                        transform.translate(-center.x(), -center.y());  // Move back
                        node.second = transform.map(node.second);
                    }
                }
            }
            else if (mouseEvent->button() == Qt::LeftButton)
            {

                if (items.size() > 0 && items[0]->type() == QGraphicsPixmapItem::Type)
                {
                    qDebug() << "Clicked item type:" << typeid(items[0]).name();
                    qDebug() << "Clicked item position:" << items[0]->pos();
                    QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(items[0]);
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
                            QGraphicsLineItem* wire1 = new QGraphicsLineItem(m_currentWire1->line().p1().x(), m_currentWire1->line().p1().y(), m_currentWire1->line().p2().x(), m_currentWire1->line().p2().y());
                            QGraphicsLineItem* wire2 = new QGraphicsLineItem(m_currentWire2->line().p1().x(), m_currentWire2->line().p1().y(), m_currentWire2->line().p2().x(), m_currentWire2->line().p2().y());
    //                        QGraphicsLineItem* wire = new QGraphicsLineItem(m_currentWireStartPoint.second.x(), m_currentWireStartPoint.second.y(), m_currentWireEndPoint.second.x(), m_currentWireEndPoint.second.y());
                            wire1->setPen(QPen(Qt::blue, 2));
                            wire2->setPen(QPen(Qt::blue, 2));
    //                        wire->setPen(QPen(Qt::blue, 2));
                            ui->graphicsView->scene()->addItem(wire1);
                            ui->graphicsView->scene()->addItem(wire2);
    //                        ui->graphicsView->scene()->addItem(wire);
                            cm.connect(m_currentWireStartPoint.first, m_currentWireEndPoint.first);
                            m_currentWire.push_front(m_currentWireStartPoint.second);
                            m_currentWire.push_back(mouseEvent->pos());
                            m_wiresMap[m_currentWireStartPoint.first].append(m_currentWire);
                            m_currentWire.clear();
                            m_wiringPivotPointChanged = false;
                        }
                        m_startWiringPosDetermined = !m_startWiringPosDetermined;
                    }
                }
                else if ((wires.size() >= 2 && wires[1]->type() == QGraphicsLineItem::Type))/* ||
                           ((!m_startWiringPosDetermined) && (wires.size() >= 1 && wires[0]->type() == QGraphicsLineItem::Type)))*/
                {
                    for (const auto& node : m_wiresMap)
                    {
                        for (const auto& segment : node.second)

                            for (uint8_t i = 0 ; i < segment.size() - 1 ; i++)
                            {
                                QGraphicsLineItem* seg = static_cast<QGraphicsLineItem*>(wires[1]);
                                if ((seg->line().p1() == segment[i]) && (seg->line().p2() == segment[i + 1]))
                                {
                                    if (!m_startWiringPosDetermined)
                                    {
                                        m_currentWireStartPoint = std::make_pair(node.first, mouseEvent->pos());
                                    }
                                    else
                                    {
                                        m_currentWireEndPoint = std::make_pair(node.first, mouseEvent->pos());
                                    }
                                }
                            }

                    }
                    if (m_startWiringPosDetermined)
                    {
                        qDebug() << "Wiring...";
                        QGraphicsLineItem* wire1 = new QGraphicsLineItem(m_currentWire1->line().p1().x(), m_currentWire1->line().p1().y(), m_currentWire1->line().p2().x(), m_currentWire1->line().p2().y());
                        QGraphicsLineItem* wire2 = new QGraphicsLineItem(m_currentWire2->line().p1().x(), m_currentWire2->line().p1().y(), m_currentWire2->line().p2().x(), m_currentWire2->line().p2().y());
                        //                        QGraphicsLineItem* wire = new QGraphicsLineItem(m_currentWireStartPoint.second.x(), m_currentWireStartPoint.second.y(), m_currentWireEndPoint.second.x(), m_currentWireEndPoint.second.y());
                        wire1->setPen(QPen(Qt::blue, 2));
                        wire2->setPen(QPen(Qt::blue, 2));
                        //                        wire->setPen(QPen(Qt::blue, 2));
                        ui->graphicsView->scene()->addItem(wire1);
                        ui->graphicsView->scene()->addItem(wire2);
                        //                        ui->graphicsView->scene()->addItem(wire);
                        cm.connect(m_currentWireStartPoint.first, m_currentWireEndPoint.first);
                        m_currentWire.push_front(m_currentWireStartPoint.second);
                        m_currentWire.push_back(mouseEvent->pos());
                        m_wiresMap[m_currentWireStartPoint.first].append(m_currentWire);
                        m_currentWire.clear();
                        m_wiringPivotPointChanged = false;
                    }
                    m_startWiringPosDetermined = !m_startWiringPosDetermined;
                }
                else if (clickedItem && clickedItem->type() == QGraphicsLineItem::Type)
                {
                    m_currentWire.push_back(mouseEvent->pos());
                    m_wiringPivotPointChanged = true;
                }
                qDebug() << "Mouse Left Button PRESSED at:" << mouseEvent->pos();
            }
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton) {
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
                qDebug() << "Mouse Left Button RELEASED at:" << mouseEvent->pos();
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
            // Convert to scene coordinates
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());

            if (mouseEvent->buttons() & Qt::LeftButton) {
                qDebug() << "Mouse DRAGGING at scene:" << scenePos;
                // handleMouseDrag(mouseEvent);
            }
            else {
                qDebug() << "Mouse MOVING (no buttons) at scene:" << scenePos;
                // handleMouseHover(mouseEvent);

                // Optional: Update status bar with cursor position
                ui->statusbar->showMessage(
                    QString("Cursor: %1, %2").arg(scenePos.x()).arg(scenePos.y())
                    );
                if (m_wiringMode && m_startWiringPosDetermined)
                {
                    if (m_currentWire1 && !m_wiringPivotPointChanged)
                    {
                        qDebug() << "Clearing m_currentWire1";
                        ui->graphicsView->scene()->removeItem(m_currentWire1);
                        delete m_currentWire1;
                        m_currentWire1 = nullptr;
                    }
                    if (m_currentWire2 && !m_wiringPivotPointChanged)
                    {
                        qDebug() << "Clearing m_currentWire2";
                        ui->graphicsView->scene()->removeItem(m_currentWire2);
                        delete m_currentWire2;
                        m_currentWire2 = nullptr;
                    }
                    QPointF startPoint = (m_currentWire.size() == 0) ? m_currentWireStartPoint.second : m_currentWire[m_currentWire.size() - 1];
                    QLineF line(startPoint, scenePos);

                    if (line.angle() < 45.0 || line.angle() > 135.0)
                    {
                        m_currentWire1 = new QGraphicsLineItem(startPoint.x(), startPoint.y(), scenePos.x(), startPoint.y());
                        m_currentWire2 = new QGraphicsLineItem(scenePos.x(), startPoint.y(), scenePos.x(), scenePos.y());
                    }
                    else
                    {
                        m_currentWire1 = new QGraphicsLineItem(startPoint.x(), startPoint.y(), startPoint.x(), scenePos.y());
                        m_currentWire2 = new QGraphicsLineItem(startPoint.x(), scenePos.y(), scenePos.x(), scenePos.y());
                    }

                    m_currentWire1->setPen(QPen(Qt::blue, 2));
                    m_currentWire2->setPen(QPen(Qt::blue, 2));

//                    m_currentWire1->setFlag(QGraphicsItem::ItemIsSelectable, false);
//                    m_currentWire1->setAcceptedMouseButtons(Qt::NoButton);
//                    m_currentWire1->setZValue(-1);

//                    m_currentWire2->setFlag(QGraphicsItem::ItemIsSelectable, false);
//                    m_currentWire2->setAcceptedMouseButtons(Qt::NoButton);
//                    m_currentWire2->setZValue(-2);

                    ui->graphicsView->scene()->addItem(m_currentWire1);
                    ui->graphicsView->scene()->addItem(m_currentWire2);
                    if (m_wiringPivotPointChanged)
                    {
                        m_wiringPivotPointChanged = false;
                    }
                }
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
//    for (int y = 0; y < image.height(); ++y) {
//        QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
//        for (int x = 0; x < image.width(); ++x) {
//            QRgb pixel = line[x];
//            int r = qRed(pixel), g = qGreen(pixel), b = qBlue(pixel);

//            // If pixel is white (or nearly white), make transparent
//            if (r > 240 && g > 240 && b > 240) {
//                line[x] = qRgba(r, g, b, 0);
//            }
//        }
//    }

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

    std::map<std::string, double> deviceParameters;
    std::shared_ptr<Device> dev = cm.createDevice(componentName.toStdString(), deviceName, deviceParameters);
    if (dev != nullptr)
    {
        qDebug() << "Device " << QString::fromStdString(dev->getName()) << " created";
        for (int i = 0 ; i < dev->getPins().size() ; i++)
        {
            QPointF sceneTerminal = component->mapToScene(analyzeComponentImage(image)[i]);
            m_componentsNodesMap[component][dev->getPins()[i]] = sceneTerminal;
            qDebug() << "Saving node " << QString::fromStdString(dev->getPins()[i]->getName()) << " into " << sceneTerminal.x() << sceneTerminal.y();
        }
    }
    else
    {
        qDebug() << "Failed to create device!";
    }
    for (auto& parameter : deviceParameters)
    {
        bool ok;
        QString valueStr = QInputDialog::getText(this, "Required Parameters", QString::fromStdString(parameter.first),
                                                 QLineEdit::Normal, QString::fromStdString(std::to_string(parameter.second)), &ok);
        if (ok)
        {
            double value = valueStr.toDouble();
            parameter.second = value;
        }
    }
    cm.setDeviceParameters(deviceName, deviceParameters);
}

MainWindow::~MainWindow() {
    delete ui;
}
