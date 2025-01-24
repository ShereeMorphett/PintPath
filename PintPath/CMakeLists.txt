cmake_minimum_required(VERSION 3.16)

project(PintPath VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

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

    # Add OpenSSL libraries to ANDROID_EXTRA_LIBS
    list(APPEND ANDROID_EXTRA_LIBS
        "${android_openssl_SOURCE_DIR}/ssl_3/x86_64/libcrypto_3.so"
        "${android_openssl_SOURCE_DIR}/ssl_3/x86_64/libssl_3.so"
    )
    set(ANDROID_EXTRA_LIBS "${ANDROID_EXTRA_LIBS}" CACHE INTERNAL "")

    # Copy OpenSSL libraries to the target directory
    add_custom_command(TARGET appPintPath POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${android_openssl_SOURCE_DIR}/ssl_3/x86_64/libcrypto_3.so"
            "${android_openssl_SOURCE_DIR}/ssl_3/x86_64/libssl_3.so"
            "${CMAKE_BINARY_DIR}/android-build-appPintPath/libs/x86_64/"
    )
endif()

qt_add_qml_module(appPintPath
    URI PintPath
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES pintbackend.h pintbackend.cpp

    RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
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
    PRIVATE Qt6::Quick ${ANDROID_EXTRA_LIBS}

)



include(GNUInstallDirs)
install(TARGETS appPintPath
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
