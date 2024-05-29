cmake_minimum_required(VERSION 3.9)
project(TileMap CXX)
#CXX standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Options:
option(SANITIZE 	"Build with sanitizer support if available" OFF)
option(VENDOR 		"Vendor libraries" OFF)
option(CHECKS 		"Run checks" OFF)
option(IWYU 		"Run include what you use" OFF)
option(CLANG_TIDY 	"Run clang tidy" OFF)
option(CPPCHECK 	"Run cppcheck" OFF)
#CMAKE_BUILD_TYPE Release Debug

if(CHECKS STREQUAL ON)
	set(IWYU ON)
	set(CLANG_TIDY ON)
	set(CPPCHECK ON)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(SRC
#Sources
src/main.cpp
src/game.cpp
src/component.cpp
src/actor.cpp
src/spriteComponent.cpp
src/tileMapComponent.cpp
src/animSpriteComponent.cpp
#Headers
include/game.hpp
include/common.hpp
include/actor.hpp
include/component.hpp
include/spriteComponent.hpp
include/tileMapComponent.hpp
include/animSpriteComponent.hpp
)

set(ASSETS_EMBED_WEB
--embed-file=assets/MapLayer1.csv@/assets/MapLayer1.csv
--embed-file=assets/MapLayer2.csv@/assets/MapLayer2.csv
--embed-file=assets/MapLayer3.csv@/assets/MapLayer3.csv
--embed-file=assets/Tiles.png@/assets/Tiles.png
--embed-file=assets/rocket.png@/assets/rocket.png
--embed-file=assets/Head.png@/assets/Head.png
)

#Check debug
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the type of build, options are:
		 Debug Release"
      	FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	message("Debug enabled")
	set(DEBUG TRUE)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

#Check platform
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	set(ANDROID TRUE)
	set(BUILD_NAME main)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(WINDOWS TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
elseif(EMSCRIPTEN)
	set(WEB TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
else()
	set(BUILD_NAME ${PROJECT_NAME})
endif()

#Check LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTOSupported OUTPUT error)

include_directories(include)
if(NOT ANDROID)
	if(WEB)
    		find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-static)
		find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-static)

		add_compile_options(-sUSE_SDL=3 -sUSE_SDL_IMAGE=3)
		add_compile_options(${ASSETS_EMBED_WEB})
		add_link_options(-sUSE_SDL=3 -sUSE_SDL_IMAGE=3)
		add_link_options(${ASSETS_EMBED_WEB})

		set(CMAKE_EXECUTABLE_SUFFIX .js)
		
		configure_file(cmake/index.html.in ${CMAKE_BINARY_DIR}/index.html)
	elseif(VENDOR)
    		add_subdirectory(external/SDL3 EXCLUDE_FROM_ALL)
		add_subdirectory(external/SDL3_image EXCLUDE_FROM_ALL)
	else()
    		find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
		find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)

		if(MACOSX AND NOT DEBUG)
			message("Building for MacOS, did you use -DCMAKE_OSX_ARCHITECTURES=arm64;x86_64?")
		endif()
	endif()
endif()

#Find and include SDL2
#Don't need for Android
if (ANDROID)
	message("Building for Android")

	add_library(${BUILD_NAME} SHARED ${SRC})

	# target_sources(${BUILD_NAME} PRIVATE ${SRC})

	target_link_libraries(${BUILD_NAME} PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)
elseif(MACOSX)
	message("Building for MacOS")

	if(DEBUG)
		add_executable(${BUILD_NAME} ${SRC})
	else()
		#Package into a application bundle
		add_executable(${BUILD_NAME} MACOSX_BUNDLE ${SRC})            
		set_target_properties(${BUILD_NAME} PROPERTIES
		    BUNDLE True
		    MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}.ico
		    MACOSX_BUNDLE_GUI_IDENTIFIER org.cyao.${BUILD_NAME}
	   	    MACOSX_BUNDLE_BUNDLE_NAME ${BUILD_NAME}
		    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
		    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
		    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/info.plist.in
		)
	
		file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/${BUILD_NAME}.app/Contents/Resources)

		add_link_options(-headerpad_max_install_names)

		file(COPY cmake/mac-patch-dylib.sh DESTINATION ${CMAKE_BINARY_DIR}) # Helper script
	endif()

	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
elseif(WINDOWS)
	message("Building for windows")

	add_executable(${BUILD_NAME} WIN32 ${SRC})
	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
else()
	message("Building for linux (probably)")

	add_executable(${BUILD_NAME} ${SRC})

	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
endif()

# Set flags
if(MSVC)
	if(DEBUG)
		target_compile_options(${BUILD_NAME} PRIVATE /W4 /WX /MDd /Zi /Ob0 /Od /RTC1 /W3 /GR /EHsc)
		target_compile_definitions(${BUILD_NAME} PRIVATE /D_DEBUG /DWIN32 /D_WINDOWS /D_DEBUG)
	else()
		target_compile_options(${BUILD_NAME} PRIVATE /MD /O2 /Ob2)
		target_compile_definitions(${BUILD_NAME} PRIVATE /D_WINDOWS /DWIND32 /DNDEBUG)
	endif()
else()
	if(DEBUG)
		target_compile_options(${BUILD_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -g)
		target_compile_definitions(${BUILD_NAME} PRIVATE -D_DEBUG)
	else()
		target_compile_options(${BUILD_NAME} PRIVATE -O3)
		target_compile_definitions(${BUILD_NAME} PRIVATE -DNDEBUG)
	endif()
endif()

# IWYU
if(IWYU STREQUAL ON)
	message("Running IWYU")
	find_program(IWYU_EXE NAMES include-what-you-use iwyu REQUIRED)
	set(IWYU_COMMAND "${IWYU_EXE}" "-Xiwyu" "--mapping_file=${CMAKE_SOURCE_DIR}/.iwyu.imp")
	set_target_properties(${BUILD_NAME} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "${IWYU_COMMAND}")
endif()

if(CLANG_TIDY STREQUAL ON)
	message("Running clang tidy")
	find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;--use-color")
	set_target_properties(${BUILD_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

if(CPPCHECK STREQUAL ON)
	message("Running cppcheck")
	find_program(CPPCHECK_EXE NAMES cppcheck REQUIRED)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CPPCHECK_COMMAND "${CPPCHECK_EXE}" "--enable=all" "--inconclusive" "--force" "--inline-suppr" "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-ignore")
	set_target_properties(${BUILD_NAME} PROPERTIES CXX_CPPCHECK "${CPPCHECK_COMMAND}")
endif()

#Sanitize
if(SANITIZE STREQUAL ON)
	message("Sanitizer emabled")
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

#do LTO
if(LTOSupported AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	if(NOT DEBUG)
		message(STATUS "IPO / LTO enabled")
		set_property(TARGET ${BUILD_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
else()
	message(STATUS "IPO / LTO not supported: <${error}>")
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

