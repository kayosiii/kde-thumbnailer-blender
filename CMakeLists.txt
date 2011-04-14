FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )
# find_package(Qt4 REQUIRED)
set(blender-thumbnailer-kde_PART_SRCS blendercreator.cpp)
set(blender_test_SRCS blendercreator.cpp main.cpp)
kde4_add_plugin(blender-thumbnailer-kde ${blender-thumbnailer-kde_PART_SRCS})

target_link_libraries(blender-thumbnailer-kde ${KDE4_KIO_LIBS})
add_executable(test_blender_thumbnailer ${blender_test_SRCS})
target_link_libraries(test_blender_thumbnailer ${KDE4_KIO_LIBS})
install(TARGETS blender-thumbnailer-kde DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES blender-thumbnailer-kde.desktop DESTINATION ${SERVICES_INSTALL_DIR})