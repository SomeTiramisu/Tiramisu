#ifndef QMLPLUGIN_H
#define QMLPLUGIN_H

#include <QQmlExtensionPlugin>

class QmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void initializeEngine(QQmlEngine *engine, const char *uri) override;
    void registerTypes(const char *uri) override;
};


#endif // QMLPLUGIN_H
