#pragma once

#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRenderTargetListener.h"

// forward declarations
namespace OgreBites {
class CameraMan;
class TrayManager;
class AdvancedRenderControls;
}

namespace chess3d {
class Overlay;
class ChessApplication
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener,
      public Ogre::RenderTargetListener {
public:
  ChessApplication();
  virtual ~ChessApplication();
  void exec();

private:
  void setup() override;
  void loadResources() override;
  bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override;

  void controlLightPosition();

  OgreBites::TrayManager* mTrayMgr = nullptr;
  OgreBites::CameraMan* mCamMan = nullptr;
  Overlay* m_ui = nullptr;
  OgreBites::AdvancedRenderControls* mCtrls = nullptr;
};
} // namespace chess3d
