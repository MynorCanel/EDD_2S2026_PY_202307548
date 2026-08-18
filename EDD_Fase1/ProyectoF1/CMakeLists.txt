cmake_minimum_required(VERSION 3.16)

project(Proyecto1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES i18n/Proyecto1_es_GT.ts)

set(PROJECT_SOURCES
    main.cpp
    ventanas/login.cpp
    ventanas/login.h
    ui/login.ui
    ventanas/crearpeli.cpp
    ventanas/crearpeli.h
    ui/crearpeli.ui
    ventanas/admin.cpp
    ventanas/admin.h
    ui/admin.ui
    ventanas/agregarpromocion.cpp
    ventanas/agregarpromocion.h
    ui/agregarpromocion.ui
    ventanas/agregarbeneficio.cpp
    ventanas/agregarbeneficio.h
    ui/agregarbeneficio.ui
    modelos/Beneficio.h
    modelos/NodoBeneficio.h
    modelos/NodoPelicula.h
    modelos/NodoPromocion.h
    modelos/NodoSolicitud.h
    modelos/Pelicula.h
    modelos/Promocion.h
    modelos/Solicitud.h
    estructuras/ArbolBinario.cpp
    estructuras/ArbolBinario.h
    estructuras/ListaCircular.cpp
    estructuras/ListaCircular.h
    estructuras/listacirculardobleenlazada.cpp
    estructuras/ListaDoble.cpp
    estructuras/ListaDoble.h
    estructuras/matrizdispersa.cpp
    controladores/admincontroller.cpp
    controladores/clientecontroller.cpp
    servicios/autentificacionService.cpp
    servicios/guardarDatosService.cpp
    servicios/guardarDatosService.h
    servicios/graphvizService.cpp
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Proyecto1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ventanas/crearpeli.h ventanas/crearpeli.cpp ui/crearpeli.ui
        ventanas/agregarpromocion.h ventanas/agregarpromocion.cpp ui/agregarpromocion.ui
        ventanas/agregarbeneficio.h ventanas/agregarbeneficio.cpp ui/agregarbeneficio.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Proyecto1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Proyecto1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Proyecto1
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Proyecto1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Proyecto1)
endif()
set_target_properties(Proyecto1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Proyecto1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Proyecto1)
endif()
