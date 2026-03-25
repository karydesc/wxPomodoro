cmake_minimum_required(VERSION 3.26)

project(wxPomodoro)

set(CMAKE_CXX_STANDARD 17)


# Add the source files to the executable
add_executable(wxPomodoro
        source/pomodoro.cpp
        source/mainFrame.cpp
        source/myApp.cpp
        source/database.cpp
)
find_package(SQLite3 REQUIRED)
# Include wxWidgets
find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net gui)
include(${wxWidgets_USE_FILE})
target_link_libraries(wxPomodoro PRIVATE ${wxWidgets_LIBRARIES} PRIVATE SQLite::SQLite3)
