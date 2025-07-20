#include "VpsTableModel.h"

VpsTableModel::VpsTableModel(QObject* parent) : QAbstractTableModel(parent) {}

int VpsTableModel::rowCount(const QModelIndex&) const {
	return dataList.size();
}

int VpsTableModel::columnCount(const QModelIndex&) const {
	return 6;
}

QVariant VpsTableModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid() || index.row() >= dataList.size())
		return QVariant();

	const VpsInfo vpsInfo = dataList.at(index.row());

	if (index.column() == 0) {
		if (role == Qt::CheckStateRole)
			return m_checkedStates.at(index.row()) ? Qt::Checked : Qt::Unchecked;
		return QVariant();
	}

	if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 1: return vpsInfo.getTitle();
		case 2: return vpsInfo.getHostname();
		case 3: return vpsInfo.getIpAddresses().join(", ");
		case 4: return vpsInfo.getVeid();
		case 5: return vpsInfo.getApiKey();
		}
	}

	return QVariant();
}

bool VpsTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (!index.isValid() || index.row() >= dataList.size())
		return false;

	if (role == Qt::CheckStateRole && index.column() == 0) {
		bool checked = (value.toInt() == Qt::Checked);
		m_checkedStates[index.row()] = checked;
		emit dataChanged(index, index, { Qt::CheckStateRole });
		return true;
	}

	return false;
	emit headerDataChanged(Qt::Horizontal, 0, 0);
}

Qt::ItemFlags VpsTableModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == 0)
		flags |= Qt::ItemIsUserCheckable;

	return flags;
}

QVariant VpsTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case 1: return "Title";
		case 2: return "Hostname";
		case 3: return "IP Addresses";
		case 4: return "VEID";
		case 5: return "API Key";
		default: return QVariant();
		}
	}
	else {
		return section + 1;
	}
}

void VpsTableModel::addVpsInfo(VpsInfo host, bool checked) {
	beginInsertRows(QModelIndex(), dataList.size(), dataList.size());
	dataList.append(host);
	m_checkedStates.append(checked);
	endInsertRows();
}

void VpsTableModel::setDataList(const QList<VpsInfo> list, const QList<bool>& checkStates) {
	clear();
	dataList = list;
	if (!checkStates.isEmpty() && checkStates.size() == list.size()) {
		m_checkedStates = checkStates;
	}
	else {
		m_checkedStates = QList<bool>(list.size(), false);
	}
}

void VpsTableModel::clear()
{
	beginResetModel();
	dataList.clear();
	m_checkedStates.clear();
	endResetModel();
}

QList<bool> VpsTableModel::getCheckStates() const
{
	return m_checkedStates;
}

QList<VpsInfo> VpsTableModel::getDataList(int type) const
{
	if (type == 2) {
		return dataList;
	}
	QList<VpsInfo> list;
	for (int i = 0; i < dataList.size(); ++i) {
		if (m_checkedStates.value(i) == type)
			list.append(dataList.at(i));
	}
	return list;
}

QJsonArray VpsTableModel::getDataList2JsonArray(int type) const
{
	QList<VpsInfo> list = getDataList(type);
	QJsonArray array;
	for (const auto& data : list) {
		array.append(data.toJsonObject());
	}
	return array;
}

QList<int> VpsTableModel::getCheckedIndex() const {
	QList<int> list;
	for (int i = 0; i < dataList.size(); ++i) {
		if (m_checkedStates.value(i))
			list.append(i);
	}
	return list;
}