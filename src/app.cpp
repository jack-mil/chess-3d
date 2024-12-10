#include <iostream>

#include "OgreAdvancedRenderControls.h"
#include "OgreCameraMan.h"
#include "OgreImGuiOverlay.h"
#include "OgreKeyFrame.h"
#include "OgreOverlaySystem.h"
#include "OgreRTShaderSystem.h"
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreTrays.h"

#include "app.hpp"
#include "engine.hpp"
#include "overlay.hpp"
#include "theme.hpp"

using namespace chess3d;

App::App()
    : OgreBites::ApplicationContext("Chess 3D!"),
      m_engine(std::make_unique<Engine>(this)),
      m_ui(std::make_unique<Overlay>(m_engine.get())) {
}

// for some reason this is required
App::~App() { }

void App::exec() {
  this->initApp();
  this->getRoot()->startRendering();
  this->closeApp();
}

void App::loadResources() {
  enableShaderCache();

  auto resourceMgr = Ogre::ResourceGroupManager::getSingletonPtr();

  // load essential resources for trays/ loading bar
  // resourceMgr->initialiseResourceGroup("Essential");

  resourceMgr->initialiseResourceGroup("OgreInternal"); // for shadow lighting...?
  resourceMgr->addResourceLocation("./resources", "FileSystem", "ChessGroup");
  resourceMgr->initialiseResourceGroup("General");
  resourceMgr->initialiseResourceGroup("ChessGroup");
  resourceMgr->setWorldResourceGroupName("ChessGroup");
}

void App::setup() {
  using namespace Ogre;

  // call base setup() first
  OgreBites::ApplicationContext::setup();

  // get a pointer to the already created root
  Root* root = getRoot();
  m_sceneMngr = root->createSceneManager(Ogre::SMT_DEFAULT, "chessScene");

  // register scene with the RTSS
  RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->getRenderState(Ogre::MSN_SHADERGEN);
  shadergen->addSceneManager(m_sceneMngr); // must be done before we do anything with the scene
  m_sceneMngr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);

  // load the entire chess scene and all objects (exported from Blender)
  auto sceneOrigin = m_sceneMngr->getRootSceneNode()->createChildSceneNode("origin");
  sceneOrigin->loadChildren("chess.scene");

  // build up map of pieces at initial chess positions
  for (auto piece : m_sceneMngr->getSceneNode("pieces")->getChildren()) {
    // I added starting position property to the pieces
    auto prop = piece->getUserObjectBindings().getUserAny("position");
    if (prop.has_value()) {
      auto position = any_cast<String>(prop);
      // add position and SceneNode pointer to map
      if (!m_positions.contains(position)) {
        std::cout << piece->getName() << " at " << position << '\n';
        m_positions.insert_or_assign(position, static_cast<Ogre::SceneNode*>(piece));
      }
    }
  }

  // Set render target to this camera output
  Camera* cam = m_sceneMngr->getCamera("camera1");
  cam->setAutoAspectRatio(true);
  Viewport* vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

  // imgui overlay
  m_sceneMngr->addRenderQueueListener(getOverlaySystem());
  auto imGUIOverlay = initialiseImGui();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  chess3d::setModernColors(ImGui::GetStyle());
  imGUIOverlay->show();
  getRenderWindow()->addListener(this);

  // attach the user input controls to the camera node
  // (named node loaded from the .scene)
  SceneNode* camNode = m_sceneMngr->getSceneNode("camera1");
  m_camMgr = std::make_unique<OgreBites::CameraMan>(camNode);
  m_camMgr->setStyle(OgreBites::CS_ORBIT);

  // Add the input listeners in order
  m_inputChain = OgreBites::InputListenerChain({this, getImGuiInputListener(), m_camMgr.get()});
  addInputListener(&m_inputChain);


  Animation::setDefaultInterpolationMode(Animation::IM_SPLINE);

  std::cout << "Setting up Done\n";
}

// Called before a frame is rendered, updates ui
// RenderTargetListener override
void App::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) {

  if (!evt.source->getOverlaysEnabled()) {
    return;
  }
  Ogre::ImGuiOverlay::NewFrame();
  m_ui->draw();

  lightPropOverlay();

  // static char from[25];
  // static char to[25];
  // ImGui::InputText("from", from, IM_ARRAYSIZE(from));
  // ImGui::InputText("to", to, IM_ARRAYSIZE(to));
  // if (ImGui::Button("Click Me)")) {
  //   this->movePiece(from, to);
  // }
}

void App::lightPropOverlay() {
  // From ImGui Demo ShowExampleAppSimpleOverlay
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                  ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                  ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
  // position the window in top right
  const float PAD = 10.0f;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 work_pos = viewport->WorkPos;
  ImVec2 work_size = viewport->WorkSize;
  ImVec2 window_pos;
  window_pos.x = (work_pos.x + work_size.x - PAD);
  window_pos.y = (work_pos.y + PAD);
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1, 0));
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGui::Begin("Light Controls", NULL, window_flags);

  ImGui::Text("Light Position");
  ImGui::Separator();
  { //
    using namespace Ogre;

    auto lightNode = m_sceneMngr->getSceneNode("light.spot1");
    auto light = m_sceneMngr->getLight("light.spot1");
    auto board = m_sceneMngr->getSceneNode("board");
    lightNode->lookAt(board->getPosition(), Node::TS_PARENT);

    ColourValue color = light->getDiffuseColour();
    Vector3 pos = lightNode->getPosition();
    Quaternion rot = lightNode->getOrientation();

    ImGui::ColorEdit3("Color", color.ptr(), ImGuiColorEditFlags_Float);
    ImGui::DragFloat3("(XYZ) Pos", pos.ptr(), 0.1f, -10.0f, 10.0f, "%0.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::BeginDisabled(); // read only
    ImGui::DragFloat4("(WXYZ) Rot", rot.ptr(), 0.1f, -1.0f, 1.0f, "%0.4f");
    ImGui::EndDisabled();

    lightNode->setPosition(pos);
    light->setDiffuseColour(color);
    light->setSpecularColour(color);
  }

  ImGui::End();
}

/** Called after all render targets have had their rendering commands
    issued, but before render windows have been asked to flip their
    buffers over.
    The usefulness of this event comes from the fact that rendering
    commands are queued for the GPU to process. These can take a little
    while to finish, and so while that is happening the CPU can be doing
    useful things. Once the request to 'flip buffers' happens, the thread
    requesting it will block until the GPU is ready, which can waste CPU
    cycles. Therefore, it is often a good idea to use this callback to
    perform per-frame processing. Of course because the frame's rendering
    commands have already been issued, any changes you make will only
    take effect from the next frame, but in most cases that's not noticeable.
    - From: FrameListener override
*/
bool App::frameRenderingQueued(const Ogre::FrameEvent& evt) {

  // update all animations
  // need to remove ones that have completed, so we can reuse names
  auto iter = m_animations.begin();
  auto end = m_animations.end();
  while (iter != end) {
    auto* anim = *iter;
    anim->addTime(evt.timeSinceLastFrame);

    if (anim->hasEnded()) {
      iter = m_animations.erase(iter);
      // removes the associated AnimationState as well
      m_sceneMngr->removeAnimation(anim->getAnimationName());
    } else {
      // process next one
      ++iter;
    }
  }

  OgreBites::ApplicationContext::frameRenderingQueued(evt);
  return true; // always want to return true, otherwise rendering stops
}

/** from/to are algebraic chess cells e.g. a1, c5, d8, etc. Assumed to be correct */
void App::movePiece(const std::string& from, const std::string& to) {
  // setup a move animations for moving piece and captured piece.
  // this could be better abstracted and handled in functions, but
  // its all here right now

  using namespace Ogre;
  static constexpr float speed = 2;
  static Vector3 sidelines(3, 0, 5);

  if (!m_positions.contains(from)) {
    return;
  }
  // get the piece pointer at this location,
  // and remove it from the mapping in one go
  auto piece = m_positions.extract(from).mapped();
  Vector3 offset = calcDestOffset(from, to);
  auto duration = offset.length() / speed; // for constant speed
  piece->setInitialState();                // freeze current position as animation state

  Animation* anim = m_sceneMngr->createAnimation(from + to, duration);
  auto* track = anim->createNodeTrack(0, piece);

  track->createNodeKeyFrame(0)->setTranslate(Vector3(0, 0, 0)); // start
  track->createNodeKeyFrame(duration)->setTranslate(offset);    // end

  auto* animState = m_sceneMngr->createAnimationState(anim->getName());
  animState->setEnabled(true);
  animState->setLoop(false);
  // store a pointer reference to the new state (owned by SceneManager)
  m_animations.push_back(animState);

  // handle a captured piece (already piece at dest cell)
  if (m_positions.contains(to)) {
    auto captured = m_positions.extract(to).mapped(); // pops from the mapping
    Vector3 exitOffset = sidelines - captured->getPosition();
    exitOffset.y = 0;                                      // only move in xz plane
    auto exitDuration = exitOffset.length() / (speed * 2); // constant speed
    auto endTime = duration + exitDuration;
    captured->setInitialState();

    Animation* exitAnim = m_sceneMngr->createAnimation(to + "captured", endTime);
    auto* exitTrack = exitAnim->createNodeTrack(0, captured);

    exitTrack->createNodeKeyFrame(duration)->setTranslate(Vector3(0, 0, 0));
    // wait until first animation complete
    exitTrack->createNodeKeyFrame(duration + exitDuration / 2)->setTranslate(Vector3(exitOffset.x / 2, 3, exitOffset.y / 2));
    // wait until first animation complete
    exitTrack->createNodeKeyFrame(endTime)->setTranslate(exitOffset); // move to side line

    auto* exitState = m_sceneMngr->createAnimationState(exitAnim->getName());
    exitState->setEnabled(true);
    exitState->setLoop(false);
    // store a pointer reference to the new state (owned by SceneManager)
    m_animations.push_back(exitState);

    sidelines.x -= 1;
  }

  // save the new piece position now (visually only true after animation finishes)
  m_positions.insert_or_assign(to, piece);
}

// InputLisener override
bool App::keyPressed(const OgreBites::KeyboardEvent& evt) {

  using namespace OgreBites;
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
    return true;
  } else if (evt.keysym.sym == SDLK_F2) {
    m_ui->toggleShow();
    return true;
  } else if (evt.keysym.sym == SDLK_F12) {
    m_ui->toggleDebug();
    return true;
  }

  return false;
}

inline Ogre::Vector3 chess3d::calcDestOffset(const std::string& from, const std::string& to) {
  // from/to are chess positions e.g. a1, c8, h4, etc.
  auto colOffset = to[0] - from[0];
  auto rowOffset = to[1] - from[1];
  // this exploits the fact that I ensured the chess cells were unit length
  // in the mode. e.g. 1 cell is 1x1 in size. So distance between cells is easy.
  return Ogre::Vector3(rowOffset, 0, colOffset);
}
