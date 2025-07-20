#ifndef VPSINFO_H
#define VPSINFO_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class VpsInfo
{
public:
	VpsInfo() = default;
	VpsInfo(const QString& title, const QString& hostname, const QStringList& ipAddresses,
		const QString& veid, const QString& apiKey);

	QString getTitle() const;
	void setTitle(const QString& title);

	QString getHostname() const;
	void setHostname(const QString& hostname);

	QStringList getIpAddresses() const;
	void setIpAddresses(const QStringList& ipAddresses);

	QString getVeid() const;
	void setVeid(const QString& veid);

	QString getApiKey() const;
	void setApiKey(const QString& apiKey);

	QString toString() const;

	QJsonObject toJsonObject() const;

private:
	QString m_title;
	QString m_hostname;
	QStringList m_ipAddresses;
	QString m_veid;
	QString m_apiKey;
};

#endif // VPSINFO_H
