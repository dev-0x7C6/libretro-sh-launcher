cmake_minimum_required(VERSION 3.16)
project(libretro-sh-launcher)

option(SPLASH "Support Qt5 splash screen" ON)

project(libretro-sh-launcher VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRETRO_ASSETS_PATH "/usr/share/libretro/assets" CACHE PATH "Path to libretro assets.")
set(LIBRETRO_CORE_INFO_PATH "/usr/share/libretro/info" CACHE PATH "Path to libretro info files")
set(LIBRETRO_CORE_PATH "/usr/lib/libretro" CACHE PATH "Path to libretro cores")
set(LIBRETRO_FIRMWARE_PATH "/usr/share/libretro/system" CACHE PATH "Path to libretro firmware, bioses and etc.")

message("-- LIBRETRO_ASSETS_PATH value is ${LIBRETRO_ASSETS_PATH}")
message("-- LIBRETRO_CORE_INFO_PATH value is ${LIBRETRO_CORE_INFO_PATH}")
message("-- LIBRETRO_CORE_PATH value is ${LIBRETRO_CORE_PATH}")
message("-- LIBRETRO_FIRMWARE_PATH value is ${LIBRETRO_FIRMWARE_PATH}")

configure_file("src/config.hpp.in" "config.hpp")

unset(CMAKE_LINK_LIBRARY_SUFFIX)

if (SPLASH)
        find_package(Qt5 COMPONENTS Core Gui OpenGL REQUIRED)
        add_executable(ra_splash apps/splash/main.cpp)
        target_link_libraries(ra_splash Qt5::Core Qt5::Widgets)
endif()

add_library(sh_launcher_libretro SHARED
    src/art.cpp
    src/art.h
    src/config.hpp
    src/libretro-sh-launcher.cpp
    src/libretro.h
    src/link.T
    src/playlist_generator.cpp
    src/playlist_generator.h
    src/third-party/crc/crc.cpp
    src/third-party/crc/crc.h
    src/third-party/ftlip/ftlip.cpp
    src/third-party/ftlip/ftlip.h
    src/third-party/https-downloader/HTTPDownloader.cpp
    src/third-party/https-downloader/HTTPDownloader.hpp
    src/utils.cpp
    src/utils.h
)

set_target_properties(sh_launcher_libretro PROPERTIES PREFIX "") #disable lib* prefix
target_link_libraries(sh_launcher_libretro stdc++fs curl Imlib2)

# install
include(GNUInstallDirs)

if (SPLASH)
	install_files(/bin FILES ra_splash)
endif()

install(TARGETS sh_launcher_libretro DESTINATION ${LIBRETRO_CORE_PATH})

#install_files(${LIBRETRO_ASSETS_PATH}/xmb/monochrome/png FILES "assets/xmb/monochrome/png/Bash - Sh Launcher - PC.png")
#install_files(${LIBRETRO_ASSETS_PATH}/xmb/monochrome/png FILES "assets/xmb/monochrome/png/Bash - Sh Launcher - PC-content.png")

install_files(${LIBRETRO_ASSETS_PATH}/xmb/monochrome/png FILES "assets/generic_art.png")
install_files(${LIBRETRO_CORE_INFO_PATH} FILES assets/info/sh_launcher_libretro.info)

install(FILES etc/999-retroarch.sh etc/999-run-retroarch.rules DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/udev/rules.d)
