void color_to_luminance(vec4 color, out float result)
{
  result = get_luminance(color.rgb, LuminanceCoefficients);
}
