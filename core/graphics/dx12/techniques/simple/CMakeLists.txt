add_library( d12SimpleRender
 render.cpp
 render.hpp 
)

target_include_directories( d12SimpleRender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( d12SimpleRender 
    PRIVATE  dx12
    PRIVATE  D3D12MemoryAllocator
)

add_library( dx12::SimpleRender ALIAS d12SimpleRender )



# Build HLSL shaders
#add_custom_target(shaders)

#set(HLSL_SHADER_FILES shaders/vs.hlsl shaders/ps.hlsl)

#set_source_files_properties(vs.hlsl PROPERTIES ShaderType "vs")
#set_source_files_properties(ps.hlsl PROPERTIES ShaderType "ps")
#set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "6_0")

#foreach(FILE ${HLSL_SHADER_FILES})
#  get_filename_component(FILE_WE ${FILE} NAME_WE)
#  get_source_file_property(shadertype ${FILE} ShaderType)
#  get_source_file_property(shadermodel ${FILE} ShaderModel)
#  add_custom_command(TARGET shaders
#                     COMMAND dxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
#                     MAIN_DEPENDENCY ${FILE}
#                     COMMENT "HLSL ${FILE}"
#                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                     VERBATIM)
#endforeach(FILE)