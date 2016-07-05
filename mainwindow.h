#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QActionGroup>

#include <QColor>
#include <QImage>

#include <QPair>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //**************************
    //!Drawing block
    enum primitives {Line, Rectangle, Ellipse};
    int currentPrimitive;
    QQueue< QPair<int, QPair< QPoint, QPoint> > > paintQueue;
    int countOfShapes;
    bool Pressed, Moved;
    QPoint lastPos, currentPos;

    int thickness;
    QColor color;
    QPainter painter;
    QPen linePen;

    QImage image;
    //**************************
    //! Window block

    Ui::MainWindow *ui;

    void create_Actions();
    void create_Menus();

    QMenu *fileMenu;
    QMenu *lineMenu;

    QActionGroup *lineActionGroup;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *lineWidthAct;
    QAction *lineColorAct;

private slots:
    void openFile();
    bool saveFile();
    void lineColor();
    void lineWidth();
    void changePrimitive(int primitive);


    void on_comboBox_activated(const QString &arg1);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
