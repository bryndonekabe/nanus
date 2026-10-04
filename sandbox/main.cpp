#include <bedrock/containers.hpp>
#include <bedrock/mat.hpp>
#include <bedrock/vec.hpp>
#include <gfx/common.hpp>
#include <gfx/mesh.hpp>
#include <gfx/texture.hpp>
#include <gfx/uniforms.hpp>
#include <input/common.hpp>
#include <input/gamepad.hpp>
#include <input/keyboard.hpp>
#include <platform/debug.hpp>

#include "util.hpp"

using namespace nanus;

// gfx::Vertex vertices[24] = {
//     // Front (+Z)

//     {.position{-0.5f, -0.5f, 0.5f},
//      .normal{0.0f, 0.0f, 1.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, -0.5f, 0.5f},
//      .normal{0.0f, 0.0f, 1.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, 0.5f},
//      .normal{0.0f, 0.0f, 1.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, 0.5f, 0.5f},
//      .normal{0.0f, 0.0f, 1.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},

//     // Back (-Z)
//     {.position{0.5f, -0.5f, -0.5f},
//      .normal{0.0f, 0.0f, -1.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, -0.5f, -0.5f},
//      .normal{0.0f, 0.0f, -1.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, 0.5f, -0.5f},
//      .normal{0.0f, 0.0f, -1.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, -0.5f},
//      .normal{0.0f, 0.0f, -1.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},

//     // Left (-X)
//     {.position{-0.5f, -0.5f, -0.5f},
//      .normal{-1.0f, 0.0f, 0.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, -0.5f, 0.5f},
//      .normal{-1.0f, 0.0f, 0.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, 0.5f, 0.5f},
//      .normal{-1.0f, 0.0f, 0.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, 0.5f, -0.5f},
//      .normal{-1.0f, 0.0f, 0.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},

//     // Right (+X)
//     {.position{0.5f, -0.5f, 0.5f},
//      .normal{1.0f, 0.0f, 0.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, -0.5f, -0.5f},
//      .normal{1.0f, 0.0f, 0.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, -0.5f},
//      .normal{1.0f, 0.0f, 0.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, 0.5f},
//      .normal{1.0f, 0.0f, 0.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},

//     // Top (+Y)
//     {.position{-0.5f, 0.5f, 0.5f},
//      .normal{0.0f, 1.0f, 0.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, 0.5f},
//      .normal{0.0f, 1.0f, 0.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, 0.5f, -0.5f},
//      .normal{0.0f, 1.0f, 0.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, 0.5f, -0.5f},
//      .normal{0.0f, 1.0f, 0.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{1, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},

//     // Bottom (-Y)
//     {.position{-0.5f, -0.5f, -0.5f},
//      .normal{0.0f, -1.0f, 0.0f},
//      .uv{0.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, -0.5f, -0.5f},
//      .normal{0.0f, -1.0f, 0.0f},
//      .uv{1.0f, 0.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{0.5f, -0.5f, 0.5f},
//      .normal{0.0f, -1.0f, 0.0f},
//      .uv{1.0f, 1.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
//     {.position{-0.5f, -0.5f, 0.5f},
//      .normal{0.0f, -1.0f, 0.0f},
//      .uv{0.0f, 1.0f},
//      .bone_ids{0, 0, 0, 0},
//      .bone_weights{255, 0, 0, 0}},
// };
// gfx::Face indices[12] = {

//     // Front
//     {0, 1, 3},
//     {1, 2, 3},
//     // Back
//     {4, 5, 7},
//     {5, 6, 7},
//     // Left
//     {8, 9, 11},
//     {9, 10, 11},
//     // Right
//     {12, 13, 15},
//     {13, 14, 15},
//     // Top
//     {16, 17, 19},
//     {17, 18, 19},
//     // Bottom
//     {20, 21, 23},
//     {21, 22, 23},
// };
// mat4 the_bones[2] = {mat4::identity(), mat4::rotate_z(rad(90.0f))};
// View<mat4> bones_view = {the_bones, 2};

gfx::Pixel pixels1[4] = {gfx::Pixel{0, 255, 0, 255}, gfx::Pixel{0, 255, 0, 255},
                         gfx::Pixel{0, 255, 0, 255}, gfx::Pixel{0, 255, 0, 0}};
gfx::Pixel pixels2[4] = {gfx::Pixel{0, 0, 255, 255}, gfx::Pixel{0, 0, 255, 255},
                         gfx::Pixel{0, 0, 255, 255},
                         gfx::Pixel{0, 0, 255, 255}};
gfx::Texture tex1 = {
    .pixels = View<gfx::Pixel>{pixels1, 4}, .width = 2, .height = 2};

struct Camera {
  vec3 pos;
  vec3 dir;
};

int main(int argc, char **argv) {
  DEBUG_PRINT("Start");
  if (argc < 2)
    return 0;

  platform::init();
  gfx::init();
  input::init();

  LoadedModel model = load_model(argv[1]);
  // apply propagation + inverse bind to transforms
  std::vector<mat4> skinned_transforms =
      bone_final(bone_globals(model.skeleton, model.local_transforms),
                 model.inverse_binds);

  gfx::MeshHandle mesh =
      gfx::submit(gfx::Mesh{{model.vertices.data(), model.vertices.size()},
                            {model.indices.data(), model.indices.size()}});

  DEBUG_PRINT("Vertices: %i\nIndices: %i\nBones: %i\n"
              "Binds: %i\nLocals: %i\nAnims: %i\n"
              "TexW: %i TexH: %i",
              model.vertices.size(), model.indices.size(),
              model.skeleton.size(), model.inverse_binds.size(),
              model.local_transforms.size(), model.animations.size(),
              model.tex.w, model.tex.h);
  for (int i = 0; i < model.animations.size(); ++i) {
    DEBUG_PRINT("Anim: %s", model.animations[i].name);
  }

  View<Bone> model_bones{model.skeleton.data(), model.skeleton.size()};
  View<mat4> model_transforms{skinned_transforms.data(),
                              skinned_transforms.size()};

  gfx::Texture model_tex =
      gfx::Texture{{model.tex.pixels.data(), model.tex.pixels.size()},
                   .width = model.tex.w,
                   .height = model.tex.h};
  gfx::TextureHandle model_tex_hdl;
  if (model.tex.w != 0 && model.tex.h != 0)
    model_tex_hdl = gfx::submit(model_tex);

  gfx::TextureHandle tex_hdl1 = gfx::submit(tex1);
  vec3 model_dir(0, 0, 0);

  const f64 aspect = (f64)gfx::width() / (f64)gfx::height();
  f64 fov = rad(75);
  Camera camera{vec3(0, 5, 10), vec3(rad(-30), 0, 0)};

  gfx::Light light{.pos{-5, 5, 0}, .color{1, 1, 1}, .intensity = 3.0f};

  gfx::clear_color({1.0f, 1.0f, 1.0f, 1.0f});

  // Main loop
  double time = 0;
  bool running = true; // main loop flag
  while (running) {
    input::poll();

    float input_scalar = 0.3;
    auto &pad = input::gamepad(0);

    // gyro movement
    vec3 gyro = pad.motion.gyro;
    float sensitivity = 1.f;
    float dt = 0.0166f;
    camera.dir.x += gyro.x * dt * sensitivity;

    camera.dir.y += gyro.y * dt * sensitivity;
    camera.dir.y += -gyro.z * dt * sensitivity;

    // analog movement
    vec3 forward{(f32)nanus::sin(camera.dir.y), 0,
                 (f32)nanus::cos(camera.dir.y)};
    vec3 right{(f32)nanus::cos(camera.dir.y), 0,
               -(f32)nanus::sin(camera.dir.y)};
    float move_speed = 0.3f;
    float look_speed = 2.5f;
    camera.pos -= forward * -pad.left_stick.y * move_speed;
    camera.pos += right * pad.left_stick.x * move_speed;
    // analog look
    camera.dir.y += rad(-pad.right_stick.x * look_speed);
    camera.dir.x += rad(-pad.right_stick.y * look_speed);

    if (pad.buttons[(usize)input::GamepadButton::FaceRight].current)
      input::rumble(0, 0.5f, 1.0f, 0.3f);

    if (input::keyboard().keys[(usize)input::Key::Q].current ||
        pad.buttons[(usize)input::GamepadButton::Plus].current)
      running = false;

    if (input::keyboard().keys[(usize)input::Key::W].current)
      camera.pos -= forward * input_scalar;
    if (input::keyboard().keys[(usize)input::Key::S].current)
      camera.pos += forward * input_scalar;
    if (input::keyboard().keys[(usize)input::Key::A].current)
      camera.pos -= right * input_scalar;
    if (input::keyboard().keys[(usize)input::Key::D].current)
      camera.pos += right * input_scalar;

    if (input::keyboard().keys[(usize)input::Key::I].current ||
        pad.buttons[(usize)input::GamepadButton::PadUp].current)
      camera.pos.y += input_scalar;
    if (input::keyboard().keys[(usize)input::Key::K].current ||
        pad.buttons[(usize)input::GamepadButton::PadDown].current)
      camera.pos.y -= input_scalar;

    if (input::keyboard().keys[(usize)input::Key::J].current)
      camera.dir.y += rad(input_scalar * 5);
    if (input::keyboard().keys[(usize)input::Key::L].current)
      camera.dir.y -= rad(input_scalar * 5);

    if (input::keyboard().keys[(usize)input::Key::O].current)
      fov -= rad(5);
    if (input::keyboard().keys[(usize)input::Key::P].current)
      fov += rad(5);

    if (input::keyboard().keys[(usize)input::Key::N].current)
      light.intensity -= 0.1f;
    if (input::keyboard().keys[(usize)input::Key::M].current)
      light.intensity += 0.1f;

    if (input::keyboard().keys[(usize)input::Key::Right].current)
      model_dir.y += rad(5);
    if (input::keyboard().keys[(usize)input::Key::Left].current)
      model_dir.y -= rad(5);
    if (input::keyboard().keys[(usize)input::Key::Up].current)
      model_dir.x += rad(5);
    if (input::keyboard().keys[(usize)input::Key::Down].current)
      model_dir.x -= rad(5);

    mat4 proj = mat4::perspective(fov, aspect, 0.1f, 1000.0f);
    gfx::proj(proj);

    const mat4 camera_mat = mat4::translate(camera.pos) *
                            mat4::rotate_y(camera.dir.y) *
                            mat4::rotate_x(camera.dir.x);
    gfx::view(camera_mat.inverse());
    gfx::light(light);

    gfx::begin();

    gfx::clear();

    // draw
    mat4 mdl_mat = mat4::rotate_x(model_dir.x) * mat4::rotate_y(model_dir.y);
    gfx::model(mdl_mat * mat4::scale(vec3(1, 2, 1)));
    if (model.tex.w != 0 && model.tex.h != 0)
      gfx::tex(model_tex_hdl);
    else
      gfx::tex(tex_hdl1);

    // run anim one if it exists
    // NOTE: VERY CRUDE and *NOT* gonna be anywhere near how this gets done in
    // future
    if (model.animations.size() > 0) {
      double dur = model.animations[0].duration;
      time += dt;
      // loop over
      time = std::fmod(time, dur);

      std::vector<mat4> anim_local_transforms =
          anim_locals(model.animations[0], model.local_transforms, time);

      // DEBUG_PRINT("time: %f duration: %f", time, dur);

      skinned_transforms =
          bone_final(bone_globals(model.skeleton, anim_local_transforms),
                     model.inverse_binds);
      model_transforms = {skinned_transforms.data(), skinned_transforms.size()};
    }
    gfx::bones(model_transforms);
    gfx::draw(mesh);

    // swap buffers
    gfx::swap();
  }

  DEBUG_PRINT("exit loop");
  gfx::deinit();
  input::deinit();
  platform::deinit();
}
