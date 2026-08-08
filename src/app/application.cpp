#include "application.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // Create the main window
    mainWindow = new MainWindow();
    mainWindow->setWindowTitle("My Qt5 Application");
    mainWindow->resize(800, 600);
    mainWindow->show();
}

Application::~Application()
{
    delete mainWindow;
}