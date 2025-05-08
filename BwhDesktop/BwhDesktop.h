#pragma once

#include "AddConfig.h"
#include "EditConfig.h"
#include "ui_BwhDesktop.h"
#include <QFile>
#include <QNetworkReply>
#include <QtWidgets/QMainWindow>
#include <QList>

class BwhDesktop : public QMainWindow
{
	Q_OBJECT

public:
	BwhDesktop(QWidget* parent = nullptr);
	~BwhDesktop();
	QNetworkAccessManager* manager;

public slots:
	void exportConfig();

	void importConfig();

	void loadData(QNetworkReply* reply);

	void loadDataJson(QJsonObject& jsonObject);

	void titleChange();

	void removeTitle(QList<int>& index);

	void addTitle(QJsonObject& info);

	void startFun();

	void stopFun();

	void resetFun();

	void hardStopFun();

private:
	Ui::BwhDesktopClass ui;
	AddConfig* addConfig;
	QString vpsUrl;
	EditConfig* editConfig;
	QList<VpsInfo> configList;
	QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
	QTimer* timer;
};
