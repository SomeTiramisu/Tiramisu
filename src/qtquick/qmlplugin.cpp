#include "qmlplugin.h"

#include <QQmlContext>
#include "backend.h"
#include "asyncpageimageprovider.h"
#include "pageitem.h"


void QmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
    engine->addImageProvider("pages", new AsyncPageImageProvider());
}

void QmlPlugin::registerTypes(const char *uri) {
    qmlRegisterType<PageItem>(uri, 1, 0, "PageView");
    qWarning("Hey You");
}
