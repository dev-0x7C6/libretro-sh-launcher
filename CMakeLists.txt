cmake_minimum_required(VERSION 3.0)
project(libretro-bash-launcher)

file(GLOB_RECURSE sources      src/libretro-sh-launcher.c src/libretro.h src/link.T)

unset(CMAKE_LINK_LIBRARY_SUFFIX)
add_library(sh_launcher_libretro SHARED ${sources})

target_compile_options(sh_launcher_libretro PUBLIC -O2)
set_target_properties(sh_launcher_libretro PROPERTIES PREFIX "") #disable lib* prefix

include(GNUInstallDirs)

install_files(/lib/x86_64-linux-gnu/libretro FILES sh_launcher_libretro.so)
install_files(/lib/x86_64-linux-gnu/libretro FILES lib/sh-launcher.shlib)
install(FILES lib/sh-launcher-pg.sh PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ DESTINATION ${CMAKE_INSTALL_LIBDIR}/libretro)
install_files(/share/libretro/assets/xmb/monochrome/png FILES "share/libretro/assets/xmb/monochrome/png/Bash - Sh Launcher - PC.png")
install_files(/share/libretro/assets/xmb/monochrome/png FILES "share/libretro/assets/xmb/monochrome/png/Bash - Sh Launcher - PC-content.png")
install(FILES etc/999-retroarch.sh etc/999-run-retroarch.rules DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/udev/rules.d)
install_files(/share/libretro/info FILES share/libretro/info/sh_launcher_libretro.info)
