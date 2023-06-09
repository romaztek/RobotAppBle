
TARGET = PuppetController

QT += bluetooth quick gamepad

SOURCES += main.cpp \
           bt_commands.cpp \
           btcontroller.cpp \
           logic.cpp \
           enums.cpp

HEADERS += \
           bt_commands.h \
           btcontroller.h \
           logic.h \
           enums.h

RESOURCES += qml.qrc

TRANSLATIONS += \
    PuppetController_ru_RU.ts

DISTFILES += qml/main.qml \
             android/AndroidManifest.xml \
             android/build.gradle \
             android/gradle/wrapper/gradle-wrapper.jar \
             android/gradle/wrapper/gradle-wrapper.properties \
             android/gradlew \
             android/gradlew.bat \
             android/res/values/libs.xml \
             qml/AudioSelectorWindow.qml \
             qml/ConnectWindow.qml \
             qml/ControlWindow.qml \
             qml/CommandButton.qml \
             qml/FaceImageSelectorWindow \
             qml/MoveButton.qml \
             qml/MyIconButton.qml \
             qml/MyIconLabel.qml \
             qml/MyIconRadioButtonLabel.qml \
             qml/ServoControlWindow.qml \
             qml/ManualDriveWindow.qml \
             qml/AudioSelectorWindow.qml

lupdate_only{
SOURCES = qml/main.qml \
          qml/ConnectWindow.qml \
          qml/ControlWindow.qml \
          qml/CommandButton.qml \
          qml/FaceImageSelectorWindow \
          qml/MoveButton.qml \
          qml/MyIconButton.qml \
          qml/MyIconLabel.qml \
          qml/MyIconRadioButtonLabel.qml \
          qml/ServoControlWindow.qml \
          qml/ManualDriveWindow.qml
}

android{
QT+= androidextras
}


ios {

QML_IMPORT_PATH += /Users/roman/Qt5.9.9/5.9.9/ios/qml/
QML2_IMPORT_PATH += /Users/roman/Qt5.9.9/5.9.9/ios/qml/

QMAKE_INFO_PLIST = $$PWD/ios/Info.plist
QMAKE_ASSET_CATALOGS += ios/Assets.xcassets
ios_icon.files = $$files($$PWD/ios/AppIcon.appiconset/Icon*.png)
QMAKE_BUNDLE_DATA += ios_icon
app_launch_screen.files = $$files($$PWD/ios/LaunchScreen.storyboard)
QMAKE_BUNDLE_DATA += app_launch_screen
app_launch_image.files += $$files($$PWD/ios/Icon256.png)
QMAKE_BUNDLE_DATA += app_launch_image

OTHER_FILES += ios/*.png

QMAKE_IOS_DEPLOYMENT_TARGET = 8.0

QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2

QMAKE_TARGET_BUNDLE_PREFIX = ru.romankartashev
QMAKE_BUNDLE = PuppetController

MY_BUNDLE_ID.name = PRODUCT_BUNDLE_IDENTIFIER
MY_BUNDLE_ID.value = ru.romankartashev.PuppetController
QMAKE_MAC_XCODE_SETTINGS += MY_BUNDLE_ID

MY_BUNDLE_VERSION.name = SHORT_VERSION_STRING
MY_BUNDLE_VERSION.value = 6.0
QMAKE_MAC_XCODE_SETTINGS += MY_BUNDLE_VERSION

MY_BUNDLE_VER.name = CURRENT_PROJECT_VERSION
MY_BUNDLE_VER.value = 6.0
QMAKE_MAC_XCODE_SETTINGS += MY_BUNDLE_VER

#CONFIG -= simulator iphonesimulator
#CONFIG += iphoneos
#CONFIG += device
#CONFIG += release
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
