#
# This file is part of cg3lib: https://github.com/cg3hci/cg3lib
# This Source Code Form is subject to the terms of the GNU GPL 3.0
#
# @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
# @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
#

!contains(DEFINES, CG3_CORE_DEFINED){
    error(DataStructures module requires Core module)
}

DEFINES += CG3_DATA_STRUCTURES_DEFINED
CONFIG += CG3_DATA_STRUCTURES
MODULES += CG3_DATA_SRUCTURES


# ----- Arrays -----

HEADERS += \
    $$PWD/data_structures/arrays/arrays.h \
    $$PWD/data_structures/arrays/array2d.h \
    $$PWD/data_structures/arrays/array3d.h \
    $$PWD/data_structures/arrays/array4d.h \
    $$PWD/data_structures/arrays/array.h

SOURCES += \
    $$PWD/data_structures/arrays/array2d.tpp \
    $$PWD/data_structures/arrays/array3d.tpp \
    $$PWD/data_structures/arrays/array4d.tpp \
    $$PWD/data_structures/arrays/array.tpp
