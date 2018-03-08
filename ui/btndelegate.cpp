#include "btndelegate.h"
#include <qapplication.h>
#include <QDebug>

BtnDelegate::BtnDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void BtnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *itemModel = index.model();

    QString finishedSubTaskNum = itemModel->data(index, Qt::DisplayRole).toString();
    QStyleOptionButton *buttonOption = new QStyleOptionButton();
    buttonOption->rect = option.rect;
    buttonOption->state|= QStyle::State_Enabled | QStyle::State_AutoRaise;
    buttonOption->iconSize = QSize(100,40);
    buttonOption->text = finishedSubTaskNum;

    QApplication::style()->drawControl(QStyle::CE_PushButton, buttonOption, painter);
}
QSize BtnDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(120,40);
}
bool BtnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(index.data().toString() == QString::fromUtf8("修复"))
        {
            emit repairSignal(index);
        }
        else if(index.data().toString() == QString::fromUtf8("打开"))
        {
            emit exptakeSignal(index);
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(index.column() == 3)
        {
            emit checkSignal(index);
        }
        else if((index.column() == 4)||(index.column() == 8))
        {
            emit deleteSignal(index);
        }
        else if(index.column()== 7)
        {
            emit editSignal(index);
        }
    }
        return true;
}
