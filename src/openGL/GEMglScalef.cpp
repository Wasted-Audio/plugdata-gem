////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//      zmoelnig@iem.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglScalef.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglScalef, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglScalef :: GEMglScalef      (t_floatarg arg0, t_floatarg arg1,
                                 t_floatarg arg2) :
  x(static_cast<GLfloat>(arg0)),
  y(static_cast<GLfloat>(arg1)),
  z(static_cast<GLfloat>(arg2))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("x"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("y"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("z"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglScalef :: ~GEMglScalef ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglScalef :: render(GemState *state)
{
  glScalef (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglScalef :: xMess (t_float arg1)        // FUN
{
  x = static_cast<GLfloat>(arg1);
  setModified();
}

void GEMglScalef :: yMess (t_float arg1)        // FUN
{
  y = static_cast<GLfloat>(arg1);
  setModified();
}

void GEMglScalef :: zMess (t_float arg1)        // FUN
{
  z = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglScalef :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScalef::xMessCallback),     gensym("x"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScalef::yMessCallback),     gensym("y"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScalef::zMessCallback),     gensym("z"),
                  A_DEFFLOAT, A_NULL);
};

void GEMglScalef :: xMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglScalef :: yMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
void GEMglScalef :: zMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->zMess ( static_cast<t_float>(arg0));
}
