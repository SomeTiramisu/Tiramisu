#include <QApplication>
#include "mainwindowreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("custro");
    QCoreApplication::setOrganizationDomain("org.custro");
    QCoreApplication::setApplicationName("reader");
    MainWindowReader mainWindowReader;
    mainWindowReader.showFullScreen();
    return a.exec();
};


/* BUILD OPTIONS

OPENCV:
ANDROID_SDK_ROOT=/home/guillaume/android-sdk/ cmake -DCMAKE_TOOLCHAIN_FILE=/home/guillaume/android-sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=android-24 -DANDROID_STL=c++_shared -DCMAKE_INSTALL_PREFIX=/run/media/guillaume/DATA/ReaderProject/opencv-4.5.0/android/prefix -DBUILD_ANDROID_EXAMPLES=off -DBUILD_JAVA=off -DBUILD_EXAMPLES=off -DBUILD_CUDA_STUBS=off -DBUILD_SHARED_LIBS=on -DBUILD_ANDROID_PROJECTS=off -DBUILD_ANDROID_SERVICE=off -DBUILD_FAT_JAVA_LIB=off -DBUILD_LIST=core,imgproc,imgcodecs,feature2d,stitching ..

LIBARCHIVE:
same without extra options
*/
