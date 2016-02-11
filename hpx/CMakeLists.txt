
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
#project(moose_hpx)

find_package(Boost COMPONENTS random chrono thread system REQUIRED)
find_package(HPX REQUIRED)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
	${HPX_INCLUDE_DIRS}
)

add_subdirectory(accumulators)

set(examples
    managed_accumulator
    simple_accumulator
    template_function_accumulator
   )

# for all targets specified above
foreach(example ${examples})
  set(sources ${example}_client.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${example}_client
    SOURCES ${sources}
    DEPENDENCIES ${example}_component
    FOLDER "Examples/Quickstart/Accumulators/${example}")

  # add a custom target for this example
  add_hpx_pseudo_target(examples.accumulator.${example})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(examples.accumulator
                              examples.accumulator.${example})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(examples.accumulator.${example}
                              ${example}_client_exe)
endforeach()





#add_hpx_component(hello_world_component
#	SOURCES hello_world_component.cpp
#	HEADERS hello_world_component.hpp
#	COMPONENT_DEPENDENCIES iostreams
#)

#add_hpx_component(managed_accumulator_component
#	SOURCES managed_accumulator.cpp
#	HEADERS managed_accumulator.hpp
#	COMPONENT_DEPENDENCIES iostreams
#)

# build your application using HPX
#add_hpx_executable(hello_world
#	ESSENTIAL
#	SOURCES hello_world_client.cpp
#	COMPONENT_DEPENDENCIES hello_world_component
#)

#add_hpx_executable(managed_accumulator
#	ESSENTIAL
#	SOURCES managed_accumulator_client.cpp
#	COMPONENT_DEPENDENCIES managed_accumulator_component
#)

#target_link_libraries(hello_world
#	${Boost_LIBRARIES}
#)

#add_subdirectory(test)
#enable_testing()
#add_test(NAME Random     COMMAND test/TestRandom  )
#add_test(NAME ThreadedId COMMAND test/TestThreadId)


