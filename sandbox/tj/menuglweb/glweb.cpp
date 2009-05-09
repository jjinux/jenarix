#include "glweb.h"
#include "glwidget.h"

GLWeb::GLWeb(QWidget *parent)
    : QWidget(parent)
{

    glWidget = new GLWidget;
    cmdOutput = new QPlainTextEdit;
    cmdWidget = new QWidget;
    cmdInput = new QLineEdit;
    webWidget = new QWidget;
    webURL = new QLineEdit;
    webView = new QWebView;
    QHBoxLayout * mainLayout = new QHBoxLayout(this);

// gl command and gl widget
    QSplitter * glSplitter = new QSplitter;
    glSplitter->addWidget(cmdWidget);
    glSplitter->addWidget(glWidget);
    glSplitter->setOrientation(Qt::Vertical);
    QList<int> sizes;
    sizes.append(100); sizes.append(500);
    glSplitter->setSizes(sizes);
//  command is composite widget for output and input
    QVBoxLayout * cmdLayout = new QVBoxLayout;
    cmdLayout->addWidget(cmdOutput);
    cmdLayout->addWidget(cmdInput);
    cmdLayout->setContentsMargins(0,0,0,0);
    cmdLayout->setSpacing(0);
    cmdOutput->setReadOnly(true);
    cmdWidget->setLayout(cmdLayout);

// web url and view
    QVBoxLayout * webLayout = new QVBoxLayout;
    webLayout->addWidget(webURL);
    webLayout->addWidget(webView);
    webLayout->setContentsMargins(0,0,0,0);
    webWidget->setLayout(webLayout);

// main split widget
    QSplitter * mainSplitter = new QSplitter;
    mainSplitter->addWidget(glSplitter);
    mainSplitter->addWidget(webWidget);
    sizes[0] = 500; sizes[1] = 300;
    mainSplitter->setSizes(sizes);
    mainLayout->addWidget(mainSplitter);

// initial values
    webView->load( QUrl("http://www.pdb.org"));
    setWindowTitle(tr("Test using GL + WebKit"));
    cmdOutput->insertPlainText(tr("Command output goes here."));

// signals and slots
   connect(webURL, SIGNAL(returnPressed()), this, SLOT(openUrl()));
   connect(cmdInput, SIGNAL(returnPressed()), this, SLOT(doCmd()));

}

void GLWeb::openUrl() {
   QString url = webURL->text();
   webView->setUrl(url);
}

void GLWeb::doCmd() {
   QString cmd = cmdInput->text();
   cmdOutput->appendPlainText(cmd);
   cmdOutput->appendPlainText(tr("output from command"));
   cmdInput->clear();
}
