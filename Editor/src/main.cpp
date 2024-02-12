#include "imgui/imgui.h"
#include "rendering/opengl/opengl_renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "scene/component.hpp"
#include "ttf/ttfParser.h"

const char* font_path = "assets/fonts/Pixelcraft.ttf";

void font_parsed(void* args, void* _font_data, int error) {
	if (error) {
		*(uint8_t*)args = error;
		Logger::LogError("Unable to parse font %s\n", font_path);
	}
	else {
		TTFFontParser::FontData* font_data = (TTFFontParser::FontData*)_font_data;
		Logger::LogInfo("Font: %s %s parsed\n", font_data->font_names.begin()->font_family.c_str(), font_data->font_names.begin()->font_style.c_str());
		Logger::LogInfo("Number of glyphs: %d\n", uint32_t(font_data->glyphs.size()));

		//step through glyph geometry
		{
			for (const auto& glyph_iterator : font_data->glyphs) {
				uint32_t num_curves = 0, num_lines = 0;
				for (const auto& path_list : glyph_iterator.second.path_list) {
					for (const auto& geometry : path_list.geometry) {
						if (geometry.is_curve)
							num_curves++;
						else
							num_lines++;
					}
				}
				Logger::LogInfo("glyph 0x%x %lc: %d quadratic curves and %d lines\n", glyph_iterator.first, glyph_iterator.first, num_curves, num_lines);
			}
		}

		*(uint8_t*)args = 1;
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Logger::OpenDefaultFile();

	Window window;
	Editor editor(window);
	const std::unique_ptr<Renderer> renderer = std::make_unique<OpenglRenderer>();

	Vector4 colorCheck = {0.5f, 0.5f, 0.5f, 0.5f};

	// Template seems to work between dll and exe
	Scene::CreateBasicScene();
	Scene& scene = *Scene::scene;
	
	uint8_t condition_variable = 0;

	std::chrono::system_clock::time_point timeStart = std::chrono::system_clock::now();

	TTFFontParser::FontData font_data;
	int8_t error = TTFFontParser::parse_file(font_path, &font_data, &font_parsed, &condition_variable);

	while (!condition_variable) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::chrono::system_clock::time_point timeEnd = std::chrono::system_clock::now();

	Logger::LogInfo("Took %d ns\n", std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count());

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();
		
		renderer->SetClearColor(colorCheck);
		renderer->ClearColorAndDepth();

		ImGui::ColorPicker4("colorPickerTest", colorCheck.Raw(), ImGuiColorEditFlags_PickerHueWheel);

		editor.EndFrame(window);
		window.SwapBuffers(); 
	}

	Logger::Stop();
	
	return 0;
}
