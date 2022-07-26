cmake_minimum_required(VERSION 3.22)
project(PMG)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)

add_executable(PMG
        lib/CommonFunctions.h
        lib/dirent.h
        lib/FileCrypt.h
        lib/Note.h
        lib/Storage.h
        pswrd.cpp)
