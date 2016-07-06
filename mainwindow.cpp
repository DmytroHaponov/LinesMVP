/**
   mainwindow.cpp
   @author Dmytro Haponov
   @version 229 6/07/16
*/

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
  setCentralWidget(paintArea);
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

void MainWindow::chooseRect()
{
  paintArea->changePrimitive(1); paintArea->setPencil(false);
  qDebug()<<"Rect chosen";
}

void MainWindow::chooseEllipse()
{
  paintArea->changePrimitive(2); paintArea->setPencil(false);
  qDebug()<<"Ellipse chosen";
}

void MainWindow::choosePencil()
{
  paintArea->changePrimitive(-1); paintArea->setPencil(true);
  qDebug()<<"Pencil chosen";
}

void MainWindow::create_Actions()
{
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

  toolsGroup = new QActionGroup(this);

  chooseLineAct = new QAction(tr("&Choose Line"), this);
  connect(chooseLineAct, SIGNAL(triggered(bool)), this, SLOT(chooseLine()));

  chooseRectAct = new QAction(tr("&Choose Rectangular"), this);
  connect(chooseRectAct, SIGNAL(triggered(bool)), this, SLOT(chooseRect()));

  chooseEllipseAct = new QAction(tr("&Choose Ellipse"), this);
  connect(chooseEllipseAct, SIGNAL(triggered(bool)), this, SLOT(chooseEllipse()));

  choosePencilAct = new QAction(tr("&Choose Pencil"), this);
  connect(choosePencilAct, SIGNAL(triggered(bool)), this, SLOT(choosePencil()));

  stepBackAct = new QAction(tr("&Step Back"), this);
  stepBackAct->setShortcut(tr("Ctrl+Z"));
  connect(stepBackAct, SIGNAL(triggered(bool)), paintArea, SLOT(stepBack()));

  toolsGroup->addAction(chooseLineAct);
  toolsGroup->addAction(chooseRectAct);
  toolsGroup->addAction(chooseEllipseAct);
  toolsGroup->addAction(choosePencilAct);
}

void MainWindow::create_Menus()
{
  saveAsMenu = new QMenu(tr("&Save As"), this);
  foreach (QAction *action, saveAsActs)
    saveAsMenu->addAction(action);

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAct);
  fileMenu->addMenu(saveAsMenu);
  fileMenu->addAction(printAct);

  toolMenu = menuBar()->addMenu(tr("&Tools"));
  toolMenu->addAction(chooseLineAct);
  toolMenu->addAction(chooseRectAct);
  toolMenu->addAction(chooseEllipseAct);
  toolMenu->addAction(choosePencilAct);
  toolMenu->addSeparator();

  optionMenu = menuBar()->addMenu(tr("&Tools Option"));
  optionMenu->addAction(penColorAct);
  optionMenu->addAction(penWidthAct);
  optionMenu->addSeparator();
  optionMenu->addAction(clearScreenAct);

  historyMenu = menuBar()->addMenu(tr("History"));
  historyMenu->addAction(stepBackAct);
}

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
