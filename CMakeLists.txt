
# check cmake requirements
cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	  
	IF(CMAKE_VERSION VERSION_GREATER 2.8.10)
		cmake_policy(SET CMP0020 NEW)	# qt warnings
	ENDIF()
endif(COMMAND cmake_policy)

######################## begin of project
project(Project640)

# load paths from the user file if exists 
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeUserPaths.cmake)
	include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeUserPaths.cmake)
endif()

option(DISABLE_QT_DEBUG "Disable Qt Debug Messages" OFF)
option(WITH_GESTURE "Compile with Kinect Gestures" ON)

# find Qt
unset(QT_QTCORE_LIBRARY CACHE)
unset(QT_QTGUI_LIBRARY CACHE)
unset(QT_QTWEBKIT_LIBRARY CACHE)
unset(QT_QTMAIN_LIBRARY CACHE)

# set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# find the Qt
if(NOT QT_QMAKE_EXECUTABLE)
	find_program(QT_QMAKE_EXECUTABLE NAMES "qmake" "qmake-qt5" "qmake.exe")
endif()
if(NOT QT_QMAKE_EXECUTABLE)
	message(FATAL_ERROR "you have to set the path to the Qt5 qmake executable")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# message(STATUS "qmake path: ${QT_QMAKE_EXECUTABLE}")
GET_FILENAME_COMPONENT(QT_QMAKE_PATH ${QT_QMAKE_EXECUTABLE} PATH)
set(CMAKE_PREFIX_PATH ${QT_QMAKE_PATH}\\..\\lib\\cmake\\Qt5Widgets ${QT_QMAKE_PATH}\\..\\lib\\cmake\\Qt5Multimedia ${QT_QMAKE_PATH}\\..\\lib\\cmake\\Qt5Network ${QT_QMAKE_PATH}\\..\\lib\\cmake\\Qt5LinguistTools)
# message(STATUS "path: ${QT_QMAKE_PATH}")
# message(STATUS "cmake path: ${CMAKE_PREFIX_PATH}")

unset(Qt5Widgets_FOUND CACHE)
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5Network)
find_package(Qt5Sql)

set(QT_INCLUDES ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS} ${Qt5LinguistTools_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS} )
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES} ${Qt5Concurrent_LIBRARIES} ${Qt5Core_LIBARIES})
if (NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "Qt5Widgets not found. Check your QT_QMAKE_EXECUTABLE path and set it to the correct location")
endif()
# message(STATUS "QT_INCLUDES: ${QT_INCLUDES}")
add_definitions(-DQT5)

if (DISABLE_QT_DEBUG)
	message (STATUS "disabling qt debug messages")
	add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
endif()

# gather information for building
include_directories (
	${QT_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src
)

if (WITH_GESTURE)
	include_directories (
		${GESTURE_INCLUDE_DIRS}
	)
endif()

file(GLOB INFOS_SOURCES "src/*.cpp")
file(GLOB INFOS_HEADERS "src/*.h")

file(GLOB INFOS_MOCS "src/*.h")

set (INFOS_RESOURCES
		src/project640.qrc
)

QT5_ADD_RESOURCES(INFOS_RCC ${INFOS_RESOURCES})
QT5_WRAP_CPP(INFOS_MOC_SRC ${INFOS_MOCS})

set(INFOS_RC src/project640.rc) #add resource file when compiling with MSVC 
set(VERSION_LIB Version.lib)
set(LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs) #add libs directory to library dirs 

# create the targets
set(BINARY_NAME ${CMAKE_PROJECT_NAME})
set(DLL_NAME lib${CMAKE_PROJECT_NAME})
set(LIB_NAME optimized ${DLL_NAME}.lib debug ${DLL_NAME}d.lib)
link_directories(${LIBRARY_DIR})
LIST(REMOVE_ITEM INFOS_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_executable(${BINARY_NAME} WIN32 src/main.cpp ${INFOS_MOC_SRC_SU} ${INFOS_QM} ${INFOS_TRANSLATIONS} ${INFOS_RC})
target_link_libraries(${BINARY_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTMAIN_LIBRARY} ${VERSION_LIB} ${LIB_NAME})

set_target_properties(${BINARY_NAME} PROPERTIES COMPILE_FLAGS "-DDK_DLL_IMPORT -DNOMINMAX")
set_target_properties(${BINARY_NAME} PROPERTIES LINK_FLAGS_REALLYRELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")
set_target_properties(${BINARY_NAME} PROPERTIES LINK_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set_target_properties(${BINARY_NAME} PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set_target_properties(${BINARY_NAME} PROPERTIES IMPORTED_IMPLIB "")
		
add_library(${DLL_NAME} SHARED ${INFOS_SOURCES} ${INFOS_UI} ${INFOS_MOC_SRC} ${INFOS_RCC} ${INFOS_HEADERS} ${INFOS_RC})
target_link_libraries(${DLL_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}  ${QT_QTMAIN_LIBRARY} ${VERSION_LIB}) 
add_dependencies(${BINARY_NAME} ${DLL_NAME})

qt5_use_modules(${BINARY_NAME} Widgets Multimedia Network Gui Concurrent)
qt5_use_modules(${DLL_NAME} Widgets Multimedia Network Gui Concurrent)

SET(CMAKE_SHARED_LINKER_FLAGS_REALLYRELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE") # /subsystem:windows does not work due to a bug in cmake (see http://public.kitware.com/Bug/view.php?id=12566)

set_target_properties(${DLL_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/libs)
set_target_properties(${DLL_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/libs)
set_target_properties(${DLL_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_REALLYRELEASE ${CMAKE_CURRENT_BINARY_DIR}/libs)
				
set_target_properties(${DLL_NAME} PROPERTIES COMPILE_FLAGS "-DDK_DLL_EXPORT -DNOMINMAX")
set_target_properties(${DLL_NAME} PROPERTIES LINK_FLAGS_REALLYRELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")
set_target_properties(${DLL_NAME} PROPERTIES LINK_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set_target_properties(${DLL_NAME} PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set_target_properties(${DLL_NAME} PROPERTIES DEBUG_OUTPUT_NAME ${DLL_NAME}d)
set_target_properties(${DLL_NAME} PROPERTIES RELEASE_OUTPUT_NAME ${DLL_NAME})

set(QTLIBLIST Qt5Core Qt5Gui Qt5Widgets Qt5Multimedia Qt5Network Qt5Concurrent Qt5OpenGL Qt5Sql)

foreach(qtlib ${QTLIBLIST})
	get_filename_component(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReallyRelease)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endforeach(qtlib)

# it might break here if you are not using Qt 5.4
set(PLUGINLIST qgif qjpeg qtiff qtga qwebp)

foreach(plugin ${PLUGINLIST})
	set(plugin_folder imageformats)
	get_filename_component(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/${plugin_folder}/)
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReallyRelease/${plugin_folder}/)
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/${plugin_folder}/)
endforeach(plugin)

	# it might break here if you are not using Qt 5.4
set(PLUGINLIST qwindows)

foreach(plugin ${PLUGINLIST})
	set(plugin_folder platforms)
	get_filename_component(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/${plugin_folder})
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReallyRelease/${plugin_folder}/)
	file(COPY ${QT_DLL_PATH_tmp}/../plugins/${plugin_folder}/${plugin}d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/${plugin_folder}/)
endforeach(plugin)

# create settings file for portable version while working
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/ReallyRelease/settings.nfo)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ReallyRelease/settings.nfo "")
endif()
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Release/settings.nfo)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Release/settings.nfo "")
endif()
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Debug/settings.nfo)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Debug/settings.nfo "")
endif()


# copy msvcpXXX.dll to Really Release
# todo: visual studio 2013
if (CMAKE_GENERATOR STREQUAL "Visual Studio 11" OR  CMAKE_GENERATOR STREQUAL "Visual Studio 11 Win64")
	set(VS_VERSION 11)
else()
	set(VS_VERSION 10)
endif()

if (MSVC11)
	get_filename_component(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\11.0\\Setup\\VS;ProductDir] REALPATH CACHE)
	set(VS_VERSION 11)
elseif (MSVC10)
	get_filename_component(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\\Setup\\VS;ProductDir] REALPATH CACHE)
	set(VS_VERSION 10)
elseif (MSVC12)
	get_filename_component(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\12.0\\Setup\\VS;ProductDir] REALPATH CACHE)
	set(VS_VERSION 12)
endif()

# set properties for Visual Studio Projects
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;ReallyRelease" CACHE STRING "limited configs" FORCE)
# add_definitions(/Zc:wchar_t-)
set(CMAKE_CXX_FLAGS_DEBUG "/W4 ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /O2 ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_REALLYRELEASE "${CMAKE_CXX_FLAGS_RELEASE}  /DQT_NO_DEBUG_OUTPUT")

if (WITH_GESTURE)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DWITH_GESTURE")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DWITH_GESTURE")
	set(CMAKE_CXX_FLAGS_REALLYRELEASE "${CMAKE_CXX_FLAGS_RELEASE}  /DQT_NO_DEBUG_OUTPUT /DWITH_GESTURE")
endif()

set(CMAKE_EXE_LINKER_FLAGS_REALLYRELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE") # /subsystem:windows does not work due to a bug in cmake (see http://public.kitware.com/Bug/view.php?id=12566)

set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_REALLYRELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")



set(_moc ${CMAKE_CURRENT_BINARY_DIR}/GeneratedFiles)
file(GLOB INFOS_AUTOMOC "${CMAKE_BINARY_DIR}/*_automoc.cpp")
source_group("Generated Files" FILES ${INFOS_MOC_SRC} ${INFOS_RCC} ${INFOS_UI} ${INFOS_RC} ${INFOS_QM} ${INFOS_AUTOMOC})
set_source_files_properties(${INFOS_TRANSLATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Translations" FILES ${INFOS_TRANSLATIONS})

# generate configuration file
if(DLL_NAME)
	get_property(INFOS_DEBUG_NAME TARGET ${DLL_NAME} PROPERTY DEBUG_OUTPUT_NAME)
	get_property(INFOS_RELEASE_NAME TARGET ${DLL_NAME} PROPERTY RELEASE_OUTPUT_NAME)
	set(INFOS_LIBS optimized ${CMAKE_BINARY_DIR}/libs/${INFOS_RELEASE_NAME}.lib debug  ${CMAKE_BINARY_DIR}/libs/${INFOS_DEBUG_NAME}.lib)
endif()
set(INOFS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INFOS_BUILD_DIRECTORY ${CMAKE_BINARY_DIR})
