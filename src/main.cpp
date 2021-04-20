#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QQuickStyle>
#include "asyncpageimageprovider.h"
#include "backend.h"
Q_DECLARE_METATYPE(PageRequest)


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("custro");
    QCoreApplication::setOrganizationDomain("org.custro");
    QCoreApplication::setApplicationName("Tiramisu");
    QQuickStyle::setStyle("Material");

    qRegisterMetaType<PageRequest>();
    QQmlApplicationEngine engine;

    Backend* backend = new Backend();

    engine.addImageProvider("pages", new AsyncPageImageProvider());
    engine.rootContext()->setContextProperty("backend", backend); //do not take ownership

    engine.load(":/res/ui.qml");

    int r = app.exec();
    backend->deleteLater();
    return r;
};


/* BUILD OPTIONS
ANDROID:
ANDROID_SDK_ROOT=/home/guillaume/android-sdk/ cmake -DCMAKE_TOOLCHAIN_FILE=/home/guillaume/android-sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=android-24 -DANDROID_STL=c++_shared -DCMAKE_INSTALL_PREFIX=/home/guillaume/ReaderProject/android_install

OPENCV:
-DBUILD_ANDROID_EXAMPLES=off -DBUILD_JAVA=off -DBUILD_EXAMPLES=off -DBUILD_CUDA_STUBS=off -DBUILD_SHARED_LIBS=on -DBUILD_ANDROID_PROJECTS=off -DBUILD_ANDROID_SERVICE=off -DBUILD_FAT_JAVA_LIB=off -DBUILD_LIST=core,imgproc,imgcodecs,feature2d,stitching ..

LIBARCHIVE:
android alone

LIBPNG
-DPNG_STATIC=OFF -DPNG_TESTS=OFF -DHAVE_LD_VERSION_SCRIPT=OFF ..

LIBFREETYPE
-DBUILD_SHARED_LIBS=ON

LIBTIFF
android alone

LIBOPENJPEG
-DBUILD_STATIC_LIBS=OFF //optional

*/
