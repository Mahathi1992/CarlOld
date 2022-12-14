
# include util to properly set up variables for configure_everything()
add_subdirectory(util)

# Configure all .in files
configure_everything()

file(GLOB_RECURSE carl_sources "*.cpp")
add_new_libraries(carl "${PROJECT_VERSION_LIB}" "${carl_sources}" "resources")

set(BOOST_LIBS_SHARED "Boost_system_SHARED")
set(BOOST_LIBS_STATIC "Boost_system_STATIC")
target_include_dirs_from(carl-objects SYSTEM PUBLIC carl-logging-shared EIGEN3 GMPXX_SHARED GMP_SHARED ${BOOST_LIBS_SHARED} ${CMAKE_DL_LIBS} pthread)
target_link_libraries(carl-shared PUBLIC carl-logging-shared EIGEN3 GMPXX_SHARED GMP_SHARED ${BOOST_LIBS_SHARED} ${CMAKE_DL_LIBS} ${LIB_FILESYSTEM} pthread)
target_link_libraries(carl-static PUBLIC carl-logging-static EIGEN3 GMPXX_STATIC GMP_STATIC ${BOOST_LIBS_STATIC} ${CMAKE_DL_LIBS} ${LIB_FILESYSTEM} pthread)
if(USE_BLISS)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC BLISS_SHARED)
	target_link_libraries(carl-shared PUBLIC BLISS_SHARED)
	target_link_libraries(carl-static PUBLIC BLISS_STATIC)
endif()
if(USE_CLN_NUMBERS)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC CLN_SHARED)
	target_link_libraries(carl-shared PUBLIC CLN_SHARED)
	target_link_libraries(carl-static PUBLIC CLN_STATIC)
endif()
if(USE_COCOA)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC COCOA_SHARED Boost_system_SHARED)
	target_link_libraries(carl-shared PUBLIC COCOA_SHARED ${BOOST_LIBS_SHARED})
	target_link_libraries(carl-static PUBLIC COCOA_STATIC ${BOOST_LIBS_STATIC})
endif()
if(USE_GINAC)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC GINAC_SHARED)
	target_link_libraries(carl-shared PUBLIC GINAC_SHARED)
	target_link_libraries(carl-static PUBLIC GINAC_STATIC)
endif()
if(USE_MPFR_FLOAT)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC MPFR_SHARED)
	target_link_libraries(carl-shared PUBLIC MPFR_SHARED)
	target_link_libraries(carl-static PUBLIC MPFR_STATIC)
endif()
if(USE_Z3_NUMBERS OR RAN_USE_Z3 OR COMPARE_WITH_Z3)
	target_include_dirs_from(carl-objects SYSTEM PUBLIC Z3_SHARED)
	target_link_libraries(carl-shared PUBLIC Z3_SHARED)
	target_link_libraries(carl-static PUBLIC Z3_STATIC)
endif()
if(USE_LIBPOLY OR RAN_USE_LIBPOLY)
target_include_dirs_from(carl-objects SYSTEM PUBLIC LIBPOLY_SHARED LIBPOLYXX_SHARED)
target_link_libraries(carl-shared PUBLIC LIBPOLY_SHARED)
target_link_libraries(carl-static PUBLIC LIBPOLY_STATIC)
target_link_libraries(carl-shared PUBLIC LIBPOLYXX_SHARED)
target_link_libraries(carl-static PUBLIC LIBPOLYXX_STATIC)
endif()

#set_target_properties( lib_carl_static PROPERTIES LINK_SEARCH_END_STATIC TRUE )
#set_target_properties( lib_carl_static PROPERTIES LINK_SEARCH_START_STATIC TRUE )

cotire(carl)
include(install)

clang_tidy_recurse("${CMAKE_SOURCE_DIR}/src/carl" "")
