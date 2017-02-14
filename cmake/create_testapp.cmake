# Creates a quick and dierty test application with the engine

function(create_testapp _application)
	file(GLOB_RECURSE SOURCE_FILES *.cpp)
	file(GLOB_RECURSE HEADER_FILES *.h)

	include_directories("${CMAKE_SOURCE_DIR}")
	include_directories("${GK2_INCLUDE_DIR}")

	add_executable(${_application} ${SOURCE_FILES} ${HEADER_FILES})

	target_link_libraries(${_application} Grafkit2)
	target_include_directories(${_application} PUBLIC include)

	set_target_properties(${_application} PROPERTIES FOLDER tests)

	msvc_set_win32(${_application})

	assign_source_group("${SOURCE_FILES}")
	assign_source_group("${HEADER_FILES}")
	
	add_dependencies(assets)
	add_dependencies(shaders)

endfunction(create_testapp)
