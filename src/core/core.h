#if !defined(CORE_H)

#ifdef CORE_EXPORT
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "asset_system/asset.h"
#include "asset_system/asset_manager.h"
#include "event_system/event_system.h"
#include "input_system/input_manager.h"
#include "window/window.h"
#include "mesh/mesh.h"
#include "camera_system/camera.h"
#include "camera_system/debug_camera.h"
#include "texture/texture.h"
#include "image/image.h"
#include "platform/opengl/opengl.h"
#include "platform/opengl/vbo.h"
#include "platform/opengl/vao.h"
#include "platform/opengl/shader.h"
#include "platform/thread.h"
#include "platform/platform_utils.h"
#include "terrain_system/terrain.h"
#include "platform/platform.h"
#include "component_system/component_manager.h"
#include "component_system/component.h"
#include "entity_system/entity.h"
#include "entity_system/entity_manager.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/math_utils.h"
#include "math/perlin.h"
#include "math/fractal_noise.h"
#include "quadtree/quadtree.h"
#include "loader.h"
#include "time/timer.h"
#include "core_return.h"
#include "system/system.h"
#include "system/system_manager.h"
#include "handle_system/handle.h"
#include "text/font_file.h"
//#include "memory_system/heap.h"

#define CORE_H
#endif
