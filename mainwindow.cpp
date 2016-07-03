#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  thickness = 3;
  color = Qt::blue;
  Pressed = false;

  lastPos.setX(20);
  lastPos.setY(20);

  currentPos.setX(40);
  currentPos.setY(20);

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

  //!
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));


}

void MainWindow::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  QPen linePen(color);
  linePen.setWidth(thickness);

//painter.drawImage(QPoint(0, 0), theImage);

  painter.setPen(linePen);
  painter.drawLine(lastPos, currentPos);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  Pressed = true;
  lastPos = event->pos();
  //qDebug()<<"lastPos is "<<lastPos;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  if(Pressed)
    {
      //QPainter painter(this);
      currentPos = event->pos();
      if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1))
        {
          this->update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Pressed = false;
}
