#pragma once

#include <QStyledItemDelegate>

namespace matchmaker::gui {

class UserListDelegate : public QStyledItemDelegate {
    Q_OBJECT;
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

    bool editorEvent(QEvent *event,
            QAbstractItemModel *model,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) override;

signals:
    void clicked_add_user();

private:
    bool is_add_user_item(const QModelIndex& index) const;
    void draw_add_user_button(QPainter *painter,
            const QStyleOptionViewItem& option) const;

    bool pressed = false;
};

}
