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

#include <exception>
#include <iostream>

#include "OgreApp.hpp"

using namespace Ogre;
using namespace OgreBites;

ChessApplication::ChessApplication()
    : ApplicationContext("Chess 3D!")
{
  std::cout << "Constructed\n";
}

ChessApplication::~ChessApplication()
{
  std::cout << "Trying destory\n";
  // std::cout << "Deleting" << mCtrls << "\n";
  // delete mCtrls;
  // std::cout << "Deleting" << mTrayMgr << "\n";
  // delete mTrayMgr;
  delete mCamMan;
  std::cout << "Destructed\n";
}

void ChessApplication::loadResources()
{
  std::cout << "Loading Resources\n";
  enableShaderCache();

  auto resourceMgr = ResourceGroupManager::getSingletonPtr();

  // load essential resources for trays/ loading bar
  resourceMgr->initialiseResourceGroup("Essential");
  createDummyScene();
  mTrayMgr = new OgreBites::TrayManager("Iterface", getRenderWindow());
  addInputListener(mTrayMgr); // takes ownership

  // show loading progress
  mTrayMgr->showLoadingBar(1, 0);
  resourceMgr->initialiseResourceGroup("OgreInternal"); // for shadow lighting...?
  resourceMgr->addResourceLocation("./resources", "FileSystem", "ChessGroup");
  resourceMgr->initialiseResourceGroup("General");
  resourceMgr->initialiseResourceGroup("ChessGroup");
  resourceMgr->setWorldResourceGroupName("ChessGroup");

  // # clean up
  mTrayMgr->hideLoadingBar();
  destroyDummyScene();
  std::cout << "Loading Resources Done\n";
}

void ChessApplication::setup()
{
  std::cout << "Settings up\n";
  // do not forget to call the base first
  ApplicationContext::setup();
  addInputListener(this);

  // get a pointer to the already created root
  Root* root = getRoot();
  SceneManager* scnMgr = root->createSceneManager();

  // register our scene with the RTSS
  RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr); // must be done before we do anything with the scene

  // load the entire chess scene and all objects (exported from Blender)
  auto sceneOrigin = scnMgr->getRootSceneNode()->createChildSceneNode();
  sceneOrigin->loadChildren("chess.scene");

  // overlay/ trays
  scnMgr->addRenderQueueListener(getOverlaySystem());
  mTrayMgr->showFrameStats(TrayLocation::TL_TOPLEFT);
  mTrayMgr->hideCursor();
  mTrayMgr->toggleAdvancedFrameStats();
  mTrayMgr->refreshCursor();

  // attach the user input controls to the camera node
  // (named node loaded from the .scene)
  // camera manager (movement) from python example
  SceneNode* camNode = scnMgr->getSceneNode("cam1");
  mCamMan = new CameraMan(camNode);
  mCamMan->setStyle(CS_ORBIT);
  addInputListener(mCamMan);
  // Extra debug controls
  // see: https://ogrecave.github.io/ogre/api/latest/class_ogre_bites_1_1_advanced_render_controls.html
  Camera* cam = scnMgr->getCamera("cam1");
  cam->setAutoAspectRatio(true);
  
  mCtrls = new AdvancedRenderControls(mTrayMgr, cam);
  addInputListener(mCtrls); // takes ownership

  // Set render target to this camera output
  Viewport* vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));
  // mCamMan->setYawPitchDist(Radian(0), Radian(0.3), 100);
  // SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // Camera* cam = scnMgr->createCamera("myCam");

  // camNode->setPosition(0, 8, 12);
  // camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);

  // cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
  // cam->setNearClipDistance(5);
  // camNode->attachObject(cam);

  scnMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
  scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

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

bool ChessApplication::keyPressed(const KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  }
  return true;
}
