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

  OgreBites::InputListenerChain m_inputChain;
  
  // std::unique_ptr<OgreBites::TrayManager> m_trayMgr;
  std::unique_ptr<OgreBites::CameraMan> m_camMgr;
  // std::unique_ptr<OgreBites::AdvancedRenderControls> m_ctrls;

  std::unique_ptr<Overlay> m_ui;
};
} // namespace chess3d
