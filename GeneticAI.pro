TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    managers/fontmanager.cpp \
    managers/texturemanager.cpp \
    models/map.cpp \
    models/player.cpp \
    models/hud.cpp \
    utils/graphics.cpp \
    utils/directory.cpp \
    utils/timer.cpp \
    graphics/animation.cpp \
    utils/file.cpp \
    models/game.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system \
        -lboost_filesystem -lboost_system

HEADERS += \
    managers/fontmanager.hpp \
    managers/texturemanager.hpp \
    models/map.hpp \
    models/player.hpp \
    models/hud.hpp \
    utils/graphics.hpp \
    utils/directory.hpp \
    utils/timer.hpp \
    graphics/animation.hpp \
    utils/file.hpp \
    models/game.hpp \
    utils/random.hpp
