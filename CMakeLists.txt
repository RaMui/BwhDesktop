cmake_minimum_required(VERSION 3.19)
project(BwhDesktop LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools Network)

qt_standard_project_setup()

qt_add_executable(BwhDesktop
    WIN32 MACOSX_BUNDLE
    main.cpp
    bwhdesktop.h bwhdesktop.cpp bwhdesktop.ui
    addconfig.h addconfig.cpp addconfig.ui
    editconfig.h editconfig.cpp editconfig.ui
    Vps.h
    VpsInfo.h VpsInfo.cpp
    VpsTableModel.h VpsTableModel.cpp
    CheckBoxHeader.h CheckBoxHeader.cpp

)

qt_add_translations(
    TARGETS BwhDesktop
    TS_FILES BwhDesktop_en_US.ts
)

qt_add_resources(BwhDesktop "resources"
    PREFIX "/icons"
    BASE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        BwhDesktop.ico
)

target_link_libraries(BwhDesktop
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS BwhDesktop
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET BwhDesktop
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
