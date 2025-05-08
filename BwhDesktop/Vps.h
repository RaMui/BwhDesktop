#pragma once

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <QJsonObject>
#include <QJsonArray>

struct Vps {
    std::string vm_type;
    std::string hostname;
    std::string node_alias;
    std::string node_location;
    std::string node_location_id;
    bool location_ipv6_ready;
    std::string plan;
    uint64_t plan_disk;
    uint64_t plan_ram;
    std::string os;
    std::string email;
    uint64_t plan_monthly_data;
    uint64_t data_counter;
    double monthly_data_multiplier;
    time_t data_next_reset;
    std::vector<std::string> ip_addresses;
    std::vector<std::string> private_ip_addresses;
    std::vector<std::string> ip_nullroutes;
    std::string iso1;
    std::string iso2;
    std::vector<std::string> available_isos;
    int plan_max_ipv6s;
    bool rdns_api_available;
    bool plan_private_network_available;
    bool location_private_network_available;
    std::map<std::string, std::string> ptr;
    bool suspended;
    bool policy_violation;
    int suspension_count;
    int total_abuse_points;
    int max_abuse_points;
    std::string ve_status;
    double ssh_port;
    double veid;
    uint64_t ve_used_disk_space_b;
    std::string ve_disk_quota_gb;
    uint64_t swap_total_kb;
    uint64_t swap_available_kb;
    uint64_t mem_available_kb;

    void parseVps(const QJsonObject& jsonObject) {
        vm_type = jsonObject.value("vm_type").toString().toStdString();
        hostname = jsonObject.value("hostname").toString().toStdString();
        node_alias = jsonObject.value("node_alias").toString().toStdString();
        node_location = jsonObject.value("node_location").toString().toStdString();
        location_ipv6_ready = jsonObject.value("location_ipv6_ready").toBool();
        plan = jsonObject.value("plan").toString().toStdString();
        plan_disk = jsonObject.value("plan_disk").toVariant().toULongLong();
        plan_ram = jsonObject.value("plan_ram").toVariant().toULongLong();
        os = jsonObject.value("os").toString().toStdString();
        email = jsonObject.value("email").toString().toStdString();
        plan_monthly_data = jsonObject.value("plan_monthly_data").toVariant().toULongLong();
        data_counter = jsonObject.value("data_counter").toVariant().toULongLong();
        monthly_data_multiplier = jsonObject.value("monthly_data_multiplier").toDouble();
        data_next_reset = static_cast<time_t>(jsonObject.value("data_next_reset").toVariant().toLongLong());


        for (const auto& ip : jsonObject.value("ip_addresses").toArray()) {
            ip_addresses.push_back(ip.toString().toStdString());
        }
        for (const auto& privateIp : jsonObject.value("private_ip_addresses").toArray()) {
            private_ip_addresses.push_back(privateIp.toString().toStdString());
        }
        for (const auto& nullRoute : jsonObject.value("ip_nullroutes").toArray()) {
            ip_nullroutes.push_back(nullRoute.toString().toStdString());
        }
        for (const auto& iso : jsonObject.value("available_isos").toArray()) {
            available_isos.push_back(iso.toString().toStdString());
        }


        QJsonObject ptrObject = jsonObject.value("ptr").toObject();
        for (const QString& key : ptrObject.keys()) {
            ptr[key.toStdString()] = ptrObject.value(key).toString().toStdString();
        }

        plan_max_ipv6s = jsonObject.value("plan_max_ipv6s").toInt();
        rdns_api_available = jsonObject.value("rdns_api_available").toBool();
        plan_private_network_available = jsonObject.value("plan_private_network_available").toBool();
        location_private_network_available = jsonObject.value("location_private_network_available").toBool();
        suspended = jsonObject.value("suspended").toBool();
        policy_violation = jsonObject.value("policy_violation").toBool();
        suspension_count = jsonObject.value("suspension_count").toInt();
        total_abuse_points = jsonObject.value("total_abuse_points").toInt();
        max_abuse_points = jsonObject.value("max_abuse_points").toInt();
        ve_status = jsonObject.value("ve_status").toString().toStdString();
        ssh_port = jsonObject.value("ssh_port").toDouble();
        veid = jsonObject.value("veid").toDouble();
        node_location_id = jsonObject.value("node_location_id").toString().toStdString();
        ve_used_disk_space_b = jsonObject.value("ve_used_disk_space_b").toVariant().toULongLong();
        ve_disk_quota_gb = jsonObject.value("ve_disk_quota_gb").toString().toStdString();
        swap_total_kb = jsonObject.value("swap_total_kb").toVariant().toULongLong();
        swap_available_kb = jsonObject.value("swap_available_kb").toVariant().toULongLong();
        mem_available_kb = jsonObject.value("mem_available_kb").toVariant().toULongLong();
    }

};