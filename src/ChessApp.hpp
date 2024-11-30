#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRenderTargetListener.h"

namespace OgreBites {
class CameraMan;
}

class ChessApplication
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener,
      public Ogre::RenderTargetListener {
public:
  ChessApplication();
  virtual ~ChessApplication();

  void setup() override;
  void loadResources() override;
  bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override;

  void exec();

private:
  // TrayManager* mTrayMgr = nullptr;
  OgreBites::CameraMan* mCamMan = nullptr;
  // AdvancedRenderControls* mCtrls = nullptr;
};
