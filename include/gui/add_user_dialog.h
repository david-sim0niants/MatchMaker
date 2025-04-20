#pragma once

#include "gui/user_descriptor.h"

#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>

namespace matchmaker::gui {

class AddUserDialogEndpoint;

class AddUserDialog : public QDialog {
    Q_OBJECT
public:
    enum : int {
        ErrorNone = 0,
        ErrorInvalidUsername = 1,
        ErrorInvalidFirstName = 2,
        ErrorInvalidLastName = 4,
        ErrorNoPreferredGames = 8,
        ErrorUserAlreadyExists = 16,
        ErrorUserDoesNotExist = 17,
    };
    using Error = int;

    AddUserDialog(AddUserDialogEndpoint& endpoint, QWidget *parent = nullptr);

    int exec() override;

private:
    void accept() override;

    void init();
    void init_layout();

    void refresh();
    void refresh_text_fields();
    void refresh_error_label();
    void refresh_checkboxes();
    void refresh_tab_order();
    void setup_checkboxes(const QStringList& labels);
    void reset_checkboxes();

    AddUserDialogEndpoint& endpoint;
    QLineEdit *username_text;
    QLineEdit *first_name_text;
    QLineEdit *last_name_text;
    QList<QCheckBox *> game_checkboxes;
    QGridLayout *checkbox_layout;
    QLabel *error_label;
    QDialogButtonBox *button_box;
    QPushButton *cancel_button, *ok_button;
};

using AddUserDialogError = AddUserDialog::Error;

using AddUserCallback = std::function<void (UserDescriptor user)>;

class AddUserDialogEndpoint {
public:
    virtual AddUserDialogError add_user(
            const QString& username,
            const QString& name,
            const QString& last_name,
            const QStringList& preferred_games) = 0;

    virtual QStringList get_games() const = 0;
};

}
