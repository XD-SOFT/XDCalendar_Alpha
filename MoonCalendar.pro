#-------------------------------------------------
#
# Project created by QtCreator 2016-08-24T16:07:01
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++14
TARGET = MoonCalendar
TEMPLATE = app

####used ColorPicker.
CONFIG(debug,debug|release) {
    LIBS += -L./libs -lColorPickerd
    LIBS += -L./libs -lQtSingleApplicationd
    #开发过程中这个定义要打开,因为最后发布的exe与开发过程中的exe路径不一样，会导致QApplication::applicationDirPath不一yang.
    DEFINES += TRUNK_VERSION
} else {
    LIBS += -L./libs -lColorPicker
    LIBS += -L./libs -lQtSingleApplication
    DEFINES += USE_RUNNING_LOG
}

#In development, defines USED_STANDARD_WRITE_LOCAL,use skin config path.
CONFIG(debug, debug|release) {
    DEFINES += USED_STANDARD_WRITE_LOCAL
}

INCLUDEPATH += ./include

SOURCES += main.cpp \
    MainScreen.cpp \
    daywidget.cpp \
    mybutton.cpp \
    date.cpp \
    database.cpp \
    setupdialog.cpp \
    batchadddialog.cpp \
    coursecellwidget.cpp \
    minidatewidget.cpp \
    minifilelistwidget.cpp \
    minimainscreen.cpp \
    miniuserinfowidget.cpp \
    gridviewweek.cpp \
    userinfoview.cpp \
    copyhelper.cpp \
    qtfilecopier.cpp \
    wavewidget.cpp \
    filelistwidget.cpp \
    userlistwidget.cpp \
    livewidget.cpp \
    searchbox.cpp \
    teacher.cpp \
    term.cpp \
    lesson.cpp \
    folder.cpp \
    filenumwidget.cpp \
    config.cpp \
    ccu.cpp \
    notewidget.cpp \
    termwidget.cpp \
    colorbutton.cpp \
    editbutton.cpp \
    editwindow.cpp \
    titlebar.cpp \
    httpop.cpp \
    logindialog.cpp \
    loginthread.cpp \
    lookandfeelconfigdialog.cpp \
    masklabel.cpp \
    handler.cpp \
    filetransfer.cpp \
    dialog.cpp \
    myfileiconprovider.cpp \
    feedbackdialog.cpp \
    modifypassworddialog.cpp \
    sidebar.cpp \
    termdb.cpp \
    scheduledb.cpp \
    scheduledetaildb.cpp \
    lessondb.cpp \
    lessondetaildb.cpp \
    lessonremarkdb.cpp \
    resfilesdb.cpp \
    semester.cpp \
    semesterdialog.cpp \
    social/friend.cpp \
    social/masklabelunclickable.cpp \
    social/livecardwidget.cpp \
    social/friendcardwidget.cpp \
    subjectinfodb.cpp \
    gradeinfodb.cpp \
    userdb.cpp \
    clientconfigdb.cpp \
    social/timediff.cpp \
    keyworddb.cpp \
    filekeyword.cpp \
    social/frienddetaildialog.cpp \
    tools.cpp \
    social/livelistwidget.cpp \
    social/livemessage.cpp \
    social/friendlistwidget.cpp\
    getdatanw.cpp \
    scheduledialog.cpp \
    tools.cpp \
    schedule.cpp \
    editsche.cpp \
    editsem.cpp \
    social/animationstackedwidget.cpp\
    operationbutton.cpp \
    editsectime.cpp \
    dlgconfig.cpp \
    flowlayout.cpp \
    schedetdb.cpp \
    uploadfiles2ftp.cpp \
    appconfig.cpp \
    downloadfile.cpp \
    baseshadowwidget.cpp \
    coursetreewidget.cpp \
    accountitem.cpp \
    infoandsettingsstackwidget.cpp \
    classinfowidget.cpp \
    editrightwidget.cpp \
    treedisplaydelegate.cpp \
    framelessmodalmovableshadowwidget.cpp \
    coursecolorpickerwidget.cpp \
    loginstatuswidget.cpp \
    userinfowidget.cpp \
    dataclassinstancemanage.cpp \
    inputtextwidget.cpp \
    settingswidget.cpp \
    mynode.cpp \
    mainwindow.cpp \
    messagedisplaywidget.cpp \
    filetransferwnd.cpp \
    aboutwlg.cpp \
    calendarwidget.cpp

HEADERS  += MainScreen.h daywidget.h mybutton.h date.h dragbox.h database.h \
    setupdialog.h \
    batchadddialog.h \
    coursecellwidget.h \
    minidatewidget.h \
    minifilelistwidget.h \
    minimode.h \
    miniuserinfowidget.h \
    gridviewweek.h \
    userinfoview.h \
    copyhelper.h \
    qtfilecopier.h \
    wavewidget.h \
    filelistwidget.h \
    userlistwidget.h \
    livewidget.h \
    searchbox.h \
    teacher.h \
    term.h \
    lesson.h \
    folder.h \
    filenumwidget.h \
    config.h \
    ccu.h \
    minimainscreen.h \
    notewidget.h \
    termwidget.h \
    colorbutton.h \
    editbutton.h \
    editwindow.h \
    titlebar.h \
    httpop.h \
    logindialog.h \
    loginthread.h \
    lookandfeelconfigdialog.h \
    masklabel.h \
    handler.h \
    filetransfer.h \
    dialog.h \
    myfileiconprovider.h \
    feedbackdialog.h \
    modifypassworddialog.h \
    sidebar.h \
    termdb.h \
    scheduledb.h \
    scheduledetaildb.h \
    lessondb.h \
    lessondetaildb.h \
    lessonremarkdb.h \
    resfilesdb.h \
    semester.h \
    semesterdialog.h \
    social/friend.h \
    social/livecardwidget.h \
    social/maskunclickablelabel.h \
    social/friendcardwidget.h \
    subjectinfodb.h \
    gradeinfodb.h \
    userdb.h \
    clientconfigdb.h \
    social/timediff.h \
    keyworddb.h \
    filekeyword.h \
    social/frienddetaildialog.h \
    tools.h \
    social/livelistwidget.h \
    social/livemessage.h \
    social/friendlistwidget.h\
    getdatanw.h \
    scheduledialog.h \
    schedule.h \
    editsche.h \
    editsem.h \
    social/animationstackedwidget.h\
    operationbutton.h \
    tools.h \
    editsectime.h \
    dlgconfig.h \
    flowlayout.h \
    schedetdb.h \
    uploadfiles2ftp.h \
    appconfig.h \
    downloadfile.h \
    baseshadowwidget.h \
    coursetreewidget.h \
    accountitem.h \
    infoandsettingsstackwidget.h \
    classinfowidget.h \
    editrightwidget.h \
    treedisplaydelegate.h \
    framelessmodalmovableshadowwidget.h \
    coursecolorpickerwidget.h \
    loginstatuswidget.h \
    userinfowidget.h \
    dataclassinstancemanage.h \
    inputtextwidget.h \
    settingswidget.h \
    mynode.h \
    mainwindow.h \
    messagedisplaywidget.h \
    filetransferwnd.h \
    aboutwlg.h \
    calendarwidget.h

RESOURCES += \
    resource.qrc

FORMS += \
    batchadddialog.ui \
    MainScreen.ui \
    logindialog.ui \
    dlgconfig.ui \
    coursetreewidget.ui \
    accountitem.ui \
    classinfowidget.ui \
    editrightwidget.ui \
    coursecolorpickerwidget.ui \
    userinfowidget.ui \
    inputtextwidget.ui \
    iconselectwidget.ui \
    settingswidget.ui \
    mainwindow.ui \
    messagedisplaywidget.ui \
    filetransferwnd.ui \
    aboutwlg.ui

TRANSLATIONS += english.ts \
settings_zh_CN.ts

win32: RC_ICONS = Icon/MoonCalendar.ico

DISTFILES +=
