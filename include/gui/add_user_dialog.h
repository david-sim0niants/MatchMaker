#pragma once

#include <QDialog>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

namespace matchmaker::gui {

class AddUserDialog : public QDialog {
public:
    AddUserDialog(QStringList&& available_games, QWidget *parent = nullptr);

private slots:
    void on_ok_click();
    void on_cancel_click();

private:
    void init_layout();
    QLayout *init_checkboxes_layout();

    QLineEdit *username_text;
    QLineEdit *first_name_text;
    QLineEdit *last_name_text;
    QList<QCheckBox *> game_checkboxes;
    QPushButton *cancel_button;
    QPushButton *ok_button;
};

}
