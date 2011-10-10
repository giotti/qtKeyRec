TEMPLATE=app
TARGET=qtKeyRec

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qtKeyRec
    desc="WiFi Key Recover"
    license=GPLv2
    version=1.0
    maintainer="giotti"
]

# Input files
FORMS +=\
    qtKeyRec.ui\
    widget.ui

HEADERS=\
    qtKeyRec.h\
    widget.h

SOURCES=\
    main.cpp\
    qtKeyRec.cpp\
    widget.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qtKeyRec.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qtKeyRec
]

