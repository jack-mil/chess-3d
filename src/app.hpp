#pragma once

#include <list>

#include "OgreApplicationContext.h"
#include "OgrePrerequisites.h"
#include "OgreRenderTargetListener.h"
#include "OgreInput.h"

// forward declaration
namespace OgreBites { class CameraMan; }

namespace chess3d {
class Overlay; // forward declaration

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
  bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;             // from FrameListener (parent of ApplicationContext)
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override; // from RenderTargetListener

  void lightPropOverlay();
  void movePiece(int id);

  Ogre::SceneManager* m_sceneMngr;  // non-owning
  std::list<Ogre::AnimationState*> m_animations; // non-owning

  OgreBites::InputListenerChain m_inputChain;

  // std::unique_ptr<OgreBites::TrayManager> m_trayMgr;
  std::unique_ptr<OgreBites::CameraMan> m_camMgr;
  // std::unique_ptr<OgreBites::AdvancedRenderControls> m_ctrls;

  std::unique_ptr<Overlay> m_ui;
};
} // namespace chess3d
