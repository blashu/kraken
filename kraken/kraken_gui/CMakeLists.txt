#######################################
# Qt
if(NOT DEFINED CMAKE_PREFIX_PATH)
    message(SEND_ERROR "You should set CMAKE_PREFIX_PATH to point the Qt library folder. For example: -DCMAKE_PREFIX_PATH=\"C:/Qt/Qt5.0.1/5.0.1/msvc2010/\"")
    return()
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

#######################################
# kraken_gui project

file(GLOB KRAKEN_GUI_HEADERS *.h)
aux_source_directory(. KRAKEN_GUI_SRC)

# Add resource.
file(GLOB KRAKEN_GUI_QRC *.qrc)
qt5_add_resources(KRAKEN_GUI_RESOURCE ${KRAKEN_GUI_QRC})

# Add UI and moc files.
file(GLOB KRAKEN_GUI_UI *.ui)
qt5_wrap_ui(KRAKEN_GUI_FORMS_HEADERS ${KRAKEN_GUI_UI})
# Include output directory so that contains IU automatically generated headers.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# create gui application
add_executable(kraken_gui ${KRAKEN_GUI_HEADERS}
                          ${KRAKEN_GUI_SRC}
                          ${KRAKEN_GUI_RESOURCE}
                          ${KRAKEN_GUI_FORMS_HEADERS})
target_link_libraries(kraken_gui kraken)
set_target_properties(kraken_gui PROPERTIES AUTOMOC true)

qt5_use_modules(kraken_gui Widgets)


####################################################
#
# Copy dlls to output

set(QT_DIR "${CMAKE_PREFIX_PATH}")
set(QT_BIN "${CMAKE_PREFIX_PATH}/bin/")

############
# Qt modules

set( QT_LIBS Qt5Core Qt5Widgets Qt5Network Qt5Gui
             libEGL libGLESv2 )

foreach(lib ${QT_LIBS})
    add_custom_command(TARGET kraken_gui POST_BUILD
                        COMMAND "${CMAKE_COMMAND}" -E
                        copy "${QT_BIN}${lib}d.dll" "$<TARGET_FILE_DIR:kraken_gui>")
endforeach()

#############################
# Qt inderect dependence dlls

set( QT_LIB_DEPENDENCES icuin49 icuuc49 icudt49 )

foreach(lib ${QT_LIB_DEPENDENCES})
    add_custom_command(TARGET kraken_gui POST_BUILD
                        COMMAND "${CMAKE_COMMAND}" -E
                        copy "${QT_BIN}${lib}.dll" "$<TARGET_FILE_DIR:kraken_gui>")
endforeach()
