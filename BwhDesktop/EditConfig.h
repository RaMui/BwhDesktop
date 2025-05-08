#pragma once

#include <QDialog>
#include "ui_EditConfig.h"
#include "VpsTableModel.h"

class BwhDesktop;
class EditConfig : public QDialog
{
	Q_OBJECT

public:
	void open() override;
	EditConfig(QWidget *parent = nullptr);
	~EditConfig();

public slots:
	void refreshData();
	void deleteFun();

private:
	Ui::EditConfigClass ui;
	VpsTableModel* model;
	BwhDesktop* bwhDesktop;
};
