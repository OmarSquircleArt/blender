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
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup cmpnodes
 */

#include "IMB_colormanagement.h"
#include "node_composite_util.hh"

/* ******************* Color Correction ********************************* */

namespace blender::nodes {

static void cmp_node_colorcorrection_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>("Image").default_value({1.0f, 1.0f, 1.0f, 1.0f});
  b.add_input<decl::Float>("Mask").default_value(1.0f).min(0.0f).max(1.0f);
  b.add_output<decl::Color>("Image");
}

}  // namespace blender::nodes

static void node_composit_init_colorcorrection(bNodeTree *UNUSED(ntree), bNode *node)
{
  NodeColorCorrection *n = (NodeColorCorrection *)MEM_callocN(sizeof(NodeColorCorrection),
                                                              "node colorcorrection");
  n->startmidtones = 0.2f;
  n->endmidtones = 0.7f;
  n->master.contrast = 1.0f;
  n->master.gain = 1.0f;
  n->master.gamma = 1.0f;
  n->master.lift = 0.0f;
  n->master.saturation = 1.0f;
  n->midtones.contrast = 1.0f;
  n->midtones.gain = 1.0f;
  n->midtones.gamma = 1.0f;
  n->midtones.lift = 0.0f;
  n->midtones.saturation = 1.0f;
  n->shadows.contrast = 1.0f;
  n->shadows.gain = 1.0f;
  n->shadows.gamma = 1.0f;
  n->shadows.lift = 0.0f;
  n->shadows.saturation = 1.0f;
  n->highlights.contrast = 1.0f;
  n->highlights.gain = 1.0f;
  n->highlights.gamma = 1.0f;
  n->highlights.lift = 0.0f;
  n->highlights.saturation = 1.0f;
  node->custom1 = 7;  // red + green + blue enabled
  node->storage = n;
}

static int node_composite_gpu_colorcorrection(GPUMaterial *mat,
                                              bNode *node,
                                              bNodeExecData *UNUSED(execdata),
                                              GPUNodeStack *in,
                                              GPUNodeStack *out)
{
  NodeColorCorrection *n = (NodeColorCorrection *)node->storage;

  float enabled_channels[3];
  for (int i = 0; i < 3; i++) {
    enabled_channels[i] = (node->custom1 & (1 << i)) ? 1.0f : 0.0f;
  }

  return GPU_stack_link(mat,
                        node,
                        "node_composite_color_correction",
                        in,
                        out,
                        GPU_constant(enabled_channels),
                        GPU_uniform(&n->startmidtones),
                        GPU_uniform(&n->endmidtones),
                        GPU_uniform(&n->master.saturation),
                        GPU_uniform(&n->master.contrast),
                        GPU_uniform(&n->master.gamma),
                        GPU_uniform(&n->master.gain),
                        GPU_uniform(&n->master.lift),
                        GPU_uniform(&n->shadows.saturation),
                        GPU_uniform(&n->shadows.contrast),
                        GPU_uniform(&n->shadows.gamma),
                        GPU_uniform(&n->shadows.gain),
                        GPU_uniform(&n->shadows.lift),
                        GPU_uniform(&n->midtones.saturation),
                        GPU_uniform(&n->midtones.contrast),
                        GPU_uniform(&n->midtones.gamma),
                        GPU_uniform(&n->midtones.gain),
                        GPU_uniform(&n->midtones.lift),
                        GPU_uniform(&n->highlights.saturation),
                        GPU_uniform(&n->highlights.contrast),
                        GPU_uniform(&n->highlights.gamma),
                        GPU_uniform(&n->highlights.gain),
                        GPU_uniform(&n->highlights.lift));
}

void register_node_type_cmp_colorcorrection(void)
{
  static bNodeType ntype;

  cmp_node_type_base(&ntype, CMP_NODE_COLORCORRECTION, "Color Correction", NODE_CLASS_OP_COLOR, 0);
  ntype.declare = blender::nodes::cmp_node_colorcorrection_declare;
  node_type_size(&ntype, 400, 200, 600);
  node_type_init(&ntype, node_composit_init_colorcorrection);
  node_type_storage(
      &ntype, "NodeColorCorrection", node_free_standard_storage, node_copy_standard_storage);
  node_type_gpu(&ntype, node_composite_gpu_colorcorrection);

  nodeRegisterType(&ntype);
}
