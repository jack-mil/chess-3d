#include <OgreFrameListener.h>

struct ChessDialog : Ogre::FrameListener {
  Ogre::String nextRenderer;
  bool saveConfig;

  bool frameStarted(const Ogre::FrameEvent&) override;
};
