#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QtGui>
#include <QActionGroup>
#include "paintarea.h"
/*
#include <QColor>
#include <QImage>
*/
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
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void open();
    void save();
    void lineColor();
    void lineWidth();

    void chooseLine();
private:
    //**************************
    //!Drawing block
    //!
    /*
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
    QPainter painter;
    QPen linePen;

    QImage image;
    */
    //**************************
    //! Window block

    Ui::MainWindow *ui;

    void create_Actions();
    void create_Menus();
    bool maybeSave();
        bool saveFile(const QByteArray &fileFormat);

        PaintArea *paintArea;

        QMenu *saveAsMenu;
        QMenu *fileMenu;
        QMenu *optionMenu;
        QMenu *helpMenu;
        QMenu *toolMenu;

        QAction *openAct;
        QList<QAction *> saveAsActs;
        QAction *exitAct;
        QAction *penColorAct;
        QAction *penWidthAct;
        QAction *printAct;
        QAction *clearScreenAct;
        QAction *chooseLineAct;

private slots:
/*
    // void openFile();
    //bool saveFile();
    void lineColor();
    void lineWidth();
    */
    void on_SelectPrimitiveBox_activated(const QString &arg1);
/*
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    */
};

#endif // MAINWINDOW_H
