#include "gui/add_user_dialog.h"

#include <QBoxLayout>
#include <QFormLayout>
#include <QKeyEvent>

namespace matchmaker::gui {

AddUserDialog::AddUserDialog(QStringList&& available_games, QWidget *parent) :
    QDialog(parent),
    username_text(new QLineEdit(this)),
    first_name_text(new QLineEdit(this)),
    last_name_text(new QLineEdit(this)),
    cancel_button(new QPushButton("Cancel", this)),
    ok_button(new QPushButton("OK", this))
{
    game_checkboxes.reserve(available_games.size());
    for (auto&& game_name : available_games)
        game_checkboxes.push_back(new QCheckBox(game_name, this));

    init_layout();
    connect(ok_button, &QPushButton::clicked, this, &AddUserDialog::on_ok_click);
    connect(cancel_button, &QPushButton::clicked, this, &AddUserDialog::on_cancel_click);
}

void AddUserDialog::on_ok_click()
{
    QStringView username = username_text->text();
    QStringView first_name = first_name_text->text();
    QStringView last_name = last_name_text->text();

    qDebug() << "Add user: " << username << ' ' << first_name << ' ' << last_name;
    qDebug() << "Preferred games: ";

    QList<QStringView> selected_games;
    for (QCheckBox *checkbox : game_checkboxes) {
        if (checkbox->isChecked()) {
            selected_games.push_back(checkbox->text());
            qDebug() << checkbox->text() << ' ';
        }
    }
    qDebug() << '\n';
}

void AddUserDialog::on_cancel_click()
{
    reject();
}

void AddUserDialog::init_layout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QFormLayout *form_layout = new QFormLayout(this);
    form_layout->addRow("Username", username_text);
    form_layout->addRow("First Name", first_name_text);
    form_layout->addRow("Last Name", last_name_text);
    form_layout->addRow("Preferred games", init_checkboxes_layout());
    layout->addLayout(form_layout);

    QHBoxLayout *bottom_layout = new QHBoxLayout(this);
    bottom_layout->addLayout(bottom_layout);
    bottom_layout->addStretch();
    bottom_layout->addWidget(cancel_button);
    bottom_layout->addWidget(ok_button);
    layout->addLayout(bottom_layout);
}

QLayout *AddUserDialog::init_checkboxes_layout()
{
    constexpr int cols = 2;
    int row = 0, col = 0;

    QGridLayout *checkbox_layout = new QGridLayout(this);
    for (QCheckBox *checkbox : game_checkboxes) {
        checkbox_layout->addWidget(checkbox, row, col);
        if (++col == cols) {
            col = 0;
            ++row;
        }
    }
    return checkbox_layout;
}

}
