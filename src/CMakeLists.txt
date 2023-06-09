
set(LIBRARY_NAME nikmon-agent_lib)
set(EXAMPLE_EXECUTABLE_NAME nikmon-agent)

configure_file(version.h.in version.h)

# Configure library
add_library(
        ${LIBRARY_NAME} OBJECT

        app/Application.h
        app/Application.cpp
        app/ServiceLocator.h
        app/ServiceLocator.cpp

        modules/ThreadedModule.h
        modules/ThreadedModule.cpp
        modules/communication-module/Client.h
        modules/communication-module/Client.cpp
        modules/communication-module/CommunicationModule.h
        modules/communication-module/CommunicationModule.cpp
        modules/configuration-module/ConfigurationModule.h
        modules/configuration-module/ConfigurationModule.cpp
        modules/task-manager-module/TaskManagerModule.h
        modules/task-manager-module/TaskManagerModule.cpp
        modules/task-manager-module/Task.h
        modules/task-manager-module/Task.cpp
        modules/task-manager-module/PeriodicTask.h
        modules/task-manager-module/PeriodicTask.cpp
        modules/task-manager-module/SingleUseTask.h
        modules/task-manager-module/SingleUseTask.cpp

        extractors/Extractor.h
        extractors/ExtractorFactory.h
        extractors/ExtractorFactory.cpp
        extractors/AgentPingExtractor.h
        extractors/AgentPingExtractor.cpp
        extractors/linux/LinuxExtractorFactory.h
        extractors/linux/LinuxExtractorFactory.cpp
        extractors/linux/SystemMachineInfoExtractor.h
        extractors/linux/SystemMachineInfoExtractor.cpp
        extractors/linux/SystemMemorySizeExtractor.h
        extractors/linux/SystemMemorySizeExtractor.cpp
        extractors/linux/SystemSwapSizeExtractor.h
        extractors/linux/SystemSwapSizeExtractor.cpp
        extractors/linux/sysinfo/MemoryInfoReader.h
        extractors/linux/sysinfo/MemoryInfoReader.cpp

        types/api/Command.h
        types/api/Command.cpp
        types/api/CommandConfirmation.h
        types/api/CommandConfirmation.cpp
        types/api/RegistrationRequest.h
        types/api/RegistrationResponse.h
        types/api/StatusRequest.h
        types/api/StatusRequest.cpp
        types/api/StatusResponse.h
        types/api/StatusResponse.cpp
        types/api/TaskItem.h

        types/enums/CommandType.h
        types/enums/TaskFrequency.h

        utils/SyncQueue.h
)

set_target_properties(
        ${LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
        ${LIBRARY_NAME}
        PRIVATE
        ${PROJECT_DEPS}
)

target_include_directories(
        ${LIBRARY_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
)

# Configure executable
add_executable(${EXAMPLE_EXECUTABLE_NAME} main.cpp)

set_target_properties(
        ${EXAMPLE_EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
)

target_link_libraries(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${LIBRARY_NAME}
        ${PROJECT_DEPS}
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit-tests)
endif()

include(GNUInstallDirs)

install(TARGETS ${EXAMPLE_EXECUTABLE_NAME} RUNTIME DESTINATION bin)