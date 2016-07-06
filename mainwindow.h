#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void open();
    void save();
    void lineColor();
    void lineWidth();

    void chooseLine();
    void chooseRect();
    void chooseEllipse();
    void choosePencil();

    void on_SelectPrimitiveBox_activated(const QString &arg1);
private:
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

        QActionGroup *toolsGroup;
        QAction *chooseLineAct;
        QAction *chooseRectAct;
        QAction *chooseEllipseAct;
        QAction *choosePencilAct;
};

#endif // MAINWINDOW_H
