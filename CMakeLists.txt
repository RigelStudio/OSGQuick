cmake_minimum_required(VERSION 2.8.12)

project(OSGQQuick)
set(PROJECT_NAME OSGQQuick)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 璁剧疆杈撳嚭璺緞锛屽尯鍒�32銆�64
set(OUTPUT_BINDIR ${CMAKE_SOURCE_DIR}/bin)

# 璁剧疆lib搴撶殑杈撳嚭璺緞锛屽尯鍒�32銆�64
set(OUTPUT_LIBDIR ${CMAKE_SOURCE_DIR}/lib)

# 瀵规瘡涓€涓钩鍙帮紝璁剧疆鍏惰緭鍑鸿矾寰�
foreach(CONF ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER "${CONF}" CONF_UC)
	set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF_UC}" ${OUTPUT_LIBDIR})
	set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF_UC}" ${OUTPUT_BINDIR})
	set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF_UC}" "${OUTPUT_BINDIR}")
endforeach()

# Debug搴撹緭鍑哄甫d,Release涓嶅甫
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
if(CMAKE_BUILD_TYPE MATCHES "Release")
	SET(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
	SET(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	SET(CMAKE_BUILD_POSTFIX "")
endif()

# 閰嶇疆闇€瑕佺殑绗笁鏂瑰簱
set(OSGDIR "" CACHE PATH "OSGDIR party directory")

# 濡傛灉涓嶉厤缃涓夋柟搴擄紝鍒欎笉杩涜涔嬪悗鐨勭敓鎴�
if(NOT OSGDIR)
	message(FATAL_ERROR "OSGDIR variable is not set appropriate!")
endif()

find_package(Qt5 COMPONENTS Core Quick REQUIRED)


# 娣诲姞鏂囦欢
set(headers
		OSGView.h
		OSGRender.h
	 )

set(qmls
    main.qml
	 )

set(sources
      main.cpp
			InputLib.cpp
			OSGView.cpp
			OSGRender.cpp
	 )

set(qrcs
    qml.qrc
   )

	 # 璁剧疆宸ョ▼杩囨护鍣�
	 source_group("Header Files" FILES ${headers})
	 source_group("Qml Files" FILES ${qmls})
	 source_group("Resource Files" FILES ${qrcs})
	 source_group("Generated Files" FILES ${Headers_Moc})

# 璁剧疆绗笁鏂瑰ご鏂囦欢涓庨摼鎺ュ簱鐨勮矾寰�
include_directories(
	${OSGDIR}/include
)

link_directories(
	${OSGDIR}/lib
)

add_definitions(-DUNICODE -D_UNICODE)

add_executable(${PROJECT_NAME} ${headers} ${sources} ${qrcs} ${qmls})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
