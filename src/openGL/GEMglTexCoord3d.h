/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 *     zmoelnig@iem.at
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 *
 *  this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD3D_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD3D_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord3d
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord3d( GLdouble s, GLdouble t, GLdouble r)"
 */

class GEM_EXTERN GEMglTexCoord3d : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord3d, GemGLBase);

public:
  // Constructor
  GEMglTexCoord3d (t_float, t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglTexCoord3d ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLdouble      s;              // VAR
  virtual void  sMess(t_float); // FUN

  GLdouble      t;              // VAR
  virtual void  tMess(t_float); // FUN

  GLdouble      r;              // VAR
  virtual void  rMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    sMessCallback (void*, t_float);
  static void    tMessCallback (void*, t_float);
  static void    rMessCallback (void*, t_float);
};
#endif // for header file
