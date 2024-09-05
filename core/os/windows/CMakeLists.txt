set(headers Window.h WindowsGame.h)
set(files Window.cpp WindowsGame.cpp os.cpp)


add_library(windows STATIC ${files} ${headers})
add_library(os::windows ALIAS windows)

target_include_directories(windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(windows PUBLIC os)
target_link_libraries(windows PUBLIC math)
target_link_libraries(windows PUBLIC util)
target_link_libraries(windows PRIVATE 
    kernel32.lib
    user32.lib
    gdi32.lib
    winspool.lib
    comdlg32.lib
    advapi32.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
    odbc32.lib
    odbccp32.lib
)
