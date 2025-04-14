#pragma once

#include <QWidget>
#include <QLineEdit>

namespace matchmaker::gui {

class UserListFilter : public QWidget {
    Q_OBJECT;
public:
    UserListFilter(QWidget *parent = nullptr);

signals:
    void trigerred(QStringView pattern) const;

private slots:
    void on_click_filter();

private:
    void init();

    QLineEdit *pattern_text;
};

}
