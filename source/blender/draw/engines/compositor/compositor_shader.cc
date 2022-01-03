/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Copyright 2021, Blender Foundation.
 */

#include "BLI_string.h"
#include "BLI_string_ref.hh"

#include "DRW_render.h"

#include "compositor_shader.hh"

extern "C" {
extern char datatoc_common_fullscreen_vert_glsl[];
extern char datatoc_common_math_lib_glsl[];
extern char datatoc_common_view_lib_glsl[];
extern char datatoc_compositor_frag_glsl[];
extern char datatoc_compositor_lib_glsl[];
extern char datatoc_compositor_nodetree_eval_lib_glsl[];
extern char datatoc_gpu_shader_codegen_lib_glsl[];
}

namespace blender::compositor {

ShaderModule::ShaderModule()
{
  shader_lib_ = DRW_shader_library_create();
  /* NOTE: These need to be ordered by dependencies. */
  DRW_SHADER_LIB_ADD(shader_lib_, compositor_lib);
  DRW_SHADER_LIB_ADD(shader_lib_, common_math_lib);
  DRW_SHADER_LIB_ADD(shader_lib_, common_view_lib);
  DRW_SHADER_LIB_ADD(shader_lib_, gpu_shader_codegen_lib);
  DRW_SHADER_LIB_ADD(shader_lib_, compositor_nodetree_eval_lib);
}

ShaderModule::~ShaderModule()
{
  DRW_shader_library_free(shader_lib_);
}

char *ShaderModule::pass_shader_code_vert_get(const GPUCodegenOutput * /*codegen*/,
                                              GPUMaterial * /*gpumat*/)
{
  return BLI_strdup(datatoc_common_fullscreen_vert_glsl);
}

char *ShaderModule::pass_shader_code_frag_get(const GPUCodegenOutput *codegen,
                                              GPUMaterial * /*gpumat*/)
{
  std::string output;

  output += codegen->uniforms;
  output += "\n";

  output += codegen->library;
  output += "\n";

  output += "vec4 nodetree_composite() {\n";
  output += codegen->surface;
  output += "}\n";

  output += datatoc_compositor_frag_glsl;

  return DRW_shader_library_create_shader_string(shader_lib_, output.c_str());
}

char *ShaderModule::pass_shader_code_defs_get()
{
  std::string defines = "";
  defines += "#define COMPOSITOR_SHADER\n";

  return BLI_strdup(defines.c_str());
}

/* WATCH: This can be called from another thread! Needs to not touch the shader module in any
 * thread unsafe manner. */
GPUShaderSource ShaderModule::pass_shader_code_generate(const GPUCodegenOutput *codegen,
                                                        GPUMaterial *gpumat)
{
  GPUShaderSource source;
  source.vertex = pass_shader_code_vert_get(codegen, gpumat);
  source.fragment = pass_shader_code_frag_get(codegen, gpumat);
  source.geometry = nullptr;
  source.defines = pass_shader_code_defs_get();
  return source;
}

static GPUShaderSource codegen_callback(void *thunk,
                                        GPUMaterial *gpumat,
                                        const GPUCodegenOutput *codegen)
{
  return ((ShaderModule *)thunk)->pass_shader_code_generate(codegen, gpumat);
}

GPUMaterial *ShaderModule::material_get(Scene *scene)
{
  /* TODO(fclem) We might have one shader per pass in the future. */
  uint64_t shader_id = 0;
  return DRW_shader_from_compositor(scene, shader_id, true, codegen_callback, this);
}

}  // namespace blender::compositor
