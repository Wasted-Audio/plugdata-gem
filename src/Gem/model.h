/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  representation of a 3D-model

  this is heavily informed by assimp's aiScene structure
  (but somewhat simplified).
  **LATER** we might extend it to include bone & animation data

  Copyright (c) 2023 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MODEL_H_
#define _INCLUDE__GEM_MODEL_H_

#include "Gem/VertexBuffer.h"
#include <vector>


namespace gem
{
  namespace plugins {
    class modelloader;
  };
  /* holds a model and renders it */
  class modelGL {
  public:
    /* ctor
     * initialize the model
     */
    modelGL(gem::plugins::modelloader&loader);
    /* update data */
    bool update(void);
    /* render the model in the current openGL context */
    void render(void);
    /* render specified meshes of the model in the currrent openGL context */
    void render(std::vector<unsigned int>&meshes);

    /* influence the rendering */
    void setDrawType(unsigned int);
    void useMaterial(bool);
    void setTexture(float w, float h);
 private:
    class PIMPL;
    PIMPL*m_pimpl;
  };
}; /* namespace gem */

#endif /* _INCLUDE__GEM_MODEL_H_ */
