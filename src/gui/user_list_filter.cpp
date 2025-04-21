#include "gui/user_list_filter.h"

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace matchmaker::gui {

UserListFilter::UserListFilter(QWidget *parent) :
    QWidget(parent), pattern_text(new QLineEdit(this))
{
    init();
}

void UserListFilter::init()
{
    QPushButton *push_button = new QPushButton("Filter", this);
    connect(push_button, &QPushButton::clicked, this, &UserListFilter::on_click_filter);
    connect(pattern_text, &QLineEdit::returnPressed, this, &UserListFilter::on_click_filter);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Username filter", this));
    layout->addWidget(pattern_text);
    layout->addWidget(push_button);
}

void UserListFilter::on_click_filter()
{
    emit triggered(pattern_text->text());
}

}
