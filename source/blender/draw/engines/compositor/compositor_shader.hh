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

#pragma once

#include "DRW_render.h"

#include "GPU_material.h"

namespace blender::compositor {

class ShaderModule {
 private:
  DRWShaderLibrary *shader_lib_ = nullptr;

 public:
  ShaderModule();
  ~ShaderModule();

  /** TODO(fclem) multipass. */
  GPUMaterial *material_get(Scene *scene);

  GPUShaderSource pass_shader_code_generate(const GPUCodegenOutput *codegen, GPUMaterial *gpumat);

 private:
  char *pass_shader_code_vert_get(const GPUCodegenOutput *codegen, GPUMaterial *gpumat);
  char *pass_shader_code_frag_get(const GPUCodegenOutput *codegen, GPUMaterial *gpumat);
  char *pass_shader_code_defs_get();
};

}  // namespace blender::compositor
