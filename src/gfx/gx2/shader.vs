#version 450
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in uvec4 bone_ids;
layout(location = 4) in vec4 bone_weights;

layout(binding = 0) uniform uniforms {
  mat4 model;
  mat4 view;
  mat4 proj;
  vec4 light_pos;
  vec4 light_color_intensity;
  vec4 tex_offset_scale;
};

layout(location = 0) out vec3 frag_pos;
layout(location = 1) out vec3 frag_normal;
layout(location = 2) out vec2 tex_coord;

void main(){
  // mat4 model = uniforms.model;
  // mat4 view = uniforms.view;
  // mat4 proj = uniforms.proj;

  vec4 world_pos = model * vec4(position, 1.0);

  mat3 normal_matrix = transpose(inverse(mat3(model)));

  frag_normal = normal_matrix * normal;
  frag_pos = world_pos.xyz;
  tex_coord = uv;
  // frag_normal = mat3(model) * normal;

  gl_Position = proj * view * world_pos;
}
