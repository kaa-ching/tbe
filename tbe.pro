TEMPLATE = app
LANGUAGE = C++
CONFIG += qt \
    warn_on \
    debug
LIBS += -lbox2d \
    -L3rdParty/libs
DEFINES += QT_NO_ASCII_CAST
INCLUDEPATH += 3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    src \
    src/model \
    src/view \
    src/base
QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
    -Wextra
QMAKE_CXX_FLAGS_RELEASE += -Wextra
QT += core \
    gui \
    xml \
    svg
HEADERS += src/tbe_global.h \
    src/base/BaseObjectSerializer.h \
    src/base/UndoDeleteCommand.h \
    src/base/UndoMoveCommand.h \
    src/base/UndoResizeCommand.h \
    src/model/BaseObject.h \
    src/model/BowlingBall.h \
    src/model/BowlingPin.h \
    src/model/Butterfly.h \
    src/model/CokeMentosBottle.h \
    src/model/Floor.h \
    src/model/Flower.h \
    src/model/Level.h \
    src/model/MovingObject.h \
    src/model/NoteIt.h \
    src/model/Position.h \
    src/model/Ramp.h \
    src/model/RectObject.h \
    src/model/ToolBoxItemListModel.h \
    src/model/VolleyBall.h \
    src/model/World.h \
    src/view/Anchors.h \
    src/view/DrawButterfly.h \
    src/view/DrawCokeMentosBottle.h \
    src/view/DrawNoteIt.h \
    src/view/DrawObject.h \
    src/view/DrawWorld.h \
    src/view/ImageStore.h \
    src/view/MainWindow.h \
    src/view/PieMenu.h \
    src/view/Popup.h \
    src/view/resizinggraphicsview.h \
    src/view/SaveLevelInfo.h \
    src/model/Scenery.h \
    src/view/SplashScreen.h \
    src/view/StartStopReset.h
SOURCES += src/main.cpp \
    src/base/BaseObjectSerializer.cpp \
    src/base/UndoDeleteCommand.cpp \
    src/base/UndoMoveCommand.cpp \
    src/base/UndoResizeCommand.cpp \
    src/model/BaseObject.cpp \
    src/model/BowlingBall.cpp \
    src/model/BowlingPin.cpp \
    src/model/Butterfly.cpp \
    src/model/CokeMentosBottle.cpp \
    src/model/Floor.cpp \
    src/model/Flower.cpp \
    src/model/Level.cpp \
    src/model/MovingObject.cpp \
    src/model/NoteIt.cpp \
    src/model/Position.cpp \
    src/model/Ramp.cpp \
    src/model/RectObject.cpp \
    src/model/ToolBoxItemListModel.cpp \
    src/model/VolleyBall.cpp \
    src/model/World.cpp \
    src/view/Anchors.cpp \
    src/view/DrawButterfly.cpp \
    src/view/DrawCokeMentosBottle.cpp \
    src/view/DrawNoteIt.cpp \
    src/view/DrawObject.cpp \
    src/view/DrawWorld.cpp \
    src/view/ImageStore.cpp \
    src/view/MainWindow.cpp \
    src/view/PieMenu.cpp \
    src/view/SaveLevelInfo.cpp \
    src/model/Scenery.cpp \
    src/view/SplashScreen.cpp \
    src/view/StartStopReset.cpp
FORMS = src/view/MainWindow.ui \
    src/view/NoteItViewer.ui \
    src/view/SaveLevelInfo.ui
DISTFILES = README \
    TODO \
    AUTHORS \
    COPYING
unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
OTHER_FILES += TODO
TRANSLATIONS = i18n/tbe_nl.ts
