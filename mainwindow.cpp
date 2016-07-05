#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //!----------------------------------------------------
    countOfShapes(0),
    //!----------------------------------------------------
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thickness = 3;
    color = Qt::blue;
    Pressed = false; Moved = false;
    currentPrimitive= 0;

    lastPos.setX(0);
    lastPos.setY(0);

    currentPos.setX(0);
    currentPos.setY(0);

    setWindowTitle(tr("Haponov Paint"));

    create_Actions();
    create_Menus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open File"),
                                                          QDir::currentPath());
    if (!fileName.isEmpty()) {

        QImage image;
        if (!image.load(fileName))


        {
            //if (!paintArea->openImage(fileName)) {
            QMessageBox::information(this, tr("Haponov Paint"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        // paintArea->adjustSize();
        this->adjustSize();
    }
}

bool MainWindow::saveFile()
{
    const QString initialPath = QDir::currentPath() + "/untitled.png";

    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                          initialPath);
    if (fileName.isEmpty()) {
        return false;
    } else {
        //     return paintArea->saveImage(fileName, "png");
    }
}

void MainWindow::lineColor()
{
    const QColor newColor = QColorDialog::getColor(color);
    if (newColor.isValid())
        color = newColor;
    /*
  const QColor newColor = QColorDialog::getColor(paintArea->lineColor());
  if (newColor.isValid())
  //  qDebug()<<"setting color ";
      paintArea->setlinehColor(newColor);
  */
}

void MainWindow::lineWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr("Haponov Paint"),
                                              tr("Select line width:"),
                                              thickness,
                                              //paintArea->brushWidth(),
                                              1, 50, 1, &ok);
    if (ok)
        //paintArea->setBrushWidth(newWidth);
        thickness = newWidth;
}

void MainWindow::changePrimitive(int primitive)
{
    currentPrimitive = primitive;
}

void MainWindow::create_Actions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    lineColorAct = new QAction(tr("&Line Color..."), this);
    connect(lineColorAct, SIGNAL(triggered()), this, SLOT(lineColor()));

    lineWidthAct = new QAction(tr("&Line Width..."), this);
    connect(lineWidthAct, SIGNAL(triggered()), this, SLOT(lineWidth()));
}

void MainWindow::create_Menus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    lineMenu = menuBar()->addMenu(tr("&Line"));
    lineMenu->addAction(lineColorAct);
    lineMenu->addAction(lineWidthAct);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen linePen(color);
    linePen.setWidth(thickness);
    painter.setPen(linePen);
    //painter.drawImage(QPoint(0, 0), theImage);
    if (paintQueue.isEmpty()){qDebug()<<"nothing to paint"; return;}
qDebug()<<"size of queue is "<<paintQueue.size();
    QPair<int, QPair< QPoint, QPoint> > tempPair;
    foreach (tempPair, paintQueue)
    {
/*
        qDebug()<<"element.first == "<<tempPair.first;
         qDebug()<<"element.second.first.X == "<<tempPair.second.first.x();
         qDebug()<<"element.second.first.Y == "<<tempPair.second.first.y();
         qDebug()<<"element.second.second.X == "<<tempPair.second.second.x();
         qDebug()<<"element.second.second.Y == "<<tempPair.second.second.y();
*/
        switch (tempPair.first)
        {
        case      Line: painter.drawLine(tempPair.second.first, tempPair.second.second); break;
        case Rectangle:
        {
            QRect rec(tempPair.second.first, tempPair.second.second);
            painter.drawRect(rec); break;
        }
        case   Ellipse:
        {

            QRect rec(tempPair.second.first, tempPair.second.second);
            painter.drawEllipse(rec); break;
        }
        default: painter.drawLine(tempPair.second.first, tempPair.second.second); break;
        }
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Pressed = true;
    lastPos = event->pos();
    qDebug()<<"PressEvent "<<lastPos;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(Pressed)
    {
        Moved = true;
        if (paintQueue.size() > (countOfShapes+1)) paintQueue.pop_back();

        currentPos = event->pos();

        QPair<QPoint, QPoint> coordinates(lastPos, currentPos);
//        qDebug()<<"moveEvent coordinates: LastPos is "<<coordinates.first.x()<<"   "<<coordinates.first.y()
//                <<" currentPos is "<<coordinates.second.x()<<"   "<<coordinates.second.y();
        QPair<int, QPair<QPoint, QPoint> > elemForDraw(currentPrimitive, coordinates);
//        qDebug()<<"primitive from pair: "<<elemForDraw.first;


        paintQueue.push_back(elemForDraw);

        if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1))
        {

            this->update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (Pressed && Moved)
    {
        ++countOfShapes;
        qDebug()<<"countOfShapes is "<<countOfShapes;

    }
    Pressed = false;
    Moved = false;
    /*
    QRect rec(lastPos, currentPos);
    this->update(rec);
    */
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    //qDebug()<<"comboBox says "<<arg1;
    if (arg1== "Line") {changePrimitive(0);}
    if (arg1== "Rectangle") {changePrimitive(1);}
    if (arg1== "Ellipse") {changePrimitive(2);}
}
