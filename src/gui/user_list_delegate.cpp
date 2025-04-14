#include "gui/user_list_delegate.h"

#include <QApplication>
#include <QPainter>
#include <QPen>

namespace matchmaker::gui {

void UserListDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const
{
    QStyleOptionViewItem item_option(option);
    item_option.state &= ~QStyle::State_HasFocus;

    if (is_add_user_item(index))
        draw_add_user_button(painter, item_option);
    else
        QStyledItemDelegate::paint(painter, item_option, index);
}

bool UserListDelegate::editorEvent(
        QEvent *event,
        QAbstractItemModel *model,
        const QStyleOptionViewItem& option,
        const QModelIndex& index)
{
    if (! is_add_user_item(index))
        return false;

    pressed = event->type() == QEvent::MouseButtonPress;

    if (event->type() == QEvent::MouseButtonRelease) {
        emit clicked_add_user();
        return true;
    }

    return pressed;
}

inline bool UserListDelegate::is_add_user_item(const QModelIndex& index) const
{
    return index.row() == index.model()->rowCount() - 1 && index.column() == 0;
}

void UserListDelegate::draw_add_user_button(
        QPainter *painter,
        const QStyleOptionViewItem& option) const
{
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = "+Add User";
    button.state = QStyle::State_Enabled;
    if (pressed)
        button.state |= QStyle::State_Sunken;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

}
