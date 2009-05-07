#include <QtGui>
#include <QtWebKit>
#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    createActions();
    createMenus();
    centralWidget = new GLWeb(this);
    setCentralWidget(centralWidget);

    connect(centralWidget->webWidget, SIGNAL(loadFinished(bool)),
        this, SLOT(updateTextEdit()));
    setStartupText();
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
        tr("This <b>glweb</b> example demonstrates how to "
           "show a [ [glWidget+commandWidget] + WebKitWidget] "
           "using split frames for resizing."));
}
void MainWindow::aboutWebKit()
{
    centralWidget->webWidget->load( QUrl("http://webkit.org/") );
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
        centralWidget->cmdWidget->setPlainText(output);
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
        centralWidget->webWidget->setUrl(url);
    }
}
//! [5]

//! [6]
void MainWindow::save()
{
    QString content = centralWidget->cmdWidget->toPlainText();
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
    QWebFrame *mainFrame = centralWidget->webWidget->page()->mainFrame();
    QString frameText = mainFrame->toHtml();
    //centralWidget->plainTextEdit->setPlainText(frameText);
}
//! [7]

//! [8]
void MainWindow::setStartupText()
{
    QString string = "<html><body><h1>HTML Viewer</h1>"
                     " <p>This example shows you how to use QWebView to"
                     " view HTML data.</p>"
                     " </body></html>";
    centralWidget->webWidget->setHtml(string);
}
//! [8]
