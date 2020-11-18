#include "mainwindowreader.h"
#include "pageview.h"

MainWindowReader::MainWindowReader(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Reader");
    resize(1280, 720);
    PageView* centralWidget = new PageView(this);
    setCentralWidget(centralWidget);
}
