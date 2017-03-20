#ifndef APPCONFIG_H
#define APPCONFIG_H
#include <QJsonObject>
#include <QObject>

class AppConfig : public QJsonObject
{
public:
    AppConfig();
    AppConfig(const QJsonObject& src);
    bool loadFromFile(const QString& fn);
    bool saveToFile(const QString& fn);
    static AppConfig defaultConfig();
};

extern AppConfig glbAppConfig;

#endif // APPCONFIG_H
