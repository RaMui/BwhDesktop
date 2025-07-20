#ifndef EDITCONFIG_H
#define EDITCONFIG_H

#include "VpsTableModel.h"

#include <QDialog>

namespace Ui {
class EditConfig;
}

class BwhDesktop;
class EditConfig : public QDialog
{
    Q_OBJECT

public:
    void open() override;
    explicit EditConfig(BwhDesktop *b, QWidget *parent = nullptr);
    ~EditConfig();

public slots:
    void refreshData();
    void deleteFun();

private:
    Ui::EditConfig *ui;
    BwhDesktop* bwhDesktop;
    VpsTableModel* model;
};

#endif // EDITCONFIG_H
