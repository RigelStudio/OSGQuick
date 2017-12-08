cmake_minimum_required(VERSION 2.8.12)

project(OSGQQuick)
set(PROJECT_NAME OSGQQuick)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# 设置输出路径，区分32、64
set(OUTPUT_BINDIR ${CMAKE_SOURCE_DIR}/bin)

# 设置lib库的输出路径，区分32、64
set(OUTPUT_LIBDIR ${CMAKE_SOURCE_DIR}/lib)

# 对每一个平台，设置其输出路径
foreach(CONF ${CMAKE_CONFIGURATION_TYPES})
	#string(TOUPPER "${CONF}" CONF_UC)
	set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY" ${OUTPUT_LIBDIR})
	set("CMAKE_RUNTIME_OUTPUT_DIRECTORY" ${OUTPUT_BINDIR})
	set("CMAKE_LIBRARY_OUTPUT_DIRECTORY" "${OUTPUT_BINDIR}")
endforeach()

# Debug库输出带d,Release不带
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
if(CMAKE_BUILD_TYPE MATCHES "Release")
	SET(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
	SET(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	SET(CMAKE_BUILD_POSTFIX "")
endif()

# 配置需要的第三方库
set(OSGDIR "" CACHE PATH "OSGDIR party directory")

# 如果不配置第三方库，则不进行之后的生成
if(NOT OSGDIR)
	message(FATAL_ERROR "OSGDIR variable is not set appropriate!")
endif()

find_package(Qt5Core)
find_package(Qt5Quick)


# 添加文件
set(headers

	 )

set(qmls
    main.qml
    MainForm.ui.qml
	 )

set(sources
      main.cpp

	 )

set(qrcs
    qml.qrc
   )

# Qt warp files
qt5_wrap_cpp(Headers_Moc ${headers})

# 设置工程过滤器
source_group("Header Files" FILES ${headers})
source_group("Qml Files" FILES ${qmls})
source_group("Resource Files" FILES ${qrcs})
source_group("Generated Files" FILES ${Forms_Ui} ${Headers_Moc})

# 设置第三方头文件与链接库的路径
include_directories(
	${OSGDIR}/include
)

link_directories(
	${OSGDIR}/lib
)

add_executable(${PROJECT_NAME} ${headers} ${sources} ${qrcs} ${qmls})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
