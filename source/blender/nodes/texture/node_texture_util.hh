/* SPDX-FileCopyrightText: 2005 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup nodes
 */

#pragma once

#include "BKE_node.hh"
#include "BKE_node_legacy_types.hh"  // IWYU pragma: export

#include "node_texture_register.hh"

#include "BLT_translation.hh"

#include "RE_texture.h"

struct bNodeThreadStack;

struct TexCallData {
  TexResult *target;
  /* all float[3] */
  const float *co;
  float *dxt, *dyt;

  int osatex;
  bool do_preview;
  bool do_manage;
  short thread;
  short which_output;
  int cfra;

  const MTex *mtex;
};

struct TexParams {
  const float *co;
  float *dxt, *dyt;
  const float *previewco;
  int cfra;
  int osatex;

  /* optional. we don't really want these here, but image
   * textures need to do mapping & color correction */
  const MTex *mtex;
};

using TexFn = void (*)(float *out, TexParams *params, bNode *node, bNodeStack **in, short thread);

struct TexDelegate {
  TexCallData *cdata;
  TexFn fn;
  bNode *node;
  bNodePreview *preview;
  bNodeStack *in[MAX_SOCKET];
  int type;
};

bool tex_node_poll_default(const blender::bke::bNodeType *ntype,
                           const bNodeTree *ntree,
                           const char **r_disabled_hint);
void tex_node_type_base(blender::bke::bNodeType *ntype, std::string idname, int type);

void tex_input_rgba(float *out, bNodeStack *in, TexParams *params, short thread);
void tex_input_vec(float *out, bNodeStack *in, TexParams *params, short thread);
float tex_input_value(bNodeStack *in, TexParams *params, short thread);

void tex_output(bNode *node,
                bNodeExecData *execdata,
                bNodeStack **in,
                bNodeStack *out,
                TexFn texfn,
                TexCallData *data);

void params_from_cdata(TexParams *out, TexCallData *in);

bNodeThreadStack *ntreeGetThreadStack(bNodeTreeExec *exec, int thread);
void ntreeReleaseThreadStack(bNodeThreadStack *nts);
bool ntreeExecThreadNodes(bNodeTreeExec *exec,
                          bNodeThreadStack *nts,
                          void *callerdata,
                          int thread);

bNodeTreeExec *ntreeTexBeginExecTree_internal(bNodeExecContext *context,
                                              bNodeTree *ntree,
                                              bNodeInstanceKey parent_key);
void ntreeTexEndExecTree_internal(bNodeTreeExec *exec);
