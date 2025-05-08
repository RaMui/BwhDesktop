#include "EditConfig.h"
#include "VpsTableModel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include "CheckBoxHeader.h"
#include "BwhDesktop.h"

const QString filename = "config.json";
EditConfig::EditConfig(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	bwhDesktop = dynamic_cast<BwhDesktop*>(parent);
	connect(ui.delete_btn, &QPushButton::clicked, this, &EditConfig::deleteFun);
	connect(ui.cancel_btn, &QPushButton::clicked, this, [=] {
		this->close();
		});
	model = new VpsTableModel(this);
	ui.tableView->setModel(model);

	CheckBoxHeader* header = new CheckBoxHeader(Qt::Horizontal, ui.tableView);
	ui.tableView->setHorizontalHeader(header);
	connect(header, &CheckBoxHeader::checkBoxClicked, this, [this](Qt::CheckState state) {
		for (int row = 0; row < model->rowCount(); ++row) {
			QModelIndex idx = model->index(row, 0);
			model->setData(idx, state, Qt::CheckStateRole);
		}
		});
	connect(model, &QAbstractItemModel::dataChanged, this, [this, header](const QModelIndex&, const QModelIndex&, const QVector<int>& roles) {
		if (roles.contains(Qt::CheckStateRole)) {
			int checkedCount = 0;
			int total = model->rowCount();
			for (int i = 0; i < total; ++i) {
				if (model->index(i, 0).data(Qt::CheckStateRole) == Qt::Checked)
					++checkedCount;
			}

			Qt::CheckState state = Qt::Unchecked;
			if (checkedCount == total)
				state = Qt::Checked;
			else if (checkedCount > 0)
				state = Qt::PartiallyChecked;

			header->setCheckState(state);
		}
		});
}

EditConfig::~EditConfig()
{
}

void EditConfig::refreshData() {
	QFile file;
	file.setFileName(filename);
	QList<VpsInfo> vpsInfoList;
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray data = file.readAll();
		file.close();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
		QJsonArray jsonArray = jsonDoc.array();
		qDebug() << "jsonArray data:" << jsonArray;
		for (const QJsonValue& value : jsonArray) {
			QJsonArray arr = value["ip_addresses"].toArray();
			QStringList list;
			std::transform(arr.begin(), arr.end(), std::back_inserter(list),
				[](const QJsonValue& v) { return v.toString(); });
			vpsInfoList.push_back(VpsInfo(value["title"].toString(), value["hostname"].toString(), list, value["veid"].toString(), value["api_key"].toString()));
		}
	}
	model->setDataList(vpsInfoList);
	auto* header = qobject_cast<CheckBoxHeader*>(ui.tableView->horizontalHeader());
	header->setCheckState(Qt::Unchecked);
}

void EditConfig::deleteFun() {
	QJsonArray list = this->model->getDataList2JsonArray(0);
	QJsonArray checkedList = this->model->getDataList2JsonArray(1);
	qDebug() << "getDataList2JsonArray data:" << list;
	QFile file;
	file.setFileName(filename);
	if (file.open(QIODevice::WriteOnly)) {
		QJsonDocument doc(list);
		file.write(doc.toJson(QJsonDocument::Indented));
		file.close();
	}
	QJsonValue value = list.at(0);
	const QString host = "https://api.64clouds.com/v1/";
	QString vpsUrl = QString("%1getLiveServiceInfo?veid=%2&api_key=%3").arg(host).arg(value["veid"].toString()).arg(value["api_key"].toString());
	QNetworkRequest res(vpsUrl);
	bwhDesktop->manager->get(res);
	QList<int> indexesToRemove = model->getCheckedIndex();
	bwhDesktop->removeTitle(indexesToRemove);
	this->close();
}

void EditConfig::open()
{
	refreshData();
	ui.tableView->resizeColumnsToContents();
	QDialog::open();
}
