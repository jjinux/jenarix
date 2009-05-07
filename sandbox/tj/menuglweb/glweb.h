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
    QPlainTextEdit * cmdWidget;
    QWebView * webWidget;

};

#endif
