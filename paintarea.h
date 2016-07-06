/**
   paintarea.h
   @author Dmytro Haponov
   @version 229 6/07/16
*/

#ifndef PAINTAREA_H
#define PAINTAREA_H

/**
    module responsible for:
  1) loading image to paint on
  2) painting lines, rectangles, ellipses
                                          and drawing with pencil
                                                                  of different colors
                                                                                      and with different width of lines
  3) saving results of painting
  4) sending picture to printer
  5) clearing all the figures and loaded picture
  6) discarding last action with pencil (discarding figures is not implemented)
*/

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtGui>

#include <QPrinter>
#include <QPrintDialog>

class PaintArea : public QWidget
{
  Q_OBJECT
public:
  explicit PaintArea(QWidget *parent = 0);

  // loads selected picture to draw on
  bool openImage(const QString &fileName);
  //saves current picture
  bool saveImage(const QString &fileName, const char *fileFormat);

  //sets color of lines of instument in use
  void setPenColor(const QColor &newColor);
  //sets width of lines of instument in use
  void setPenWidth(int newWidth);


  //sets instrument to draw line, rectangle or ellipse
  void changePrimitive(int primitive);
  //sets instrument to draw with pencil
  void setPencil(bool pencil);

  //indicates presence of changes made to picture in use
  bool isModified() const { return modified; }

  //returns setting of instrument to draw - current color of lines
  QColor penColor() const { return color; }
  //returns setting of instrument to draw - current width of lines
  int penWidth() const { return thickness; }

public slots:
  //makes current picture blank (white)
  void clearImage();

  //sends picture to printer
  void print();

  //discards last action with pencil (one or several pixels)
  void stepBack();

protected:
  //processing mouse pressing
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  //processing mouse movement
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  //processing mouse release
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

  //overriding painting of window
  void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
  void resizeImage(QImage *image, const QSize &newSize);

  //figures available for drawing
  enum primitives {Line, Rectangle, Ellipse};
  //figure that is selected to be drawn with next mouse pressing
  int currentPrimitive;

  //contains all data needed to redraw each figure that
  //was drawn in application
  struct ShapeToDraw
  {
    QColor color;
    int thickness, currentPrimitive;
    QPoint lastPos, currentPos;
  };

  //contains order of figures drawn in application
  QQueue<ShapeToDraw> paintQueue;

  //number of drawn figures;
  //it's a sum of autofigures (each of them is counted as 1) and each movement of pencil
  //(1 0r several pixels depending on speed of movement)
  int countOfShapes;

  //first indicates whether mouse is pressed;
  //second indicates whether pressed mouse did move
  bool Pressed, Moved;

  // indicates usage of pencil
  bool Pencil;

  //coordinates of:
  //fist - mouse position when first pressed,
  //second - mouse current position
  QPoint lastPos, currentPos;

  //width of lines to be drawn
  int thickness;
  //color of lines to be drawn
  QColor color;

  QImage image;

  bool modified;
};

#endif // PAINTAREA_H
