cmake_minimum_required(VERSION 3.16)
project(NoctScript LANGUAGES CXX)

set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)

#
# Libraries
#

find_package(Qt6 REQUIRED COMPONENTS Widgets)

#
# Dependency
#

# Noct2D
set(USE_BUNDLE OFF CACHE BOOL "Enable macOS bundle" FORCE)
add_subdirectory(Noct2D)

#
# Source
#

# Main sources.
set(SOURCES
    src/mainwindow.cpp
    src/texteditor.cpp
    src/texteditor.h
)

# Windows-specific sources.
if(WIN32)
    list(APPEND SOURCES
        src/win32.cpp
    )
endif()

# macOS-specific sources.
if(APPLE)
    list(APPEND SOURCES
        src/cocoa.mm
    )
endif()

# Finalize.
qt_standard_project_setup()
qt_add_executable(NoctScript ${SOURCES})

#
# Resource
#

if(WIN32)
    target_sources(NoctScript PRIVATE src/resource.rc)
endif()

#
# Standards
#

# C++17
set_target_properties(NoctScript PROPERTIES
    CXX_STANDARD 17
)

# Objective-C++17
if(APPLE)
    set_target_properties(NoctScript PROPERTIES
        OBJCXX_STANDARD 17
    )
endif()

#
# Link
#

target_link_libraries(NoctScript PRIVATE
    # Use Qt6
    Qt6::Widgets
)

# Windows-specific link options.
if(WIN32)
  # No console.
  set_target_properties(NoctScript PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

#
# Resouces
#

# Intra-App resources.
qt_add_resources(NoctScript  resources
  PREFIX "/"
  FILES resources/resources.qrc
)

#
# Packaging
#

# Windows
if(WIN32)
  # windeployqt
  add_custom_command(TARGET NoctScript POST_BUILD
    COMMENT "windeployqt"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/app"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:NoctScript>" "${CMAKE_BINARY_DIR}/app/NoctScript.exe"
    COMMAND ${Qt6_DIR}/../../../bin/windeployqt --release "${CMAKE_BINARY_DIR}/app/NoctScript.exe"
  )

  # Copy noct.exe and noct2d.exe.
  add_custom_command(TARGET NoctScript POST_BUILD
    COMMENT "Copy NoctLang and Noct2D"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:noct>" "${CMAKE_BINARY_DIR}/app/noct.exe"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:noct2d>" "${CMAKE_BINARY_DIR}/app/noct2d.exe"
  )

  # Resources at share/.
  add_custom_command(TARGET NoctScript POST_BUILD
    COMMENT "Copying binary resource."
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/app/share"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/bin" "${CMAKE_BINARY_DIR}/app/share/bin"
  )
endif()

# macOS
if(APPLE AND NOT IOS)
  # Make an app bundle.
  set_target_properties(NoctScript PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "io.noctvm.noctscript"
    MACOSX_BUNDLE_BUNDLE_NAME "NoctScript"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/Info.plist"
  )

  # Set an icon.
  set(APP_ICON_MAC "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns")
  set_source_files_properties(${APP_ICON_MAC} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
  )
  target_sources(NoctScript PRIVATE ${APP_ICON_MAC})
  set_target_properties(NoctScript PROPERTIES
    MACOSX_BUNDLE_ICON_FILE "icon.icns"
  )

  # Copy `noct` and `noct2d`.
  add_custom_command(TARGET NoctScript POST_BUILD
    COMMENT "Copying binary resource."
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:noct>
            $<TARGET_BUNDLE_DIR:NoctScript>/Contents/MacOS/noct
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:noct2d>
            $<TARGET_BUNDLE_DIR:NoctScript>/Contents/MacOS/noct2d
  )

  # Resources at share/.
  #add_custom_command(TARGET NoctScript POST_BUILD
  #  COMMENT "Copying binary resource."
  #  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_BUNDLE_DIR:NoctScript>/Contents/Resources/share
  #  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/bin $<TARGET_BUNDLE_DIR:NoctScript>/Contents/Resources/share/bin
  #)

  # macdeployqt
  add_custom_command(TARGET NoctScript POST_BUILD
    COMMENT "Running macdeployqt."
    COMMAND ${Qt6_DIR}/../../../bin/macdeployqt
      "$<TARGET_BUNDLE_DIR:NoctScript>"
      -executable="$<TARGET_BUNDLE_DIR:NoctScript>/Contents/MacOS/NoctScript"
      -verbose=1
  )
endif()

# iOS
if(IOS)
  set_target_properties(NoctScript PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/ios/Info.plist"
    XCODE_ATTRIBUTE_SUPPORTED_DESTINATIONS "iphoneos,ipad"
  )
endif()

#
# Install
#

if(LINUX)
  install(TARGETS NoctScript RUNTIME DESTINATION bin)
  install(TARGETS noct2d RUNTIME DESTINATION bin)
  install(TARGETS noct RUNTIME DESTINATION bin)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/bin DESTINATION share/noctscript)
endif()
