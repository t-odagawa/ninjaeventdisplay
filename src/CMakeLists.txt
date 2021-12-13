message (STATUS "Event display Ninja Sharing file...")

add_executable(EventDisplayNinjaSf
	EventDisplayNinjaSf.cpp
	)

target_link_libraries(EventDisplayNinjaSf
	${Boost_LIBRARIES}
)

add_executable(TrackerDisplayNinjaSf
	TrackerDisplayNinjaSf.cpp
	)

target_link_libraries(TrackerDisplayNinjaSf
	${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${NTBM_LIBRARY})

# install the execute in the bin folder
install(TARGETS EventDisplayNinjaSf DESTINATION "${CMAKE_INSTALL_BINDIR}/EventDisplayNinjaSf")
install(TARGETS TrackerDisplayNinjaSf DESTINATION "${CMAKE_INSTALL_BINDIR}/TrackerDisplayNinjaSf")