#ifndef CABINETCELLPACKAGE_H
#define CABINETCELLPACKAGE_H

#include <QWidget>
#include "addcabinetwidget.h"
#include "checkcabinrtwidget.h"
#include "msgbox.h"
namespace Ui {
class cabinetCellPackage;
}

class cabinetCellPackage : public QWidget
{
    Q_OBJECT
    
public:
    explicit cabinetCellPackage(QWidget *parent = 0);
    ~cabinetCellPackage();
    void init();
    void readInfo();

private slots:
    void cancelBtnClicked();
    void addCabinetClicked();

    void checkCabinet(QModelIndex index);
    void deleteCabinet(QModelIndex index);

    void updataInfo();

    void msgboxOK_slot();

private:
    Ui::cabinetCellPackage *ui;
    QStringList cabinetID;
    QStringList cabinetType;
    QStringList cabinetNum;
    takeViewModel * m_model;
    BtnDelegate * m_buttonDelegate;

    addCabinetWidget *addCabinet;
    checkCabinrtWidget *checkCabinrt;
    MsgBox                     *myMsgbox;

    QModelIndex             myindex;
};

#endif // CABINETCELLPACKAGE_H
