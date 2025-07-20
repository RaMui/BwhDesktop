#ifndef ADDCONFIG_H
#define ADDCONFIG_H

#include <QDialog>
#include <QFile>
#include <QJsonArray>
#include <QNetworkReply>

namespace Ui {
class AddConfig;
}
class BwhDesktop;
class AddConfig : public QDialog
{
    Q_OBJECT

public:
    void open() override;
    explicit AddConfig(BwhDesktop *b, QWidget *parent = nullptr);
    ~AddConfig();

public slots:
    void confirm();

private:
    Ui::AddConfig *ui;
    BwhDesktop* bwhDesktop;
    QFile file;
    QNetworkReply* reply;
    QString vpsUrl;
    QNetworkAccessManager* manager;
    QJsonArray list;
};

#endif // ADDCONFIG_H
