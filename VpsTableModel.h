// VpsTableModel.h
#ifndef VPSTABLEMODEL_H
#define VPSTABLEMODEL_H

#include <QAbstractTableModel>
#include "VpsInfo.h"

class VpsTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit VpsTableModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void addVpsInfo(VpsInfo host, bool checked = false);
	void setDataList(QList<VpsInfo> list, const QList<bool>& checkStates = {});
	void clear();
	QList<bool> getCheckStates() const;
	QList<VpsInfo> getDataList(int type = 2) const;
	QJsonArray getDataList2JsonArray(int type = 2) const;
	QList<int> getCheckedIndex() const;

private:
	QList<VpsInfo> dataList;
	QList<bool> m_checkedStates;
};

#endif // VPSTABLEMODEL_H
