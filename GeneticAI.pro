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
    models/game.cpp \
    ai/neat/gene.cpp \
    ai/neat/genome.cpp \
    ai/neat/innovation.cpp \
    ai/neat/link.cpp \
    ai/neat/neat.cpp \
    ai/neat/network.cpp \
    ai/neat/nnode.cpp \
    ai/neat/organism.cpp \
    ai/neat/population.cpp \
    ai/neat/species.cpp \
    ai/neat/trait.cpp

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
    utils/random.hpp \
    ai/neat/gene.h \
    ai/neat/genome.h \
    ai/neat/innovation.h \
    ai/neat/link.h \
    ai/neat/neat.h \
    ai/neat/network.h \
    ai/neat/nnode.h \
    ai/neat/organism.h \
    ai/neat/population.h \
    ai/neat/species.h \
    ai/neat/trait.h

