#include "qmlplugin.h"

#include <QQmlContext>
#include "backend.h"
#include "pageitem.h"


void QmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
    Q_UNUSED(engine)
    Q_UNUSED(uri)
}

void QmlPlugin::registerTypes(const char *uri) {
    qmlRegisterType<PageItem>(uri, 1, 0, "PageView");
}
