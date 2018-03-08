#ifndef TAKEVIEWMODEL_H
#define TAKEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QTextCodec>

class takeViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit takeViewModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setHorizontalHeaderList(QStringList hlist);
    void setModelData(QList<QStringList> &datalist);

    void refrushModel();
    QVariant mydata(const QModelIndex &index) const;

signals:
    
public slots:

private:
    QTextCodec *tc;
    QStringList mHorList;
    QList<QStringList> mDataList;
    
};

#endif // TAKEVIEWMODEL_H
