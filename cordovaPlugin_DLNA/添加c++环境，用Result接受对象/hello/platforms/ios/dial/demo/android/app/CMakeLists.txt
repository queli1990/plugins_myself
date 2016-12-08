cmake_minimum_required(VERSION 3.4.1)

include_directories(../../../depe/tinyxml2)
include_directories(../../../depe/rapidjson/include)

add_library( dlnaclient
             SHARED
             ../../../discovery.cc
             ../../../service.cc
             ../../../util.cc
             ../../../log.cc
             ../../../http.cc
             ../../../dial.cc
             ../../../media-renderer.cc
             ../../../depe/tinyxml2/tinyxml2.cpp
             ../../../android.cc
           )

target_link_libraries( dlnaclient log android )
