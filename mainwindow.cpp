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
  paintArea = new PaintArea;
  //this->addDockWidget(
  setCentralWidget(paintArea);
  ui->SelectPrimitiveBox->raise();
  setWindowTitle(tr("Haponov Paint"));

  create_Actions();
  create_Menus();
  resize(1024,768);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
      event->accept();
    } else {
      event->ignore();
    }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::open()
{
  if (maybeSave()) {
      QString fileName = QFileDialog::getOpenFileName(this,
                                                      tr("Open File"), QDir::currentPath());
      if (!fileName.isEmpty())
        paintArea->openImage(fileName);
    }
  //paintArea->update();
}

void MainWindow::save()
{
  QAction *action = qobject_cast<QAction *>(sender());
  QByteArray fileFormat = action->data().toByteArray();
  saveFile(fileFormat);
}

void MainWindow::lineColor()
{
  const QColor newColor = QColorDialog::getColor(paintArea->penColor());
  if (newColor.isValid())
    paintArea->setPenColor(newColor);
}

void MainWindow::lineWidth()
{
  bool ok;
  const int newWidth = QInputDialog::getInt(this, tr("Haponov Paint"),
                                            tr("Select line width:"),
                                            paintArea->penWidth(),
                                            1, 50, 1, &ok);
  if (ok)
    paintArea->setPenWidth(newWidth);
}

void MainWindow::chooseLine()
{
  paintArea->changePrimitive(0); paintArea->setPencil(false);
  qDebug()<<"Line chosen";
}
/*
void MainWindow::changePrimitive(int primitive)
{
  currentPrimitive = primitive;
}
*/
void MainWindow::create_Actions()
{
  //  openAct = new QAction(tr("&Open..."), this);
  //  openAct->setShortcuts(QKeySequence::Open);
  //  connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

  //  saveAsAct = new QAction(tr("&Save As..."), this);
  //  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  //  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFile()));

  //  exitAct = new QAction(tr("&Exit"), this);
  //  exitAct->setShortcuts(QKeySequence::Quit);
  //  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  foreach (QByteArray format, QImageWriter::supportedImageFormats())
    {
      QString text = tr("%1...").arg(QString(format).toUpper());

      QAction *action = new QAction(text, this);
      action->setData(format);
      connect(action, SIGNAL(triggered()), this, SLOT(save()));
      saveAsActs.append(action);
    }

  printAct = new QAction(tr("&Print..."), this);
  connect(printAct, SIGNAL(triggered()), paintArea, SLOT(print()));

  penColorAct = new QAction(tr("&Pen Color..."), this);
  connect(penColorAct, SIGNAL(triggered()), this, SLOT(lineColor()));

  penWidthAct = new QAction(tr("Pen &Width..."), this);
  connect(penWidthAct, SIGNAL(triggered()), this, SLOT(lineWidth()));

  clearScreenAct = new QAction(tr("&Clear Screen"), this);
  clearScreenAct->setShortcut(tr("Ctrl+L"));
  connect(clearScreenAct, SIGNAL(triggered()), paintArea, SLOT(clearImage()));


  chooseLineAct = new QAction(tr("&Choose Line"), this);
  connect(chooseLineAct, SIGNAL(triggered(bool)), this, SLOT(chooseLine()));
  /*
  lineColorAct = new QAction(tr("&Line Color..."), this);
  connect(lineColorAct, SIGNAL(triggered()), this, SLOT(lineColor()));

  lineWidthAct = new QAction(tr("&Line Width..."), this);
  connect(lineWidthAct, SIGNAL(triggered()), this, SLOT(lineWidth()));
  */
}

void MainWindow::create_Menus()
{
  //  fileMenu = menuBar()->addMenu(tr("&File"));
  //  fileMenu->addAction(openAct);
  //  fileMenu->addAction(saveAsAct);
  //  fileMenu->addSeparator();
  //  fileMenu->addAction(exitAct);
  saveAsMenu = new QMenu(tr("&Save As"), this);
  foreach (QAction *action, saveAsActs)
    saveAsMenu->addAction(action);

  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(openAct);
  fileMenu->addMenu(saveAsMenu);
  fileMenu->addAction(printAct);

  toolMenu = menuBar()->addMenu(tr("&Tools"));
  toolMenu->addAction(chooseLineAct);
  //toolMenu->addAction(brushWidthAct);
  toolMenu->addSeparator();

  optionMenu = new QMenu(tr("&Tools Option"), this);
  optionMenu->addAction(penColorAct);
  optionMenu->addAction(penWidthAct);
  optionMenu->addSeparator();
  optionMenu->addAction(clearScreenAct);

  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(optionMenu);
  /*
  lineMenu = menuBar()->addMenu(tr("&Line"));
  lineMenu->addAction(lineColorAct);
  lineMenu->addAction(lineWidthAct);
  */
}

//void MainWindow::paintEvent(QPaintEvent * /* event */)
//{
//  QPainter painter(this);

//  //painter.drawImage(QPoint(0, 0), theImage);
//  if (paintQueue.isEmpty()){qDebug()<<"nothing to paint"; return;}
//  //qDebug()<<"size of queue is "<<paintQueue.size();

//  ShapeToDraw each;
//  foreach (each, paintQueue)
//    {
//      QPen linePen(each.color);
//      linePen.setWidth(each.thickness);
//      painter.setPen(linePen);
//      switch (each.currentPrimitive)
//        {
//        case      Line: painter.drawLine(each.lastPos, each.currentPos); break;
//        case Rectangle:
//          {
//            QRect rec(each.lastPos, each.currentPos);
//            painter.drawRect(rec); break;
//          }
//        case   Ellipse:
//          {

//            QRect rec(each.lastPos, each.currentPos);
//            painter.drawEllipse(rec); break;
//          }
//        default: painter.drawLine(each.lastPos, each.currentPos); break;
//        }
//    }
//}

//void MainWindow::mousePressEvent(QMouseEvent *event)
//{
//  Pressed = true;
//  lastPos = event->pos();
//  //qDebug()<<"PressEvent "<<lastPos;
//}

//void MainWindow::mouseMoveEvent(QMouseEvent *event)
//{
//  if(Pressed)
//    {
//      Moved = true;
//      if (!Pencil && (paintQueue.size() > (countOfShapes+1))) paintQueue.pop_back();

//      currentPos = event->pos();

//      ShapeToDraw tempShape;
//      tempShape.color = color;
//      tempShape.currentPos = currentPos;
//      tempShape.currentPrimitive = currentPrimitive;
//      tempShape.lastPos = lastPos;
//      tempShape.thickness = thickness;

//      paintQueue.push_back(tempShape);

//      if ((event->buttons() & Qt::LeftButton) /*&& lastPos != QPoint(-1, -1)*/)
//        {
//          if (Pencil) {update(QRect(lastPos, currentPos)); lastPos = currentPos;}
//          else        this->update();
//        }
//    }
//}

//void MainWindow::mouseReleaseEvent(QMouseEvent * /* event */ )
//{
//  if (Pressed && Moved && !Pencil)
//    {
//      ++countOfShapes;
//      //qDebug()<<"countOfShapes is "<<countOfShapes;
//    }
//  if (Pressed && Moved && Pencil)
//    {
//      countOfShapes = paintQueue.size();
//      //qDebug()<<"countOfShapes is "<<countOfShapes;
//    }
//  Pressed = false;
//  Moved = false;
//  this->update();
//}

bool MainWindow::maybeSave()
{
  if (paintArea->isModified()) {
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this, tr("MyPaint"),
                                 tr("The image has been modified.\n"
                                    "Do you want to save your changes?"),
                                 QMessageBox::Save | QMessageBox::Discard
                                 | QMessageBox::Cancel);
      if (ret == QMessageBox::Save) {
          return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
          return false;
        }
    }
  return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
  QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                  initialPath,
                                                  tr("%1 Files (*.%2);;All Files (*)")
                                                  .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                  .arg(QString::fromLatin1(fileFormat)));
  if (fileName.isEmpty()) {
      return false;
    } else {
      return paintArea->saveImage(fileName, fileFormat.constData());
    }
}
/*
void MainWindow::on_checkBox_toggled(bool checked)
{
  paintArea->setPencil(checked);
  if (!checked) paintArea->changePrimitive(-1);
}
*/
void MainWindow::on_SelectPrimitiveBox_activated(const QString &arg1)
{
  qDebug()<<"comboBox says "<<arg1;
  if (arg1== "Line")      {paintArea->changePrimitive(0); paintArea->setPencil(false);}
  if (arg1== "Rectangle") {paintArea->changePrimitive(1); paintArea->setPencil(false);}
  if (arg1== "Ellipse")   {paintArea->changePrimitive(2); paintArea->setPencil(false);}
  if (arg1 == "PENCIL")   {paintArea->changePrimitive(-1); paintArea->setPencil(true);}
}
