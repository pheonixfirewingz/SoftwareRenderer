
ADD_FILES(
	imconfig.h
	imgui_draw.cpp
	imgui_internal.h
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.h
	imgui.cpp
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
)
add_subdirectory(glad)
add_subdirectory(backend)
add_subdirectory(meshoptimizer)