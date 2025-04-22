
set(LIBRARY_NAME solocraft)

#Add PCH Files
set(solocraft_PCH "${CMAKE_CURRENT_SOURCE_DIR}/solocraftpch.h")

#Base Files
file(GLOB SoloCraft_Source ${CMAKE_CURRENT_SOURCE_DIR}/solocraft/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/solocraft/*.h)
    source_group("SoloCraft" FILES ${SoloCraft_Source})

#Define base source library
set(LIBRARY_SRCS
    ${SoloCraft_Source}
    ${solocraft_PCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/solocraftpch.cpp
)

if (BUILD_SOLOCRAFT)
    add_definitions(-DENABLE_SOLOCRAFT)
endif()

# Define Expansion
add_definitions(-DSOLOCRAFT_CLASSIC)
if ( ${CMAKE_PROJECT_NAME} MATCHES "TBC")
  add_definitions(-DSOLOCRAFT_TBC)
endif()
if ( ${CMAKE_PROJECT_NAME} MATCHES "WoTLK")
  add_definitions(-DSOLOCRAFT_TBC)
  add_definitions(-DSOLOCRAFT_WOTLK)
endif()

if(PCH)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
  PRIVATE shared
)

target_include_directories(${LIBRARY_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Install config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SoloCraft.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist)
if (NOT CONF_INSTALL_DIR)
  set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist DESTINATION ${CONF_INSTALL_DIR})

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "SoloCraft")

# Generate PCH
if(PCH)
  target_precompile_headers(${LIBRARY_NAME} PRIVATE ${solocraft_PCH})
endif()