QT       += core gui
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProtectedStorage
TEMPLATE = app

QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64 -std=c++14

LIBS += `pkg-config fuse --cflags --libs`

SOURCES += src/main.cpp             \
    src/StorageWindow.cpp       \
    src/VirtualFilesystem.cpp   \
    src/GostCipher.cpp          \
    src/GostHash.cpp            \
    src/ProtectedStorage.cpp \
    src/CipherHelper.cpp

HEADERS  += src/StorageWindow.h     \
    src/VirtualFilesystem.h \
    src/GostCipher.h        \
    src/GostHash.h          \
    src/ProtectedStorage.h \
    src/CipherHelper.h

FORMS    += src/StorageWindow.ui

