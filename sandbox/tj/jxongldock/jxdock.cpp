#include "jxdock.h"
#include "glwidget.h"

GLDock::GLDock(QWidget *parent)
    : QWidget(parent)
{

    //glWidget = new GLWidget;

//  command is composite widget for output and input
    cmdWidget = new QWidget;
    cmdOutput = new QPlainTextEdit(cmdWidget);
    cmdInput = new QLineEdit(cmdWidget);
    QVBoxLayout * cmdLayout = new QVBoxLayout;
    cmdLayout->addWidget(cmdOutput);
    cmdLayout->addWidget(cmdInput);
    cmdLayout->setContentsMargins(0,0,0,0);
    cmdLayout->setSpacing(0);
    cmdOutput->setReadOnly(true);
    cmdWidget->setLayout(cmdLayout);
    cmdOutput->setMinimumSize(300,300);
    cmdInput->setMinimumWidth(300);
    cmdDock = new QDockWidget("Command", this);
    cmdDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cmdDock->setWidget(cmdWidget);


// web url and view
    webWidget = new QWidget;
    webView = new QWebView(webWidget);
    webURL = new QLineEdit(webWidget);
    QVBoxLayout * webLayout = new QVBoxLayout;
    webLayout->addWidget(webURL);
    webLayout->addWidget(webView);
    webLayout->setContentsMargins(0,0,0,0);
    webLayout->setSpacing(0);
    webWidget->setLayout(webLayout);
    webDock = new QDockWidget(tr("Web"));
    webView->setMinimumSize(300,300);
    webURL->setMinimumWidth(300);
    webView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    webDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    webDock->setWidget(webWidget);

// initial values
    QString url = QString("http://www.pdb.org");
    webView->load( QUrl(url) );
    webURL->setText(url);

    setWindowTitle(tr("Test using GL + WebKit"));
    cmdOutput->insertPlainText(tr("Command output goes here."));

// signals and slots
   connect(webURL, SIGNAL(returnPressed()), this, SLOT(openUrl()));
   connect(cmdInput, SIGNAL(returnPressed()), this, SLOT(doCmd()));

}

void GLDock::openUrl() {
   QString url = webURL->text();
   webView->setUrl(url);
}

void GLDock::doCmd() {
   QString cmd = cmdInput->text();
   cmdOutput->appendPlainText(cmd);
   cmdOutput->appendPlainText(tr("output from command"));
   cmdInput->clear();
}
