#ifndef GLWEB_H
#define GLWEB_H

#include <QWidget>
#include <QtGui>
#include <QWebView>

#include "glwidget.h"


class GLWeb : public QWidget
{
    Q_OBJECT

public:
    GLWeb(QWidget *parent = 0);
    GLWidget * glWidget;
    QPlainTextEdit * cmdOutput;
    QWidget * cmdWidget;
    QLineEdit * cmdInput;
    QWidget * webWidget;
    QWebView * webView;
    QLineEdit * webURL;

private slots:
    void openUrl();
    void doCmd();

};

#endif
