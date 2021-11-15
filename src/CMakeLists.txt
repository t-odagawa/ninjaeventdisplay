message (STATUS "Event display...")

add_executable(EventDisplayNinjaSf
	EventDisplayNinjaSf.cpp
#	EventDisplayNinjaSf.hpp
	)

target_link_libraries(EventDisplayNinjaSf
	${ROOT_LIBRARIES}
	${Geant4_LIBRARIES}
	${Boost_LIBRARIES}
	${B2MC_LIBRARY}
	${NTBM_LIBRARY}
)

# install the execute in the bin folder
install(TARGETS EventDisplayNinjaSf DESTINATION "${CMAKE_INSTALL_BINDIR}/EventDisplayNinjaSf")