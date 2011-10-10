#-------------------------------------------------
#
# Project created by QtCreator 2011-05-04T15:52:15
#
#-------------------------------------------------

QT       += core gui

TARGET = qtKeyRec
TEMPLATE = app


SOURCES += main.cpp\
    qtKeyRec.cpp \
    WiRouter_KeyRec_1.0.5/src/agpf.c \
    WiRouter_KeyRec_1.0.5/src/wirouterkeyrec.c \
    WiRouter_KeyRec_1.0.5/src/telsey.c \
    WiRouter_KeyRec_1.0.5/src/socket.c \
    WiRouter_KeyRec_1.0.5/src/sha256.c \
    WiRouter_KeyRec_1.0.5/src/pirelli.c \
    WiRouter_KeyRec_1.0.5/src/md5.c \
    WiRouter_KeyRec_1.0.5/src/hashword.c

HEADERS  += \
    qtKeyRec.h \
    WiRouter_KeyRec_1.0.5/src/wirouterkeyrec.h \
    WiRouter_KeyRec_1.0.5/src/telsey.h \
    WiRouter_KeyRec_1.0.5/src/socket.h \
    WiRouter_KeyRec_1.0.5/src/sha256.h \
    WiRouter_KeyRec_1.0.5/src/pirelli.h \
    WiRouter_KeyRec_1.0.5/src/md5.h \
    WiRouter_KeyRec_1.0.5/src/hashword.h \
    WiRouter_KeyRec_1.0.5/src/agpf.h

FORMS    += \
    qtKeyRec.ui
