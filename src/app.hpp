#pragma once

#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRenderTargetListener.h"

// forward declarations
namespace OgreBites {
class CameraMan;
class TrayManager;
class AdvancedRenderControls;
} // namespace OgreBites

namespace chess3d {
// forward declaration
class Overlay;

class App
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener,
      public Ogre::RenderTargetListener {
public:
  App();
  virtual ~App();
  void exec();

private:
  void setup() override;                                                       // from ApplicationContext
  void loadResources() override;                                               // from ApplicationContext
  bool keyPressed(const OgreBites::KeyboardEvent& evt) override;               // from InputListener
  void frameRendered(const Ogre::FrameEvent& evt) override;                    // from InputListener
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override; // from RenderTargetListener

  void controlLightPosition();

  OgreBites::TrayManager* m_trayMgr = nullptr;
  OgreBites::CameraMan* m_camMgr = nullptr;
  OgreBites::AdvancedRenderControls* m_ctrls = nullptr;

  Overlay* m_ui = nullptr;
};
} // namespace chess3d
