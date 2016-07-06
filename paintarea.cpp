#include "paintarea.h"
#include <QDebug>

#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

PaintArea::PaintArea(QWidget *parent) : QWidget(parent),
  countOfShapes(0),
  thickness(3),
  Pressed(false), Moved(false),
  modified(false),
  currentPrimitive(0)
{ 
  setAttribute(Qt::WA_StaticContents);
  color = Qt::blue;

  lastPos.setX(0);
  lastPos.setY(0);

  currentPos.setX(0);
  currentPos.setY(0);
}

bool PaintArea::openImage(const QString &fileName)
{
  QImage loadedImage;
  if (!loadedImage.load(fileName))
    return false;

  QSize newSize = loadedImage.size().expandedTo(size());
  resizeImage(&loadedImage, newSize);
  image = loadedImage;
  modified = false;
  update();
  return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
  QImage visibleImage = image;
  resizeImage(&visibleImage, size());

  if (visibleImage.save(fileName, fileFormat))
    {
      modified = false;
      return true;
    }
  else {  return false;  }
}

void PaintArea::setPenColor(const QColor &newColor)
{
  color = newColor;
}

void PaintArea::setPenWidth(int newWidth)
{
  thickness = newWidth;
}

void PaintArea::changePrimitive(int primitive)
{
  currentPrimitive = primitive;
}

void PaintArea::setPencil(bool pencil)
{
  Pencil = pencil;
}

void PaintArea::clearImage()
{
  image.fill(qRgb(255, 255, 255));
  modified = true;
  update();
}

void PaintArea::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
  QPrinter printer(QPrinter::HighResolution);

  QPrintDialog printDialog(&printer, this);
  if (printDialog.exec() == QDialog::Accepted) {
      QPainter painter(&printer);
      QRect rect = painter.viewport();
      QSize size = image.size();
      size.scale(rect.size(), Qt::KeepAspectRatio);
      painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
      painter.setWindow(image.rect());
      painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}

void PaintArea::paintEvent(QPaintEvent * /* event */)
{

QPainter painter(this);

  painter.drawImage(QPoint(0, 0), image);
  if (paintQueue.isEmpty()){qDebug()<<"nothing to paint"; return;}
  qDebug()<<"size of queue is "<<paintQueue.size();

  ShapeToDraw each;
  foreach (each, paintQueue)
    {
      QPen linePen(each.color);
      linePen.setWidth(each.thickness);
      painter.setPen(linePen);
      switch (each.currentPrimitive)
        {
        case      Line: painter.drawLine(each.lastPos, each.currentPos); break;
        case Rectangle:
          {
            QRect rec(each.lastPos, each.currentPos);
            painter.drawRect(rec); break;
          }
        case   Ellipse:
          {

            QRect rec(each.lastPos, each.currentPos);
            painter.drawEllipse(rec); break;
          }
        default: painter.drawLine(each.lastPos, each.currentPos); break;
        }
    }
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
  Pressed = true;
  lastPos = event->pos();
  //qDebug()<<"PressEvent "<<lastPos;
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
  if(Pressed)
    {
      Moved = true;
      if (!Pencil && (paintQueue.size() > (countOfShapes+1))) paintQueue.pop_back();

      currentPos = event->pos();

      ShapeToDraw tempShape;
      tempShape.color = color;
      tempShape.currentPos = currentPos;
      tempShape.currentPrimitive = currentPrimitive;
      tempShape.lastPos = lastPos;
      tempShape.thickness = thickness;

      paintQueue.push_back(tempShape);

      if ((event->buttons() & Qt::LeftButton) /*&& lastPos != QPoint(-1, -1)*/)
        {
          modified = true;
          if (Pencil) {update(QRect(lastPos, currentPos)); lastPos = currentPos;}
          else update();       //this->update();
        }
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent * /* event */ )
{
  if (Pressed && Moved && !Pencil)
    {
      ++countOfShapes;
      //qDebug()<<"countOfShapes is "<<countOfShapes;
      QPainter painter(&image);
      if (paintQueue.isEmpty()){qDebug()<<"nothing to paint"; return;}
      qDebug()<<"size of queue is "<<paintQueue.size();

      ShapeToDraw each;
      foreach (each, paintQueue)
        {
          QPen linePen(each.color);
          linePen.setWidth(each.thickness);
          painter.setPen(linePen);
          switch (each.currentPrimitive)
            {
            case      Line: painter.drawLine(each.lastPos, each.currentPos); break;
            case Rectangle:
              {
                QRect rec(each.lastPos, each.currentPos);
                painter.drawRect(rec); break;
              }
            case   Ellipse:
              {

                QRect rec(each.lastPos, each.currentPos);
                painter.drawEllipse(rec); break;
              }
            default: painter.drawLine(each.lastPos, each.currentPos); break;
            }
        }
    }
  if (Pressed && Moved && Pencil)
    {
      countOfShapes = paintQueue.size();
      //qDebug()<<"countOfShapes is "<<countOfShapes;
    }
  Pressed = false;
  Moved = false;
  this->update();
}

void PaintArea::resizeEvent(QResizeEvent *event)
{
  if (width() > image.width() || height() > image.height()) {
      int newWidth = qMax(width() + 128, image.width());
      int newHeight = qMax(height() + 128, image.height());
      resizeImage(&image, QSize(newWidth, newHeight));
      update();
    }
  QWidget::resizeEvent(event);
}

void PaintArea::resizeImage(QImage *image, const QSize &newSize)
{
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}
