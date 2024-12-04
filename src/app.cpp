// https://github.com/OGRECave/ogre/blob/master/Samples/Tutorials/BasicTutorial2.cpp
// With addition from the python example @
// https://github.com/OGRECave/ogre/blob/v1.11.1/Samples/Python/bites_sample.py#L23

/* Controls:

    LMB: Orbit Camera
    RMB: Pan Camera

    F: Toggle frame rate stats on/off
    G: Toggle advanced frame stats on/off
    P Toggle profiler window on/ off
    R: Render mode
        Wireframe
        Points
        Solid
    T: Cycle texture filtering
        Bilinear
        Trilinear
        Anisotropic(8)
        None
    F2: RTSS: Set the main viewport material scheme to default material manager scheme.
    F3: RTSS: Toggle default shader generator lighting model from per vertex to per pixel.
    F4: RTSS: Switch vertex shader outputs compaction policy.
    F5: Reload all textures
    F6: Take a screenshot


*/

// #include <exception>
#include <iostream>

#include "OgreAdvancedRenderControls.h"
#include "OgreCameraMan.h"
#include "OgreImGuiOverlay.h"
#include "OgreOverlaySystem.h"
#include "OgreRTShaderSystem.h"
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreTrays.h"

#include "app.hpp"
#include "overlay.hpp"
#include "theme.hpp"

using namespace chess3d;

App::App()
    : OgreBites::ApplicationContext("Chess 3D!"),
      m_ui(new Overlay()) {
  std::cout << "Constructed\n";
}

// for some reason this is required
App::~App() {
  //   std::cout << "Trying destory\n";
  // //   // std::cout << "Deleting" << m_ctrls << "\n";
  // //   // delete m_ctrls;
  // //   // std::cout << "Deleting" << m_trayMgr << "\n";
  // //   // delete m_trayMgr;
  // //   // delete m_camMgr;
  //   std::cout << "Destructed\n";
}

void App::exec() {
  this->initApp();
  this->getRoot()->startRendering();
  this->closeApp();
}

void App::loadResources() {
  std::cout << "Loading Resources\n";
  enableShaderCache();

  auto resourceMgr = Ogre::ResourceGroupManager::getSingletonPtr();

  // load essential resources for trays/ loading bar
  // createDummyScene();
  resourceMgr->initialiseResourceGroup("Essential");
  // m_trayMgr = std::make_unique<OgreBites::TrayManager>("Iterface", getRenderWindow());
  // m_trayMgr->toggleAdvancedFrameStats();
  // m_trayMgr->hideCursor();

  // show loading progress
  // m_trayMgr->showLoadingBar(3, 3);
  resourceMgr->initialiseResourceGroup("OgreInternal"); // for shadow lighting...?
  resourceMgr->addResourceLocation("./resources", "FileSystem", "ChessGroup");
  resourceMgr->initialiseResourceGroup("General");
  resourceMgr->initialiseResourceGroup("ChessGroup");
  resourceMgr->setWorldResourceGroupName("ChessGroup");

  // # clean up
  // m_trayMgr->hideLoadingBar();
  // destroyDummyScene();
  std::cout << "Loading Resources Done\n";
}

void App::setup() {
  using namespace Ogre;

  std::cout << "Setting up\n";
  // do not forget to call the base first
  OgreBites::ApplicationContext::setup();

  // get a pointer to the already created root
  Root* root = getRoot();
  m_sceneMngr = root->createSceneManager(Ogre::SMT_DEFAULT, "chessScene");

  // register our scene with the RTSS
  RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  // shadergen->getRenderState(Ogre::MSN_SHADERGEN);
  shadergen->addSceneManager(m_sceneMngr); // must be done before we do anything with the scene
  m_sceneMngr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);

  // load the entire chess scene and all objects (exported from Blender)
  auto sceneOrigin = m_sceneMngr->getRootSceneNode()->createChildSceneNode();
  sceneOrigin->loadChildren("chess.scene");

  // Set render target to this camera output
  Camera* cam = m_sceneMngr->getCamera("camera1");
  cam->setAutoAspectRatio(true);
  Viewport* vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

  // overlay/ trays

  // imgui overlay
  // float vpScale = getDisplayDPI()/96;
  // Ogre::OverlayManager::getSingleton().setPixelRatio(vpScale);
  m_sceneMngr->addRenderQueueListener(getOverlaySystem());
  auto imGUIOverlay = initialiseImGui();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  chess3d::setModernColors(ImGui::GetStyle());
  // ImGui::GetIO().FontGlobalScale = std::round(vpScale);
  imGUIOverlay->show();
  getRenderWindow()->addListener(this);
  // getRoot()->addFrameListener(&mChessDialog);

  // attach the user input controls to the camera node
  // (named node loaded from the .scene)
  // camera manager (movement) from python example


  SceneNode* camNode = m_sceneMngr->getSceneNode("camera1");
  // SceneNode* boardNode = scnMgr->getSceneNode("board");
  // const auto& pos = camNode->getPosition();
  // const auto& dist = (camNode->getPosition() - boardNode->_getDerivedPosition()).length();
  // const auto& q = camNode->getOrientation();
  m_camMgr = std::make_unique<OgreBites::CameraMan>(camNode);
  // m_camMgr->setTarget(boardNode);
  m_camMgr->setStyle(OgreBites::CS_ORBIT);
  // camNode->setPosition(pos);
  // camNode->setOrientation(q);

  // m_camMgr->setFixedYaw(false);

  // Extra debug controls
  // see: https://ogrecave.github.io/ogre/api/latest/class_ogre_bites_1_1_advanced_render_controls.html
  // m_ctrls = std::make_unique<OgreBites::AdvancedRenderControls>(m_trayMgr.get(), cam);
  // Add the input listeners in order
  m_inputChain = OgreBites::InputListenerChain({this, /* m_ctrls.get(), m_trayMgr.get(), */ getImGuiInputListener(), m_camMgr.get()});
  addInputListener(&m_inputChain);


  // m_camMgr->setYawPitchDist(Radian(0), Radian(0.3), 100);
  // SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // Camera* cam = scnMgr->createCamera("myCam");

  // camNode->setPosition(0, 8, 12);
  // camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);

  // cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
  // cam->setNearClipDistance(5);
  // camNode->attachObject(cam);

  // m_sceneMngr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));


  // Entity* ninjaEntity = scnMgr->createEntity("ninja.mesh");
  // ninjaEntity->setCastShadows(true);

  // scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

  // Plane plane(Vector3::UNIT_Y, 0);

  // MeshManager::getSingleton().createPlane(
  //     "ground", RGN_DEFAULT,
  //     plane,
  //     1500, 1500, 20, 20,
  //     true,
  //     1, 5, 5,
  //     Vector3::UNIT_Z);

  // Entity* groundEntity = scnMgr->createEntity("ground");
  // scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

  // groundEntity->setCastShadows(false);

  // auto* boardEntity = scnMgr->getEntity("board");
  // boardEntity->setMaterialName("Examples/Rockwall");

  // Light* spotLight = scnMgr->createLight("spot2");

  // spotLight->setDiffuseColour(0.9, 0.9, 0.9);
  // spotLight->setSpecularColour(0.9, 0.90, 0.9);

  // spotLight->setType(Light::LT_SPOTLIGHT);
  // spotLight->setSpotlightRange(Degree(90), Degree(110));

  // SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // spotLightNode->attachObject(spotLight);
  // spotLightNode->setDirection(0, -1, 0);
  // spotLightNode->setPosition(Vector3(0, 5.75, 0));

  // Light* area1 = scnMgr->createLight("DirectionalLight");
  // area1->setType(Light::LT_RECTLIGHT);

  // // area1->setSourceSize()

  // // directionalLight->setDiffuseColour(ColourValue(0.4, 0, 0));
  // // directionalLight->setSpecularColour(ColourValue(0.4, 0, 0));

  // SceneNode* area1Node = scnMgr->getRootSceneNode()->createChildSceneNode();
  // area1Node->attachObject(area1);
  // area1Node->setDirection(Vector3(0, -1, 1));

  // Light* pointLight = scnMgr->createLight("PointLight");
  // pointLight->setType(Light::LT_POINT);

  // pointLight->setDiffuseColour(0.3, 0.3, 0.3);
  // pointLight->setSpecularColour(0.3, 0.3, 0.3);

  // SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // pointLightNode->attachObject(pointLight);
  // pointLightNode->setPosition(Vector3(11, 8.6, 0.295));

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

  controlLightPosition();
}

void App::controlLightPosition() {
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
    lightNode->lookAt(board->getPosition(), Node::TS_WORLD);

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
    - from: FrameListener override
*/
bool App::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  // ensure super-class gets called (calls frameRendered on all InputListeners)
  OgreBites::ApplicationContext::frameRenderingQueued(evt);
  // can use evt.timeSinceLastFrame for time based updates,
  // or do fixed rate loop of some sort
  // need to update the chess pieces (over time visually?) here.
  // see: Samples/Character/include/SinbadCharacterController.h
  // updateAnimations(evt.timeSinceLastFrame);
  // updateBody(evt.timeSinceLastFrame);
  // updateCamera(evt.timeSinceLastFrame);
  auto pawn7 = m_sceneMngr->getSceneNode("pawn.w.7");
  static constexpr float unitsPerSecond = 0.5;
  auto displ = evt.timeSinceLastFrame * unitsPerSecond;
  pawn7->translate(displ, 0, -displ); // move in the +y direction
  return true; // always want to return true, otherwise rendering stops
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
  } else if (evt.keysym.sym == 'f') {
    // if (m_trayMgr->areFrameStatsVisible()) {
    //   m_trayMgr->hideFrameStats();
    // } else {
    //   m_trayMgr->showFrameStats(TrayLocation::TL_TOPRIGHT);
    // }

    // m_ctrls->buttonPressed
    return true;
  }

  return false;
}
