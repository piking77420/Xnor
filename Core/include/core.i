%module Core

%include <csharp/stl.i>
%include <csharp/std_array.i>
%include <csharp/std_unordered_map.i>

%include <windows.i>

%include "core.hpp"

// Ignore these macros
#define DEFAULT_COPY_MOVE_OPERATIONS(...)
#define DELETE_COPY_MOVE_OPERATIONS(...)
#define REFLECTABLE_IMPL_MINIMAL(...)
#define REFLECTABLE_IMPL_MINIMAL_DLL(...)
#define REFLECTABLE_IMPL_H(...)
#define REFLECTABLE_IMPL_H_DLL(...)
#define REFLECTABLE_IMPL_CPP(...)
#define REFL_AUTO(...)

%{
#include <format>

#include "file/directory.hpp"
#include "file/entry.hpp"
#include "file/file.hpp"
#include "file/file_manager.hpp"

#include "input/gamepad_input.hpp"
#include "input/input.hpp"
#include "input/keyboard_input.hpp"
#include "input/mouse_input.hpp"
#include "input/time.hpp"

#include "rendering/light/directional_light.hpp"
#include "rendering/light/light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"

#include "rendering/render_systems/light_manager.hpp"
#include "rendering/render_systems/skybox_renderer.hpp"
#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/material.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/renderer.hpp"
#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "rendering/uniform_buffer.hpp"
#include "rendering/vertex.hpp"
#include "rendering/viewport.hpp"
#include "rendering/viewport_data.hpp"

#include "resource/cubemap.hpp"
#include "resource/font.hpp"
#include "resource/model.hpp"
#include "resource/resource.hpp"
#include "resource/resource_manager.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

#include "scene/component/camera_component.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/script_component.hpp"

#include "scene/component.hpp"
#include "scene/entity.hpp"
#include "scene/scene.hpp"

#include "serialization/serializer.hpp"

#include "utils/color.hpp"
#include "utils/formatter.hpp"
#include "utils/guid.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"
#include "utils/pointer.hpp"
#include "utils/poly_ptr.hpp"
#include "utils/reference_counter.hpp"
#include "utils/serializable.hpp"
#include "utils/ts_queue.hpp"
#include "utils/utils.hpp"

#include "world/scene_graph.hpp"
#include "world/skybox.hpp"
#include "world/world.hpp"

#include "application.hpp"
#include "core.hpp"
#include "screen.hpp"
#include "transform.hpp"
#include "window.hpp"
%}

%include "utils/logger.hpp"
%include "utils/pointer.hpp"

%include "utils/guid.hpp"

%include "resource/resource.hpp"

%include "file/entry.hpp"
%include "file/directory.hpp"
%include "file/file.hpp"
%include "file/file_manager.hpp"

%include "resource/cubemap.hpp"
%include "resource/font.hpp"
%include "resource/model.hpp"
%include "resource/resource_manager.hpp"

%include "rendering/rhi_typedef.hpp"

%include "resource/shader.hpp"
%include "resource/texture.hpp"

%include "input/gamepad_input.hpp"
%include "input/keyboard_input.hpp"
%include "input/mouse_input.hpp"
%include "input/input.hpp"
%include "input/time.hpp"

%include "scene/component.hpp"

%include "utils/color.hpp"

%include "rendering/light/light.hpp"
%include "rendering/light/directional_light.hpp"
%include "rendering/light/point_light.hpp"
%include "rendering/light/spot_light.hpp"

%include "rendering/camera.hpp"

%include "scene/component/camera_component.hpp"

%include "rendering/material.hpp"

%include "scene/component/mesh_renderer.hpp"
%include "scene/component/script_component.hpp"

%include "transform.hpp"

%include "scene/entity.hpp"

%include "world/skybox.hpp"
%include "utils/list.hpp"

%include "scene/scene.hpp"
%include "utils/utils.hpp"

%include "world/scene_graph.hpp"
%include "world/world.hpp"

%include "screen.hpp"
%include "window.hpp"
