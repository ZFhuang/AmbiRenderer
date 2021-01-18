# 将下列的文件加入起名的库中
set(target_name "ImGui")

set(IMGUI_FILES
	 "lib/imconfig.h"
	 "lib/imgui.cpp"
	 "lib/imgui.h"
	 "lib/imgui_demo.cpp"
	 "lib/imgui_draw.cpp"
     "dx12example_main.cpp"
	 "lib/imgui_impl_dx12.cpp"
	 "lib/imgui_impl_dx12.h"
	 "lib/imgui_impl_win32.cpp"
	 "lib/imgui_impl_win32.h"
	 "lib/imgui_internal.h"
	 "lib/imgui_widgets.cpp"
	 "lib/imstb_rectpack.h"
	 "lib/imstb_textedit.h"
	 "lib/imstb_truetype.h"
	 )

# 使用静态链接
add_library(${target_name} STATIC ${IMGUI_FILES})

# 设置目录和编译模式
target_include_directories(${target_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)
