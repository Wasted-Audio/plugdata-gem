/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    camera control gem object
    tigital@mac.com

    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MANIPS_CAMERA_H_
#define _INCLUDE__GEM_MANIPS_CAMERA_H_

#include "Base/GemBase.h"
#include <stdlib.h>
#include <math.h>

#include "Utils/Vector.h"

#define PI      3.1415926535897932384626433832795

#ifndef _WIN32
using namespace std;
#endif


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    camera

    camera gem object

DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN camera : public GemBase
//class GEM_EXTERN camera : public CPPExtern
{
  CPPEXTERN_HEADER(camera, GemBase);
  //CPPEXTERN_HEADER(camera, CPPExtern);

public:

  //////////
  // Constructor
  camera(int argc, t_atom *argv);
  //CCamera       g_Camera;

protected:

  //////////
  // Destructor
  virtual ~camera();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  //////////
  // Angles changed
  void            hAngleMess(t_float val);
  void            vAngleMess(t_float val);
  void            distanceMess(t_float val);

  void            resetState();

  bool    left, right, up, down, forward, reverse, m_mode;
  float   m_speed;
  t_float hAngle, vAngle, distance;

  // This changes the position, view, and up vector of the camera.
  // (Used for initialization)
  void PositionCamera(float positionX, float positionY, float positionZ,
                      float viewX,     float viewY,     float viewZ,
                      float upVectorX, float upVectorY, float upVectorZ);

  // This rotates the camera's view around the position using axis-angle rotation
  void RotateView(float angle, float x, float y, float z);

  // This will move the camera forward or backward depending on the speed
  void MoveCamera( float speed );
  void SlideCamera( float speed );
  void calcCameraVals( void );
  void calcUpVector( void );
  void incHRot( float val );
  void decHRot( float val );
  void incVRot( float val );
  void decVRot( float val );


private:

  CVector3 m_vPosition;                   // The camera's position
  CVector3 m_vView;                       // The camera's View
  CVector3 m_vUpVector;                   // The camera's UpVector
  CVector3 m_vSlide;                      // The camera's slide

  //////////
  // static member functions
  static void     hAngleMessCallback(void *data, t_float val);
  static void     vAngleMessCallback(void *data, t_float val);
  static void     distanceMessCallback(void *data, t_float val);
  static void     speedMessCallback(void *data, t_float val);
  static void     forwardMessCallback(void *data, t_float val);
  static void     reverseMessCallback(void *data, t_float val);
  static void     leftMessCallback(void *data, t_float val);
  static void     rightMessCallback(void *data, t_float val);
  static void     upMessCallback(void *data, t_float val);
  static void     downMessCallback(void *data, t_float val);
  static void     resetMessCallback(void *);
  static void     modeMessCallback(void *data, t_float val);
  static void     lookXMessCallback(void *data, t_float val);
  static void     lookYMessCallback(void *data, t_float val);
  static void     lookZMessCallback(void *data, t_float val);
};

#endif  // for header file
