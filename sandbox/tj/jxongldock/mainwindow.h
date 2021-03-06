#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jxdock.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


private slots:
    void open();
    void openUrl();
    void save();
    void about();
    void aboutWebKit();
    void updateTextEdit();
//! [0]

//! [1]
private:
    GLWidget * glWidget;
    GLDock *glDock;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *openUrlAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutWebKitAct;

    void createActions();
    void createMenus();
    void setStartupText();
};
//! [1]

#endif
