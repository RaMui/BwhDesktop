#include "VpsInfo.h"

VpsInfo::VpsInfo(const QString& title, const QString& hostname, const QStringList& ipAddresses,
	const QString& veid, const QString& apiKey)
	: m_title(title),
	m_hostname(hostname),
	m_ipAddresses(ipAddresses),
	m_veid(veid),
	m_apiKey(apiKey)
{
}

QString VpsInfo::getTitle() const {
	return m_title;
}

void VpsInfo::setTitle(const QString& title) {
	m_title = title;
}

QString VpsInfo::getHostname() const {
	return m_hostname;
}

void VpsInfo::setHostname(const QString& hostname) {
	m_hostname = hostname;
}

QStringList VpsInfo::getIpAddresses() const {
	return m_ipAddresses;
}

void VpsInfo::setIpAddresses(const QStringList& ipAddresses) {
	m_ipAddresses = ipAddresses;
}

QString VpsInfo::getVeid() const {
	return m_veid;
}

void VpsInfo::setVeid(const QString& veid) {
	m_veid = veid;
}

QString VpsInfo::getApiKey() const {
	return m_apiKey;
}

void VpsInfo::setApiKey(const QString& apiKey) {
	m_apiKey = apiKey;
}

QString VpsInfo::toString() const {
	return QString("Title: %1 Hostname: %2 IP Addresses: %3 VEID: %4 API Key: %5")
		.arg(m_title)
		.arg(m_hostname)
		.arg(m_ipAddresses.join(", "))
		.arg(m_veid)
		.arg(m_apiKey);
}

QJsonObject VpsInfo::toJsonObject() const
{
	QJsonObject obj;
	obj["title"] = m_title;
	obj["hostname"] = m_hostname;
	obj["ip_addresses"] = QJsonArray::fromStringList(m_ipAddresses);
	obj["veid"] = m_veid;
	obj["api_key"] = m_apiKey;
	return obj;
}
