QT -= gui core

LIBS -= pthread

CONFIG(c++14){
    DEFINES += CPP_14
    QMAKE_CXXFLAGS += -std=c++1y
    #-ftemplate-depth=30#101 -ftemplate-backtrace-limit=0
    #-Wmissing-field-initializers
} else {
    DEFINES += CPP_11
    QMAKE_CXXFLAGS += -std=c++11
}


contains(QMAKE_CXX, clang++) {
CONFIG(c++14) {
QMAKE_CXXFLAGS += -std=c++1y
message("$${_PRO_FILE_}")
} else {
QMAKE_CXXFLAGS += -Wc++1y-extensions
message("qmake CONFIG+=c++14 -spec linux-clang")
}
QT += core
message("qmake CONFIG+=c++11 -spec linux-g++")
}
CURR_BUILD = "$$QMAKE_CXX $$DEFINES"
write_file("current_build.txt", CURR_BUILD)
touch("main.cpp", "current_build.txt")
touch("probs.cpp", "current_build.txt")
message('Using $$DEFINES $${OUT_PWD}/$$TARGET')
message('Compiling with $$QMAKE_CXX $$QMAKE_CXXFLAGS ')

HEADERS += \
    field_test.h \
    field_types.h \
    string_utils.h \
    field_helpers.h \
    byte_stream.h \
    cmd_field_info.h \
    forward_cmd_params.h \
    extract_cmd_params_types.h

SOURCES += \
    main.cpp \
    probs.cpp \
    byte_stream.cpp
