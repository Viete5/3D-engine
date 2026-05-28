add_executable(brownian_demo
    main.cpp
    brownianSimulation.cpp
    brownianScene.cpp
)

target_link_libraries(brownian_demo
    PRIVATE
        engine
)

target_compile_definitions(brownian_demo
    PRIVATE
        OPENGL_PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
)

set_target_properties(brownian_demo PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

get_filename_component(MINGW_RUNTIME_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
set(MINGW_STDCXX_DLL "${MINGW_RUNTIME_DIR}/libstdc++-6.dll")
set(MINGW_GCC_DLL "${MINGW_RUNTIME_DIR}/libgcc_s_seh-1.dll")
set(MINGW_WINPTHREAD_DLL "${MINGW_RUNTIME_DIR}/libwinpthread-1.dll")

add_custom_command(TARGET brownian_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:assimp::assimp>
        $<TARGET_FILE_DIR:brownian_demo>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:brownian_demo>
)

if(EXISTS "${MINGW_STDCXX_DLL}")
    add_custom_command(TARGET brownian_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_STDCXX_DLL}
            $<TARGET_FILE_DIR:brownian_demo>
    )
endif()

if(EXISTS "${MINGW_GCC_DLL}")
    add_custom_command(TARGET brownian_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_GCC_DLL}
            $<TARGET_FILE_DIR:brownian_demo>
    )
endif()

if(EXISTS "${MINGW_WINPTHREAD_DLL}")
    add_custom_command(TARGET brownian_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_WINPTHREAD_DLL}
            $<TARGET_FILE_DIR:brownian_demo>
    )
endif()
