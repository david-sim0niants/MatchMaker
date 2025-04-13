#include "gui/mainwindow.h"
#include "ui_mainwindow.h"

namespace matchmaker::gui {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() = default;

}
