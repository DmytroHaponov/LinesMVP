#ifndef PAINTAREA_H
#define PAINTAREA_H

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

  bool openImage(const QString &fileName);
  bool saveImage(const QString &fileName, const char *fileFormat);

  void setPenColor(const QColor &newColor);
  void setPenWidth(int newWidth);
  void changePrimitive(int primitive);
  void setPencil(bool pencil);

  bool isModified() const { return modified; }
  QColor penColor() const { return color; }
  int penWidth() const { return thickness; }

public slots:
  void clearImage();
  void print();

protected:
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
  void resizeImage(QImage *image, const QSize &newSize);

  enum primitives {Line, Rectangle, Ellipse};
  int currentPrimitive;

  struct ShapeToDraw
  {
    QColor color;
    int thickness, currentPrimitive;
    QPoint lastPos, currentPos;
  };

  QQueue<ShapeToDraw> paintQueue;

  int countOfShapes;
  bool Pressed, Moved;
  bool Pencil;
  QPoint lastPos, currentPos;

  int thickness;
  QColor color;
//  QPainter painter;
//  QPen linePen;

  QImage image;


  bool modified;
};

#endif // PAINTAREA_H
