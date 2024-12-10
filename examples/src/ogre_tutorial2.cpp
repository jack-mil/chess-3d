// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

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

//! [starter]

#include <exception>
#include <iostream>

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include "OgreAdvancedRenderControls.h"

using namespace Ogre;
using namespace OgreBites;

class TutorialApplication
    : public ApplicationContext,
      public InputListener {
public:
  TutorialApplication();
  virtual ~TutorialApplication();

  void setup();
  void loadResources();
  bool keyPressed(const KeyboardEvent& evt);

private:
  TrayManager* mTrayMgr;
  CameraMan* mCamMan;
  AdvancedRenderControls* mCtrls;
};

TutorialApplication::TutorialApplication()
    : ApplicationContext("OgreTutorialApp")
{
}

TutorialApplication::~TutorialApplication()
{
  delete mCtrls;
  delete mCamMan;
  delete mTrayMgr;
}

void TutorialApplication::loadResources()
{
  enableShaderCache();

  // load essential resources for trays/ loading bar
  ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
  createDummyScene();
  mTrayMgr = new OgreBites::TrayManager("Iterface", getRenderWindow());
  addInputListener(mTrayMgr);

  // show loading progress
  mTrayMgr->showLoadingBar(1, 0);
  ApplicationContext::loadResources();

  // # clean up
  mTrayMgr->hideLoadingBar();
  destroyDummyScene();
}

void TutorialApplication::setup()
{
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
  mTrayMgr->toggleAdvancedFrameStats();
  mTrayMgr->refreshCursor();

  
  // -- tutorial section start --
  //! [cameracreate]
  SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  Camera* cam = scnMgr->createCamera("myCam");
  //! [cameracreate]


  // camera manager (movement) from python example
  mCamMan = new CameraMan(camNode);
  mCamMan->setStyle(CS_ORBIT);
  // mCamMan->setYawPitchDist(Radian(0), Radian(0.3), 100);
  addInputListener(mCamMan);

  //! [cameraposition]
  camNode->setPosition(200, 300, 400);
  camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
  //! [cameraposition]

  //! [cameralaststep]
  cam->setNearClipDistance(5);
  cam->setAutoAspectRatio(true);
  camNode->attachObject(cam);
  //! [cameralaststep]

  // Extra debug controls
  // see: https://ogrecave.github.io/ogre/api/latest/class_ogre_bites_1_1_advanced_render_controls.html
  mCtrls = new AdvancedRenderControls(mTrayMgr, cam);
  addInputListener(mCtrls);

  //! [addviewport]
  Viewport* vp = getRenderWindow()->addViewport(cam);
  //! [addviewport]

  //! [viewportback]
  vp->setBackgroundColour(ColourValue(0.3, 0, 0.3));
  //! [viewportback]

  //! [cameraratio]
  // cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
  //! [cameraratio]

  //! [lightingsset]
  scnMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
  scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
  //! [lightingsset]

  //! [ninja]
  Entity* ninjaEntity = scnMgr->createEntity("ninja.mesh");
  ninjaEntity->setCastShadows(true);

  scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);
  //! [ninja]

  //! [plane]
  Plane plane(Vector3::UNIT_Y, 0);
  //! [plane]

  //! [planedefine]
  MeshManager::getSingleton().createPlane(
      "ground", RGN_DEFAULT,
      plane,
      1500, 1500, 20, 20,
      true,
      1, 5, 5,
      Vector3::UNIT_Z);
  //! [planedefine]

  //! [planecreate]
  Entity* groundEntity = scnMgr->createEntity("ground");
  scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
  //! [planecreate]

  //! [planenoshadow]
  groundEntity->setCastShadows(false);
  //! [planenoshadow]

  //! [planesetmat]
  groundEntity->setMaterialName("Examples/Rockwall");
  //! [planesetmat]

  //! [spotlight]
  Light* spotLight = scnMgr->createLight("SpotLight");
  //! [spotlight]

  //! [spotlightcolor]
  spotLight->setDiffuseColour(0, 0, 1.0);
  spotLight->setSpecularColour(0, 0, 1.0);
  //! [spotlightcolor]

  //! [spotlighttype]
  spotLight->setType(Light::LT_SPOTLIGHT);
  //! [spotlighttype]

  //! [spotlightposrot]
  SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  spotLightNode->attachObject(spotLight);
  spotLightNode->setDirection(-1, -1, 0);
  spotLightNode->setPosition(Vector3(200, 200, 0));
  //! [spotlightposrot]

  //! [spotlightrange]
  spotLight->setSpotlightRange(Degree(35), Degree(50));
  //! [spotlightrange]

  //! [directlight]
  Light* directionalLight = scnMgr->createLight("DirectionalLight");
  directionalLight->setType(Light::LT_DIRECTIONAL);
  //! [directlight]

  //! [directlightcolor]
  directionalLight->setDiffuseColour(ColourValue(0.4, 0, 0));
  directionalLight->setSpecularColour(ColourValue(0.4, 0, 0));
  //! [directlightcolor]

  //! [directlightdir]
  SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  directionalLightNode->attachObject(directionalLight);
  directionalLightNode->setDirection(Vector3(0, -1, 1));
  //! [directlightdir]

  //! [pointlight]
  Light* pointLight = scnMgr->createLight("PointLight");
  pointLight->setType(Light::LT_POINT);
  //! [pointlight]

  //! [pointlightcolor]
  pointLight->setDiffuseColour(0.3, 0.3, 0.3);
  pointLight->setSpecularColour(0.3, 0.3, 0.3);
  //! [pointlightcolor]

  //! [pointlightpos]
  SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  pointLightNode->attachObject(pointLight);
  pointLightNode->setPosition(Vector3(0, 150, 250));
  //! [pointlightpos]
  // -- tutorial section end --
}

bool TutorialApplication::keyPressed(const KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  }
  return true;
}

int main(/* int argc, char **argv */)
{
  try {
    TutorialApplication app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
  }
  catch (const std::exception& e) {
    std::cerr << "Error occurred during execution: " << e.what() << '\n';
    return 1;
  }

  return 0;
}

//! [starter]
