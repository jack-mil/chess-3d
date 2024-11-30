#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include "OgreAdvancedRenderControls.h"
#include "OgreImGuiOverlay.h"


using namespace Ogre;
using namespace OgreBites;

class ChessApplication
    : public ApplicationContext,
      public InputListener,
      public RenderTargetListener {
public:
  ChessApplication();
  virtual ~ChessApplication();

  void setup() override;
  void loadResources() override;
  bool keyPressed(const KeyboardEvent& evt) override;
  void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override;

private:
  TrayManager* mTrayMgr = nullptr;
  CameraMan* mCamMan = nullptr;
  AdvancedRenderControls* mCtrls = nullptr;
};
