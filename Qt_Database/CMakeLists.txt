cmake_minimum_required(VERSION 3.5)

project(08_PreDataBase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Sql)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
		#dbdata.h
		#dbdata.cpp
		#dbdata.ui
		logindialogsql.cpp
		logindialogsql.h
		logindialogsql.ui
)

add_executable(08_PreDataBase
	${PROJECT_SOURCES}
)


target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(08_PreDataBase PRIVATE	Qt6::Widgets
	                                            Qt6::Concurrent
												Qt6::Sql
												libcrypto-3-x64
												libiconv-2
												libintl-9
												libpq
												libssl-3-x64)


