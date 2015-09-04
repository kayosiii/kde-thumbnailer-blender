project(blender-thumbnailer-kde)
set(PROJECT_VERSION "0.5")
set(PROJECT_VERSION_MAJOR 0)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

include(FeatureSummary)

set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.3.0")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui)
# find_package(Qt5Test ${QT_MIN_VERSION} CONFIG QUIET)
# 
# set_package_properties(Qt5Test PROPERTIES PURPOSE "Required for tests" TYPE OPTIONAL)
# add_feature_info("Qt5Test" Qt5Test_FOUND "Required for building tests")
# 
# if (NOT Qt5Test_FOUND)
#     set (BUILD_TESTING OFF CACHE BOOL "Build the testing tree")
# endif()

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    KIO
)


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMMarkNonGuiExecutable)
include(ECMMarkAsTest)
include(ECMPackageConfigHelpers)
include(ECMOptionalAddSubdirectory)

include_directories(${KF5_INCLUDES})


set(blender-thumbnailer-kde_PART_SRCS blendercreator.cpp)
add_library(blendercreator MODULE ${blender-thumbnailer-kde_PART_SRCS})
target_link_libraries(blendercreator PRIVATE
    Qt5::Gui
    KF5::Archive
    KF5::KIOWidgets
)
    
install(TARGETS blendercreator DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES blenderthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})

set(blender_test_SRCS blendercreator.cpp main.cpp)
add_executable(test_blender_thumbnailer ${blender_test_SRCS})
target_link_libraries(test_blender_thumbnailer
    Qt5Gui
    KF5::Archive
    KF5::KIOWidgets
)
