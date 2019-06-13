# Debug configuration
CONFIG(debug, debug|release){
    DEFINES += DEBUG
}

# Release configuration
CONFIG(release, debug|release){
    DEFINES -= DEBUG

    # Uncomment next line if you want to ignore asserts and got a more optimized binary
    #CONFIG += FINAL_RELEASE
}

# Final release optimization
FINAL_RELEASE {
    unix:!macx{
        QMAKE_CXXFLAGS_RELEASE -= -g -O2
        QMAKE_CXXFLAGS += -Os -DNDEBUG
    }
}

# cg3lib works with c++11
CONFIG += c++11

# Cg3lib configuration. Available options:
#
#   CG3_ALL                 -- All the modules
#
#   CG3_CORE                -- Core of the library. Geometry primitives and utilities
#   CG3_VIEWER              -- Module containing utilities for creating viewers (Qt and OpenGL)
#
# Example:  CONFIG += CG3_CORE CG3_VIEWER CG3_DATA_STRUCTURES CG3_DCEL
CONFIG += CG3_CORE CG3_VIEWER

# Include the chosen modules
include (cg3lib/cg3.pri)
message($$MODULES)


DISTFILES += \
    LICENSE

SOURCES += \
    main.cpp \
    managers/delaunaymanager.cpp \
    utils/delaunay_checker.cpp \
    utils/fileutils.cpp

FORMS += \
    managers/delaunaymanager.ui

HEADERS += \
    managers/delaunaymanager.h \
    utils/delaunay_checker.h \
    utils/fileutils.h





