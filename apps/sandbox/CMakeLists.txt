add_executable(sandbox
    main.cpp
    sandbox.cpp
)

target_link_libraries(sandbox
    PRIVATE
        engine
)

target_compile_definitions(sandbox
    PRIVATE
        OPENGL_PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
)

set_target_properties(sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

get_filename_component(MINGW_RUNTIME_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
set(MINGW_STDCXX_DLL "${MINGW_RUNTIME_DIR}/libstdc++-6.dll")
set(MINGW_GCC_DLL "${MINGW_RUNTIME_DIR}/libgcc_s_seh-1.dll")
set(MINGW_WINPTHREAD_DLL "${MINGW_RUNTIME_DIR}/libwinpthread-1.dll")

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:assimp::assimp>
        $<TARGET_FILE_DIR:sandbox>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:sandbox>
)

if(EXISTS "${MINGW_STDCXX_DLL}")
    add_custom_command(TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_STDCXX_DLL}
            $<TARGET_FILE_DIR:sandbox>
    )
endif()

if(EXISTS "${MINGW_GCC_DLL}")
    add_custom_command(TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_GCC_DLL}
            $<TARGET_FILE_DIR:sandbox>
    )
endif()

if(EXISTS "${MINGW_WINPTHREAD_DLL}")
    add_custom_command(TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_WINPTHREAD_DLL}
            $<TARGET_FILE_DIR:sandbox>
    )
endif()
