set(headers Window.h WindowsGame.h)
set(files Window.cpp WindowsGame.cpp)

add_library(windows STATIC ${files} ${headers})
