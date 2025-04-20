#include "gui/add_user_dialog.h"

#include <QBoxLayout>
#include <QFormLayout>
#include <QKeyEvent>

namespace matchmaker::gui {

static QString make_error_message_from(AddUserDialogError error);

AddUserDialog::AddUserDialog(AddUserDialogEndpoint& endpoint, QWidget *parent) :
    QDialog(parent),
    endpoint(endpoint),
    username_text(new QLineEdit(this)),
    first_name_text(new QLineEdit(this)),
    last_name_text(new QLineEdit(this)),
    checkbox_layout(new QGridLayout),
    error_label(new QLabel(this)),
    button_box(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
    cancel_button(button_box->button(QDialogButtonBox::Cancel)),
    ok_button(button_box->button(QDialogButtonBox::Ok))
{
    init();
}

int AddUserDialog::exec()
{
    refresh();
    return QDialog::exec();
}

void AddUserDialog::accept()
{
    QString username = username_text->text();
    QString first_name = first_name_text->text();
    QString last_name = last_name_text->text();

    QStringList preferred_games;
    for (QCheckBox *checkbox : game_checkboxes)
        if (checkbox->isChecked())
            preferred_games.push_back(checkbox->text());

    AddUserDialogError e = endpoint.add_user(username, first_name, last_name, preferred_games);

    if (e) {
        QString err_msg = make_error_message_from(e);
        error_label->setText(err_msg);
        error_label->show();
    } else {
        QDialog::accept();
    }
}

void AddUserDialog::init()
{
    error_label->setStyleSheet("color: red;");
    error_label->hide();

    init_layout();
    ok_button->setDefault(true);
    ok_button->setFocus();

    connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void AddUserDialog::init_layout()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    setLayout(main_layout);

    QFormLayout *form_layout = new QFormLayout;
    form_layout->addRow("Username", username_text);
    form_layout->addRow("First Name", first_name_text);
    form_layout->addRow("Last Name", last_name_text);
    form_layout->addRow("Preferred games", checkbox_layout);
    main_layout->addLayout(form_layout);

    QHBoxLayout *bottom_layout = new QHBoxLayout;
    bottom_layout->addWidget(error_label);
    bottom_layout->addStretch();
    bottom_layout->addWidget(cancel_button);
    bottom_layout->addWidget(ok_button);
    main_layout->addLayout(bottom_layout);
}

void AddUserDialog::refresh()
{
    refresh_text_fields();
    refresh_error_label();
    refresh_checkboxes();
    refresh_tab_order();
    ok_button->setFocus();
}

void AddUserDialog::refresh_text_fields()
{
    username_text->clear();
    first_name_text->clear();
    last_name_text->clear();
}

void AddUserDialog::refresh_error_label()
{
    error_label->hide();
    error_label->clear();
}

void AddUserDialog::refresh_checkboxes()
{
    reset_checkboxes();
    setup_checkboxes(endpoint.get_games());
}

void AddUserDialog::refresh_tab_order()
{
    if (game_checkboxes.empty())
        return;

    setTabOrder(last_name_text, game_checkboxes.front());
    for (qsizetype i = 1; i < game_checkboxes.size(); ++i)
        setTabOrder(game_checkboxes[i - 1], game_checkboxes[i]);
    setTabOrder(game_checkboxes.back(), cancel_button);
}

void AddUserDialog::setup_checkboxes(const QStringList& labels)
{
    constexpr int cols = 2;
    int row = 0, col = 0;

    for (auto&& label : labels) {
        QCheckBox *checkbox = new QCheckBox(label, this);
        game_checkboxes.push_back(checkbox);
        checkbox_layout->addWidget(checkbox, row, col);

        if (++col == cols) {
            col = 0;
            ++row;
        }
    }
}

void AddUserDialog::reset_checkboxes()
{
    while (QLayoutItem *item = checkbox_layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    game_checkboxes.clear();
}

static void join_human_readable(const QStringList& items, QString& dest)
{
    qsizetype count = items.size();

    if (count == 1)
        dest += items[0];
    if (count == 2)
        dest += items[0] + " and " + items[1];

    if (count < 3)
        return;

    for (int i = 0; i < count - 1; ++i)
        dest += items[i] + ", ";

    dest.chop(2);
    dest += " and " + items.last();
}

static QString make_error_message_from(AddUserDialogError error)
{
    if (error == AddUserDialog::ErrorUserAlreadyExists)
        return "User already exists";

    QStringList error_msgs;

    if (error & AddUserDialog::ErrorInvalidUsername)
        error_msgs.push_back("username");

    if (error & AddUserDialog::ErrorInvalidFirstName)
        error_msgs.push_back("first name");

    if (error & AddUserDialog::ErrorInvalidLastName)
        error_msgs.push_back("last name");

    QString error_msg = "";
    if (! error_msgs.empty()) {
        error_msg = "Invalid ";
        join_human_readable(error_msgs, error_msg);
    }

    if (error & AddUserDialog::ErrorNoPreferredGames) {
        if (! error_msg.isEmpty())
            error_msg += '\n';
        error_msg += "User has no preferred games";
    }

    return error_msg;
}

}
