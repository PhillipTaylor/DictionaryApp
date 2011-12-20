project(DictionaryApp)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(DictionaryApp_SRCS Dictionary.cpp DictTypes.h worditerator.cpp DictionaryApp.cpp main.cpp)
qt4_automoc(${DictionaryApp_SRCS})
add_executable(DictionaryApp ${DictionaryApp_SRCS})
target_link_libraries(DictionaryApp ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
