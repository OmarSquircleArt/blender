void node_composite_luminance_matte(
    vec4 color, float high, float low, out vec4 result, out float matte)
{
  float luminance = get_luminance(color.rgb, LuminanceCoefficients);
  float alpha = clamp(0.0, 1.0, (luminance - low) / (high - low));
  matte = min(alpha, color.a);
  result = color * matte;
}
