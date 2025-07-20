#include "addconfig.h"
#include "bwhdesktop.h"
#include "ui_addconfig.h"

#include <QMessageBox>
#include <QPushButton>

const QString host = "https://api.64clouds.com/v1/";
const QString filename = "config.json";
AddConfig::AddConfig(BwhDesktop *b, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddConfig), bwhDesktop(b)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddConfig::confirm);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [=] {
        this->close();
    });
}

AddConfig::~AddConfig()
{
    delete ui;
}

void AddConfig::confirm() {
    QString veid = ui->veid_value->text();
    QString api_key = ui->api_key_value->text();

    file.setFileName(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray oldData = file.readAll();
        list = QJsonDocument::fromJson(oldData).array();
        for (const QJsonValue& value : list) {
            // repeat check
            if (value["veid"] == veid && value["api_key"] == api_key) {
                QMessageBox::warning(this, "Repeat Error", "veid and api_key already exists");
                return;
            }
        }
        file.remove();
    }

    manager = new QNetworkAccessManager(this);
    vpsUrl = host + "getLiveServiceInfo?veid=" + veid + "&api_key=" + api_key;
    QNetworkRequest res(vpsUrl);
    reply = manager->get(res);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (resCode != 200) {
            QMessageBox::warning(this, "network error", "unable to access,please check network");
            qDebug() << "error:" << reply->errorString();
            return;
        }

        if (reply->error() == QNetworkReply::NoError && resCode == 200) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject obj = jsonDoc.object();
            if (obj.value("error") == 0) {

                QJsonObject info;
                info["title"] = obj["hostname"].toString() + " [" + obj["plan"].toString() + "] " + obj["vm_type"].toString();
                info["hostname"] = obj.value("hostname");
                info["veid"] = veid;
                info["api_key"] = api_key;
                info["ip_addresses"] = obj.value("ip_addresses");

                list.append(info);
                qDebug() << "list:" << list;
                if (!file.open(QIODevice::WriteOnly)) {
                    qDebug() << "File write failed!" << file.errorString();
                    QMessageBox::warning(this, "File Error", "File write failed!");
                    return;
                }

                QJsonDocument doc(list);
                file.write(doc.toJson(QJsonDocument::Indented));
                file.close();
                if (bwhDesktop == nullptr) {
                    qDebug() << "bwhDesktop is null";
                    return;
                }

                if (obj.isEmpty()) {
                    qDebug() << "obj is empty, cannot update parent window.";
                    return;
                }
                bwhDesktop->loadDataJson(obj);
                bwhDesktop->addTitle(info);
            }
            else {
                QMessageBox::warning(this, "parameter error", "veid or api_key error,please check again");
                qDebug() << "veid:" << veid;
                qDebug() << "api_key:" << api_key;
                return;
            }
        }
    });
    this->close();
}

void AddConfig::open()
{
    ui->veid_value->setText("");
    ui->api_key_value->setText("");
    QDialog::open();
}
