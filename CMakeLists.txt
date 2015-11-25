PROJECT(SHTTPD)
AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_EXECUTABLE(shttpd ${SRC_LIST})
target_link_libraries(shttpd pthread)
