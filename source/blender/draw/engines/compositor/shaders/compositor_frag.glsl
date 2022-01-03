
#pragma BLENDER_REQUIRE(compositor_lib.glsl)
#pragma BLENDER_REQUIRE(common_view_lib.glsl)
#pragma BLENDER_REQUIRE(compositor_nodetree_eval_lib.glsl)

in vec4 uvcoordsvar;

layout(location = 0) out vec4 out_color;

void main()
{
  g_data.uv_render_layer = uvcoordsvar.xy;
  g_data.uv_texco = uvcoordsvar.xy * CameraTexCoFactors.xy + CameraTexCoFactors.zw;

  out_color = nodetree_composite();
}
