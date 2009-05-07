#include "glweb.h"
#include "glwidget.h"

GLWeb::GLWeb(QWidget *parent)
    : QWidget(parent)
{

    glWidget = new GLWidget;
    cmdWidget = new QPlainTextEdit;
    webWidget = new QWebView;

    QSplitter * glSplitter = new QSplitter;
    glSplitter->addWidget(glWidget);
    glSplitter->addWidget(cmdWidget);
    glSplitter->setOrientation(Qt::Vertical);
    QList<int> sizes;
    sizes.append(500); sizes.append(100);
    glSplitter->setSizes(sizes);

    QSplitter * mainSplitter = new QSplitter;
    mainSplitter->addWidget(glSplitter);
    mainSplitter->addWidget(webWidget);
    sizes[0] = 500; sizes[1] = 300;
    mainSplitter->setSizes(sizes);
    
    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mainSplitter);

    webWidget->load( QUrl("http://www.pdb.org"));
    setWindowTitle(tr("Test using GL + WebKit"));
    cmdWidget->insertPlainText("Command input/output goes here.\n> ");
}
