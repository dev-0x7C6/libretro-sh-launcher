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

unset(CMAKE_LINK_LIBRARY_SUFFIX)

if (SPLASH)
        find_package(Qt5 COMPONENTS Core Gui OpenGL REQUIRED)
        add_executable(ra_splash apps/splash/main.cpp)
        target_link_libraries(ra_splash Qt5::Core Qt5::Widgets)
endif()

add_library(sh_launcher_libretro SHARED
    src/third-party/crc/crc.cpp
    src/third-party/crc/crc.h
    src/third-party/ftlip/ftlip.cpp
    src/third-party/ftlip/ftlip.h
    src/third-party/https-downloader/HTTPDownloader.cpp
    src/third-party/https-downloader/HTTPDownloader.hpp
    src/utils.cpp
    src/utils.h
    src/art.cpp
    src/art.h
    src/playlist_generator.cpp
    src/playlist_generator.h
    src/libretro-sh-launcher.cpp
    src/libretro.h
    src/link.T
)

set_target_properties(sh_launcher_libretro PROPERTIES PREFIX "") #disable lib* prefix
target_link_libraries(sh_launcher_libretro stdc++fs curl Imlib2)

# install
include(GNUInstallDirs)

if (SPLASH)
	install_files(/bin FILES ra_splash)
endif()
install_files(/lib/x86_64-linux-gnu/libretro FILES sh_launcher_libretro.so)
install_files(/lib/x86_64-linux-gnu/libretro FILES lib/sh-launcher.shlib)
#install_files(/share/libretro/assets/xmb/monochrome/png FILES "assets/xmb/monochrome/png/Bash - Sh Launcher - PC.png")
#install_files(/share/libretro/assets/xmb/monochrome/png FILES "assets/xmb/monochrome/png/Bash - Sh Launcher - PC-content.png")
install_files(/share/libretro/assets/xmb/monochrome/png FILES "assets/generic_art.png")
install(FILES etc/999-retroarch.sh etc/999-run-retroarch.rules DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/udev/rules.d)
install_files(/share/libretro/info FILES assets/info/sh_launcher_libretro.info)
