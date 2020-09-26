set(CMAKE_PREFIX_PATH /Users/mark/Qt5.12.0//5.12.0/clang_64/lib/cmake/Qt5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)
add_executable(OlylumoGUI)
target_sources(OlylumoGUI PRIVATE
    source/imageresultwidget.cpp
    source/main.cpp
    source/raycastworker.cpp
    source/scenemodel.cpp
    source/scenewidget.cpp
    source/viewerwidget.cpp
    resources/olylumogui.qrc
)
target_link_libraries(OlylumoGUI PRIVATE RayTrace Qt5::Widgets Qt5::Xml)
