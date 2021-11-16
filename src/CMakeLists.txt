message (STATUS "Event display Ninja Sharing file...")

add_executable(EventDisplayNinjaSf
	EventDisplayNinjaSf.cpp
	)

target_link_libraries(EventDisplayNinjaSf
	${Boost_LIBRARIES}
)

# install the execute in the bin folder
install(TARGETS EventDisplayNinjaSf DESTINATION "${CMAKE_INSTALL_BINDIR}/EventDisplayNinjaSf")