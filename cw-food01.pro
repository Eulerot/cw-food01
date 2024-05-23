QT += core gui widgets
QT += charts
QT += printsupport

TEMPLATE = app

TRANSLATIONS += translations/cw-food01_en_US.ts \
                translations/cw-food01_ru_RU.ts \
                translations/cw-food01_fr_FR.ts

SOURCES += \
    chartdialog.cpp \
    main.cpp \
    restaurant.cpp \
    restaurantmodel.cpp \
    restaurantcontroller.cpp \
    mainwindow.cpp \
    restaurantdetailswindow.cpp

HEADERS += \
    chartdialog.h \
    restaurant.h \
    restaurantmodel.h \
    restaurantcontroller.h \
    mainwindow.h \
    restaurantdetailswindow.h


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


