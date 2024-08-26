// #include "../stdafx.h"
#include "Game.h"
#include <array>
#include <functional>
#include <optional>
#include <span>
#include <vector>
// #include "Entity.h"
// #include "LinearMotion.h"

#include "../engine/Camera.h"
#include "../engine/Event.h"
#include "../engine/GameEvent.h"
#include "../engine/Visuals.h"
#include "../game-gwell/crossfire.h"
#include "../procedural/Geometry.h"
#include "../util/soa.hpp"
#include <algorithm>

const unsigned short VK_LEFT = 0x25;
const unsigned short VK_UP = 0x26;
const unsigned short VK_RIGHT = 0x27;
const unsigned short VK_DOWN = 0x28;
const unsigned short VK_SPACE = 0x20;

namespace Game {

Engine::Visuals::Basic::Visual box_visual;
Engine::Visuals::Basic::Visual row_visual;
Engine::Visuals::Basic::Visual col_visual;
Engine::Visuals::Basic::Visual sphere_visual;
Engine::Visuals::Basic::Visual teapot_visual;

util::soa::SOA<util::soa::FixedArray<24>, crossfire::Entity,
               Engine::Visuals::Basic::Visual>
    visual_map;

float rotAngle = 0.f;
Engine::Camera cam;

crossfire::Entity theOne;
crossfire::Entity randomBox;

int player_lives{3};

std::size_t lastPositionForRandomBox{0};

struct CrossFireVisual {
  Engine::Visuals::Basic::Visual visualId{};
  float x = 0;
  float y = 0;

  static constexpr float gameUnitToFloat(int value) {
    return (static_cast<float>(value));
  }
};

std::vector<CrossFireVisual> dynamic_elements;
std::vector<CrossFireVisual> static_elements;

static const int Team1 = 1;
static const int Team2 = 2;

void Initalize() {
  Events::Event<Engine::NextLogicFrame>::Listen(&onLogicEvent);
  Events::Event<Engine::NextRenderFrame>::Listen(&onRenderEvent);
  Events::Event<Engine::GameInitalizeData>::Listen(&onGameInitalizeEvent);
}

void onLogicEvent(const Engine::NextLogicFrame &frame) {
  crossfire::gametime::on_logic_event();
  crossfire::keyboardinput::on_logic_tick();
  crossfire::linear::run(1.0f);
  auto values = crossfire::linear::getEntitiesPositions();
  crossfire::collider::do_collisions(values);

  auto collisions = crossfire::collider::get_collisions();
  crossfire::collision_behavior::run(collisions);

  dynamic_elements.clear();
  dynamic_elements.reserve(values.size());
  for (int i = 0; i < values.size(); ++i) {
    auto findVisual = visual_map.find<crossfire::Entity>(values.entities[i]);
    if (findVisual != visual_map.end()) {

      dynamic_elements.emplace_back(
          findVisual.get<Engine::Visuals::Basic::Visual>(),
          CrossFireVisual::gameUnitToFloat(values.positions[i].x),
          CrossFireVisual::gameUnitToFloat(values.positions[i].y));
    }
  };
  crossfire::entities::on_logic_finished();
}

void onRenderEvent(const Engine::NextRenderFrame &frame) {
  namespace vs = Engine::Visuals::Basic;
  namespace math = Engine::Math;
  vs::RenderState state{cam};

  for (auto const &renderable : dynamic_elements) {
    Engine::Matrix world;
    Engine::fMatrix worldf;

    worldf =
        math::XMMatrixTranslation(renderable.x + 5, renderable.y + 5, -205);

    math::XMStoreFloat4x4(&world, worldf);

    state.states.push_back({world, renderable.visualId});

    state.lights.push_back(Engine::Lights::AnyLight{
        Engine::Lights::Point{{renderable.x + 5, renderable.y + 5, -180},
                              {0, 0.3, 0, 0},
                              0.01,
                              0.02,
                              0.0003}});
  }

  for (int nbrLives = 0; nbrLives < player_lives; ++nbrLives) {
    Engine::Matrix world;
    Engine::fMatrix worldf;
    worldf = math::XMMatrixMultiply(
        math::XMMatrixScaling(.7f, .7f, .7f),
        math::XMMatrixTranslation(-50 + nbrLives * 15, 0, -205));
    math::XMStoreFloat4x4(&world, worldf);

    state.states.push_back({world, box_visual});
  }

  int count = 0;
  for (auto const &renderable : static_elements) {

    Engine::Matrix world;
    Engine::fMatrix worldf;
    worldf = math::XMMatrixTranslation(renderable.x, renderable.y, -200);

    math::XMStoreFloat4x4(&world, worldf);
    state.states.push_back({world, renderable.visualId});
    ++count;
  }

  state.GlobalAmbientLight = {0.1, 0.1, 0.1, 1};
  state.lights.push_back(
      Engine::Lights::Directional{{0, 0, -1}, {0.3, 0.3, 0.3, 0}});

  
  vs::Render(std::move(state));
}

void onGameInitalizeEvent(const Engine::GameInitalizeData &data) {
  namespace vs = Engine::Visuals::Basic;

  Geometry::VertexCollection vbs;
  Geometry::IndexCollection ids;
  auto screenInfo = vs::Get_ScreenSize();

  {
    vbs.clear();
    ids.clear();
    Geometry::ComputeBox(vbs, ids, {10, 10, 10}, false, false);
    // Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

    Engine::MeshView mesh_view{Engine::make_meshview(vbs, ids)};
    Engine::Material mat{{0, 0, 0, 0},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 1, 0.9, 1},
                         128};
    box_visual = vs::Create(mesh_view, mat);
  }
  {
    vbs.clear();
    ids.clear();
    Geometry::ComputeSphere(vbs, ids, 10, 4, false, false);
    // Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

    Engine::MeshView mesh_view{Engine::make_meshview(vbs, ids)};
    Engine::Material mat{{0, 0, 0, 0},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 1, 0.9, 1},
                         128};
    sphere_visual = vs::Create(mesh_view, mat);
  }
  {
    vbs.clear();
    ids.clear();
    Geometry::ComputeTeapot(vbs, ids, 10, 4, false);

    Engine::MeshView mesh_view{Engine::make_meshview(vbs, ids)};
    Engine::Material mat{{0, 0, 0, 0},
                         {0.5, 0.4, 0.3, 1},
                         {0.2, 0.2, 0.2, 1},
                         {0.6, 1, 0.1, 1},
                         128};
    teapot_visual = vs::Create(mesh_view, mat);
  }
  {
    vbs.clear();
    ids.clear();
    auto path = crossfire::map::getRowPath();
    Geometry::ComputeBox(vbs, ids,
                         {static_cast<float>(path.x2 - path.x),
                          static_cast<float>(path.y2 - path.y), .01f},
                         false, false);
    // Geometry::ComputeBox(vbs, ids, { 10,80,0.1f }, false, false);

    Engine::MeshView mesh_view{Engine::make_meshview(vbs, ids)};
    Engine::Material mat{{0, 0, 0, 0},
                         {0.1, 0.1, 0.5, 1},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 1, 0.9, 1},
                         128};
    row_visual = vs::Create(mesh_view, mat);
  }
  {
    vbs.clear();
    ids.clear();
    auto path = crossfire::map::getColPath();
    Geometry::ComputeBox(vbs, ids,
                         {static_cast<float>(path.x2 - path.x),
                          static_cast<float>(path.y2 - path.y), .001f},
                         false, false);
    // Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

    Engine::MeshView mesh_view{Engine::make_meshview(vbs, ids)};
    Engine::Material mat{{0, 0, 0, 0},
                         {0.5, 0.1, 0.1, 1},
                         {0.9, 0.9, 0.9, 1},
                         {0.9, 1, 0.9, 1},
                         128};
    col_visual = vs::Create(mesh_view, mat);
  }

  auto paths = crossfire::map::getPaths();
  auto isRow = [](const crossfire::map::path &p) -> bool {
    return (p.x2 - p.x) > (p.y2 - p.y);
  };

  for (const auto &p : paths) {
    const auto asRow = isRow(p);
    static_elements.emplace_back(asRow ? row_visual : col_visual,
                                 CrossFireVisual::gameUnitToFloat(p.x) +
                                     static_cast<float>(p.x2 - p.x) / 2,
                                 CrossFireVisual::gameUnitToFloat(p.y) +
                                     static_cast<float>(p.y2 - p.y) / 2);
  }

  cam.setPerspectiveFOV(Engine::Math::XMConvertToRadians(45),
                        screenInfo.aspect_ratio(), .1f, 500.f);
  // cam.setPosition({ 0,0,-10,0 });
  // cam.setRotation({ })
  cam.lookAt({85, 85, 50, 0}, {85, 85, -1, 0}, {0, -1, 0, 0});


  Events::Event<crossfire::entities::EntityRemovedEvent>::Listen(
      [](const crossfire::entities::EntityRemovedEvent &values) {
        for (auto &value : values.toBeRemoved) {

          auto it = visual_map.find<crossfire::Entity>(value);
          if (it != visual_map.end())
            visual_map.remove(it);
        }
      });
  Events::Event<crossfire::missle_shooter::MissleFiredEvent>::Listen(
      [](const crossfire::missle_shooter::MissleFiredEvent &evt) {
        visual_map.push_back(evt.missle, sphere_visual);
      });

  crossfire::keyboardinput::intialize();

  theOne = crossfire::entities::create(Team1);
  visual_map.push_back(theOne, box_visual);

  randomBox = crossfire::entities::create(Team2);
  visual_map.push_back(randomBox, teapot_visual);

  crossfire::linear::create(theOne, crossfire::Heading::Right, 0, 0,
                            crossfire::Velocity::Normal);
  crossfire::linear::create(randomBox, crossfire::Heading::Stopped, 60, 60);

  crossfire::keyboardinput::create(theOne, {});

  const int MAP_BORDER_EXTENTS = 500;

  auto map_extents = crossfire::map::getMapExtents();

  const auto player_behavior = crossfire::collision_behavior::create(
      [=](crossfire::Entity self, crossfire::Entity other) {
        if (other == crossfire::entities::get_environment()) {
          crossfire::linear::changeHeading(self, crossfire::Heading::Stopped);
          crossfire::linear::force_entity_onto_map(self);
          // crossfire::linear::changePosition(self, 0, 0);
          return;
        }

        if (crossfire::entities::is_same_team(self, other))
          return;

        --player_lives;
        if (player_lives < 0)
          player_lives = 3;

        crossfire::linear::changePosition(self, 0, 0);
      });

  crossfire::collision_behavior::set_entity(theOne, player_behavior);

  const auto player_missle_behavior = crossfire::collision_behavior::create(
      [](crossfire::Entity self, crossfire::Entity other) {
        if (crossfire::entities::is_same_team(self, other))
          return;
        crossfire::entities::remove(self);
      });

  crossfire::missle_shooter::set(
      theOne, player_missle_behavior, 4,
      crossfire::gametime::Ticks::from_milliseconds(500));

  const auto random_box_behavior = crossfire::collision_behavior::create(
      [=](crossfire::Entity self, crossfire::Entity other) {
        const crossfire::CurrentPosition randomPositions[] = {
            {crossfire::Heading::Stopped, 40, 0},
            {crossfire::Heading::Stopped, 20, 40},
            {crossfire::Heading::Stopped, 60, 60},
            {crossfire::Heading::Stopped, 80, 40}};

        const std::size_t randomPositionsSize = 3;
        ++lastPositionForRandomBox;
        if (lastPositionForRandomBox > randomPositionsSize)
          lastPositionForRandomBox = 0;
        crossfire::linear::changePosition(
            randomBox, randomPositions[lastPositionForRandomBox].x,
            randomPositions[lastPositionForRandomBox].y);
      });

  crossfire::collision_behavior::set_entity(randomBox, random_box_behavior);

  crossfire::collision_behavior::create(
      [](crossfire::Entity self, crossfire::Entity other) {
        if (crossfire::entities::is_same_team(self, other))
          return;
        crossfire::entities::remove(self);
      });

  // Left most wall

  crossfire::collider::add_static_collider(
      crossfire::entities::get_environment(),
      map_extents.x - MAP_BORDER_EXTENTS, map_extents.y - 1, map_extents.x - 1,
      map_extents.y2 + 1);

  // Right most wall
  crossfire::collider::add_static_collider(
      crossfire::entities::get_environment(), map_extents.x2 + 1,
      map_extents.y - 1, map_extents.x2 + MAP_BORDER_EXTENTS,
      map_extents.y2 + 1);

  // Top wall
  crossfire::collider::add_static_collider(
      crossfire::entities::get_environment(), map_extents.x - 1,
      map_extents.y - MAP_BORDER_EXTENTS, map_extents.x2 + 1,
      map_extents.y - 1);

  // Bottom Wall
  crossfire::collider::add_static_collider(
      crossfire::entities::get_environment(), map_extents.x - 1,
      map_extents.y2 + 1, map_extents.x2 + 1,
      map_extents.y2 + MAP_BORDER_EXTENTS);


}
} // namespace Game

// Engine::fVector4 rotVector = math::XMVectorSet(0, 1, 1, 0);

// rotAngle += 0.01;

// worldf = math::XMMatrixRotationAxis(rotVector,
// Engine::Math::XMConvertToRadians(rotAngle));