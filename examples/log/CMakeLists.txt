ADD_EXECUTABLE(LogPrint_test LogPrint_test.cpp)
TARGET_LINK_LIBRARIES(LogPrint_test tools_cxx)

ADD_EXECUTABLE(Log_test Log_test.cpp)
TARGET_LINK_LIBRARIES(Log_test tools_cxx)

ADD_EXECUTABLE(LogFile_test LogFile_test.cpp)
TARGET_LINK_LIBRARIES(LogFile_test tools_cxx)