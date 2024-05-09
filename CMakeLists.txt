cmake_minimum_required(VERSION 3.10)

project(
  SQLMigrator
  VERSION 1.0
  DESCRIPTION "a useful and simple tool for migrating your sql queries"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Migrator)

set_target_properties(
  Migrator PROPERTIES
  DESCRIPTION ${PROJECT_DESCRIPTION}
  OUTPUT_NAME "sql_migrator"
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.hpp")

target_sources(Migrator PRIVATE ${SRC_FILES})
