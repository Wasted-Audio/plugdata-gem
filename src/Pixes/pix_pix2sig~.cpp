////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2000 Guenter Geiger geiger@epy.co.at
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_pix2sig~.h"
#include "Gem/State.h"

CPPEXTERN_NEW_NAMED(pix_pix2sig, "pix_pix2sig~");

/////////////////////////////////////////////////////////
//
// pix_pix2sig
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_pix2sig :: pix_pix2sig(void)
  : m_fillType(CLEAR)
  , m_offsetX(0), m_offsetY(0)
  , m_line(0)
{
  int i=4;
  while(i--) {
    o_col[i]=outlet_new(this->x_obj, &s_signal);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_pix2sig :: ~pix_pix2sig()
{
  int i=4;
  while(i--) {
    outlet_free(o_col[i]);
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// processImage
/////////////////////////////////////////////////////////
void pix_pix2sig :: render(GemState *state)
{
  pixBlock*img=NULL;
  if(state) {
    state->get(GemState::_PIX, img);
  }
  if(img) {
    img->image.copy2ImageStruct(&m_image);
  }
}
void pix_pix2sig :: filltypeMess(t_symbol*s, int argc, t_atom*argv) {
  if(!argc || A_SYMBOL != argv->a_type) {
    error("usage: %s <type> [<params>]", s->s_name);
    return;
  }
  std::string type(atom_getsymbol(argv)->s_name);
  filltype_t filltype = INVALID;
  if (0) {
  } else if("clear" == type) {
    filltype = CLEAR;
  } else if("fill" == type) {
    filltype = FILL;
  } else if("line" == type) {
    filltype = (2 == argc && A_FLOAT == argv[1].a_type)?WATERFALL:LINE;
  } else if("waterfall" == type) {
    filltype = WATERFALL;
  }

  switch(filltype) {
  case INVALID:
    error("invalid %s '%s' (must be one of 'clear', 'fill', 'line' or 'waterfall')", s->s_name, type.c_str());
    return;
  case WATERFALL:
    switch(argc) {
    case 1:
      m_line = 0;
      break;
    case 2:
      m_line = atom_getfloat(argv+1);
      break;
    default:
      error("usage: %s %s [<line>]", s->s_name, type.c_str());
      return;
    }
    break;
  default:
    break;
  }

  m_offsetX = m_offsetY = 0;
  m_fillType = filltype;
}



/////////////////////////////////////////////////////////
// signal Performance
namespace {
  template<typename T>
  void perform_pix2sig(t_sample**out, size_t N, void*data_, size_t dataoffset, unsigned int format, t_sample scale, bool swap) {
    T*data = static_cast<T*>(data_);
    t_sample*out_red   = out[0];
    t_sample*out_green = out[1];
    t_sample*out_blue  = out[2];
    t_sample*out_alpha = out[3];
    size_t n = N;

    const int R = (swap)?3:0;
    const int G = (swap)?2:1;
    const int B = (swap)?1:2;
    const int A = (swap)?0:3;

    switch(format) {
    default:
    case GEM_RAW_RGBA:
      data += dataoffset*4;
      while(n--) {
        *(out_red  ++) = scale * static_cast<t_sample>(data[R]);
        *(out_green++) = scale * static_cast<t_sample>(data[G]);
        *(out_blue ++) = scale * static_cast<t_sample>(data[B]);
        *(out_alpha++) = scale * static_cast<t_sample>(data[A]);
        data+=4;
      }
      break;
    case GEM_RAW_BGRA:
      data += dataoffset*4;
      while(n--) {
        *(out_red  ++) = scale * static_cast<t_sample>(data[B]);
        *(out_green++) = scale * static_cast<t_sample>(data[G]);
        *(out_blue ++) = scale * static_cast<t_sample>(data[R]);
        *(out_alpha++) = scale * static_cast<t_sample>(data[A]);
        data+=4;
      }
      break;
    case GEM_YUV:
      data += dataoffset*2;
      n/=2;
      while(n--) {
        t_sample y0 = scale * static_cast<t_sample>(data[chY0]);
        t_sample y1 = scale * static_cast<t_sample>(data[chY1]);
        t_sample u  = scale * static_cast<t_sample>(data[chU]);
        t_sample v  = scale * static_cast<t_sample>(data[chV]);
        *(out_red  ++) = y0;
        *(out_red  ++) = y1;
        *(out_green++) = u;
        *(out_green++) = u;
        *(out_blue ++) = v;
        *(out_blue ++) = v;
        *(out_alpha++) = 1.;
        *(out_alpha++) = 1.;
        data+=4;
      }
      break;
    case GEM_GRAY:
      data += dataoffset*1;
      while(n--) {
        t_sample g = scale * static_cast<t_sample>(data[chGray]);
        *(out_red  ++) = g;
        *(out_green++) = g;
        *(out_blue ++) = g;
        *(out_alpha++) = 1.;
        data++;
      }
      break;
    }
    out[0] += N;
    out[1] += N;
    out[2] += N;
    out[3] += N;
  }
};



void pix_pix2sig :: perform(t_sample**out, size_t N)
{
  unsigned char* data = m_image.data;
  const size_t width = m_image.xsize;
  const size_t height = m_image.ysize;
  const size_t pixsize = width * height;
  t_sample*outsignal[] = {
    out[0], out[1], out[2], out[3]
  };

  /* metadata for the actual converters */
  typedef void (*performer_t)(t_sample**out, size_t N, void*data, size_t offset, unsigned int format, t_sample scale, bool swap);
  performer_t p2s_perform;
  t_sample scale = 1.;
  bool swap = false;

  size_t processed = 0; /* number of converted pixels */
  int line; /* linenumber in waterfall mode */

  if(!pixsize || !data)
    goto cleanup;

  line = m_line;
  if(line>0 && line>height)
    goto cleanup;
  if(line<0 && line<-height)
    goto cleanup;
  if(line<0)
    line = height + line;

  switch(m_image.type) {
  case GL_UNSIGNED_INT_8_8_8_8:
    swap = true;
    /* fallthrough */
  default:
    p2s_perform = perform_pix2sig<unsigned char>;
    scale = 1./255.;
    break;
  case GL_FLOAT:
    p2s_perform = perform_pix2sig<GLfloat>;
    scale = 1.;
    break;
  case GL_DOUBLE:
    p2s_perform = perform_pix2sig<GLdouble>;
    scale = 1.;
    break;
  }

  m_offsetX%=width;
  m_offsetY%=height;

  switch(m_fillType) {
  case CLEAR: /* flat (always from the beginning) */
    m_offsetX = m_offsetY = 0;
    /* fallthrough */
  case FILL:  /* flat (start where we left */
    if(m_offsetX) {
      /* fill the remainder of the current line */
      size_t count = N-processed;
      m_offsetX %= width;
      m_offsetY %= height;
      size_t r = m_image.upsidedown?m_offsetY:(height-m_offsetY);
      if ((m_offsetX + count) > width) count = (width - m_offsetX);
      p2s_perform(outsignal, count, data, r*width+m_offsetX, m_image.format, scale, swap);
      processed += count;
      m_offsetX = 0;
      m_offsetY = (m_offsetY+1)%height;
    }
    while ((processed < N) && (processed+width < pixsize)) {
      /* fill the lines */
      size_t r = m_offsetY;
      if(!m_image.upsidedown) r = height-r-1;
      size_t count = N-processed;
      if (count>width) count = width;
      p2s_perform(outsignal, count, data, r*width, m_image.format, scale, swap);
      processed += count;
      m_offsetY = (m_offsetY+1)%height;
    }
    /* fill the final (possibly truncated) line */
    if((processed < N) && processed < pixsize) {
        size_t r = m_offsetY;
        if(!m_image.upsidedown) r = height-r-1;
        size_t count = N-processed;
        if (count>width) count = width;
        p2s_perform(outsignal, count, data, r*width+m_offsetX, m_image.format, scale, swap);
        processed += count;
        m_offsetX = (count % width);
        m_offsetY += !m_offsetX;
      }
    break;
  case WATERFALL: /* linewise (always the same line) */
    m_offsetY = line;
    /* fallthrough */
  case LINE: /* linewise (start where we left) */
    m_offsetX = 0;
    m_offsetY %= height;
    /* fill the remainder of the current line */
    if(1) {
      size_t count = N;
      size_t r = m_offsetY;
      if (!m_image.upsidedown) r = height-r-1;
      if ((m_offsetX + count) > width) count = (width - m_offsetX);
      p2s_perform(outsignal, count, data, r*width+m_offsetX, m_image.format, scale, swap);
      processed += count;
      m_offsetX = 0;
      m_offsetY = (m_offsetY+1)%height;
    }
  }

 cleanup:
  if(processed < N) {
    t_sample*out_red   = outsignal[0];
    t_sample*out_green = outsignal[1];
    t_sample*out_blue  = outsignal[2];
    t_sample*out_alpha = outsignal[3];
    for(size_t remn=processed; remn<N; remn++) {
      *out_red++=*out_green++=*out_blue++=*out_alpha++=0;
    }
  }
}

void pix_pix2sig :: dspMess(t_signal** sp)
{
  struct DSPCallbackClass {
    static t_int* callback(t_int *w) {
      pix_pix2sig *x = GetMyClass((void*)w[1]);
      t_sample**signals = (t_sample**)w+2;
      int n = (int)(w[6]);
      if(n>0)
        x->perform(signals, n);
      return (w+7);
    }
  };
  DSPCallbackClass cb;

  dsp_add(cb.callback, 6, this->x_obj, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec,
          sp[3]->s_vec, sp[0]->s_n);
}

/////////////////////////////////////////////////////////
// Callback functions
//
/////////////////////////////////////////////////////////

void pix_pix2sig :: obj_setupCallback(t_class *classPtr)
{
  struct dspCallbackClass {
    static void callback(void*data, t_signal** sp) {
      GetMyClass(data)->dspMess(sp);
    }
  };
  dspCallbackClass dspCB;
  class_addmethod(classPtr, reinterpret_cast<t_method>(dspCB.callback), gensym("dsp"), A_CANT, 0);
  CPPEXTERN_MSG (classPtr, "mode", filltypeMess);
}
