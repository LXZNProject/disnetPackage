#ifndef CHECKCABINRTWIDGET_H
#define CHECKCABINRTWIDGET_H

#include <QWidget>
#include "t_cabinet_cell.h"
#include "editcabinetwidget.h"
#include "takeviewmodel.h"
#include "btndelegate.h"
#include "addcellwidget.h"
#include "msgbox.h"
namespace Ui {
class checkCabinrtWidget;
}

class checkCabinrtWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit checkCabinrtWidget(QWidget *parent = 0);
    ~checkCabinrtWidget();

    void readInfo(QString tempvalue);

private:
    void init();

signals:
    void deleteSignal();

private slots:
    void deleteCabinet(QModelIndex index);
    void myeditCabinet(QModelIndex index);

    void cancelClicked();
    void addCell_slot();

    void updataUI();

    void msgboxOK_slots();

private:
    Ui::checkCabinrtWidget *ui;
    QString       cabinetIdStr;

    takeViewModel * m_model;
    BtnDelegate * m_buttonDelegate;
    //TableModel * m_model;
    //ButtonDelegate * m_buttonDelegate;
    QVector <basicCell>    m_cellValueList;
    editCabinetWidget      *editCabinet;

    addCellWidget            *myaddCell;
    MsgBox                       *myMsgbox;
    QModelIndex             myindex;
};

#endif // CHECKCABINRTWIDGET_H
