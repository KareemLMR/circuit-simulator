#include "MainWindow.h"
#include "inventory.h"
#include "qdebug.h"
#include "ui_MainWindow.h"

QVector<QVector<QPointF>> terminals(2);

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

    connect(ui->wiringButton, &QPushButton::clicked, [this]() {
        QList<QGraphicsItem*> allItems = ui->graphicsView->scene()->items();
        QVector<QPointF> terminals1 = getComponentTerminalsInScene((allItems[0]));
        QVector<QPointF> terminals2 = getComponentTerminalsInScene(qgraphicsitem_cast<QGraphicsPixmapItem*>((allItems[1])));

        QVector<QPointF> sceneTerminals1;

        for (const QPointF& imageTerminal : terminals[0]) {
            QPointF sceneTerminal = allItems[0]->mapToScene(imageTerminal);
            sceneTerminals1.append(sceneTerminal);
        }

        QVector<QPointF> sceneTerminals2;

        for (const QPointF& imageTerminal : terminals[1]) {
            QPointF sceneTerminal = allItems[1]->mapToScene(imageTerminal);
            sceneTerminals2.append(sceneTerminal);
        }

        QGraphicsScene* scene = ui->graphicsView->scene();
        if (!scene) {
            qDebug() << "No scene!";
            return;
        }

        // Draw line directly in the scene
        QGraphicsLineItem* wire = new QGraphicsLineItem(sceneTerminals1[0].x(), sceneTerminals1[0].y(), sceneTerminals2[1].x(), sceneTerminals2[1].y());
        wire->setPen(QPen(Qt::black, 2));
        scene->addItem(wire);

        qDebug() << "Line drawn from (50,50) to (200,200)";
    });
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

void MainWindow::onComponentSelected(const QString &componentName)
{
    static int index = 0;
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
    terminals[index] = analyzeComponentImage(image);
    component->setPos(100, 100);
    component->setFlag(QGraphicsItem::ItemIsMovable, true);
    ui->graphicsView->scene()->addItem(component);

    // Optional: Set stacking order
    component->setZValue(0);

    component->setPos(100, 100);

    component->setFlag(QGraphicsItem::ItemIsMovable, true);

    ui->graphicsView->scene()->addItem(component);
    index++;
}

MainWindow::~MainWindow() {
    delete ui;
}
