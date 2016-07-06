/**
   mainwindow.h
   @author Dmytro Haponov
   @version 229 6/07/16
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
    module responsible for:
  1) gui
*/

#include <QMainWindow>
#include <QList>
#include <QtGui>
#include <QActionGroup>
#include "paintarea.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
protected:

  //event of closing application;
  //prevents closing application without saving results of painting
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:

  // provides system dialog menu for choosing a file to load
  // and prevents losing unsaved changes to current picture before that
  void open();
  //saves current picture
  void save();

  // provides system dialog menu for choosing color to draw
  void lineColor();
  //provides menu to choose width of line to draw next
  void lineWidth();

  //switches drawing instrument draw lines, forbids pencil usage
  void chooseLine();
  //switches drawing instrument draw rectangle, forbids pencil usage
  void chooseRect();
  //switches drawing instrument draw ellipses, forbids pencil usage
  void chooseEllipse();
  //switches drawing instrument draw with pencil
  void choosePencil();

private:
  Ui::MainWindow *ui;

  //creates possible gui actions
  void create_Actions();
  //creates all menus
  void create_Menus();

  //monitors whether application has a picture with unsaved changes
  bool maybeSave();

  //provides system dialog menu for placing file that is about to be saved
  bool saveFile(const QByteArray &fileFormat);

  //widget that implements drawing, see "paintarea.h"
  PaintArea *paintArea;

  QMenu *saveAsMenu;
  QMenu *fileMenu;

  //menu for choosing color and width of lines
  QMenu *optionMenu;
  //menu for choosing instrument that draws: line, rectangle, ellipse of pencil
  QMenu *toolMenu;

  //intended to discard and redo last changes;
  //redo is not implemented, discard works for pencil
  QMenu *historyMenu;

  //action of loading picture to draw on
  QAction *openAct;

  //possible saving options of picture
  QList<QAction *> saveAsActs;
  QAction *exitAct;
  //choosing color to draw with
  QAction *penColorAct;
  //choosing width of line to draw
  QAction *penWidthAct;
  //sending to printer
  QAction *printAct;
  QAction *clearScreenAct;

  //discards last drawn action of pencil
  //does not work for figures
  QAction *stepBackAct;

  QActionGroup *toolsGroup;
  //action to choose instrument of drawing Lines
  QAction *chooseLineAct;
  //action to choose instrument of drawing rectangles
  QAction *chooseRectAct;
  //action to choose instrument of drawing ellipses
  QAction *chooseEllipseAct;
  //action to choose instrument of drawing with pencil
  QAction *choosePencilAct;
};

#endif // MAINWINDOW_H
