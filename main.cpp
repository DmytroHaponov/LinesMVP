/**
   main.cpp
   @author Dmytro Haponov
   @version 229 6/07/16
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
