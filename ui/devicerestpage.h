#ifndef DEVICERESTPAGE_H
#define DEVICERESTPAGE_H

#include <QDialog>

namespace Ui {
class devicerestpage;
}

class devicerestpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit devicerestpage(QWidget *parent = 0);
    ~devicerestpage();
    
private:
    Ui::devicerestpage *ui;


     void initPage();
};

#endif // DEVICERESTPAGE_H
