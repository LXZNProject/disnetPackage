#ifndef BTNDELEGATE_H
#define BTNDELEGATE_H

#include <QStyledItemDelegate>

class BtnDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BtnDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void checkSignal(QModelIndex index);
    void deleteSignal(QModelIndex index);
    void editSignal(QModelIndex index);
    void repairSignal(QModelIndex index);
    void exptakeSignal(QModelIndex index);
};

#endif // BTNDELEGATE_H
