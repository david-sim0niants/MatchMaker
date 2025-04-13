#pragma once

#include <QMenuBar>

namespace matchmaker::gui {

class MenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
};

}
