#pragma once

#include <QDialog>
#include "ui_AddConfig.h"
#include <QFile>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>

class BwhDesktop;
class AddConfig : public QDialog
{
	Q_OBJECT

public:
	void open() override;
	AddConfig(QWidget *parent = nullptr);
	~AddConfig();

public slots:
	void confirm();

private:
	Ui::AddConfigClass ui;
	QFile file;
	QNetworkReply* reply;
	QString vpsUrl;
	QNetworkAccessManager* manager;
	QJsonArray list;
	BwhDesktop* bwhDesktop;
};
