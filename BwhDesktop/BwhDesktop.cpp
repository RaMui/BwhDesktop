#include "BwhDesktop.h"
#include <QFileDialog>
#include <QMessageBox>
#include "Vps.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>

const QString host = "https://api.64clouds.com/v1/";
const QString configFilename = "config.json";
BwhDesktop::BwhDesktop(QWidget* parent)
	: QMainWindow(parent), timer(new QTimer(this))
{
	ui.setupUi(this);

	addConfig = new AddConfig(this);
	editConfig = new EditConfig(this);

	connect(ui.config_export, &QAction::triggered, this, &BwhDesktop::exportConfig);
	connect(ui.config_import, &QAction::triggered, this, &BwhDesktop::importConfig);
	connect(ui.config_create, &QAction::triggered, addConfig, &QDialog::open);
	connect(ui.config_edit, &QAction::triggered, editConfig, &QDialog::open);
	connect(ui.button_start, &QPushButton::clicked, this, &BwhDesktop::startFun);
	connect(ui.button_stop, &QPushButton::clicked, this, &BwhDesktop::stopFun);
	connect(ui.button_reset, &QPushButton::clicked, this, &BwhDesktop::resetFun);
	connect(ui.button_hard_stop, &QPushButton::clicked, this, &BwhDesktop::hardStopFun);

	QFile file;
	file.setFileName(configFilename);
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray data = file.readAll();
		file.close();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
		QJsonArray jsonArray = jsonDoc.array();
		qDebug() << "jsonArray data:" << jsonArray;
		if (!jsonArray.isEmpty()) {
			QJsonValue value = jsonArray.at(0);
			vpsUrl = QString("%1getLiveServiceInfo?veid=%2&api_key=%3").arg(host).arg(value["veid"].toString()).arg(value["api_key"].toString());
			manager = new QNetworkAccessManager(this);
			qDebug() << "vpsUrl" << vpsUrl;
			QNetworkRequest res(vpsUrl);
			manager->get(res);
			for (const auto& val : jsonArray) {
				QString title = val.toObject().value("title").toString();
				ui.vps_title_comboBox->addItem(title);
				QJsonArray arr = val.toObject()["ip_addresses"].toArray();
				QStringList list;
				std::transform(arr.begin(), arr.end(), std::back_inserter(list),
					[](const QJsonValue& v) { return v.toString(); });
				configList.push_back(VpsInfo(val.toObject()["title"].toString(), val.toObject()["hostname"].toString(), list, val.toObject()["veid"].toString(), val.toObject()["api_key"].toString()));
			}
		}
	}
	connect(manager, &QNetworkAccessManager::finished, this, &BwhDesktop::loadData);
	connect(netManager, &QNetworkAccessManager::finished, this, [=]() {
		QTimer::singleShot(3000, this, [=]() {
			titleChange();
			});
		});
	connect(ui.vps_title_comboBox, &QComboBox::currentTextChanged, this, &BwhDesktop::titleChange);

	connect(timer, &QTimer::timeout, this, &BwhDesktop::titleChange);
	timer->start(1000 * 15);
}

BwhDesktop::~BwhDesktop()
{
	delete addConfig;
	delete editConfig;
}

void BwhDesktop::exportConfig()
{
	QFile file;
	QByteArray arraydata;
	file.setFileName(configFilename);
	QList<VpsInfo> vpsInfoList;
	if (file.open(QIODevice::ReadOnly)) {
		arraydata = file.readAll();
		file.close();
	}
	QString filename = QFileDialog::getSaveFileName(this, tr("Config Export"), "", "Json File(*.json)");
	if (filename.isEmpty())
	{
		return;
	}
	file.setFileName(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, "file error", "no json file name");
		return;
	}
	else
	{
		file.write(arraydata);
		file.close();
	}
}

void BwhDesktop::importConfig()
{
	QFile file;
	QByteArray arraydata;
	QString filename = QFileDialog::getOpenFileName(this, tr("Config Import"), "", "*.json");
	file.setFileName(filename);
	QList<VpsInfo> vpsInfoList;
	if (file.open(QIODevice::ReadOnly)) {
		arraydata = file.readAll();
		file.close();
	}
	file.setFileName(configFilename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "File open failed!";
	}
	file.write(arraydata);
	file.close();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(arraydata);
	QJsonArray jsonArray = jsonDoc.array();
	if (!jsonArray.isEmpty()) {
		QJsonValue value = jsonArray.at(0);
		vpsUrl = QString("%1getLiveServiceInfo?veid=%2&api_key=%3").arg(host).arg(value["veid"].toString()).arg(value["api_key"].toString());
		qDebug() << "vpsUrl" << vpsUrl;
		QNetworkRequest res(vpsUrl);
		manager->get(res);
	}
}

void BwhDesktop::loadData(QNetworkReply* reply)
{
	int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if (reply->error() == QNetworkReply::NoError && resCode == 200) {
		QByteArray data = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
		QJsonObject obj = jsonDoc.object();
		loadDataJson(obj);
	}
	else {
		QMessageBox::warning(this, "network error", "something error");
		qDebug() << "error:" << reply->errorString();
	}
}

void BwhDesktop::loadDataJson(QJsonObject& jsonObject)
{
	Vps vps;
	vps.parseVps(jsonObject);

	ui.physical_location_value->setText(QString::fromStdString(vps.node_location) + " Node ID: " + QString::fromStdString(vps.node_alias) + "  VM ID: " + QString::number(vps.veid));
	ui.public_ip_address_value->setText(QString::fromStdString(vps.ip_addresses[0]));
	ui.ssh_port_value->setText(QString::number(vps.ssh_port));
	ui.status_value->setText(QString::fromStdString(vps.ve_status));
	ui.os_value->setText(QString::fromStdString(vps.os));
	ui.hostname_value->setText(QString::fromStdString(vps.hostname));

	double ram_total = vps.plan_ram / (1024 * 1024);
	double ram_usage = vps.plan_ram / 1024 - vps.mem_available_kb;
	ui.ram_value->setText(QString::number(ram_usage / 1024, 'f', 2) + "/" + QString::number(ram_total) + " MB");
	ui.progressBar_ram->setValue(static_cast<double>(ram_usage) / (vps.plan_ram / 1024) * 100);

	double swap_total = static_cast<double>(vps.swap_total_kb) / 1024;
	double swap_usage = static_cast<double>((vps.swap_total_kb - vps.swap_available_kb)) / 1024;
	ui.progressBar_swap->setValue(swap_usage / swap_total * 100);
	ui.swap_value->setText(QString::number(swap_usage, 'f', swap_usage == 0 ? 0 : 2) + "/" + QString::number(swap_total, 'f', 0) + " MB");

	double used_disk = static_cast<double>(vps.ve_used_disk_space_b) / (1024 * 1024 * 1024);
	ui.disk_usage_value->setText(QString::number(used_disk, 'f', 1) + "/" + QString::fromStdString(vps.ve_disk_quota_gb) + " GB");
	double disk_usage = static_cast<double>(vps.ve_used_disk_space_b) / vps.plan_disk * 100;
	ui.progressBar_disk_usage->setValue(disk_usage);

	QDateTime dateTime = QDateTime::fromSecsSinceEpoch(vps.data_next_reset);
	dateTime = dateTime.toLocalTime();
	QString dateStr = dateTime.toString("yyyy-MM-dd");
	ui.bandwidth_resets->setText(dateStr);
	ui.progressBar_bandwidth_usage->setValue(static_cast<double>(vps.data_counter) / vps.plan_monthly_data * 100);
	// bytes to GB
	double counter = static_cast<double>(vps.data_counter) * vps.monthly_data_multiplier / (1024 * 1024 * 1024);
	double monthly_data = vps.plan_monthly_data * vps.monthly_data_multiplier / (1024 * 1024 * 1024);
	ui.bandwidth_usage_value->setText(QString::number(counter, 'f', 2) + "/" + QString::number(monthly_data, 'f', 0) + " GB - " + QString::fromStdString(vps.node_location_id) + " Premium Bandwidth Multiplier: " + QString::number(vps.monthly_data_multiplier));

}

void BwhDesktop::titleChange() {
	if (configList.isEmpty()) {
		return;
	}
	int selectedIndex = ui.vps_title_comboBox->currentIndex();
	vpsUrl = QString("%1getLiveServiceInfo?veid=%2&api_key=%3").arg(host).arg(configList.at(selectedIndex).getVeid()).arg(configList.at(selectedIndex).getApiKey());
	QNetworkRequest res(vpsUrl);
	manager->get(res);
}

void BwhDesktop::removeTitle(QList<int>& indexesToRemove) {
	std::sort(indexesToRemove.begin(), indexesToRemove.end(), std::greater<int>());
	for (int index : indexesToRemove) {
		if (index >= 0 && index < ui.vps_title_comboBox->count()) {
			ui.vps_title_comboBox->removeItem(index);
			configList.removeAt(index);
		}
	}
	if (!configList.isEmpty()) {
		ui.vps_title_comboBox->setCurrentIndex(0);
	}
	else {
		// todo 刷新页面全部置为空
	}
}

void BwhDesktop::addTitle(QJsonObject& info) {
	ui.vps_title_comboBox->addItem(info["title"].toString());
	QJsonArray arr = info["ip_addresses"].toArray();
	QStringList list;
	std::transform(arr.begin(), arr.end(), std::back_inserter(list),
		[](const QJsonValue& v) { return v.toString(); });
	configList.push_back(VpsInfo(info["title"].toString(), info["hostname"].toString(), list, info["veid"].toString(), info["api_key"].toString()));
	ui.vps_title_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	QSignalBlocker blocker(ui.vps_title_comboBox);
	ui.vps_title_comboBox->setCurrentIndex(ui.vps_title_comboBox->count() - 1);
}


void BwhDesktop::startFun() {
	int selectedIndex = ui.vps_title_comboBox->currentIndex();
	QString url = QString("%1start?veid=%2&api_key=%3").arg(host).arg(configList.at(selectedIndex).getVeid()).arg(configList.at(selectedIndex).getApiKey());
	QNetworkRequest res(url);
	netManager->get(res);
	QMessageBox::information(this, "Actions success", "Server will start in a few seconds.");
}

void BwhDesktop::stopFun() {
	int selectedIndex = ui.vps_title_comboBox->currentIndex();
	QString url = QString("%1stop?veid=%2&api_key=%3").arg(host).arg(configList.at(selectedIndex).getVeid()).arg(configList.at(selectedIndex).getApiKey());
	QNetworkRequest request;
	request.setUrl(url);
	netManager->get(request);
	QMessageBox::information(this, "Actions success", "Server will stop in a few seconds.");
}

void BwhDesktop::resetFun() {
	int selectedIndex = ui.vps_title_comboBox->currentIndex();
	QString url = QString("%1restart?veid=%2&api_key=%3").arg(host).arg(configList.at(selectedIndex).getVeid()).arg(configList.at(selectedIndex).getApiKey());
	QNetworkRequest res(url);
	netManager->get(res);
	QMessageBox::information(this, "Actions success", "Server will restart in a few seconds.");
}

void BwhDesktop::hardStopFun() {
	int selectedIndex = ui.vps_title_comboBox->currentIndex();
	QString url = QString("%1kill?veid=%2&api_key=%3").arg(host).arg(configList.at(selectedIndex).getVeid()).arg(configList.at(selectedIndex).getApiKey());
	QNetworkRequest res(url);
	netManager->get(res);
	QMessageBox::information(this, "Actions success", "Server will stop in a few seconds.");
}
