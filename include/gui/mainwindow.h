#pragma once

#include <QMainWindow>

#include <memory>

namespace Ui { class MainWindow; }

namespace matchmaker::gui {

class MainWindow : public QMainWindow {
    Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;
};

}
