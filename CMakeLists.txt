cmake_minimum_required(VERSION 3.16)

project(StudentManagement VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tải bộ thư viện Widgets, Sql và Charts của Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Charts)

# Khai báo tất cả các file mã nguồn trong dự án
set(PROJECT_SOURCES
        main.cpp

        # Views
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        loginwindow.h
        loginwindow.cpp
        loginwindow.ui
        studentdialog.h
        studentdialog.cpp
        studentdialog.ui
        studentprofiledialog.h      # <--- Bổ sung file header Profile
        studentprofiledialog.cpp    # <--- Bổ sung file C++ Profile
        studentprofiledialog.ui     # <--- Bổ sung file giao diện Profile

        # Models
        student.h
        student.cpp
        itstudent.h
        itstudent.cpp
        mathstudent.h
        mathstudent.cpp
        biostudent.h
        biostudent.cpp
        studentgroup.h
        studentgroup.cpp

        # Controllers & Database
        databasehelper.h
        databasehelper.cpp
        studentfactory.h
        studentfactory.cpp
        studentmanager.h
        studentmanager.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StudentManagement
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(StudentManagement SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(StudentManagement
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Liên kết ứng dụng với các thư viện Qt Widgets, Sql và Charts
target_link_libraries(StudentManagement PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Charts    # <--- Bổ sung liên kết thư viện Charts
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.StudentManagement)
endif()

set_target_properties(StudentManagement PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS StudentManagement
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StudentManagement)
endif()