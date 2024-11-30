#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include "OgreImGuiOverlay.h"
#include "OgreImGuiInputListener.h"
#include "OgreAdvancedRenderControls.h"

using namespace Ogre;
using namespace OgreBites;

class ChessApplication
    : public ApplicationContext,
      public InputListener {
public:
  ChessApplication();
  virtual ~ChessApplication();

  void setup();
  void loadResources();
  bool keyPressed(const KeyboardEvent& evt);

private:
  TrayManager* mTrayMgr = nullptr;
  CameraMan* mCamMan = nullptr;
  AdvancedRenderControls* mCtrls = nullptr;
};
