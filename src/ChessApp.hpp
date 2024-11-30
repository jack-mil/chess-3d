#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRenderTargetListener.h"

// forward declarations
namespace OgreBites {
class CameraMan;
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

  void setup() override;
  void loadResources() override;
  bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override;

  void exec();

private:
  // TrayManager* mTrayMgr = nullptr;
  OgreBites::CameraMan* mCamMan = nullptr;
  Overlay* m_ui = nullptr;
  // AdvancedRenderControls* mCtrls = nullptr;
};
} // namespace chess3d
