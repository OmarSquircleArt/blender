/* Keep in sync with DRWCompositorUboStorage. */
layout(std140) uniform compositorBlock
{
  vec3 LuminanceCoefficients;
  float FrameNumber;
};
