#ifndef BWHDESKTOP_H
#define BWHDESKTOP_H

#include "VpsInfo.h"

#include <QMainWindow>

#include <QtNetwork/QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
class BwhDesktop;
}
QT_END_NAMESPACE

class BwhDesktop : public QMainWindow
{
    Q_OBJECT

public:
    BwhDesktop(QWidget *parent = nullptr);
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
    Ui::BwhDesktop *ui;
    QString vpsUrl;
    QList<VpsInfo> configList;
    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    QTimer* timer;
};
#endif // BWHDESKTOP_H
