#ifndef JXDOCK_H
#define JXDOCK_H

#include <QWidget>
#include <QtGui>
#include <QObject>
#include <QWebView>

#include "glwidget.h"


class GLDock : public QWidget
{
    Q_OBJECT

public:
    GLDock(QWidget *parent = 0);
    GLWidget * glWidget;
    QPlainTextEdit * cmdOutput;
    QWidget * cmdWidget;
    QLineEdit * cmdInput;
    QWidget * webWidget;
    QWebView * webView;
    QLineEdit * webURL;
    QDockWidget * cmdDock;
    QDockWidget * webDock;

private slots:
    void openUrl();
    void doCmd();

};

#endif
