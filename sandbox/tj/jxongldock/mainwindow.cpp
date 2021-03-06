#include <QtGui>
#include <QtWebKit>
#include "mainwindow.h"
#include "glwidget.h"

//! [0]
MainWindow::MainWindow()
{
    glWidget = new GLWidget;
    setCentralWidget(glWidget);
    glDock = new GLDock(this);
    createActions();
    createMenus();

    addDockWidget(Qt::RightDockWidgetArea, glDock->cmdDock);
    viewMenu->addAction(glDock->cmdDock->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea, glDock->webDock);
    viewMenu->addAction(glDock->webDock->toggleViewAction());


    connect(glDock->webView, SIGNAL(loadFinished(bool)),
        this, SLOT(updateTextEdit()));
    //setStartupText();
    resize(1000,600);

}
//! [0]

//! [1]
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing HTML file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    openUrlAct = new QAction(tr("Open &URL..."), this);
    openUrlAct->setShortcut(tr("Ctrl+U"));
    openUrlAct->setStatusTip(tr("Open a URL"));
    connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));
//! [1]

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the HTML file to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutWebKitAct = new QAction(tr("About &WebKit"), this);
    aboutWebKitAct->setStatusTip(tr("Show WebKit's website"));
    openUrlAct->setShortcut(tr("Ctrl+W"));
    connect(aboutWebKitAct, SIGNAL(triggered()), this, SLOT(aboutWebKit()));
}

//! [2]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(openUrlAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutWebKitAct);
}
//! [2]

//! [3]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
        tr("This <b>jxdock</b> example demonstrates how to "
           "use dock widgets with OpenGL, webkit and command text"));
}
void MainWindow::aboutWebKit()
{
    glDock->webView->load( QUrl("http://webkit.org/") );
}
//! [3]

//! [4]
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        // read from file
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        QString output = out.readAll();

        // display contents
        glDock->cmdOutput->setPlainText(output);
    }
}
//! [4]

//! [5]
void MainWindow::openUrl()
{
    bool ok;
    QString url = QInputDialog::getText(this, tr("Enter a URL"),
                  tr("URL:"), QLineEdit::Normal, "http://", &ok);

    if (ok && !url.isEmpty()) {
        glDock->webView->setUrl(url);
        glDock->webURL->setText(url);
    }
}
//! [5]

//! [6]
void MainWindow::save()
{
    QString content = glDock->cmdOutput->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this);

    if (!fileName.isEmpty()) {
        // save to file
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);
        in << content;
    }
}
//! [6]

//! [7]
void MainWindow::updateTextEdit()
{
    QWebFrame *mainFrame = glDock->webView->page()->mainFrame();
    QString frameText = mainFrame->toHtml();
    //glDock->plainTextEdit->setPlainText(frameText);
}
//! [7]

//! [8]
void MainWindow::setStartupText()
{
    QString string = "<html><body><h1>HTML Viewer</h1>"
                     " <p>This example shows you how to use QWebView to"
                     " view HTML data.</p>"
                     " </body></html>";
    glDock->webView->setHtml(string);
}
//! [8]
