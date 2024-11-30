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

  // overlay/ trays
  scnMgr->addRenderQueueListener(getOverlaySystem());
  mTrayMgr->showFrameStats(TrayLocation::TL_TOPLEFT);
  mTrayMgr->hideCursor();
  mTrayMgr->toggleAdvancedFrameStats();
  mTrayMgr->refreshCursor();

  
  // -- tutorial section start --
  SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  Camera* cam = scnMgr->createCamera("myCam");


  // camera manager (movement) from python example
  mCamMan = new CameraMan(camNode);
  mCamMan->setStyle(CS_ORBIT);
  // mCamMan->setYawPitchDist(Radian(0), Radian(0.3), 100);
  addInputListener(mCamMan);

  camNode->setPosition(0, 8, 12);
  camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);

  cam->setNearClipDistance(5);
  cam->setAutoAspectRatio(true);
  camNode->attachObject(cam);

  // Extra debug controls
  // see: https://ogrecave.github.io/ogre/api/latest/class_ogre_bites_1_1_advanced_render_controls.html
  mCtrls = new AdvancedRenderControls(mTrayMgr, cam);
  addInputListener(mCtrls); // takes ownership

  Viewport* vp = getRenderWindow()->addViewport(cam);

  vp->setBackgroundColour(ColourValue(0.3, 0, 0.3));

  // cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

  scnMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
  scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

  auto sceneOrigin = scnMgr->getRootSceneNode()->createChildSceneNode();

  // load the entire chess scene and all objects (exported from Blender)
  sceneOrigin->loadChildren("chess.scene");

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

  // groundEntity->setMaterialName("Examples/Rockwall");

  // Light* spotLight = scnMgr->createLight("SpotLight");

  // spotLight->setDiffuseColour(0, 0, 1.0);
  // spotLight->setSpecularColour(0, 0, 1.0);

  // spotLight->setType(Light::LT_SPOTLIGHT);

  // SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // spotLightNode->attachObject(spotLight);
  // spotLightNode->setDirection(-1, -1, 0);
  // spotLightNode->setPosition(Vector3(200, 200, 0));

  // spotLight->setSpotlightRange(Degree(35), Degree(50));

  // Light* directionalLight = scnMgr->createLight("DirectionalLight");
  // directionalLight->setType(Light::LT_DIRECTIONAL);

  // directionalLight->setDiffuseColour(ColourValue(0.4, 0, 0));
  // directionalLight->setSpecularColour(ColourValue(0.4, 0, 0));

  // SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // directionalLightNode->attachObject(directionalLight);
  // directionalLightNode->setDirection(Vector3(0, -1, 1));

  // Light* pointLight = scnMgr->createLight("PointLight");
  // pointLight->setType(Light::LT_POINT);

  // pointLight->setDiffuseColour(0.3, 0.3, 0.3);
  // pointLight->setSpecularColour(0.3, 0.3, 0.3);

  // SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  // pointLightNode->attachObject(pointLight);
  // pointLightNode->setPosition(Vector3(0, 150, 250));
  // -- tutorial section end --
  std::cout << "Setting up Done\n";
}

bool ChessApplication::keyPressed(const KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  }
  return true;
}
