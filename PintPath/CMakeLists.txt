
cmake_minimum_required(VERSION 3.16)

project(PintPath VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Location Positioning)

qt_standard_project_setup(REQUIRES 6.8)


qt_add_executable(appPintPath
    main.cpp
)

if (ANDROID)
    include(FetchContent)
    FetchContent_Declare(
        android_openssl
        GIT_REPOSITORY https://github.com/KDAB/android_openssl.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(android_openssl)

    # Dynamically set OpenSSL directory based on the ABI
    if (ANDROID_ABI STREQUAL "x86_64")
        set(OPENSSL_DIR "${android_openssl_SOURCE_DIR}/ssl_3/x86_64")
    elseif (ANDROID_ABI STREQUAL "arm64-v8a")
        set(OPENSSL_DIR "${android_openssl_SOURCE_DIR}/ssl_3/arm64-v8a")
    else()
        message(FATAL_ERROR "Unsupported ABI: ${ANDROID_ABI}")
    endif()

    # Add OpenSSL libraries to ANDROID_EXTRA_LIBS
    list(APPEND ANDROID_EXTRA_LIBS
        "${OPENSSL_DIR}/libcrypto_3.so"
        "${OPENSSL_DIR}/libssl_3.so"
    )

    # Copy OpenSSL libraries to the output directory
    add_custom_command(TARGET appPintPath POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/android-build-appPintPath/libs/${ANDROID_ABI}/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${OPENSSL_DIR}/libcrypto_3.so"
            "${OPENSSL_DIR}/libssl_3.so"
            "${CMAKE_BINARY_DIR}/android-build-appPintPath/libs/${ANDROID_ABI}/"
    )

    # Cache ANDROID_EXTRA_LIBS
    set(ANDROID_EXTRA_LIBS "${ANDROID_EXTRA_LIBS}" CACHE INTERNAL "")
endif()


qt_add_qml_module(appPintPath
    URI PintPath
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES pintbackend.h pintbackend.cpp
    RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
    RESOURCES android/resources/logo.png
    SOURCES vendormodel.h vendormodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPintPath PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPintPath
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


target_link_libraries(appPintPath
    PRIVATE Qt6::Quick Qt6::Location Qt6::Positioning ${ANDROID_EXTRA_LIBS}
)

set_property(TARGET appPintPath APPEND PROPERTY
    ANDROID_EXTRA_LIBS ${ANDROID_EXTRA_LIBS}
)

set_property(TARGET appPintPath PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
          "${CMAKE_CURRENT_SOURCE_DIR}/android")


include(GNUInstallDirs)
install(TARGETS appPintPath
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
