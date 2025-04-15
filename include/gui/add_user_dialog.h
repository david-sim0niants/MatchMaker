#pragma once

#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

namespace matchmaker::gui {

class AddUserDialogEndpoint;

class AddUserDialog : public QDialog {
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

private slots:
    void on_ok_click();
    void on_cancel_click();

private:
    void init();
    void init_layout();

    void refresh();
    void refresh_text_fields();
    void refresh_error_label();
    void refresh_checkboxes();
    void setup_checkboxes(const QStringList& labels);
    void reset_checkboxes();

    AddUserDialogEndpoint& endpoint;
    QLineEdit *username_text;
    QLineEdit *first_name_text;
    QLineEdit *last_name_text;
    QList<QCheckBox *> game_checkboxes;
    QGridLayout *checkbox_layout;
    QLabel *error_label;
    QPushButton *cancel_button;
    QPushButton *ok_button;
};

using AddUserDialogError = AddUserDialog::Error;

class AddUserDialogEndpoint {
public:
    virtual AddUserDialogError add_user(
            const QString& username,
            const QString& name,
            const QString& last_name,
            const QStringList& preferred_games) = 0;
    virtual QStringList get_available_games() const = 0;
};

}
