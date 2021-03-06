# Creates a quick and dierty test application with the engine
# caiwan/IR

function(create_testapp _application)
	find_package(GTest REQUIRED)

	file(GLOB_RECURSE SOURCE_FILES *.cpp *.c *.inl)
	file(GLOB_RECURSE HEADER_FILES *.h *.hpp *.inc)

	include_directories("${CMAKE_SOURCE_DIR}")
	include_directories("${GK2_INCLUDE_DIR}")
	include_directories(${GTEST_INCLUDE_DIRS})

    set(ASSET_DIR ${CMAKE_BINARY_DIR}/tests/assets)
    
	add_executable(${_application} ${SOURCE_FILES} ${HEADER_FILES})
	add_test(Tests ${_application})
	
    set_target_properties(${_application} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${ASSET_DIR}")

	target_link_libraries(${_application} Grafkit2 ${GTEST_BOTH_LIBRARIES})
	target_include_directories(${_application} PUBLIC include)

	set_target_properties(${_application} PROPERTIES FOLDER tests)

	msvc_set_win32(${_application})

	assign_source_group("${SOURCE_FILES}")
	assign_source_group("${HEADER_FILES}")
	
endfunction(create_testapp)
