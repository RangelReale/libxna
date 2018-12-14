cmake_minimum_required (VERSION 2.8.11)
project (libxna)

find_package(PNG REQUIRED)

include_directories(include)
include_directories(BinaryLib/src)

add_library(libxna STATIC
    src/BitBuffer.cpp
    src/Content.cpp
    src/LzxDecoder.cpp
    src/xna_exception.cpp
    src/XNB.cpp

	BinaryLib/src/BinaryLibUtil.cpp
	BinaryLib/src/BinaryReader.cpp
	BinaryLib/src/BinaryWriter.cpp
)

add_executable(testapp
    testapp/convertxnb.cpp
)

target_link_libraries(testapp libxna PNG::PNG)
