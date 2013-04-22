#######################################
# Qt
if(NOT DEFINED CMAKE_PREFIX_PATH)
    message(SEND_ERROR "You should set CMAKE_PREFIX_PATH to point the Qt library folder. For example: -DCMAKE_PREFIX_PATH=\"C:/Qt/Qt5.0.1/5.0.1/msvc2010/\"")
    return()
endif()

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)

#######################################
# kraken_gui project

file(GLOB KRAKEN_GUI_QML *.qml)
file(GLOB KRAKEN_GUI_QRC *.qrc)

file(GLOB KRAKEN_GUI_HEADERS *.h)
aux_source_directory(. KRAKEN_GUI_SRC)

QT5_ADD_RESOURCES(KRAKEN_GUI_RESOURCE ${KRAKEN_GUI_QRC})

add_executable(kraken_gui ${KRAKEN_GUI_SRC} ${KRAKEN_GUI_SRC_HEADERS} ${KRAKEN_GUI_QML} ${KRAKEN_GUI_RESOURCE})
target_link_libraries(kraken_gui kraken)

set_target_properties(kraken_gui PROPERTIES AUTOMOC true)

set(QT_DIR "${CMAKE_PREFIX_PATH}")
set(QT_BIN "${CMAKE_PREFIX_PATH}/bin/")

############################
# QT lib dependences

set( QT_LIB_DEPENDENCES icuin49 icuuc49 icudt49 )

foreach(lib ${QT_LIB_DEPENDENCES})
    add_custom_command(TARGET kraken_gui POST_BUILD
                        COMMAND "${CMAKE_COMMAND}" -E
                        copy "${QT_BIN}${lib}.dll" "$<TARGET_FILE_DIR:kraken_gui>")
endforeach()

###########################
# QT libs

set( QT_LIBS Qt5Core Qt5Qml Qt5Network Qt5V8 Qt5Quick Qt5Gui
             libEGL libGLESv2 )

foreach(lib ${QT_LIBS})
    add_custom_command(TARGET kraken_gui POST_BUILD
                        COMMAND "${CMAKE_COMMAND}" -E
                        copy "${QT_BIN}${lib}d.dll" "$<TARGET_FILE_DIR:kraken_gui>")
endforeach()

qt5_use_modules(kraken_gui Qml Quick)
