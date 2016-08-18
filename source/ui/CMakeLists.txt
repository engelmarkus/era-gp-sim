########################################
# SOURCES
########################################

set(UI_SOURCES
    main.cpp
    registeritem.cpp
    registermodel.cpp
    registerdata.cpp
    qproject.cpp
    syntaxhighlighter.cpp
    editor-component.cpp
)

set(UI_HEADERS
    ../../include/ui/registeritem.hpp
    ../../include/ui/registermodel.hpp
    ../../include/ui/qproject.hpp
    ../../include/ui/syntaxhighlighter.hpp
    ../../include/ui/registerdata.hpp
    ../../include/ui/editor-component.hpp
)

########################################
# EXECUTABLE
########################################

# See http://doc.qt.io/qt-5/cmake-manual.html for reference
# about the used Qt5 CMake Makros
qt5_add_resources(UI_RESOURCES qml.qrc)
qt5_wrap_cpp(UI_HEADERS_WRAPPED ${UI_HEADERS})

add_executable(gui ${UI_SOURCES} ${UI_RESOURCES} ${UI_HEADERS_WRAPPED})

qt5_use_modules(gui Widgets Qml Quick)
