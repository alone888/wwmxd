/*
 * hicumL2V2p31n.core.cpp - device implementations for hicumL2V2p31n module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "hicumL2V2p31n.analogfunction.h"
#include "component.h"
#include "device.h"
#include "hicumL2V2p31n.core.h"

#ifndef CIR_hicumL2V2p31n
#define CIR_hicumL2V2p31n -1
#endif

// external nodes
#define c 0
#define b 1
#define e 2
#define s 3
#define tnode 4
// internal nodes
#define ci 5
#define ei 6
#define bp 7
#define bi 8
#define si 9
#define xf1 10
#define xf2 11
#define xf 12
#define n1 13
#define n2 14

// useful macro definitions
#define NP(node) real (getV (node))
#define BP(pnode,nnode) (NP(pnode) - NP(nnode))
#define _load_static_residual2(pnode,nnode,current)\
	_rhs[pnode] -= current;\
	_rhs[nnode] += current;
#define _load_static_augmented_residual2(pnode,nnode,current)\
	_rhs[pnode] -= current;\
	_rhs[nnode] += current;
#define _load_static_residual1(node,current)\
	_rhs[node] -= current;
#define _load_static_augmented_residual1(node,current)\
	_rhs[node] -= current;
#define _load_static_jacobian4(pnode,nnode,vpnode,vnnode,conductance)\
	_jstat[pnode][vpnode] += conductance;\
	_jstat[nnode][vnnode] += conductance;\
	_jstat[pnode][vnnode] -= conductance;\
	_jstat[nnode][vpnode] -= conductance;\
	if (doHB) {\
	_ghs[pnode] += conductance * BP(vpnode,vnnode);\
	_ghs[nnode] -= conductance * BP(vpnode,vnnode);\
        } else {\
	_rhs[pnode] += conductance * BP(vpnode,vnnode);\
	_rhs[nnode] -= conductance * BP(vpnode,vnnode);\
	}
#define _load_static_jacobian2p(node,vpnode,vnnode,conductance)\
	_jstat[node][vpnode] += conductance;\
	_jstat[node][vnnode] -= conductance;\
	if (doHB) {\
        _ghs[node] += conductance * BP(vpnode,vnnode);\
        } else {\
        _rhs[node] += conductance * BP(vpnode,vnnode);\
	}
#define _load_static_jacobian2s(pnode,nnode,node,conductance)\
	_jstat[pnode][node] += conductance;\
	_jstat[nnode][node] -= conductance;\
	if (doHB) {\
	_ghs[pnode] += conductance * NP(node);\
	_ghs[nnode] -= conductance * NP(node);\
        } else {\
	_rhs[pnode] += conductance * NP(node);\
	_rhs[nnode] -= conductance * NP(node);\
	}
#define _load_static_jacobian1(node,vnode,conductance)\
	_jstat[node][vnode] += conductance;\
	if (doHB) {\
	_ghs[node] += conductance * NP(vnode);\
        } else {\
	_rhs[node] += conductance * NP(vnode);\
	}
#define _load_dynamic_residual2(pnode,nnode,charge)\
	if (doTR) _charges[pnode][nnode] += charge;\
	if (doHB) {\
	_qhs[pnode] -= charge;\
	_qhs[nnode] += charge;\
	}
#define _load_dynamic_residual1(node,charge)\
	if (doTR) _charges[node][node] += charge;\
	if (doHB) {\
	_qhs[node] -= charge;\
	}
#define _load_dynamic_jacobian4(pnode,nnode,vpnode,vnnode,capacitance)\
	if (doAC) {\
	_jdyna[pnode][vpnode] += capacitance;\
	_jdyna[nnode][vnnode] += capacitance;\
	_jdyna[pnode][vnnode] -= capacitance;\
	_jdyna[nnode][vpnode] -= capacitance;\
	}\
        if (doTR) {\
        _caps[pnode][nnode][vpnode][vnnode] += capacitance;\
  	}\
	if (doHB) {\
	_chs[pnode] += capacitance * BP(vpnode,vnnode);\
	_chs[nnode] -= capacitance * BP(vpnode,vnnode);\
	}
#define _load_dynamic_jacobian2s(pnode,nnode,vnode,capacitance)\
	if (doAC) {\
	_jdyna[pnode][vnode] += capacitance;\
	_jdyna[nnode][vnode] -= capacitance;\
	}\
	if (doTR) {\
	_caps[pnode][nnode][vnode][vnode] += capacitance;\
	}\
	if (doHB) {\
	_chs[pnode] += capacitance * NP(vnode);\
	_chs[nnode] -= capacitance * NP(vnode);\
	}
#define _load_dynamic_jacobian2p(node,vpnode,vnnode,capacitance)\
	if (doAC) {\
	_jdyna[node][vpnode] += capacitance;\
        _jdyna[node][vnnode] -= capacitance;\
        }\
	if (doTR) {\
        _caps[node][node][vpnode][vnnode] += capacitance;\
	}\
	if (doHB) {\
	_chs[node] += capacitance * BP(vpnode,vnnode);\
	}
#define _load_dynamic_jacobian1(node,vnode,capacitance)\
	if (doAC) {\
	_jdyna[node][vnode] += capacitance;\
	}\
	if (doTR) {\
	_caps[node][node][vnode][vnode] += capacitance;\
	}\
	if (doHB) {\
	_chs[node] += capacitance * NP(vnode);\
	}

#define _save_whitenoise1(n1,pwr,type)\
	_white_pwr[n1][n1] += pwr;
#define _save_whitenoise2(n1,n2,pwr,type)\
	_white_pwr[n1][n2] += pwr;
#define _save_flickernoise1(n1,pwr,exp,type)\
	_flicker_pwr[n1][n1] += pwr;\
	_flicker_exp[n1][n1] += exp;
#define _save_flickernoise2(n1,n2,pwr,exp,type)\
	_flicker_pwr[n1][n2] += pwr;\
	_flicker_exp[n1][n2] += exp;
#define _load_whitenoise2(n1,n2,pwr)\
	cy (n1,n2) -= pwr/kB/T0; cy (n2,n1) -= pwr/kB/T0;\
	cy (n1,n1) += pwr/kB/T0; cy (n2,n2) += pwr/kB/T0;
#define _load_whitenoise1(n1,pwr)\
	cy (n1,n1) += pwr/kB/T0;
#define _load_flickernoise2(n1,n2,pwr,exp)\
	cy (n1,n2) -= pwr*pow(_freq,-exp)/kB/T0;\
	cy (n2,n1) -= pwr*pow(_freq,-exp)/kB/T0;\
	cy (n1,n1) += pwr*pow(_freq,-exp)/kB/T0;\
	cy (n2,n2) += pwr*pow(_freq,-exp)/kB/T0;
#define _load_flickernoise1(n1,pwr,exp)\
	cy (n1,n1) += pwr*pow(_freq,-exp)/kB/T0;

// derivative helper macros
// transcendental LRM p. 59
#define m00_cos(v00,x)          v00 = cos(x);
#define m10_cos(v10,v00,x)      v10 = (-sin(x));
#define m00_sin(v00,x)          v00 = sin(x);
#define m10_sin(v10,v00,x)      v10 = (cos(x));
#define m00_tan(v00,x)          v00 = tan(x);
#define m10_tan(v10,v00,x)      v10 = (1.0/cos(x)/cos(x));
#define m00_cosh(v00,x)         v00 = cosh(x);
#define m10_cosh(v10,v00,x)     v10 = (sinh(x));
#define m00_sinh(v00,x)         v00 = sinh(x);
#define m10_sinh(v10,v00,x)     v10 = (cosh(x));
#define m00_tanh(v00,x)         v00 = tanh(x);
#define m10_tanh(v10,v00,x)     v10 = (1.0/cosh(x)/cosh(x));
#define m00_acos(v00,x)         v00 = acos(x);
#define m10_acos(v10,v00,x)     v10 = (-1.0/sqrt(1-x*x));
#define m00_asin(v00,x)         v00 = asin(x);
#define m10_asin(v10,v00,x)     v10 = (+1.0/sqrt(1-x*x));
#define m00_atan(v00,x)         v00 = atan(x);
#define m10_atan(v10,v00,x)     v10 = (+1.0/(1+x*x));
#define m00_hypot(v00,x,y)      v00 = sqrt((x)*(x)+(y)*(y));
#define m10_hypot(v10,v00,x,y)  v10 = (x)/(v00);
#define m11_hypot(v11,v00,x,y)  v11 = (y)/(v00);
#define m00_atan2(v00,x,y)      v00 = atan2(x,y);
// TODO atan2 derivatives ?
#define m00_acosh(v00,x)        v00 = acosh(x);
#define m10_acosh(v10,v00,x)    v10 = (1.0/(sqrt(x-1)*sqrt(x+1)));
#define m00_asinh(v00,x)        v00 = asinh(x);
#define m10_asinh(v10,v00,x)    v10 = (1.0/(sqrt(x*x+1)));
#define m00_atanh(v00,x)        v00 = atanh(x);
#define m10_atanh(v10,v00,x)    v10 = (1.0/(1-x*x));


// standard functions LRM p.58
#define m00_logE(v00,x)         v00 = log(x);
#define m10_logE(v10,v00,x)     v10 = (1.0/x);
#define m00_log10(v00,x)        v00 = log10(x);
#define m10_log10(v10,v00,x)    v10 = (1.0/x/M_LN10);
#define m00_exp(v00,x)          v00 = exp(x);
#define m10_exp(v10,v00,x)      v10 = v00;
#define m00_sqrt(v00,x)         v00 = sqrt(x);
#define m10_sqrt(v10,v00,x)     v10 = (0.5/v00);
#define m00_min(v00,x,y)        v00 = ((x)<(y))?(x):(y);
#define m10_min(v10,v00,x,y)    v10 = ((x)<(y))?1.0:0.0;
#define m11_min(v11,v00,x,y)    v11 = ((x)<(y))?0.0:1.0;
#define m00_max(v00,x,y)        v00 = ((x)>(y))?(x):(y);
#define m10_max(v10,v00,x,y)    v10 = ((x)>(y))?1.0:0.0;
#define m11_max(v11,v00,x,y)    v11 = ((x)>(y))?0.0:1.0;
#define m00_pow(v00,x,y)        v00 = pow(x,y);
#define m10_pow(v10,v00,x,y)    v10 = (x==0.0)?0.0:(v00)*(y)/(x);
#define m11_pow(v11,v00,x,y)    v11 = (x==0.0)?0.0:(log(x)*(v00));
#define m00_abs(v00,x)          v00 = ((x)<(0)?(-(x)):(x));
#define m10_abs(v10,v00,x)      v10 = (((x)>=0)?(+1.0):(-1.0));
#define m00_floor(v00,x)        v00 = floor(x);
#define m10_floor(v10,v00,x)    v10 = 1.0;

#define m00_ceil(v00,x)         v00 = ceil(x);
// TODO ceil derivative, needed?

// analog operator, LRM p.61
#define m00_limexp(v00,x)       v00 = ((x)<80.0?exp(x):exp(80.0)*(x-79.0));
#define m10_limexp(v10,v00,x)   v10 = ((x)<80.0?(v00):exp(80.0));

// analog kernel parameter system functions, LRM p.215
#define m00_vt(x)               (kBoverQ*(x))
#define m10_vt(x)               (kBoverQ)

// extra functions (?)
#define m00_div(v00,v10,x,y)    double v10=1/(y); double v00=(x)*v10;
#define m10_div(v10,v00,vv,x,y)
#define m11_div(v11,v00,vv,x,y) double v11 = -v00*vv;
#define m00_mult(v00,v10,v11,x,y) double v10=(x); double v11=(y); double v00=v10*v11;
#define m00_add(v00,x,y)        double v00=(x)+(y);

// second derivatives
#define m20_logE(v00)           (-1.0/v00/v00)
#define m20_exp(v00)            exp(v00)
#define m20_limexp(v00)         ((v00)<80.0?exp(v00):0.0)
#define m20_sqrt(v00)           (-0.25/(v00)/sqrt(v00))
#define m20_abs(v00)           0.0
#define m20_pow(x,y)            ((y)*((y)-1.0)*pow(x,y)/(x)/(x))


// simulator specific definitions
#define _modelname              "hicumL2V2p31n"
#define _instancename           getName()
#define _circuit_temp	        (getPropertyDouble("Temp")+273.15)
#define _param_given(p)		(isPropertyGiven(p)?1:0)


// $vt and $vt() functions
#define _vt_nom                 (kBoverQ*_circuit_temp)

using namespace qucs::device;
using qucs::matrix;

/* Device constructor. */
hicumL2V2p31n::hicumL2V2p31n() : circuit (15)
{
  type = CIR_hicumL2V2p31n;
}

/* Initialization of model. */
void hicumL2V2p31n::initModel (void)
{
  // create internal nodes
  setInternalNode (ci, "ci");
  setInternalNode (ei, "ei");
  setInternalNode (bp, "bp");
  setInternalNode (bi, "bi");
  setInternalNode (si, "si");
  setInternalNode (xf1, "xf1");
  setInternalNode (xf2, "xf2");
  setInternalNode (xf, "xf");
  setInternalNode (n1, "n1");
  setInternalNode (n2, "n2");

  // get device model parameters
  loadVariables ();
  // evaluate global model equations
  initializeModel ();
  // evaluate initial step equations
  initialStep ();
  // evaluate global instance equations
  initializeInstance ();
}

/* Initialization of DC analysis. */
void hicumL2V2p31n::initDC (void)
{
  allocMatrixMNA ();
  initModel ();
  pol = 1;
  restartDC ();
  doAC = 1;
  doTR = 0;
  doHB = 0;
}

/* Run when DC is restarted (fallback algorithms). */
void hicumL2V2p31n::restartDC (void)
{
}

/* Initialize Verilog-AMS code. */
void hicumL2V2p31n::initVerilog (void)
{
  // initialization of noise variables
  _white_pwr[n2][n2] = 0.0;
  _white_pwr[n1][n1] = 0.0;
  _white_pwr[ci][bi] = 0.0;
  _white_pwr[si][s] = 0.0;
  _white_pwr[ei][e] = 0.0;
  _white_pwr[ci][c] = 0.0;
  _white_pwr[si][ci] = 0.0;
  _white_pwr[bp][ei] = 0.0;
  _white_pwr[bp][bi] = 0.0;
  _white_pwr[b][bp] = 0.0;
  _white_pwr[bp][ci] = 0.0;
  _white_pwr[ci][ei] = 0.0;
  _white_pwr[bi][ci] = 0.0;
  _white_pwr[bi][ei] = 0.0;
  _flicker_pwr[ei][e] = 0.0;
  _flicker_exp[ei][e] = 0.0;
  _flicker_pwr[bp][ei] = 0.0;
  _flicker_exp[bp][ei] = 0.0;
  _flicker_pwr[bi][ei] = 0.0;
  _flicker_exp[bi][ei] = 0.0;

  int i1, i2, i3, i4;

  // zero charges
  for (i1 = 0; i1 < 15; i1++) {
  for (i2 = 0; i2 < 15; i2++) {
    _charges[i1][i2] = 0.0;
  } }

  // zero capacitances
  for (i1 = 0; i1 < 15; i1++) {
  for (i2 = 0; i2 < 15; i2++) {
  for (i3 = 0; i3 < 15; i3++) {
  for (i4 = 0; i4 < 15; i4++) {
    _caps[i1][i2][i3][i4] = 0.0;
  } } } }

  // zero right hand side, static and dynamic jacobian
  for (i1 = 0; i1 < 15; i1++) {
    _rhs[i1] = 0.0;
    _qhs[i1] = 0.0;
    _chs[i1] = 0.0;
    _ghs[i1] = 0.0;
    for (i2 = 0; i2 < 15; i2++) {
      _jstat[i1][i2] = 0.0;
      _jdyna[i1][i2] = 0.0;
    }
  }
}

/* Load device model input parameters. */
void hicumL2V2p31n::loadVariables (void)
{
  c10 = getPropertyDouble ("c10");
  qp0 = getPropertyDouble ("qp0");
  ich = getPropertyDouble ("ich");
  hf0 = getPropertyDouble ("hf0");
  hfe = getPropertyDouble ("hfe");
  hfc = getPropertyDouble ("hfc");
  hjei = getPropertyDouble ("hjei");
  ahjei = getPropertyDouble ("ahjei");
  rhjei = getPropertyDouble ("rhjei");
  hjci = getPropertyDouble ("hjci");
  ibeis = getPropertyDouble ("ibeis");
  mbei = getPropertyDouble ("mbei");
  ireis = getPropertyDouble ("ireis");
  mrei = getPropertyDouble ("mrei");
  ibeps = getPropertyDouble ("ibeps");
  mbep = getPropertyDouble ("mbep");
  ireps = getPropertyDouble ("ireps");
  mrep = getPropertyDouble ("mrep");
  mcf = getPropertyDouble ("mcf");
  tbhrec = getPropertyDouble ("tbhrec");
  ibcis = getPropertyDouble ("ibcis");
  mbci = getPropertyDouble ("mbci");
  ibcxs = getPropertyDouble ("ibcxs");
  mbcx = getPropertyDouble ("mbcx");
  ibets = getPropertyDouble ("ibets");
  abet = getPropertyDouble ("abet");
  tunode = getPropertyInteger ("tunode");
  favl = getPropertyDouble ("favl");
  qavl = getPropertyDouble ("qavl");
  alfav = getPropertyDouble ("alfav");
  alqav = getPropertyDouble ("alqav");
  rbi0 = getPropertyDouble ("rbi0");
  rbx = getPropertyDouble ("rbx");
  fgeo = getPropertyDouble ("fgeo");
  fdqr0 = getPropertyDouble ("fdqr0");
  fcrbi = getPropertyDouble ("fcrbi");
  fqi = getPropertyDouble ("fqi");
  re = getPropertyDouble ("re");
  rcx = getPropertyDouble ("rcx");
  itss = getPropertyDouble ("itss");
  msf = getPropertyDouble ("msf");
  iscs = getPropertyDouble ("iscs");
  msc = getPropertyDouble ("msc");
  tsf = getPropertyDouble ("tsf");
  rsu = getPropertyDouble ("rsu");
  csu = getPropertyDouble ("csu");
  cjei0 = getPropertyDouble ("cjei0");
  vdei = getPropertyDouble ("vdei");
  zei = getPropertyDouble ("zei");
  ajei = getPropertyDouble ("ajei");
  cjep0 = getPropertyDouble ("cjep0");
  vdep = getPropertyDouble ("vdep");
  zep = getPropertyDouble ("zep");
  ajep = getPropertyDouble ("ajep");
  cjci0 = getPropertyDouble ("cjci0");
  vdci = getPropertyDouble ("vdci");
  zci = getPropertyDouble ("zci");
  vptci = getPropertyDouble ("vptci");
  cjcx0 = getPropertyDouble ("cjcx0");
  vdcx = getPropertyDouble ("vdcx");
  zcx = getPropertyDouble ("zcx");
  vptcx = getPropertyDouble ("vptcx");
  fbcpar = getPropertyDouble ("fbcpar");
  fbepar = getPropertyDouble ("fbepar");
  cjs0 = getPropertyDouble ("cjs0");
  vds = getPropertyDouble ("vds");
  zs = getPropertyDouble ("zs");
  vpts = getPropertyDouble ("vpts");
  t0 = getPropertyDouble ("t0");
  dt0h = getPropertyDouble ("dt0h");
  tbvl = getPropertyDouble ("tbvl");
  tef0 = getPropertyDouble ("tef0");
  gtfe = getPropertyDouble ("gtfe");
  thcs = getPropertyDouble ("thcs");
  ahc = getPropertyDouble ("ahc");
  fthc = getPropertyDouble ("fthc");
  rci0 = getPropertyDouble ("rci0");
  vlim = getPropertyDouble ("vlim");
  vces = getPropertyDouble ("vces");
  vpt = getPropertyDouble ("vpt");
  tr = getPropertyDouble ("tr");
  vcbar = getPropertyDouble ("vcbar");
  icbar = getPropertyDouble ("icbar");
  acbar = getPropertyDouble ("acbar");
  delck = getPropertyDouble ("delck");
  cbepar = getPropertyDouble ("cbepar");
  cbcpar = getPropertyDouble ("cbcpar");
  alqf = getPropertyDouble ("alqf");
  alit = getPropertyDouble ("alit");
  flnqs = getPropertyInteger ("flnqs");
  kf = getPropertyDouble ("kf");
  af = getPropertyDouble ("af");
  cfbe = getPropertyInteger ("cfbe");
  flcono = getPropertyInteger ("flcono");
  kfre = getPropertyDouble ("kfre");
  afre = getPropertyDouble ("afre");
  latb = getPropertyDouble ("latb");
  latl = getPropertyDouble ("latl");
  vgb = getPropertyDouble ("vgb");
  alt0 = getPropertyDouble ("alt0");
  kt0 = getPropertyDouble ("kt0");
  zetaci = getPropertyDouble ("zetaci");
  alvs = getPropertyDouble ("alvs");
  alces = getPropertyDouble ("alces");
  zetarbi = getPropertyDouble ("zetarbi");
  zetarbx = getPropertyDouble ("zetarbx");
  zetarcx = getPropertyDouble ("zetarcx");
  zetare = getPropertyDouble ("zetare");
  zetacx = getPropertyDouble ("zetacx");
  vge = getPropertyDouble ("vge");
  vgc = getPropertyDouble ("vgc");
  vgs = getPropertyDouble ("vgs");
  f1vg = getPropertyDouble ("f1vg");
  f2vg = getPropertyDouble ("f2vg");
  zetact = getPropertyDouble ("zetact");
  zetabet = getPropertyDouble ("zetabet");
  alb = getPropertyDouble ("alb");
  dvgbe = getPropertyDouble ("dvgbe");
  zetahjei = getPropertyDouble ("zetahjei");
  zetavgbe = getPropertyDouble ("zetavgbe");
  flsh = getPropertyInteger ("flsh");
  rth = getPropertyDouble ("rth");
  zetarth = getPropertyDouble ("zetarth");
  alrth = getPropertyDouble ("alrth");
  cth = getPropertyDouble ("cth");
  flcomp = getPropertyDouble ("flcomp");
  tnom = getPropertyDouble ("tnom");
  dt = getPropertyDouble ("dt");
}

/* #define's for translated code */
#undef  _DDT
#define _DDT(q) q
#define _DYNAMIC
#define _DERIVATE
#define _DDX
#define _DERIVATEFORDDX

/* Evaluate Verilog-AMS equations in model initialization. */
void hicumL2V2p31n::initializeModel (void)
{
#if defined(_DERIVATE)
double rth_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hfc_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hfe_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hf0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hjei0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ahjei_t_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vpts_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjs0_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vds_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double tsf_t_Vtnode_GND;
#endif
#endif
#if defined(_DERIVATE)
double re_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rbx_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rcx_t_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjcx02_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjcx01_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vptcx_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cratio_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vdcx_t_Vtnode_GND;
#endif
#endif
#if defined(_DERIVATE)
double abet_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibets_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ajep_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjep0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdep_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rbi0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qavl_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double favl_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double thcs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double t0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vptci_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjci0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdci_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vces_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rci0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double c10_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double tef0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vlim_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qp0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double iscs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double itss_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibcxs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibcis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ireps_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibeps_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ireis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibeis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double a_Vbi_ei;
double a_Vbi_ci;
double a_Vci_ei;
double a_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double r_VgVT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double V_gT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ajei_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjei0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdei_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdt_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdjt_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vgbe_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vge_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vgb_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k2_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k1_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ln_qtt0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qtt0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double dT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double VT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double Tdev_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
#endif
#if defined(_DYNAMIC)
double C_1;
#endif
double vge_t0;
double vgb_t0;
double k20;
double k10;
{
Tnom=(tnom+273.15);
Tamb=_circuit_temp;
vt0=((1.3806503e-23*Tnom)/1.602176462e-19);
{
double m00_logE(d00_logE0,Tnom)
k10=((f1vg*Tnom)*d00_logE0);
}
k20=(f2vg*Tnom);
avs=(alvs*Tnom);
vgb_t0=((vgb+k10)+k20);
vge_t0=((vge+k10)+k20);
vgbe_t0=((vgb_t0+vge_t0)/2);
vgbe0=((vgb+vge)/2);
vgbc0=((vgb+vgc)/2);
vgsc0=((vgs+vgc)/2);
mg=(3-((1.602176462e-19*f1vg)/1.3806503e-23));
zetabci=((mg+1)-zetaci);
zetabcxt=((mg+1)-zetacx);
zetasct=(mg-1.5);
#if defined(_DYNAMIC)
C_1=((1.0-fbcpar)*(cjcx0+cbcpar));
#endif
#ifdef _DYNAMIC
if
(C_1>=cbcpar)
{
#if defined(_DYNAMIC)
cbcpar1=cbcpar;
#endif
#if defined(_DYNAMIC)
cbcpar2=0.0;
#endif
#if defined(_DYNAMIC)
cjcx01=(C_1-cbcpar);
#endif
#if defined(_DYNAMIC)
cjcx02=(cjcx0-cjcx01);
#endif
}
else
{
#if defined(_DYNAMIC)
cbcpar1=C_1;
#endif
#if defined(_DYNAMIC)
cbcpar2=(cbcpar-cbcpar1);
#endif
#if defined(_DYNAMIC)
cjcx01=0.0;
#endif
#if defined(_DYNAMIC)
cjcx02=cjcx0;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
cbepar2=(fbepar*cbepar);
#endif
#if defined(_DYNAMIC)
cbepar1=(cbepar-cbepar2);
#endif
if
(ich!=0.0)
{
Oich=(1.0/ich);
}
else
{
Oich=0.0;
}
if
(tbhrec!=0.0)
{
Otbhrec=(1.0/tbhrec);
}
else
{
Otbhrec=0.0;
}
if
((flsh==0)||(rth<0.001))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=0.0;
#endif
Tdev=(Tamb+dt);
if
(Tdev<((-100.0)+273.15))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=0.0;
#endif
Tdev=((-100.0)+273.15);
}
else
{
if
(Tdev>(326.85+273.15))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=0.0;
#endif
Tdev=(326.85+273.15);
}
}
#if defined(_DERIVATE)
VT_Vtnode_GND=0.0;
#endif
VT=((1.3806503e-23*Tdev)/1.602176462e-19);
#if defined(_DERIVATE)
dT_Vtnode_GND=0.0;
#endif
dT=(Tdev-Tnom);
#if defined(_DERIVATE)
qtt0_Vtnode_GND=0.0;
#endif
qtt0=(Tdev/Tnom);
{
double m00_logE(d00_logE0,qtt0)
#if defined(_DERIVATE)
ln_qtt0_Vtnode_GND=0.0;
#endif
ln_qtt0=d00_logE0;
}
{
double m00_logE(d00_logE0,Tdev)
#if defined(_DERIVATE)
k1_Vtnode_GND=0.0;
#endif
k1=((f1vg*Tdev)*d00_logE0);
}
#if defined(_DERIVATE)
k2_Vtnode_GND=0.0;
#endif
k2=(f2vg*Tdev);
#if defined(_DERIVATE)
vgb_t_Vtnode_GND=0.0;
#endif
vgb_t=((vgb+k1)+k2);
#if defined(_DERIVATE)
vge_t_Vtnode_GND=0.0;
#endif
vge_t=((vge+k1)+k2);
#if defined(_DERIVATE)
vgbe_t_Vtnode_GND=0.0;
#endif
vgbe_t=((vgb_t+vge_t)/2);
if
(cjei0>0.0)
{
{
double m00_exp(d00_exp0,((vdei*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdei)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=0.0;
#endif
vdjt=(((vdj0*qtt0)+(vgbe0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
vdt_Vtnode_GND=0.0;
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdei_t_Vtnode_GND=0.0;
#endif
vdei_t=vdt;
{
double m00_logE(d00_logE0,(vdei/vdei_t))
double m00_exp(d00_exp1,(zei*d00_logE0))
#if defined(_DERIVATE)
cjei0_t_Vtnode_GND=0.0;
#endif
cjei0_t=(cjei0*d00_exp1);
}
if
(1==1)
{
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=0.0;
#endif
ajei_t=((ajei*vdei_t)/vdei);
}
else
{
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=0.0;
#endif
ajei_t=ajei;
}
}
else
{
#if defined(_DERIVATE)
cjei0_t_Vtnode_GND=0.0;
#endif
cjei0_t=cjei0;
#if defined(_DERIVATE)
vdei_t_Vtnode_GND=0.0;
#endif
vdei_t=vdei;
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=0.0;
#endif
ajei_t=ajei;
}
if
((flcomp==0.0)||(flcomp==2.1))
{
#if defined(_DERIVATE)
V_gT_Vtnode_GND=0.0;
#endif
V_gT=(((3.0*VT)*ln_qtt0)+(vgb*(qtt0-1.0)));
#if defined(_DERIVATE)
r_VgVT_Vtnode_GND=0.0;
#endif
r_VgVT=(V_gT/VT);
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(((mcf*r_VgVT)/mbei)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ibeis_t_Vtnode_GND=0.0;
#endif
ibeis_t=(ibeis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(((mcf*r_VgVT)/mrei)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=0.0;
#endif
ireis_t=(ireis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(((mcf*r_VgVT)/mbep)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ibeps_t_Vtnode_GND=0.0;
#endif
ibeps_t=(ibeps*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(((mcf*r_VgVT)/mrep)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=0.0;
#endif
ireps_t=(ireps*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(r_VgVT/mbci);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ibcis_t_Vtnode_GND=0.0;
#endif
ibcis_t=(ibcis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(r_VgVT/mbcx);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
ibcxs_t_Vtnode_GND=0.0;
#endif
ibcxs_t=(ibcxs*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(r_VgVT/msf);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
itss_t_Vtnode_GND=0.0;
#endif
itss_t=(itss*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(r_VgVT/msc);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
iscs_t_Vtnode_GND=0.0;
#endif
iscs_t=(iscs*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(vdei_t/vdei);
#if defined(_DERIVATE)
qp0_t_Vtnode_GND=0.0;
#endif
qp0_t=(qp0*(1.0+((0.5*zei)*(1.0-a))));
{
double m00_exp(d00_exp0,(zetaci*ln_qtt0))
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=((vlim*(1.0-(alvs*dT)))*d00_exp0);
}
#if defined(_DERIVATE)
k_Vtnode_GND=0.0;
#endif
k=((a-VT)/VT);
if
(k<11.0)
{
{
double m00_exp(d00_exp0,k)
double m00_logE(d00_logE1,(1.0+d00_exp0))
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=0.0;
#endif
vlim_t=(VT+(VT*d00_logE1));
}
}
else
{
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=0.0;
#endif
vlim_t=a;
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=(1.0+(alb*dT));
{
double m00_sqrt(d00_sqrt0,((a*a)+0.01))
#if defined(_DERIVATE)
k_Vtnode_GND=0.0;
#endif
k=(0.5*(a+d00_sqrt0));
}
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=0.0;
#endif
tef0_t=((tef0*qtt0)/k);
}
else
{
{
double m00_exp(d00_exp0,((zetabet*ln_qtt0)+((vge/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ibeis_t_Vtnode_GND=0.0;
#endif
ibeis_t=(ibeis*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((mg/mrei)*ln_qtt0)+((vgbe0/(mrei*VT))*(qtt0-1))))
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=0.0;
#endif
ireis_t=(ireis*d00_exp0);
}
}
else
{
{
double m00_exp(d00_exp0,(((0.5*mg)*ln_qtt0)+(((0.5*vgbe0)/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=0.0;
#endif
ireis_t=(ireis*d00_exp0);
}
}
{
double m00_exp(d00_exp0,((zetabet*ln_qtt0)+((vge/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ibeps_t_Vtnode_GND=0.0;
#endif
ibeps_t=(ibeps*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((mg/mrep)*ln_qtt0)+((vgbe0/(mrep*VT))*(qtt0-1))))
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=0.0;
#endif
ireps_t=(ireps*d00_exp0);
}
}
else
{
{
double m00_exp(d00_exp0,(((0.5*mg)*ln_qtt0)+(((0.5*vgbe0)/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=0.0;
#endif
ireps_t=(ireps*d00_exp0);
}
}
{
double m00_exp(d00_exp0,((zetabci*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ibcis_t_Vtnode_GND=0.0;
#endif
ibcis_t=(ibcis*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetabcxt*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
ibcxs_t_Vtnode_GND=0.0;
#endif
ibcxs_t=(ibcxs*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetasct*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
itss_t_Vtnode_GND=0.0;
#endif
itss_t=(itss*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetasct*ln_qtt0)+((vgs/VT)*(qtt0-1))))
#if defined(_DERIVATE)
iscs_t_Vtnode_GND=0.0;
#endif
iscs_t=(iscs*d00_exp0);
}
{
double m00_logE(d00_logE0,(vdei_t/vdei))
double m00_exp(d00_exp1,(zei*d00_logE0))
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=0.0;
#endif
a=d00_exp1;
}
#if defined(_DERIVATE)
qp0_t_Vtnode_GND=0.0;
#endif
qp0_t=(qp0*(2.0-a));
{
double m00_exp(d00_exp0,((zetaci-avs)*ln_qtt0))
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=0.0;
#endif
vlim_t=(vlim*d00_exp0);
}
if
(flcomp>=2.3)
{
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=0.0;
#endif
tef0_t=tef0;
}
else
{
zetatef=((zetabet-zetact)-0.5);
dvg0=(vgb-vge);
{
double m00_exp(d00_exp0,((zetatef*ln_qtt0)-((dvg0/VT)*(qtt0-1))))
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=0.0;
#endif
tef0_t=(tef0*d00_exp0);
}
}
}
{
double m00_exp(d00_exp0,((zetact*ln_qtt0)+((vgb/VT)*(qtt0-1))))
#if defined(_DERIVATE)
c10_t_Vtnode_GND=0.0;
#endif
c10_t=(c10*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetaci*ln_qtt0))
#if defined(_DERIVATE)
rci0_t_Vtnode_GND=0.0;
#endif
rci0_t=(rci0*d00_exp0);
}
#if defined(_DERIVATE)
vces_t_Vtnode_GND=0.0;
#endif
vces_t=(vces*(1+(alces*dT)));
if
(cjci0>0.0)
{
{
double m00_exp(d00_exp0,((vdci*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdci)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=0.0;
#endif
vdjt=(((vdj0*qtt0)+(vgbc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
vdt_Vtnode_GND=0.0;
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdci_t_Vtnode_GND=0.0;
#endif
vdci_t=vdt;
{
double m00_logE(d00_logE0,(vdci/vdci_t))
double m00_exp(d00_exp1,(zci*d00_logE0))
#if defined(_DERIVATE)
cjci0_t_Vtnode_GND=0.0;
#endif
cjci0_t=(cjci0*d00_exp1);
}
if
(0==1)
{
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=0.0;
#endif
vptci_t=((vptci*vdci_t)/vdci);
}
else
{
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=0.0;
#endif
vptci_t=vptci;
}
}
else
{
#if defined(_DERIVATE)
cjci0_t_Vtnode_GND=0.0;
#endif
cjci0_t=cjci0;
#if defined(_DERIVATE)
vdci_t_Vtnode_GND=0.0;
#endif
vdci_t=vdci;
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=0.0;
#endif
vptci_t=vptci;
}
#if defined(_DERIVATE)
t0_t_Vtnode_GND=0.0;
#endif
t0_t=(t0*((1+(alt0*dT))+((kt0*dT)*dT)));
{
double m00_exp(d00_exp0,((zetaci-1)*ln_qtt0))
#if defined(_DERIVATE)
thcs_t_Vtnode_GND=0.0;
#endif
thcs_t=(thcs*d00_exp0);
}
{
double m00_exp(d00_exp0,(alfav*dT))
#if defined(_DERIVATE)
favl_t_Vtnode_GND=0.0;
#endif
favl_t=(favl*d00_exp0);
}
{
double m00_exp(d00_exp0,(alqav*dT))
#if defined(_DERIVATE)
qavl_t_Vtnode_GND=0.0;
#endif
qavl_t=(qavl*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetarbi*ln_qtt0))
#if defined(_DERIVATE)
rbi0_t_Vtnode_GND=0.0;
#endif
rbi0_t=(rbi0*d00_exp0);
}
if
(cjep0>0.0)
{
{
double m00_exp(d00_exp0,((vdep*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdep)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=0.0;
#endif
vdjt=(((vdj0*qtt0)+(vgbe0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
vdt_Vtnode_GND=0.0;
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdep_t_Vtnode_GND=0.0;
#endif
vdep_t=vdt;
{
double m00_logE(d00_logE0,(vdep/vdep_t))
double m00_exp(d00_exp1,(zep*d00_logE0))
#if defined(_DERIVATE)
cjep0_t_Vtnode_GND=0.0;
#endif
cjep0_t=(cjep0*d00_exp1);
}
if
(1==1)
{
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=0.0;
#endif
ajep_t=((ajep*vdep_t)/vdep);
}
else
{
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=0.0;
#endif
ajep_t=ajep;
}
}
else
{
#if defined(_DERIVATE)
cjep0_t_Vtnode_GND=0.0;
#endif
cjep0_t=cjep0;
#if defined(_DERIVATE)
vdep_t_Vtnode_GND=0.0;
#endif
vdep_t=vdep;
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=0.0;
#endif
ajep_t=ajep;
}
{
double a_eg;
double ab;
double aa;
ab=1.0;
aa=1.0;
a_eg=(vgbe_t0/vgbe_t);
if
(((tunode==1)&&(cjep0>0.0))&&(vdep>0.0))
{
{
double m00_sqrt(d00_sqrt0,a_eg)
ab=(((((cjep0_t/cjep0)*d00_sqrt0)*vdep_t)*vdep_t)/(vdep*vdep));
}
{
double m00_pow(d00_pow0,a_eg,(-1.5))
aa=(((vdep/vdep_t)*(cjep0/cjep0_t))*d00_pow0);
}
}
else
{
if
(((tunode==0)&&(cjei0>0.0))&&(vdei>0.0))
{
{
double m00_sqrt(d00_sqrt0,a_eg)
ab=(((((cjei0_t/cjei0)*d00_sqrt0)*vdei_t)*vdei_t)/(vdei*vdei));
}
{
double m00_pow(d00_pow0,a_eg,(-1.5))
aa=(((vdei/vdei_t)*(cjei0/cjei0_t))*d00_pow0);
}
}
}
#if defined(_DERIVATE)
ibets_t_Vtnode_GND=0.0;
#endif
ibets_t=(ibets*ab);
#if defined(_DERIVATE)
abet_t_Vtnode_GND=0.0;
#endif
abet_t=(abet*aa);
}
if
(1.0>0.0)
{
{
double m00_exp(d00_exp0,((vdcx*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdcx)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=0.0;
#endif
vdjt=(((vdj0*qtt0)+(vgbc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
vdt_Vtnode_GND=0.0;
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdcx_t_Vtnode_GND=0.0;
#endif
vdcx_t=vdt;
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vdcx/vdcx_t))
double m00_exp(d00_exp1,(zcx*d00_logE0))
#if defined(_DERIVATE)
cratio_t_Vtnode_GND=0.0;
#endif
cratio_t=(1.0*d00_exp1);
}
#endif
#ifdef _DYNAMIC
if
(0==1)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=0.0;
#endif
vptcx_t=((vptcx*vdcx_t)/vdcx);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=0.0;
#endif
vptcx_t=vptcx;
#endif
}
#endif /* if (...) */
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cratio_t_Vtnode_GND=0.0;
#endif
cratio_t=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdcx_t_Vtnode_GND=0.0;
#endif
vdcx_t=vdcx;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=0.0;
#endif
vptcx_t=vptcx;
#endif
}
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjcx01_t_Vtnode_GND=0.0;
#endif
cjcx01_t=(cratio_t*cjcx01);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjcx02_t_Vtnode_GND=0.0;
#endif
cjcx02_t=(cratio_t*cjcx02);
#endif
{
double m00_exp(d00_exp0,(zetarcx*ln_qtt0))
#if defined(_DERIVATE)
rcx_t_Vtnode_GND=0.0;
#endif
rcx_t=(rcx*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetarbx*ln_qtt0))
#if defined(_DERIVATE)
rbx_t_Vtnode_GND=0.0;
#endif
rbx_t=(rbx*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetare*ln_qtt0))
#if defined(_DERIVATE)
re_t_Vtnode_GND=0.0;
#endif
re_t=(re*d00_exp0);
}
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((zetacx-1.0)*ln_qtt0))
#if defined(_DERIVATE)
tsf_t_Vtnode_GND=0.0;
#endif
tsf_t=(tsf*d00_exp0);
}
#endif
if
(cjs0>0.0)
{
{
double m00_exp(d00_exp0,((vds*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vds)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=0.0;
#endif
vdjt=(((vdj0*qtt0)+(vgsc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
vdt_Vtnode_GND=0.0;
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vds_t_Vtnode_GND=0.0;
#endif
vds_t=vdt;
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vds/vds_t))
double m00_exp(d00_exp1,(zs*d00_logE0))
#if defined(_DERIVATE)
cjs0_t_Vtnode_GND=0.0;
#endif
cjs0_t=(cjs0*d00_exp1);
}
#endif
#ifdef _DYNAMIC
if
(0==1)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=0.0;
#endif
vpts_t=((vpts*vds_t)/vds);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=0.0;
#endif
vpts_t=vpts;
#endif
}
#endif /* if (...) */
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjs0_t_Vtnode_GND=0.0;
#endif
cjs0_t=cjs0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vds_t_Vtnode_GND=0.0;
#endif
vds_t=vds;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=0.0;
#endif
vpts_t=vpts;
#endif
}
{
double m00_exp(d00_exp0,(zetahjei*ln_qtt0))
#if defined(_DERIVATE)
ahjei_t_Vtnode_GND=0.0;
#endif
ahjei_t=(ahjei*d00_exp0);
}
{
double m00_logE(d00_logE0,qtt0)
double m00_exp(d00_exp1,(zetavgbe*d00_logE0))
double m00_exp(d00_exp2,((dvgbe/VT)*(d00_exp1-1)))
#if defined(_DERIVATE)
hjei0_t_Vtnode_GND=0.0;
#endif
hjei0_t=(hjei*d00_exp2);
}
{
double m00_exp(d00_exp0,((dvgbe/VT)*(qtt0-1)))
#if defined(_DERIVATE)
hf0_t_Vtnode_GND=0.0;
#endif
hf0_t=(hf0*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((vgb-vge)/VT)*(qtt0-1)))
#if defined(_DERIVATE)
hfe_t_Vtnode_GND=0.0;
#endif
hfe_t=(hfe*d00_exp0);
}
{
double m00_exp(d00_exp0,(((vgb-vgc)/VT)*(qtt0-1)))
#if defined(_DERIVATE)
hfc_t_Vtnode_GND=0.0;
#endif
hfc_t=(hfc*d00_exp0);
}
}
else
{
#if defined(_DERIVATE)
hfe_t_Vtnode_GND=0.0;
#endif
hfe_t=hfe;
#if defined(_DERIVATE)
hfc_t_Vtnode_GND=0.0;
#endif
hfc_t=hfc;
}
{
double m00_exp(d00_exp0,(zetarth*ln_qtt0))
#if defined(_DERIVATE)
rth_t_Vtnode_GND=0.0;
#endif
rth_t=((rth*d00_exp0)*(1+(alrth*dT)));
}
}
}
}

/* Evaluate Verilog-AMS equations in instance initialization. */
void hicumL2V2p31n::initializeInstance (void)
{
}

/* Evaluate Verilog-AMS equations in initial step. */
void hicumL2V2p31n::initialStep (void)
{
}

/* Evaluate Verilog-AMS equations in final step. */
void hicumL2V2p31n::finalStep (void)
{
}

/* Evaluate Verilog-AMS equations in analog block. */
void hicumL2V2p31n::calcVerilog (void)
{

/* ----------------- evaluate verilog analog equations -------------------- */
#if defined(_DYNAMIC)
double Qxf;
#if defined(_DERIVATE)
double Qxf_Vxf_GND;
#endif
#endif
double Ixf;
#if defined(_DERIVATE)
double Ixf_Vxf_GND;
double Ixf_Vci_ei;
double Ixf_Vtnode_GND;
double Ixf_Vbi_ci;
double Ixf_Vbi_ei;
#endif
double fact;
#if defined(_DERIVATE)
double fact_Vbi_ei;
double fact_Vci_ei;
double fact_Vtnode_GND;
double fact_Vbi_ci;
#endif
double Vxf;
#if defined(_DERIVATE)
double Vxf_Vxf_GND;
#endif
#if defined(_DYNAMIC)
double Qxf2;
#if defined(_DERIVATE)
double Qxf2_Vxf2_GND;
#endif
#endif
#if defined(_DYNAMIC)
double Qxf1;
#if defined(_DERIVATE)
double Qxf1_Vxf1_GND;
#endif
#endif
double Ixf2;
#if defined(_DERIVATE)
double Ixf2_Vxf2_GND;
double Ixf2_Vxf1_GND;
double Ixf2_Vbi_ei;
double Ixf2_Vci_ei;
double Ixf2_Vtnode_GND;
double Ixf2_Vbi_ci;
#endif
double Ixf1;
#if defined(_DERIVATE)
double Ixf1_Vxf1_GND;
double Ixf1_Vxf2_GND;
double Ixf1_Vci_ei;
double Ixf1_Vtnode_GND;
double Ixf1_Vbi_ei;
double Ixf1_Vbi_ci;
#endif
double Vxf2;
#if defined(_DERIVATE)
double Vxf2_Vxf2_GND;
#endif
double Vxf1;
#if defined(_DERIVATE)
double Vxf1_Vxf1_GND;
#endif
#if defined(_DYNAMIC)
double Qdeix;
#if defined(_DERIVATE)
double Qdeix_Vxf_GND;
double Qdeix_Vci_ei;
double Qdeix_Vtnode_GND;
double Qdeix_Vbi_ci;
double Qdeix_Vbi_ei;
#endif
#endif
double Itxf;
#if defined(_DERIVATE)
double Itxf_Vxf2_GND;
double Itxf_Vci_ei;
double Itxf_Vtnode_GND;
double Itxf_Vbi_ei;
double Itxf_Vbi_ci;
#endif
double pterm;
#if defined(_DERIVATE)
double pterm_Vb_bp;
double pterm_Vci_c;
double pterm_Vei_e;
double pterm_Vbp_bi;
double pterm_Vbp_ei;
double pterm_Vbp_ci;
double pterm_Vsi_ci;
double pterm_Vci_ei;
double pterm_Vtnode_GND;
double pterm_Vbi_ei;
double pterm_Vbi_ci;
#endif
#if defined(_DERIVATE)
double ijsc_Vtnode_GND;
double ijsc_Vsi_ci;
double ijsc_Vbp_ci;
double ijsc_Vbp_ei;
double ijsc_Vbi_ci;
double ijsc_Vbi_ei;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double betadc_Vci_ei;
double betadc_Vtnode_GND;
double betadc_Vbi_ei;
double betadc_Vbi_ci;
#endif
#endif
#if defined(_DYNAMIC)
double betan;
#if defined(_DERIVATE)
double betan_Vci_ei;
double betan_Vtnode_GND;
double betan_Vbi_ei;
double betan_Vbi_ci;
#endif
#endif
double betadin;
#if defined(_DYNAMIC)
double betad;
#if defined(_DERIVATE)
double betad_Vtnode_GND;
double betad_Vbi_ei;
#endif
#endif
#if defined(_DYNAMIC)
double Qdsu;
#if defined(_DERIVATE)
double Qdsu_Vtnode_GND;
double Qdsu_Vbp_ci;
#endif
#endif
double HSI_Tsu;
#if defined(_DERIVATE)
double HSI_Tsu_Vtnode_GND;
double HSI_Tsu_Vbp_ci;
double HSI_Tsu_Vsi_ci;
#endif
double HSb;
#if defined(_DERIVATE)
double HSb_Vsi_ci;
double HSb_Vtnode_GND;
#endif
double HSa;
#if defined(_DERIVATE)
double HSa_Vbp_ci;
double HSa_Vtnode_GND;
#endif
double HSUM;
#if defined(_DERIVATE)
double HSUM_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
double Qjs;
#if defined(_DERIVATE)
double Qjs_Vbp_ei;
double Qjs_Vbi_ei;
double Qjs_Vsi_ci;
double Qjs_Vb_ci;
double Qjs_Vbp_ci;
double Qjs_Vtnode_GND;
double Qjs_Vbi_ci;
#endif
#endif
#if defined(_DYNAMIC)
double qjcx0_t_i;
#if defined(_DERIVATE)
double qjcx0_t_i_Vbp_ei;
double qjcx0_t_i_Vbi_ei;
double qjcx0_t_i_Vb_ci;
double qjcx0_t_i_Vbp_ci;
double qjcx0_t_i_Vtnode_GND;
double qjcx0_t_i_Vbi_ci;
#endif
#endif
#if defined(_DERIVATE)
double ijbcx_Vtnode_GND;
double ijbcx_Vbp_ci;
double ijbcx_Vbp_ei;
double ijbcx_Vbi_ci;
double ijbcx_Vbi_ei;
#endif
#if defined(_DYNAMIC)
double qjcx0_t_ii;
#if defined(_DERIVATE)
double qjcx0_t_ii_Vbp_ei;
double qjcx0_t_ii_Vbi_ei;
double qjcx0_t_ii_Vbp_ci;
double qjcx0_t_ii_Vtnode_GND;
double qjcx0_t_ii_Vbi_ci;
#endif
#endif
double c_dummy;
double ibet;
#if defined(_DERIVATE)
double ibet_Vbp_ei;
double ibet_Vtnode_GND;
double ibet_Vbi_ci;
double ibet_Vbi_ei;
#endif
#if defined(_DYNAMIC)
double Qjep;
#if defined(_DERIVATE)
double Qjep_Vbp_ei;
double Qjep_Vbi_ci;
double Qjep_Vtnode_GND;
double Qjep_Vbi_ei;
#endif
#endif
double Cjep;
#if defined(_DERIVATE)
double Cjep_Vbp_ei;
double Cjep_Vbi_ci;
double Cjep_Vtnode_GND;
double Cjep_Vbi_ei;
#endif
double irep;
#if defined(_DERIVATE)
double irep_Vtnode_GND;
double irep_Vbp_ei;
double irep_Vbi_ci;
double irep_Vbi_ei;
#endif
#if defined(_DERIVATE)
double ibep_Vtnode_GND;
double ibep_Vbp_ei;
double ibep_Vbi_ci;
double ibep_Vbi_ei;
#endif
#if defined(_DERIVATE)
double rbi_Vtnode_GND;
double rbi_Vbi_ei;
double rbi_Vbi_ci;
double rbi_Vci_ei;
#endif
double ibh_rec;
#if defined(_DERIVATE)
double ibh_rec_Vtnode_GND;
double ibh_rec_Vbi_ei;
double ibh_rec_Vbi_ci;
double ibh_rec_Vci_ei;
#endif
#if defined(_DERIVATE)
double iavl_Vci_ei;
double iavl_Vtnode_GND;
double iavl_Vbi_ei;
double iavl_Vbi_ci;
#endif
#if defined(_DERIVATE)
double ibci_Vtnode_GND;
double ibci_Vbi_ci;
double ibci_Vbi_ei;
#endif
#if defined(_DYNAMIC)
double qrbi;
#if defined(_DERIVATE)
double qrbi_Vtnode_GND;
double qrbi_Vbi_ei;
double qrbi_Vbi_ci;
double qrbi_Vci_ei;
double qrbi_Vbp_bi;
#endif
#endif
#if defined(_DYNAMIC)
double Crbi;
#if defined(_DERIVATE)
double Crbi_Vtnode_GND;
double Crbi_Vbi_ei;
double Crbi_Vbi_ci;
double Crbi_Vci_ei;
#endif
#endif
#if defined(_DYNAMIC)
double Cdci;
#if defined(_DERIVATE)
double Cdci_Vci_ei;
double Cdci_Vtnode_GND;
double Cdci_Vbi_ci;
double Cdci_Vbi_ei;
#endif
#endif
#if defined(_DYNAMIC)
double Cdei;
#if defined(_DERIVATE)
double Cdei_Vtnode_GND;
double Cdei_Vbi_ci;
double Cdei_Vci_ei;
double Cdei_Vbi_ei;
#endif
#endif
#if defined(_DYNAMIC)
double Qdci;
#if defined(_DERIVATE)
double Qdci_Vci_ei;
double Qdci_Vtnode_GND;
double Qdci_Vbi_ci;
double Qdci_Vbi_ei;
#endif
#endif
double Qdei;
#if defined(_DERIVATE)
double Qdei_Vci_ei;
double Qdei_Vtnode_GND;
double Qdei_Vbi_ci;
double Qdei_Vbi_ei;
#endif
#if defined(_DERIVATE)
double it_Vci_ei;
double it_Vtnode_GND;
double it_Vbi_ei;
double it_Vbi_ci;
#endif
double d_Q0;
double d_Q;
#if defined(_DERIVATE)
double d_Q_Vtnode_GND;
double d_Q_Vbi_ei;
double d_Q_Vbi_ci;
double d_Q_Vci_ei;
#endif
double Q_pT;
#if defined(_DERIVATE)
double Q_pT_Vtnode_GND;
double Q_pT_Vbi_ei;
double Q_pT_Vbi_ci;
double Q_pT_Vci_ei;
#endif
int l_it;
double Qr;
#if defined(_DERIVATE)
double Qr_Vci_ei;
double Qr_Vtnode_GND;
double Qr_Vbi_ci;
double Qr_Vbi_ei;
#endif
double FFdTbfc;
#if defined(_DERIVATE)
double FFdTbfc_Vtnode_GND;
double FFdTbfc_Vbi_ei;
double FFdTbfc_Vbi_ci;
double FFdTbfc_Vci_ei;
#endif
double FFdQbfc;
#if defined(_DERIVATE)
double FFdQbfc_Vtnode_GND;
double FFdQbfc_Vbi_ei;
double FFdQbfc_Vbi_ci;
double FFdQbfc_Vci_ei;
#endif
double FCd_f;
#if defined(_DERIVATE)
double FCd_f_Vci_ei;
double FCd_f_Vtnode_GND;
double FCd_f_Vbi_ei;
double FCd_f_Vbi_ci;
#endif
double FCz_1;
#if defined(_DERIVATE)
double FCz_1_Vci_ei;
double FCz_1_Vtnode_GND;
double FCz_1_Vbi_ei;
double FCz_1_Vbi_ci;
#endif
double FCdfCT_ditf;
#if defined(_DERIVATE)
double FCdfCT_ditf_Vbi_ei;
double FCdfCT_ditf_Vbi_ci;
double FCdfCT_ditf_Vci_ei;
double FCdfCT_ditf_Vtnode_GND;
#endif
double FCdfCT_dw;
#if defined(_DERIVATE)
double FCdfCT_dw_Vbi_ei;
double FCdfCT_dw_Vbi_ci;
double FCdfCT_dw_Vci_ei;
double FCdfCT_dw_Vtnode_GND;
#endif
double lnz;
#if defined(_DERIVATE)
double lnz_Vci_ei;
double lnz_Vtnode_GND;
double lnz_Vbi_ei;
double lnz_Vbi_ci;
#endif
double FCdfc_ditf;
#if defined(_DERIVATE)
double FCdfc_ditf_Vci_ei;
double FCdfc_ditf_Vtnode_GND;
double FCdfc_ditf_Vbi_ei;
double FCdfc_ditf_Vbi_ci;
#endif
double FCdw_ditf;
#if defined(_DERIVATE)
double FCdw_ditf_Vci_ei;
double FCdw_ditf_Vtnode_GND;
double FCdw_ditf_Vbi_ei;
double FCdw_ditf_Vbi_ci;
#endif
double FCdfc_dw;
#if defined(_DERIVATE)
double FCdfc_dw_Vci_ei;
double FCdfc_dw_Vtnode_GND;
double FCdfc_dw_Vbi_ei;
double FCdfc_dw_Vbi_ci;
#endif
double FCf_ci;
#if defined(_DERIVATE)
double FCf_ci_Vci_ei;
double FCf_ci_Vtnode_GND;
double FCf_ci_Vbi_ei;
double FCf_ci_Vbi_ci;
#endif
double FCdf3_dw;
#if defined(_DERIVATE)
double FCdf3_dw_Vci_ei;
double FCdf3_dw_Vtnode_GND;
double FCdf3_dw_Vbi_ei;
double FCdf3_dw_Vbi_ci;
#endif
double FCf3;
#if defined(_DERIVATE)
double FCf3_Vbi_ei;
double FCf3_Vbi_ci;
double FCf3_Vci_ei;
double FCf3_Vtnode_GND;
#endif
double FCdf2_dw;
#if defined(_DERIVATE)
double FCdf2_dw_Vci_ei;
double FCdf2_dw_Vtnode_GND;
double FCdf2_dw_Vbi_ei;
double FCdf2_dw_Vbi_ci;
#endif
double FCf2;
#if defined(_DERIVATE)
double FCf2_Vbi_ei;
double FCf2_Vbi_ci;
double FCf2_Vci_ei;
double FCf2_Vtnode_GND;
#endif
double r;
double a3;
#if defined(_DERIVATE)
double a3_Vbi_ei;
double a3_Vbi_ci;
double a3_Vci_ei;
double a3_Vtnode_GND;
#endif
double a2;
#if defined(_DERIVATE)
double a2_Vbi_ei;
double a2_Vbi_ci;
double a2_Vci_ei;
double a2_Vtnode_GND;
#endif
double x;
#if defined(_DERIVATE)
double x_Vci_ei;
double x_Vtnode_GND;
double x_Vbi_ei;
double x_Vbi_ci;
#endif
double lnzb;
#if defined(_DERIVATE)
double lnzb_Vci_ei;
double lnzb_Vtnode_GND;
double lnzb_Vbi_ei;
double lnzb_Vbi_ci;
#endif
double z;
#if defined(_DERIVATE)
double z_Vci_ei;
double z_Vtnode_GND;
double z_Vbi_ei;
double z_Vbi_ci;
#endif
double FCdf1_dw;
#if defined(_DERIVATE)
double FCdf1_dw_Vci_ei;
double FCdf1_dw_Vtnode_GND;
double FCdf1_dw_Vbi_ei;
double FCdf1_dw_Vbi_ci;
#endif
double FCw2;
#if defined(_DERIVATE)
double FCw2_Vci_ei;
double FCw2_Vtnode_GND;
double FCw2_Vbi_ei;
double FCw2_Vbi_ci;
#endif
double FCf_CT;
#if defined(_DERIVATE)
double FCf_CT_Vbi_ei;
double FCf_CT_Vbi_ci;
double FCf_CT_Vci_ei;
double FCf_CT_Vtnode_GND;
#endif
double FCf1;
#if defined(_DERIVATE)
double FCf1_Vci_ei;
double FCf1_Vtnode_GND;
double FCf1_Vbi_ei;
double FCf1_Vbi_ci;
#endif
double FCda1_dw;
#if defined(_DERIVATE)
double FCda1_dw_Vci_ei;
double FCda1_dw_Vtnode_GND;
double FCda1_dw_Vbi_ei;
double FCda1_dw_Vbi_ci;
#endif
double FCdw_daick;
#if defined(_DERIVATE)
double FCdw_daick_Vci_ei;
double FCdw_daick_Vtnode_GND;
double FCdw_daick_Vbi_ei;
double FCdw_daick_Vbi_ci;
#endif
double FCw;
#if defined(_DERIVATE)
double FCw_Vci_ei;
double FCw_Vtnode_GND;
double FCw_Vbi_ei;
double FCw_Vbi_ci;
#endif
double FCd_a;
#if defined(_DERIVATE)
double FCd_a_Vci_ei;
double FCd_a_Vtnode_GND;
double FCd_a_Vbi_ei;
double FCd_a_Vbi_ci;
#endif
double FCa1;
#if defined(_DERIVATE)
double FCa1_Vci_ei;
double FCa1_Vtnode_GND;
double FCa1_Vbi_ei;
double FCa1_Vbi_ci;
#endif
double FCln;
double FCxb;
double FCxl;
double FCz;
#if defined(_DERIVATE)
double FCz_Vci_ei;
double FCz_Vtnode_GND;
double FCz_Vbi_ei;
double FCz_Vbi_ci;
#endif
double FCdaick_ditf;
#if defined(_DERIVATE)
double FCdaick_ditf_Vci_ei;
double FCdaick_ditf_Vtnode_GND;
double FCdaick_ditf_Vbi_ei;
double FCdaick_ditf_Vbi_ci;
#endif
double FCa_ck;
#if defined(_DERIVATE)
double FCa_ck_Vci_ei;
double FCa_ck_Vtnode_GND;
double FCa_ck_Vbi_ei;
double FCa_ck_Vbi_ci;
#endif
double FCrt;
#if defined(_DERIVATE)
double FCrt_Vci_ei;
double FCrt_Vtnode_GND;
double FCrt_Vbi_ei;
double FCrt_Vbi_ci;
#endif
double FCa;
#if defined(_DERIVATE)
double FCa_Vci_ei;
double FCa_Vtnode_GND;
double FCa_Vbi_ei;
double FCa_Vbi_ci;
#endif
double FFdTcfcT;
#if defined(_DERIVATE)
double FFdTcfcT_Vtnode_GND;
double FFdTcfcT_Vbi_ei;
double FFdTcfcT_Vbi_ci;
double FFdTcfcT_Vci_ei;
#endif
double FFdQcfcT;
#if defined(_DERIVATE)
double FFdQcfcT_Vtnode_GND;
double FFdQcfcT_Vbi_ei;
double FFdQcfcT_Vbi_ci;
double FFdQcfcT_Vci_ei;
#endif
double FFdTcfc;
#if defined(_DERIVATE)
double FFdTcfc_Vtnode_GND;
double FFdTcfc_Vbi_ei;
double FFdTcfc_Vbi_ci;
double FFdTcfc_Vci_ei;
#endif
double FFdQcfc;
#if defined(_DERIVATE)
double FFdQcfc_Vtnode_GND;
double FFdQcfc_Vbi_ei;
double FFdQcfc_Vbi_ci;
double FFdQcfc_Vci_ei;
#endif
double FFdTfhc;
#if defined(_DERIVATE)
double FFdTfhc_Vtnode_GND;
double FFdTfhc_Vbi_ei;
double FFdTfhc_Vbi_ci;
double FFdTfhc_Vci_ei;
#endif
double FFdQfhc;
#if defined(_DERIVATE)
double FFdQfhc_Vtnode_GND;
double FFdQfhc_Vbi_ei;
double FFdQfhc_Vbi_ci;
double FFdQfhc_Vci_ei;
#endif
double FFw;
#if defined(_DERIVATE)
double FFw_Vtnode_GND;
double FFw_Vbi_ei;
double FFw_Vbi_ci;
double FFw_Vci_ei;
#endif
double FFic;
#if defined(_DERIVATE)
double FFic_Vtnode_GND;
double FFic_Vbi_ei;
double FFic_Vbi_ci;
double FFic_Vci_ei;
#endif
double FFdTbfb;
#if defined(_DERIVATE)
double FFdTbfb_Vtnode_GND;
double FFdTbfb_Vbi_ei;
double FFdTbfb_Vbi_ci;
double FFdTbfb_Vci_ei;
#endif
double FFdQbfb;
#if defined(_DERIVATE)
double FFdQbfb_Vtnode_GND;
double FFdQbfb_Vbi_ei;
double FFdQbfb_Vbi_ci;
double FFdQbfb_Vci_ei;
#endif
double FFdib_ditf;
#if defined(_DERIVATE)
double FFdib_ditf_Vtnode_GND;
double FFdib_ditf_Vbi_ei;
double FFdib_ditf_Vbi_ci;
double FFdib_ditf_Vci_ei;
#endif
double FFfcbar;
#if defined(_DERIVATE)
double FFfcbar_Vtnode_GND;
double FFfcbar_Vbi_ei;
double FFfcbar_Vbi_ci;
double FFfcbar_Vci_ei;
#endif
double FFib;
#if defined(_DERIVATE)
double FFib_Vtnode_GND;
double FFib_Vbi_ei;
double FFib_Vbi_ci;
double FFib_Vci_ei;
#endif
double FFdVc_ditf;
#if defined(_DERIVATE)
double FFdVc_ditf_Vtnode_GND;
double FFdVc_ditf_Vbi_ei;
double FFdVc_ditf_Vbi_ci;
double FFdVc_ditf_Vci_ei;
#endif
double FFdVc;
#if defined(_DERIVATE)
double FFdVc_Vtnode_GND;
double FFdVc_Vbi_ei;
double FFdVc_Vbi_ci;
double FFdVc_Vci_ei;
#endif
double FFdQef;
#if defined(_DERIVATE)
double FFdQef_Vtnode_GND;
double FFdQef_Vbi_ei;
double FFdQef_Vbi_ci;
double FFdQef_Vci_ei;
#endif
double FFdTef;
#if defined(_DERIVATE)
double FFdTef_Vtnode_GND;
double FFdTef_Vbi_ei;
double FFdTef_Vbi_ci;
double FFdTef_Vci_ei;
#endif
double FFitf_ick;
#if defined(_DERIVATE)
double FFitf_ick_Vtnode_GND;
double FFitf_ick_Vbi_ei;
double FFitf_ick_Vbi_ci;
double FFitf_ick_Vci_ei;
#endif
double T_fT;
#if defined(_DERIVATE)
double T_fT_Vbi_ei;
double T_fT_Vci_ei;
double T_fT_Vtnode_GND;
double T_fT_Vbi_ci;
#endif
double Q_fT;
#if defined(_DERIVATE)
double Q_fT_Vci_ei;
double Q_fT_Vtnode_GND;
double Q_fT_Vbi_ci;
double Q_fT_Vbi_ei;
#endif
double Qf;
#if defined(_DERIVATE)
double Qf_Vci_ei;
double Qf_Vtnode_GND;
double Qf_Vbi_ci;
double Qf_Vbi_ei;
#endif
#if defined(_DERIVATE)
double Tf_Vbi_ei;
double Tf_Vci_ei;
double Tf_Vtnode_GND;
double Tf_Vbi_ci;
#endif
double Q_bf;
#if defined(_DERIVATE)
double Q_bf_Vtnode_GND;
double Q_bf_Vbi_ei;
double Q_bf_Vbi_ci;
double Q_bf_Vci_ei;
#endif
double itr;
#if defined(_DERIVATE)
double itr_Vci_ei;
double itr_Vtnode_GND;
double itr_Vbi_ci;
double itr_Vbi_ei;
#endif
double itf;
#if defined(_DERIVATE)
double itf_Vci_ei;
double itf_Vtnode_GND;
double itf_Vbi_ei;
double itf_Vbi_ci;
#endif
double a_h;
#if defined(_DERIVATE)
double a_h_Vci_ei;
double a_h_Vtnode_GND;
double a_h_Vbi_ei;
double a_h_Vbi_ci;
#endif
double I_Tf1;
#if defined(_DERIVATE)
double I_Tf1_Vci_ei;
double I_Tf1_Vtnode_GND;
double I_Tf1_Vbi_ei;
double I_Tf1_Vbi_ci;
#endif
double A;
#if defined(_DERIVATE)
double A_Vtnode_GND;
double A_Vbi_ei;
double A_Vbi_ci;
#endif
double Q_p;
#if defined(_DERIVATE)
double Q_p_Vtnode_GND;
double Q_p_Vbi_ei;
double Q_p_Vbi_ci;
#endif
double ick;
#if defined(_DERIVATE)
double ick_Vci_ei;
double ick_Vtnode_GND;
#endif
double ICKa;
#if defined(_DERIVATE)
double ICKa_Vci_ei;
double ICKa_Vtnode_GND;
#endif
double ick2;
#if defined(_DERIVATE)
double ick2_Vci_ei;
double ick2_Vtnode_GND;
#endif
double ick1;
#if defined(_DERIVATE)
double ick1_Vci_ei;
double ick1_Vtnode_GND;
#endif
double Odelck;
double a11;
#if defined(_DERIVATE)
double a11_Vci_ei;
double a11_Vtnode_GND;
#endif
double a1;
#if defined(_DERIVATE)
double a1_Vci_ei;
double a1_Vtnode_GND;
#endif
double vceff;
#if defined(_DERIVATE)
double vceff_Vci_ei;
double vceff_Vtnode_GND;
#endif
double d1;
#if defined(_DERIVATE)
double d1_Vci_ei;
double d1_Vtnode_GND;
#endif
double Ovpt;
double vc;
#if defined(_DERIVATE)
double vc_Vci_ei;
double vc_Vtnode_GND;
#endif
double T_f0;
#if defined(_DERIVATE)
double T_f0_Vtnode_GND;
double T_f0_Vbi_ci;
#endif
double cc;
#if defined(_DERIVATE)
double cc_Vtnode_GND;
double cc_Vbi_ci;
#endif
double Cjcit;
#if defined(_DERIVATE)
double Cjcit_Vtnode_GND;
double Cjcit_Vbi_ci;
#endif
double b_q;
#if defined(_DERIVATE)
double b_q_Vtnode_GND;
double b_q_Vbi_ei;
double b_q_Vbi_ci;
#endif
double Q_bpt;
#if defined(_DERIVATE)
double Q_bpt_Vtnode_GND;
#endif
double Q_0;
#if defined(_DERIVATE)
double Q_0_Vtnode_GND;
double Q_0_Vbi_ei;
double Q_0_Vbi_ci;
#endif
double a_bpt;
double Qjci;
#if defined(_DERIVATE)
double Qjci_Vbi_ei;
double Qjci_Vtnode_GND;
double Qjci_Vbi_ci;
#endif
double DQ_j3;
#if defined(_DERIVATE)
double DQ_j3_Vsi_ci;
double DQ_j3_Vb_ci;
double DQ_j3_Vbp_ci;
double DQ_j3_Vtnode_GND;
double DQ_j3_Vbi_ci;
#endif
double DQ_j2;
#if defined(_DERIVATE)
double DQ_j2_Vsi_ci;
double DQ_j2_Vb_ci;
double DQ_j2_Vbp_ci;
double DQ_j2_Vtnode_GND;
double DQ_j2_Vbi_ci;
#endif
double DQ_j1;
#if defined(_DERIVATE)
double DQ_j1_Vsi_ci;
double DQ_j1_Vb_ci;
double DQ_j1_Vbp_ci;
double DQ_j1_Vtnode_GND;
double DQ_j1_Vbi_ci;
#endif
double Cjci;
#if defined(_DERIVATE)
double Cjci_Vbi_ei;
double Cjci_Vtnode_GND;
double Cjci_Vbi_ci;
#endif
double DC_j3;
#if defined(_DERIVATE)
double DC_j3_Vsi_ci;
double DC_j3_Vb_ci;
double DC_j3_Vbp_ci;
double DC_j3_Vtnode_GND;
double DC_j3_Vbi_ci;
#endif
double DC_j2;
#if defined(_DERIVATE)
double DC_j2_Vsi_ci;
double DC_j2_Vb_ci;
double DC_j2_Vbp_ci;
double DC_j2_Vtnode_GND;
double DC_j2_Vbi_ci;
#endif
double DC_j1;
#if defined(_DERIVATE)
double DC_j1_Vsi_ci;
double DC_j1_Vb_ci;
double DC_j1_Vbp_ci;
double DC_j1_Vtnode_GND;
double DC_j1_Vbi_ci;
#endif
double Dzr1;
double Dz1;
double DCln2;
#if defined(_DERIVATE)
double DCln2_Vsi_ci;
double DCln2_Vb_ci;
double DCln2_Vbp_ci;
double DCln2_Vtnode_GND;
double DCln2_Vbi_ci;
#endif
double DCln1;
#if defined(_DERIVATE)
double DCln1_Vsi_ci;
double DCln1_Vb_ci;
double DCln1_Vbp_ci;
double DCln1_Vtnode_GND;
double DCln1_Vbi_ci;
#endif
double Dv_j4;
#if defined(_DERIVATE)
double Dv_j4_Vsi_ci;
double Dv_j4_Vb_ci;
double Dv_j4_Vbp_ci;
double Dv_j4_Vbi_ci;
double Dv_j4_Vtnode_GND;
#endif
double Dv_j2;
#if defined(_DERIVATE)
double Dv_j2_Vsi_ci;
double Dv_j2_Vb_ci;
double Dv_j2_Vbp_ci;
double Dv_j2_Vtnode_GND;
double Dv_j2_Vbi_ci;
#endif
double De_2;
#if defined(_DERIVATE)
double De_2_Vsi_ci;
double De_2_Vb_ci;
double De_2_Vbp_ci;
double De_2_Vtnode_GND;
double De_2_Vbi_ci;
#endif
double Dv_r;
#if defined(_DERIVATE)
double Dv_r_Vsi_ci;
double Dv_r_Vb_ci;
double Dv_r_Vbp_ci;
double Dv_r_Vtnode_GND;
double Dv_r_Vbi_ci;
#endif
double Da;
#if defined(_DERIVATE)
double Da_Vtnode_GND;
#endif
double Dv_j1;
#if defined(_DERIVATE)
double Dv_j1_Vsi_ci;
double Dv_j1_Vb_ci;
double Dv_j1_Vbp_ci;
double Dv_j1_Vtnode_GND;
double Dv_j1_Vbi_ci;
#endif
double De_1;
#if defined(_DERIVATE)
double De_1_Vsi_ci;
double De_1_Vb_ci;
double De_1_Vbp_ci;
double De_1_Vtnode_GND;
double De_1_Vbi_ci;
#endif
double De;
#if defined(_DERIVATE)
double De_Vsi_ci;
double De_Vb_ci;
double De_Vbp_ci;
double De_Vtnode_GND;
double De_Vbi_ci;
#endif
double Dv_e;
#if defined(_DERIVATE)
double Dv_e_Vsi_ci;
double Dv_e_Vb_ci;
double Dv_e_Vbp_ci;
double Dv_e_Vtnode_GND;
double Dv_e_Vbi_ci;
#endif
double DC_c;
#if defined(_DERIVATE)
double DC_c_Vtnode_GND;
#endif
double DC_max;
#if defined(_DERIVATE)
double DC_max_Vtnode_GND;
#endif
double DV_f;
#if defined(_DERIVATE)
double DV_f_Vtnode_GND;
#endif
double Dv_p;
#if defined(_DERIVATE)
double Dv_p_Vtnode_GND;
#endif
double Dz_r;
double vj_z;
#if defined(_DERIVATE)
double vj_z_Vtnode_GND;
double vj_z_Vbi_ei;
#endif
double vj;
#if defined(_DERIVATE)
double vj_Vtnode_GND;
double vj_Vbi_ei;
#endif
double hjei_vbe;
#if defined(_DERIVATE)
double hjei_vbe_Vtnode_GND;
double hjei_vbe_Vbi_ei;
#endif
double Qjei;
#if defined(_DERIVATE)
double Qjei_Vtnode_GND;
double Qjei_Vbi_ei;
#endif
double DFQ_j;
#if defined(_DERIVATE)
double DFQ_j_Vsi_ci;
double DFQ_j_Vb_ci;
double DFQ_j_Vbp_ci;
double DFQ_j_Vbp_ei;
double DFQ_j_Vbi_ci;
double DFQ_j_Vtnode_GND;
double DFQ_j_Vbi_ei;
#endif
double Cjei;
#if defined(_DERIVATE)
double Cjei_Vtnode_GND;
double Cjei_Vbi_ei;
#endif
double DFC_j1;
#if defined(_DERIVATE)
double DFC_j1_Vsi_ci;
double DFC_j1_Vb_ci;
double DFC_j1_Vbp_ci;
double DFC_j1_Vbp_ei;
double DFC_j1_Vbi_ci;
double DFC_j1_Vtnode_GND;
double DFC_j1_Vbi_ei;
#endif
double DFb;
#if defined(_DERIVATE)
double DFb_Vsi_ci;
double DFb_Vb_ci;
double DFb_Vbp_ci;
double DFb_Vbp_ei;
double DFb_Vbi_ci;
double DFb_Vtnode_GND;
double DFb_Vbi_ei;
#endif
double DFdvj_dv;
#if defined(_DERIVATE)
double DFdvj_dv_Vsi_ci;
double DFdvj_dv_Vb_ci;
double DFdvj_dv_Vbp_ci;
double DFdvj_dv_Vbp_ei;
double DFdvj_dv_Vbi_ci;
double DFdvj_dv_Vtnode_GND;
double DFdvj_dv_Vbi_ei;
#endif
double DFv_j;
#if defined(_DERIVATE)
double DFv_j_Vsi_ci;
double DFv_j_Vb_ci;
double DFv_j_Vbp_ci;
double DFv_j_Vbp_ei;
double DFv_j_Vbi_ci;
double DFv_j_Vtnode_GND;
double DFv_j_Vbi_ei;
#endif
double DFs_q2;
#if defined(_DERIVATE)
double DFs_q2_Vsi_ci;
double DFs_q2_Vb_ci;
double DFs_q2_Vbp_ci;
double DFs_q2_Vbp_ei;
double DFs_q2_Vbi_ci;
double DFs_q2_Vtnode_GND;
double DFs_q2_Vbi_ei;
#endif
double DFs_q;
#if defined(_DERIVATE)
double DFs_q_Vsi_ci;
double DFs_q_Vb_ci;
double DFs_q_Vbp_ci;
double DFs_q_Vbp_ei;
double DFs_q_Vbi_ci;
double DFs_q_Vtnode_GND;
double DFs_q_Vbi_ei;
#endif
double DFv_e;
#if defined(_DERIVATE)
double DFv_e_Vsi_ci;
double DFv_e_Vb_ci;
double DFv_e_Vbp_ci;
double DFv_e_Vbp_ei;
double DFv_e_Vbi_ci;
double DFv_e_Vtnode_GND;
double DFv_e_Vbi_ei;
#endif
double DFV_f;
#if defined(_DERIVATE)
double DFV_f_Vtnode_GND;
#endif
double i_0r;
#if defined(_DERIVATE)
double i_0r_Vtnode_GND;
double i_0r_Vbi_ci;
#endif
double i_0f;
#if defined(_DERIVATE)
double i_0f_Vtnode_GND;
double i_0f_Vbi_ei;
#endif
double VT_f;
#if defined(_DERIVATE)
double VT_f_Vtnode_GND;
#endif
double Tr;
double Orci0_t;
#if defined(_DERIVATE)
double Orci0_t_Vtnode_GND;
#endif
double irei;
#if defined(_DERIVATE)
double irei_Vtnode_GND;
double irei_Vbi_ei;
#endif
#if defined(_DERIVATE)
double ibei_Vtnode_GND;
double ibei_Vbi_ei;
#endif
double le;
#if defined(_DERIVATE)
double le_Vsi_ci;
double le_Vbp_ci;
double le_Vbp_ei;
double le_Vbi_ci;
double le_Vbi_ei;
double le_Vtnode_GND;
#endif
double DIOY;
#if defined(_DERIVATE)
double DIOY_Vsi_ci;
double DIOY_Vbp_ci;
double DIOY_Vbp_ei;
double DIOY_Vbi_ci;
double DIOY_Vbi_ei;
double DIOY_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rth_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hfc_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hfe_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hf0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double hjei0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ahjei_t_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vpts_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjs0_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vds_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double tsf_t_Vtnode_GND;
#endif
#endif
#if defined(_DERIVATE)
double re_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rbx_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rcx_t_Vtnode_GND;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjcx02_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cjcx01_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vptcx_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double cratio_t_Vtnode_GND;
#endif
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
double vdcx_t_Vtnode_GND;
#endif
#endif
#if defined(_DERIVATE)
double abet_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibets_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ajep_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjep0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdep_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rbi0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qavl_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double favl_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double thcs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double t0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vptci_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjci0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdci_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vces_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double rci0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double c10_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double tef0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vlim_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qp0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double iscs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double itss_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibcxs_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibcis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ireps_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibeps_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ireis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ibeis_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double a_Vbi_ei;
double a_Vbi_ci;
double a_Vci_ei;
double a_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double r_VgVT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double V_gT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ajei_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cjei0_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdei_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdt_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vdjt_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vgbe_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vge_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double vgb_t_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k2_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double k1_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ln_qtt0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double qtt0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double dT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double VT_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double Tdev_Vtnode_GND;
#endif
double Vsici;
#if defined(_DERIVATE)
double Vsici_Vsi_ci;
#endif
#if defined(_DYNAMIC)
double Vbci;
#if defined(_DERIVATE)
double Vbci_Vb_ci;
#endif
#endif
double Vbpci;
#if defined(_DERIVATE)
double Vbpci_Vbp_ci;
#endif
double Vbpei;
#if defined(_DERIVATE)
double Vbpei_Vbp_ei;
#endif
double Vciei;
#if defined(_DERIVATE)
double Vciei_Vci_ei;
#endif
double Vbici;
#if defined(_DERIVATE)
double Vbici_Vbi_ci;
#endif
double Vbiei;
#if defined(_DERIVATE)
double Vbiei_Vbi_ei;
#endif
int type;
type=1;
#if defined(_DERIVATE)
Vbiei_Vbi_ei=(type);
#endif
Vbiei=(type*BP(bi,ei));
#if defined(_DERIVATE)
Vbici_Vbi_ci=(type);
#endif
Vbici=(type*BP(bi,ci));
#if defined(_DERIVATE)
Vciei_Vci_ei=(type);
#endif
Vciei=(type*BP(ci,ei));
#if defined(_DERIVATE)
Vbpei_Vbp_ei=(type);
#endif
Vbpei=(type*BP(bp,ei));
#if defined(_DERIVATE)
Vbpci_Vbp_ci=(type);
#endif
Vbpci=(type*BP(bp,ci));
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Vbci_Vb_ci=(type);
#endif
Vbci=(type*BP(b,ci));
#endif
#if defined(_DERIVATE)
Vsici_Vsi_ci=(type);
#endif
Vsici=(type*BP(si,ci));
if
((flsh!=0)&&(rth>=0.001))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=1.0;
#endif
Tdev=((Tamb+dt)+NP(tnode));
if
(Tdev<((-100.0)+273.15))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=0.0;
#endif
Tdev=((-100.0)+273.15);
}
else
{
if
(Tdev>(326.85+273.15))
{
#if defined(_DERIVATE)
Tdev_Vtnode_GND=0.0;
#endif
Tdev=(326.85+273.15);
}
}
#if defined(_DERIVATE)
VT_Vtnode_GND=((1.3806503e-23*Tdev_Vtnode_GND)/1.602176462e-19);
#endif
VT=((1.3806503e-23*Tdev)/1.602176462e-19);
#if defined(_DERIVATE)
dT_Vtnode_GND=Tdev_Vtnode_GND;
#endif
dT=(Tdev-Tnom);
#if defined(_DERIVATE)
qtt0_Vtnode_GND=(Tdev_Vtnode_GND/Tnom);
#endif
qtt0=(Tdev/Tnom);
{
double m00_logE(d00_logE0,qtt0)
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,qtt0)
#endif
#if defined(_DERIVATE)
ln_qtt0_Vtnode_GND=qtt0_Vtnode_GND*d10_logE0;
#endif
ln_qtt0=d00_logE0;
}
{
double m00_logE(d00_logE0,Tdev)
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,Tdev)
#endif
#if defined(_DERIVATE)
k1_Vtnode_GND=(((f1vg*Tdev_Vtnode_GND)*d00_logE0)+((f1vg*Tdev)*Tdev_Vtnode_GND*d10_logE0));
#endif
k1=((f1vg*Tdev)*d00_logE0);
}
#if defined(_DERIVATE)
k2_Vtnode_GND=(f2vg*Tdev_Vtnode_GND);
#endif
k2=(f2vg*Tdev);
#if defined(_DERIVATE)
vgb_t_Vtnode_GND=(k1_Vtnode_GND+k2_Vtnode_GND);
#endif
vgb_t=((vgb+k1)+k2);
#if defined(_DERIVATE)
vge_t_Vtnode_GND=(k1_Vtnode_GND+k2_Vtnode_GND);
#endif
vge_t=((vge+k1)+k2);
#if defined(_DERIVATE)
vgbe_t_Vtnode_GND=((vgb_t_Vtnode_GND+vge_t_Vtnode_GND)/2);
#endif
vgbe_t=((vgb_t+vge_t)/2);
if
(cjei0>0.0)
{
{
double m00_exp(d00_exp0,((vdei*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdei)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=(((vdj0*qtt0_Vtnode_GND)+(vgbe0*(-qtt0_Vtnode_GND)))-(((mg*VT_Vtnode_GND)*ln_qtt0)+((mg*VT)*ln_qtt0_Vtnode_GND)));
#endif
vdjt=(((vdj0*qtt0)+(vgbe0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_sqrt(d10_sqrt1,d00_sqrt1,(1+(4*d00_exp0)))
double m10_logE(d10_logE2,d00_logE2,(0.5*(1+d00_sqrt1)))
#endif
#if defined(_DERIVATE)
vdt_Vtnode_GND=(vdjt_Vtnode_GND+(((2*VT_Vtnode_GND)*d00_logE2)+((2*VT)*(0.5*(4*(((-vdjt_Vtnode_GND)*VT-(-vdjt)*VT_Vtnode_GND)/VT/VT)*d10_exp0)*d10_sqrt1)*d10_logE2)));
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdei_t_Vtnode_GND=vdt_Vtnode_GND;
#endif
vdei_t=vdt;
{
double m00_logE(d00_logE0,(vdei/vdei_t))
double m00_exp(d00_exp1,(zei*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vdei/vdei_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
cjei0_t_Vtnode_GND=(cjei0*(zei*(-vdei*vdei_t_Vtnode_GND/vdei_t/vdei_t)*d10_logE0)*d10_exp1);
#endif
cjei0_t=(cjei0*d00_exp1);
}
if
(1==1)
{
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=((ajei*vdei_t_Vtnode_GND)/vdei);
#endif
ajei_t=((ajei*vdei_t)/vdei);
}
else
{
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=0.0;
#endif
ajei_t=ajei;
}
}
else
{
#if defined(_DERIVATE)
cjei0_t_Vtnode_GND=0.0;
#endif
cjei0_t=cjei0;
#if defined(_DERIVATE)
vdei_t_Vtnode_GND=0.0;
#endif
vdei_t=vdei;
#if defined(_DERIVATE)
ajei_t_Vtnode_GND=0.0;
#endif
ajei_t=ajei;
}
if
((flcomp==0.0)||(flcomp==2.1))
{
#if defined(_DERIVATE)
V_gT_Vtnode_GND=((((3.0*VT_Vtnode_GND)*ln_qtt0)+((3.0*VT)*ln_qtt0_Vtnode_GND))+(vgb*qtt0_Vtnode_GND));
#endif
V_gT=(((3.0*VT)*ln_qtt0)+(vgb*(qtt0-1.0)));
#if defined(_DERIVATE)
r_VgVT_Vtnode_GND=((V_gT_Vtnode_GND*VT-V_gT*VT_Vtnode_GND)/VT/VT);
#endif
r_VgVT=(V_gT/VT);
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(((mcf*r_VgVT_Vtnode_GND)/mbei)-(alb*dT_Vtnode_GND));
#endif
a=(((mcf*r_VgVT)/mbei)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibeis_t_Vtnode_GND=(ibeis*a_Vtnode_GND*d10_exp0);
#endif
ibeis_t=(ibeis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(((mcf*r_VgVT_Vtnode_GND)/mrei)-(alb*dT_Vtnode_GND));
#endif
a=(((mcf*r_VgVT)/mrei)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=(ireis*a_Vtnode_GND*d10_exp0);
#endif
ireis_t=(ireis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(((mcf*r_VgVT_Vtnode_GND)/mbep)-(alb*dT_Vtnode_GND));
#endif
a=(((mcf*r_VgVT)/mbep)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibeps_t_Vtnode_GND=(ibeps*a_Vtnode_GND*d10_exp0);
#endif
ibeps_t=(ibeps*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(((mcf*r_VgVT_Vtnode_GND)/mrep)-(alb*dT_Vtnode_GND));
#endif
a=(((mcf*r_VgVT)/mrep)-(alb*dT));
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=(ireps*a_Vtnode_GND*d10_exp0);
#endif
ireps_t=(ireps*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(r_VgVT_Vtnode_GND/mbci);
#endif
a=(r_VgVT/mbci);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibcis_t_Vtnode_GND=(ibcis*a_Vtnode_GND*d10_exp0);
#endif
ibcis_t=(ibcis*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(r_VgVT_Vtnode_GND/mbcx);
#endif
a=(r_VgVT/mbcx);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibcxs_t_Vtnode_GND=(ibcxs*a_Vtnode_GND*d10_exp0);
#endif
ibcxs_t=(ibcxs*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(r_VgVT_Vtnode_GND/msf);
#endif
a=(r_VgVT/msf);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
itss_t_Vtnode_GND=(itss*a_Vtnode_GND*d10_exp0);
#endif
itss_t=(itss*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(r_VgVT_Vtnode_GND/msc);
#endif
a=(r_VgVT/msc);
{
double m00_exp(d00_exp0,a)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
iscs_t_Vtnode_GND=(iscs*a_Vtnode_GND*d10_exp0);
#endif
iscs_t=(iscs*d00_exp0);
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(vdei_t_Vtnode_GND/vdei);
#endif
a=(vdei_t/vdei);
#if defined(_DERIVATE)
qp0_t_Vtnode_GND=(qp0*((0.5*zei)*(-a_Vtnode_GND)));
#endif
qp0_t=(qp0*(1.0+((0.5*zei)*(1.0-a))));
{
double m00_exp(d00_exp0,(zetaci*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(((vlim*(-(alvs*dT_Vtnode_GND)))*d00_exp0)+((vlim*(1.0-(alvs*dT)))*(zetaci*ln_qtt0_Vtnode_GND)*d10_exp0));
#endif
a=((vlim*(1.0-(alvs*dT)))*d00_exp0);
}
#if defined(_DERIVATE)
k_Vtnode_GND=(((a_Vtnode_GND-VT_Vtnode_GND)*VT-(a-VT)*VT_Vtnode_GND)/VT/VT);
#endif
k=((a-VT)/VT);
if
(k<11.0)
{
{
double m00_exp(d00_exp0,k)
double m00_logE(d00_logE1,(1.0+d00_exp0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_logE(d10_logE1,d00_logE1,(1.0+d00_exp0))
#endif
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=(VT_Vtnode_GND+((VT_Vtnode_GND*d00_logE1)+(VT*k_Vtnode_GND*d10_exp0*d10_logE1)));
#endif
vlim_t=(VT+(VT*d00_logE1));
}
}
else
{
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=a_Vtnode_GND;
#endif
vlim_t=a;
}
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(alb*dT_Vtnode_GND);
#endif
a=(1.0+(alb*dT));
{
double m00_sqrt(d00_sqrt0,((a*a)+0.01))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((a*a)+0.01))
#endif
#if defined(_DERIVATE)
k_Vtnode_GND=(0.5*(a_Vtnode_GND+((a_Vtnode_GND*a)+(a*a_Vtnode_GND))*d10_sqrt0));
#endif
k=(0.5*(a+d00_sqrt0));
}
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=(((tef0*qtt0_Vtnode_GND)*k-(tef0*qtt0)*k_Vtnode_GND)/k/k);
#endif
tef0_t=((tef0*qtt0)/k);
}
else
{
{
double m00_exp(d00_exp0,((zetabet*ln_qtt0)+((vge/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibeis_t_Vtnode_GND=(ibeis*((zetabet*ln_qtt0_Vtnode_GND)+(((-vge*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vge/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ibeis_t=(ibeis*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((mg/mrei)*ln_qtt0)+((vgbe0/(mrei*VT))*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=(ireis*(((mg/mrei)*ln_qtt0_Vtnode_GND)+(((-vgbe0*(mrei*VT_Vtnode_GND)/(mrei*VT)/(mrei*VT))*(qtt0-1))+((vgbe0/(mrei*VT))*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ireis_t=(ireis*d00_exp0);
}
}
else
{
{
double m00_exp(d00_exp0,(((0.5*mg)*ln_qtt0)+(((0.5*vgbe0)/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireis_t_Vtnode_GND=(ireis*(((0.5*mg)*ln_qtt0_Vtnode_GND)+(((-(0.5*vgbe0)*VT_Vtnode_GND/VT/VT)*(qtt0-1))+(((0.5*vgbe0)/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ireis_t=(ireis*d00_exp0);
}
}
{
double m00_exp(d00_exp0,((zetabet*ln_qtt0)+((vge/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibeps_t_Vtnode_GND=(ibeps*((zetabet*ln_qtt0_Vtnode_GND)+(((-vge*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vge/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ibeps_t=(ibeps*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((mg/mrep)*ln_qtt0)+((vgbe0/(mrep*VT))*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=(ireps*(((mg/mrep)*ln_qtt0_Vtnode_GND)+(((-vgbe0*(mrep*VT_Vtnode_GND)/(mrep*VT)/(mrep*VT))*(qtt0-1))+((vgbe0/(mrep*VT))*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ireps_t=(ireps*d00_exp0);
}
}
else
{
{
double m00_exp(d00_exp0,(((0.5*mg)*ln_qtt0)+(((0.5*vgbe0)/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ireps_t_Vtnode_GND=(ireps*(((0.5*mg)*ln_qtt0_Vtnode_GND)+(((-(0.5*vgbe0)*VT_Vtnode_GND/VT/VT)*(qtt0-1))+(((0.5*vgbe0)/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ireps_t=(ireps*d00_exp0);
}
}
{
double m00_exp(d00_exp0,((zetabci*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibcis_t_Vtnode_GND=(ibcis*((zetabci*ln_qtt0_Vtnode_GND)+(((-vgc*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vgc/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ibcis_t=(ibcis*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetabcxt*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibcxs_t_Vtnode_GND=(ibcxs*((zetabcxt*ln_qtt0_Vtnode_GND)+(((-vgc*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vgc/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
ibcxs_t=(ibcxs*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetasct*ln_qtt0)+((vgc/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
itss_t_Vtnode_GND=(itss*((zetasct*ln_qtt0_Vtnode_GND)+(((-vgc*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vgc/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
itss_t=(itss*d00_exp0);
}
{
double m00_exp(d00_exp0,((zetasct*ln_qtt0)+((vgs/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
iscs_t_Vtnode_GND=(iscs*((zetasct*ln_qtt0_Vtnode_GND)+(((-vgs*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vgs/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
iscs_t=(iscs*d00_exp0);
}
{
double m00_logE(d00_logE0,(vdei_t/vdei))
double m00_exp(d00_exp1,(zei*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vdei_t/vdei))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=0.0;
a_Vtnode_GND=(zei*(vdei_t_Vtnode_GND/vdei)*d10_logE0)*d10_exp1;
#endif
a=d00_exp1;
}
#if defined(_DERIVATE)
qp0_t_Vtnode_GND=(qp0*(-a_Vtnode_GND));
#endif
qp0_t=(qp0*(2.0-a));
{
double m00_exp(d00_exp0,((zetaci-avs)*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
vlim_t_Vtnode_GND=(vlim*((zetaci-avs)*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
vlim_t=(vlim*d00_exp0);
}
if
(flcomp>=2.3)
{
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=0.0;
#endif
tef0_t=tef0;
}
else
{
zetatef=((zetabet-zetact)-0.5);
dvg0=(vgb-vge);
{
double m00_exp(d00_exp0,((zetatef*ln_qtt0)-((dvg0/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
tef0_t_Vtnode_GND=(tef0*((zetatef*ln_qtt0_Vtnode_GND)-(((-dvg0*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((dvg0/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
tef0_t=(tef0*d00_exp0);
}
}
}
{
double m00_exp(d00_exp0,((zetact*ln_qtt0)+((vgb/VT)*(qtt0-1))))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
c10_t_Vtnode_GND=(c10*((zetact*ln_qtt0_Vtnode_GND)+(((-vgb*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((vgb/VT)*qtt0_Vtnode_GND)))*d10_exp0);
#endif
c10_t=(c10*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetaci*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
rci0_t_Vtnode_GND=(rci0*(zetaci*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
rci0_t=(rci0*d00_exp0);
}
#if defined(_DERIVATE)
vces_t_Vtnode_GND=(vces*(alces*dT_Vtnode_GND));
#endif
vces_t=(vces*(1+(alces*dT)));
if
(cjci0>0.0)
{
{
double m00_exp(d00_exp0,((vdci*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdci)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=(((vdj0*qtt0_Vtnode_GND)+(vgbc0*(-qtt0_Vtnode_GND)))-(((mg*VT_Vtnode_GND)*ln_qtt0)+((mg*VT)*ln_qtt0_Vtnode_GND)));
#endif
vdjt=(((vdj0*qtt0)+(vgbc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_sqrt(d10_sqrt1,d00_sqrt1,(1+(4*d00_exp0)))
double m10_logE(d10_logE2,d00_logE2,(0.5*(1+d00_sqrt1)))
#endif
#if defined(_DERIVATE)
vdt_Vtnode_GND=(vdjt_Vtnode_GND+(((2*VT_Vtnode_GND)*d00_logE2)+((2*VT)*(0.5*(4*(((-vdjt_Vtnode_GND)*VT-(-vdjt)*VT_Vtnode_GND)/VT/VT)*d10_exp0)*d10_sqrt1)*d10_logE2)));
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdci_t_Vtnode_GND=vdt_Vtnode_GND;
#endif
vdci_t=vdt;
{
double m00_logE(d00_logE0,(vdci/vdci_t))
double m00_exp(d00_exp1,(zci*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vdci/vdci_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
cjci0_t_Vtnode_GND=(cjci0*(zci*(-vdci*vdci_t_Vtnode_GND/vdci_t/vdci_t)*d10_logE0)*d10_exp1);
#endif
cjci0_t=(cjci0*d00_exp1);
}
if
(0==1)
{
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=((vptci*vdci_t_Vtnode_GND)/vdci);
#endif
vptci_t=((vptci*vdci_t)/vdci);
}
else
{
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=0.0;
#endif
vptci_t=vptci;
}
}
else
{
#if defined(_DERIVATE)
cjci0_t_Vtnode_GND=0.0;
#endif
cjci0_t=cjci0;
#if defined(_DERIVATE)
vdci_t_Vtnode_GND=0.0;
#endif
vdci_t=vdci;
#if defined(_DERIVATE)
vptci_t_Vtnode_GND=0.0;
#endif
vptci_t=vptci;
}
#if defined(_DERIVATE)
t0_t_Vtnode_GND=(t0*((alt0*dT_Vtnode_GND)+(((kt0*dT_Vtnode_GND)*dT)+((kt0*dT)*dT_Vtnode_GND))));
#endif
t0_t=(t0*((1+(alt0*dT))+((kt0*dT)*dT)));
{
double m00_exp(d00_exp0,((zetaci-1)*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
thcs_t_Vtnode_GND=(thcs*((zetaci-1)*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
thcs_t=(thcs*d00_exp0);
}
{
double m00_exp(d00_exp0,(alfav*dT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
favl_t_Vtnode_GND=(favl*(alfav*dT_Vtnode_GND)*d10_exp0);
#endif
favl_t=(favl*d00_exp0);
}
{
double m00_exp(d00_exp0,(alqav*dT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
qavl_t_Vtnode_GND=(qavl*(alqav*dT_Vtnode_GND)*d10_exp0);
#endif
qavl_t=(qavl*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetarbi*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
rbi0_t_Vtnode_GND=(rbi0*(zetarbi*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
rbi0_t=(rbi0*d00_exp0);
}
if
(cjep0>0.0)
{
{
double m00_exp(d00_exp0,((vdep*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdep)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#if defined(_DERIVATE)
vdjt_Vtnode_GND=(((vdj0*qtt0_Vtnode_GND)+(vgbe0*(-qtt0_Vtnode_GND)))-(((mg*VT_Vtnode_GND)*ln_qtt0)+((mg*VT)*ln_qtt0_Vtnode_GND)));
#endif
vdjt=(((vdj0*qtt0)+(vgbe0*(1-qtt0)))-((mg*VT)*ln_qtt0));
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_sqrt(d10_sqrt1,d00_sqrt1,(1+(4*d00_exp0)))
double m10_logE(d10_logE2,d00_logE2,(0.5*(1+d00_sqrt1)))
#endif
#if defined(_DERIVATE)
vdt_Vtnode_GND=(vdjt_Vtnode_GND+(((2*VT_Vtnode_GND)*d00_logE2)+((2*VT)*(0.5*(4*(((-vdjt_Vtnode_GND)*VT-(-vdjt)*VT_Vtnode_GND)/VT/VT)*d10_exp0)*d10_sqrt1)*d10_logE2)));
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#if defined(_DERIVATE)
vdep_t_Vtnode_GND=vdt_Vtnode_GND;
#endif
vdep_t=vdt;
{
double m00_logE(d00_logE0,(vdep/vdep_t))
double m00_exp(d00_exp1,(zep*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vdep/vdep_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
cjep0_t_Vtnode_GND=(cjep0*(zep*(-vdep*vdep_t_Vtnode_GND/vdep_t/vdep_t)*d10_logE0)*d10_exp1);
#endif
cjep0_t=(cjep0*d00_exp1);
}
if
(1==1)
{
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=((ajep*vdep_t_Vtnode_GND)/vdep);
#endif
ajep_t=((ajep*vdep_t)/vdep);
}
else
{
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=0.0;
#endif
ajep_t=ajep;
}
}
else
{
#if defined(_DERIVATE)
cjep0_t_Vtnode_GND=0.0;
#endif
cjep0_t=cjep0;
#if defined(_DERIVATE)
vdep_t_Vtnode_GND=0.0;
#endif
vdep_t=vdep;
#if defined(_DERIVATE)
ajep_t_Vtnode_GND=0.0;
#endif
ajep_t=ajep;
}
if
((Vbpei<0.0)||(Vbiei<0.0))
{
double a_eg;
double ab;
double aa;
#if defined(_DERIVATE)
double a_eg_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ab_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double aa_Vtnode_GND;
#endif
#if defined(_DERIVATE)
ab_Vtnode_GND=0.0;
#endif
ab=1.0;
#if defined(_DERIVATE)
aa_Vtnode_GND=0.0;
#endif
aa=1.0;
#if defined(_DERIVATE)
a_eg_Vtnode_GND=(-vgbe_t0*vgbe_t_Vtnode_GND/vgbe_t/vgbe_t);
#endif
a_eg=(vgbe_t0/vgbe_t);
if
(((tunode==1)&&(cjep0>0.0))&&(vdep>0.0))
{
{
double m00_sqrt(d00_sqrt0,a_eg)
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,a_eg)
#endif
#if defined(_DERIVATE)
ab_Vtnode_GND=((((((((cjep0_t_Vtnode_GND/cjep0)*d00_sqrt0)+((cjep0_t/cjep0)*a_eg_Vtnode_GND*d10_sqrt0))*vdep_t)+(((cjep0_t/cjep0)*d00_sqrt0)*vdep_t_Vtnode_GND))*vdep_t)+((((cjep0_t/cjep0)*d00_sqrt0)*vdep_t)*vdep_t_Vtnode_GND))/(vdep*vdep));
#endif
ab=(((((cjep0_t/cjep0)*d00_sqrt0)*vdep_t)*vdep_t)/(vdep*vdep));
}
{
double m00_pow(d00_pow0,a_eg,(-1.5))
#if defined(_DERIVATE)
double m10_pow(d10_pow0,d00_pow0,a_eg,(-1.5))
#endif
#if defined(_DERIVATE)
aa_Vtnode_GND=(((((-vdep*vdep_t_Vtnode_GND/vdep_t/vdep_t)*(cjep0/cjep0_t))+((vdep/vdep_t)*(-cjep0*cjep0_t_Vtnode_GND/cjep0_t/cjep0_t)))*d00_pow0)+(((vdep/vdep_t)*(cjep0/cjep0_t))*(d10_pow0*a_eg_Vtnode_GND)));
#endif
aa=(((vdep/vdep_t)*(cjep0/cjep0_t))*d00_pow0);
}
}
else
{
if
(((tunode==0)&&(cjei0>0.0))&&(vdei>0.0))
{
{
double m00_sqrt(d00_sqrt0,a_eg)
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,a_eg)
#endif
#if defined(_DERIVATE)
ab_Vtnode_GND=((((((((cjei0_t_Vtnode_GND/cjei0)*d00_sqrt0)+((cjei0_t/cjei0)*a_eg_Vtnode_GND*d10_sqrt0))*vdei_t)+(((cjei0_t/cjei0)*d00_sqrt0)*vdei_t_Vtnode_GND))*vdei_t)+((((cjei0_t/cjei0)*d00_sqrt0)*vdei_t)*vdei_t_Vtnode_GND))/(vdei*vdei));
#endif
ab=(((((cjei0_t/cjei0)*d00_sqrt0)*vdei_t)*vdei_t)/(vdei*vdei));
}
{
double m00_pow(d00_pow0,a_eg,(-1.5))
#if defined(_DERIVATE)
double m10_pow(d10_pow0,d00_pow0,a_eg,(-1.5))
#endif
#if defined(_DERIVATE)
aa_Vtnode_GND=(((((-vdei*vdei_t_Vtnode_GND/vdei_t/vdei_t)*(cjei0/cjei0_t))+((vdei/vdei_t)*(-cjei0*cjei0_t_Vtnode_GND/cjei0_t/cjei0_t)))*d00_pow0)+(((vdei/vdei_t)*(cjei0/cjei0_t))*(d10_pow0*a_eg_Vtnode_GND)));
#endif
aa=(((vdei/vdei_t)*(cjei0/cjei0_t))*d00_pow0);
}
}
}
#if defined(_DERIVATE)
ibets_t_Vtnode_GND=(ibets*ab_Vtnode_GND);
#endif
ibets_t=(ibets*ab);
#if defined(_DERIVATE)
abet_t_Vtnode_GND=(abet*aa_Vtnode_GND);
#endif
abet_t=(abet*aa);
}
#ifdef _DYNAMIC
if
(1.0>0.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((vdcx*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vdcx)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdjt_Vtnode_GND=(((vdj0*qtt0_Vtnode_GND)+(vgbc0*(-qtt0_Vtnode_GND)))-(((mg*VT_Vtnode_GND)*ln_qtt0)+((mg*VT)*ln_qtt0_Vtnode_GND)));
#endif
vdjt=(((vdj0*qtt0)+(vgbc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_sqrt(d10_sqrt1,d00_sqrt1,(1+(4*d00_exp0)))
double m10_logE(d10_logE2,d00_logE2,(0.5*(1+d00_sqrt1)))
#endif
#if defined(_DERIVATE)
vdt_Vtnode_GND=(vdjt_Vtnode_GND+(((2*VT_Vtnode_GND)*d00_logE2)+((2*VT)*(0.5*(4*(((-vdjt_Vtnode_GND)*VT-(-vdjt)*VT_Vtnode_GND)/VT/VT)*d10_exp0)*d10_sqrt1)*d10_logE2)));
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdcx_t_Vtnode_GND=vdt_Vtnode_GND;
#endif
vdcx_t=vdt;
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vdcx/vdcx_t))
double m00_exp(d00_exp1,(zcx*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vdcx/vdcx_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
cratio_t_Vtnode_GND=(1.0*(zcx*(-vdcx*vdcx_t_Vtnode_GND/vdcx_t/vdcx_t)*d10_logE0)*d10_exp1);
#endif
cratio_t=(1.0*d00_exp1);
}
#endif
#ifdef _DYNAMIC
if
(0==1)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=((vptcx*vdcx_t_Vtnode_GND)/vdcx);
#endif
vptcx_t=((vptcx*vdcx_t)/vdcx);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=0.0;
#endif
vptcx_t=vptcx;
#endif
}
#endif /* if (...) */
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cratio_t_Vtnode_GND=0.0;
#endif
cratio_t=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdcx_t_Vtnode_GND=0.0;
#endif
vdcx_t=vdcx;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vptcx_t_Vtnode_GND=0.0;
#endif
vptcx_t=vptcx;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjcx01_t_Vtnode_GND=cratio_t_Vtnode_GND*cjcx01;
#endif
cjcx01_t=(cratio_t*cjcx01);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjcx02_t_Vtnode_GND=cratio_t_Vtnode_GND*cjcx02;
#endif
cjcx02_t=(cratio_t*cjcx02);
#endif
{
double m00_exp(d00_exp0,(zetarcx*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
rcx_t_Vtnode_GND=(rcx*(zetarcx*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
rcx_t=(rcx*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetarbx*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
rbx_t_Vtnode_GND=(rbx*(zetarbx*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
rbx_t=(rbx*d00_exp0);
}
{
double m00_exp(d00_exp0,(zetare*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
re_t_Vtnode_GND=(re*(zetare*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
re_t=(re*d00_exp0);
}
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((zetacx-1.0)*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
tsf_t_Vtnode_GND=(tsf*((zetacx-1.0)*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
tsf_t=(tsf*d00_exp0);
}
#endif
#ifdef _DYNAMIC
if
(cjs0>0.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((vds*0.5)/vt0))
double m00_exp(d00_exp1,(((-0.5)*vds)/vt0))
double m00_logE(d00_logE2,(d00_exp0-d00_exp1))
vdj0=((2*vt0)*d00_logE2);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vdjt_Vtnode_GND=(((vdj0*qtt0_Vtnode_GND)+(vgsc0*(-qtt0_Vtnode_GND)))-(((mg*VT_Vtnode_GND)*ln_qtt0)+((mg*VT)*ln_qtt0_Vtnode_GND)));
#endif
vdjt=(((vdj0*qtt0)+(vgsc0*(1-qtt0)))-((mg*VT)*ln_qtt0));
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((-vdjt)/VT))
double m00_sqrt(d00_sqrt1,(1+(4*d00_exp0)))
double m00_logE(d00_logE2,(0.5*(1+d00_sqrt1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
double m10_sqrt(d10_sqrt1,d00_sqrt1,(1+(4*d00_exp0)))
double m10_logE(d10_logE2,d00_logE2,(0.5*(1+d00_sqrt1)))
#endif
#if defined(_DERIVATE)
vdt_Vtnode_GND=(vdjt_Vtnode_GND+(((2*VT_Vtnode_GND)*d00_logE2)+((2*VT)*(0.5*(4*(((-vdjt_Vtnode_GND)*VT-(-vdjt)*VT_Vtnode_GND)/VT/VT)*d10_exp0)*d10_sqrt1)*d10_logE2)));
#endif
vdt=(vdjt+((2*VT)*d00_logE2));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vds_t_Vtnode_GND=vdt_Vtnode_GND;
#endif
vds_t=vdt;
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vds/vds_t))
double m00_exp(d00_exp1,(zs*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vds/vds_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
cjs0_t_Vtnode_GND=(cjs0*(zs*(-vds*vds_t_Vtnode_GND/vds_t/vds_t)*d10_logE0)*d10_exp1);
#endif
cjs0_t=(cjs0*d00_exp1);
}
#endif
#ifdef _DYNAMIC
if
(0==1)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=((vpts*vds_t_Vtnode_GND)/vds);
#endif
vpts_t=((vpts*vds_t)/vds);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=0.0;
#endif
vpts_t=vpts;
#endif
}
#endif /* if (...) */
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
cjs0_t_Vtnode_GND=0.0;
#endif
cjs0_t=cjs0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vds_t_Vtnode_GND=0.0;
#endif
vds_t=vds;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
vpts_t_Vtnode_GND=0.0;
#endif
vpts_t=vpts;
#endif
}
#endif /* if (...) */
{
double m00_exp(d00_exp0,(zetahjei*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ahjei_t_Vtnode_GND=(ahjei*(zetahjei*ln_qtt0_Vtnode_GND)*d10_exp0);
#endif
ahjei_t=(ahjei*d00_exp0);
}
{
double m00_logE(d00_logE0,qtt0)
double m00_exp(d00_exp1,(zetavgbe*d00_logE0))
double m00_exp(d00_exp2,((dvgbe/VT)*(d00_exp1-1)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,qtt0)
#define d10_exp1 d00_exp1
#define d10_exp2 d00_exp2
#endif
#if defined(_DERIVATE)
hjei0_t_Vtnode_GND=(hjei*(((-dvgbe*VT_Vtnode_GND/VT/VT)*(d00_exp1-1))+((dvgbe/VT)*(zetavgbe*qtt0_Vtnode_GND*d10_logE0)*d10_exp1))*d10_exp2);
#endif
hjei0_t=(hjei*d00_exp2);
}
{
double m00_exp(d00_exp0,((dvgbe/VT)*(qtt0-1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
hf0_t_Vtnode_GND=(hf0*(((-dvgbe*VT_Vtnode_GND/VT/VT)*(qtt0-1))+((dvgbe/VT)*qtt0_Vtnode_GND))*d10_exp0);
#endif
hf0_t=(hf0*d00_exp0);
}
if
(flcomp>=2.3)
{
{
double m00_exp(d00_exp0,(((vgb-vge)/VT)*(qtt0-1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
hfe_t_Vtnode_GND=(hfe*(((-(vgb-vge)*VT_Vtnode_GND/VT/VT)*(qtt0-1))+(((vgb-vge)/VT)*qtt0_Vtnode_GND))*d10_exp0);
#endif
hfe_t=(hfe*d00_exp0);
}
{
double m00_exp(d00_exp0,(((vgb-vgc)/VT)*(qtt0-1)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
hfc_t_Vtnode_GND=(hfc*(((-(vgb-vgc)*VT_Vtnode_GND/VT/VT)*(qtt0-1))+(((vgb-vgc)/VT)*qtt0_Vtnode_GND))*d10_exp0);
#endif
hfc_t=(hfc*d00_exp0);
}
}
else
{
#if defined(_DERIVATE)
hfe_t_Vtnode_GND=0.0;
#endif
hfe_t=hfe;
#if defined(_DERIVATE)
hfc_t_Vtnode_GND=0.0;
#endif
hfc_t=hfc;
}
{
double m00_exp(d00_exp0,(zetarth*ln_qtt0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
rth_t_Vtnode_GND=(((rth*(zetarth*ln_qtt0_Vtnode_GND)*d10_exp0)*(1+(alrth*dT)))+((rth*d00_exp0)*(alrth*dT_Vtnode_GND)));
#endif
rth_t=((rth*d00_exp0)*(1+(alrth*dT)));
}
}
{
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=(Vbiei_Vbi_ei/(mbei*VT));
DIOY_Vtnode_GND=(-Vbiei*(mbei*VT_Vtnode_GND)/(mbei*VT)/(mbei*VT));
#endif
DIOY=(Vbiei/(mbei*VT));
if
(ibeis>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
ibei_Vtnode_GND=((ibeis_t_Vtnode_GND*(le-1.0))+(ibeis_t*le_Vtnode_GND));
ibei_Vbi_ei=(ibeis_t*le_Vbi_ei);
#endif
ibei=(ibeis_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
ibei_Vtnode_GND=(-ibeis_t_Vtnode_GND);
ibei_Vbi_ei=0.0;
#endif
ibei=(-ibeis_t);
}
}
else
{
#if defined(_DERIVATE)
ibei_Vtnode_GND=0.0;
ibei_Vbi_ei=0.0;
#endif
ibei=0.0;
}
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=(Vbiei_Vbi_ei/(mrei*VT));
DIOY_Vtnode_GND=(-Vbiei*(mrei*VT_Vtnode_GND)/(mrei*VT)/(mrei*VT));
#endif
DIOY=(Vbiei/(mrei*VT));
if
(ireis>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
irei_Vtnode_GND=((ireis_t_Vtnode_GND*(le-1.0))+(ireis_t*le_Vtnode_GND));
irei_Vbi_ei=(ireis_t*le_Vbi_ei);
#endif
irei=(ireis_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
irei_Vtnode_GND=(-ireis_t_Vtnode_GND);
irei_Vbi_ei=0.0;
#endif
irei=(-ireis_t);
}
}
else
{
#if defined(_DERIVATE)
irei_Vtnode_GND=0.0;
irei_Vbi_ei=0.0;
#endif
irei=0.0;
}
#if defined(_DERIVATE)
Orci0_t_Vtnode_GND=(-rci0_t_Vtnode_GND/rci0_t/rci0_t);
#endif
Orci0_t=(1.0/rci0_t);
Tr=tr;
#if defined(_DERIVATE)
VT_f_Vtnode_GND=(mcf*VT_Vtnode_GND);
#endif
VT_f=(mcf*VT);
{
double m00_limexp(d00_limexp0,(Vbiei/VT_f))
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,(Vbiei/VT_f))
#endif
#if defined(_DERIVATE)
i_0f_Vtnode_GND=((c10_t_Vtnode_GND*d00_limexp0)+(c10_t*(-Vbiei*VT_f_Vtnode_GND/VT_f/VT_f)*d10_limexp0));
i_0f_Vbi_ei=(c10_t*(Vbiei_Vbi_ei/VT_f)*d10_limexp0);
#endif
i_0f=(c10_t*d00_limexp0);
}
{
double m00_limexp(d00_limexp0,(Vbici/VT))
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,(Vbici/VT))
#endif
#if defined(_DERIVATE)
i_0r_Vtnode_GND=((c10_t_Vtnode_GND*d00_limexp0)+(c10_t*(-Vbici*VT_Vtnode_GND/VT/VT)*d10_limexp0));
i_0r_Vbi_ci=(c10_t*(Vbici_Vbi_ci/VT)*d10_limexp0);
#endif
i_0r=(c10_t*d00_limexp0);
}
if
(cjei0_t>0.0)
{
{
double m00_logE(d00_logE0,ajei_t)
double m00_exp(d00_exp1,((-d00_logE0)/zei))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,ajei_t)
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=((vdei_t_Vtnode_GND*(1.0-d00_exp1))+(vdei_t*(-((-ajei_t_Vtnode_GND*d10_logE0)/zei)*d10_exp1)));
#endif
DFV_f=(vdei_t*(1.0-d00_exp1));
}
#if defined(_DERIVATE)
DFv_e_Vsi_ci=0.0;
DFv_e_Vb_ci=0.0;
DFv_e_Vbp_ci=0.0;
DFv_e_Vbp_ei=0.0;
DFv_e_Vbi_ci=0.0;
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vbiei)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=((-Vbiei_Vbi_ei)/VT);
#endif
DFv_e=((DFV_f-Vbiei)/VT);
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=0.0;
DFs_q_Vb_ci=0.0;
DFs_q_Vbp_ci=0.0;
DFs_q_Vbp_ei=0.0;
DFs_q_Vbi_ci=0.0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=0.0;
DFs_q2_Vb_ci=0.0;
DFs_q2_Vbp_ci=0.0;
DFs_q2_Vbp_ei=0.0;
DFs_q2_Vbi_ci=0.0;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#if defined(_DERIVATE)
DFv_j_Vsi_ci=0.0;
DFv_j_Vb_ci=0.0;
DFv_j_Vbp_ci=0.0;
DFv_j_Vbp_ei=0.0;
DFv_j_Vbi_ci=0.0;
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=0.0;
DFdvj_dv_Vb_ci=0.0;
DFdvj_dv_Vbp_ci=0.0;
DFdvj_dv_Vbp_ei=0.0;
DFdvj_dv_Vbi_ci=0.0;
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vdei_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vdei_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=0.0;
DFb_Vb_ci=0.0;
DFb_Vbp_ci=0.0;
DFb_Vbp_ei=0.0;
DFb_Vbi_ci=0.0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vdei_t-DFv_j*vdei_t_Vtnode_GND)/vdei_t/vdei_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vdei_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
{
double m00_exp(d00_exp0,((-zei)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=0.0;
DFC_j1_Vb_ci=0.0;
DFC_j1_Vbp_ci=0.0;
DFC_j1_Vbp_ei=0.0;
DFC_j1_Vbi_ci=0.0;
DFC_j1_Vtnode_GND=((((cjei0_t_Vtnode_GND*d00_exp0)+(cjei0_t*((-zei)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjei0_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjei0_t*((-zei)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjei0_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjei0_t*d00_exp0)*DFdvj_dv);
}
#if defined(_DERIVATE)
Cjei_Vtnode_GND=(DFC_j1_Vtnode_GND+((((ajei_t_Vtnode_GND*cjei0_t)+(ajei_t*cjei0_t_Vtnode_GND))*(1.0-DFdvj_dv))+((ajei_t*cjei0_t)*(-DFdvj_dv_Vtnode_GND))));
Cjei_Vbi_ei=(DFC_j1_Vbi_ei+((ajei_t*cjei0_t)*(-DFdvj_dv_Vbi_ei)));
#endif
Cjei=(DFC_j1+((ajei_t*cjei0_t)*(1.0-DFdvj_dv)));
{
double m00_exp(d00_exp0,(DFb*(1.0-zei)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=0.0;
DFQ_j_Vb_ci=0.0;
DFQ_j_Vbp_ci=0.0;
DFQ_j_Vbp_ei=0.0;
DFQ_j_Vbi_ci=0.0;
DFQ_j_Vtnode_GND=(((((cjei0_t_Vtnode_GND*vdei_t)+(cjei0_t*vdei_t_Vtnode_GND))*(1.0-d00_exp0))+((cjei0_t*vdei_t)*(-DFb_Vtnode_GND*(1.0-zei)*d10_exp0)))/(1.0-zei));
DFQ_j_Vbi_ei=(((cjei0_t*vdei_t)*(-DFb_Vbi_ei*(1.0-zei)*d10_exp0))/(1.0-zei));
#endif
DFQ_j=(((cjei0_t*vdei_t)*(1.0-d00_exp0))/(1.0-zei));
}
#if defined(_DERIVATE)
Qjei_Vtnode_GND=(DFQ_j_Vtnode_GND+((((ajei_t_Vtnode_GND*cjei0_t)+(ajei_t*cjei0_t_Vtnode_GND))*(Vbiei-DFv_j))+((ajei_t*cjei0_t)*(-DFv_j_Vtnode_GND))));
Qjei_Vbi_ei=(DFQ_j_Vbi_ei+((ajei_t*cjei0_t)*(Vbiei_Vbi_ei-DFv_j_Vbi_ei)));
#endif
Qjei=(DFQ_j+((ajei_t*cjei0_t)*(Vbiei-DFv_j)));
}
else
{
#if defined(_DERIVATE)
Cjei_Vtnode_GND=0.0;
Cjei_Vbi_ei=0.0;
#endif
Cjei=0.0;
#if defined(_DERIVATE)
Qjei_Vtnode_GND=0.0;
Qjei_Vbi_ei=0.0;
#endif
Qjei=0.0;
}
if
(ahjei==0.0)
{
#if defined(_DERIVATE)
hjei_vbe_Vtnode_GND=0.0;
hjei_vbe_Vbi_ei=0.0;
#endif
hjei_vbe=hjei;
}
else
{
#if defined(_DERIVATE)
vj_Vtnode_GND=((vdei_t_Vtnode_GND*(rhjei*VT)-(vdei_t-Vbiei)*(rhjei*VT_Vtnode_GND))/(rhjei*VT)/(rhjei*VT));
vj_Vbi_ei=((-Vbiei_Vbi_ei)/(rhjei*VT));
#endif
vj=((vdei_t-Vbiei)/(rhjei*VT));
{
double m00_sqrt(d00_sqrt0,((vj*vj)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((vj*vj)+1.921812))
#endif
#if defined(_DERIVATE)
vj_Vtnode_GND=(vdei_t_Vtnode_GND-(((rhjei*VT_Vtnode_GND)*(vj+d00_sqrt0))+((rhjei*VT)*(vj_Vtnode_GND+((vj_Vtnode_GND*vj)+(vj*vj_Vtnode_GND))*d10_sqrt0)))*0.5);
vj_Vbi_ei=(-((rhjei*VT)*(vj_Vbi_ei+((vj_Vbi_ei*vj)+(vj*vj_Vbi_ei))*d10_sqrt0))*0.5);
#endif
vj=(vdei_t-(((rhjei*VT)*(vj+d00_sqrt0))*0.5));
}
#if defined(_DERIVATE)
vj_Vtnode_GND=(((vj_Vtnode_GND-VT_Vtnode_GND)*VT-(vj-VT)*VT_Vtnode_GND)/VT/VT);
vj_Vbi_ei=(vj_Vbi_ei/VT);
#endif
vj=((vj-VT)/VT);
{
double m00_sqrt(d00_sqrt0,((vj*vj)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((vj*vj)+1.921812))
#endif
#if defined(_DERIVATE)
vj_Vtnode_GND=((VT_Vtnode_GND*(1.0+((vj+d00_sqrt0)*0.5)))+(VT*(vj_Vtnode_GND+((vj_Vtnode_GND*vj)+(vj*vj_Vtnode_GND))*d10_sqrt0)*0.5));
vj_Vbi_ei=(VT*(vj_Vbi_ei+((vj_Vbi_ei*vj)+(vj*vj_Vbi_ei))*d10_sqrt0)*0.5);
#endif
vj=(VT*(1.0+((vj+d00_sqrt0)*0.5)));
}
{
double m00_logE(d00_logE0,(1.0-(vj/vdei_t)))
double m00_exp(d00_exp1,(zei*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(vj/vdei_t)))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
vj_z_Vtnode_GND=(((-(zei*(-((vj_Vtnode_GND*vdei_t-vj*vdei_t_Vtnode_GND)/vdei_t/vdei_t))*d10_logE0)*d10_exp1)*ahjei_t)+((1.0-d00_exp1)*ahjei_t_Vtnode_GND));
vj_z_Vbi_ei=(-(zei*(-(vj_Vbi_ei/vdei_t))*d10_logE0)*d10_exp1)*ahjei_t;
#endif
vj_z=((1.0-d00_exp1)*ahjei_t);
}
{
double m00_exp(d00_exp0,vj_z)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
hjei_vbe_Vtnode_GND=((((hjei0_t_Vtnode_GND*(d00_exp0-1.0))+(hjei0_t*vj_z_Vtnode_GND*d10_exp0))*vj_z-(hjei0_t*(d00_exp0-1.0))*vj_z_Vtnode_GND)/vj_z/vj_z);
hjei_vbe_Vbi_ei=(((hjei0_t*vj_z_Vbi_ei*d10_exp0)*vj_z-(hjei0_t*(d00_exp0-1.0))*vj_z_Vbi_ei)/vj_z/vj_z);
#endif
hjei_vbe=((hjei0_t*(d00_exp0-1.0))/vj_z);
}
}
if
(vptci_t<1.0e2)
{
if
(cjci0_t>0.0)
{
Dz_r=(zci/4.0);
#if defined(_DERIVATE)
Dv_p_Vtnode_GND=(vptci_t_Vtnode_GND-vdci_t_Vtnode_GND);
#endif
Dv_p=(vptci_t-vdci_t);
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zci))
#if defined(_DERIVATE)
DV_f_Vtnode_GND=vdci_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DV_f=(vdci_t*(1.0-d00_exp1));
}
#if defined(_DERIVATE)
DC_max_Vtnode_GND=(2.4*cjci0_t_Vtnode_GND);
#endif
DC_max=(2.4*cjci0_t);
{
double m00_logE(d00_logE0,(vptci_t/vdci_t))
double m00_exp(d00_exp1,((Dz_r-zci)*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vptci_t/vdci_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DC_c_Vtnode_GND=((cjci0_t_Vtnode_GND*d00_exp1)+(cjci0_t*((Dz_r-zci)*((vptci_t_Vtnode_GND*vdci_t-vptci_t*vdci_t_Vtnode_GND)/vdci_t/vdci_t)*d10_logE0)*d10_exp1));
#endif
DC_c=(cjci0_t*d00_exp1);
}
#if defined(_DERIVATE)
Dv_e_Vsi_ci=0.0;
Dv_e_Vb_ci=0.0;
Dv_e_Vbp_ci=0.0;
Dv_e_Vtnode_GND=((DV_f_Vtnode_GND*VT-(DV_f-Vbici)*VT_Vtnode_GND)/VT/VT);
Dv_e_Vbi_ci=((-Vbici_Vbi_ci)/VT);
#endif
Dv_e=((DV_f-Vbici)/VT);
if
(Dv_e<80.0)
{
{
double m00_exp(d00_exp0,Dv_e)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=0.0;
De_Vbp_ci=0.0;
De_Vtnode_GND=Dv_e_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_e_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=0.0;
De_1_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_1_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_1=(De/(1.0+De));
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=0.0;
Dv_j1_Vbp_ci=0.0;
Dv_j1_Vtnode_GND=(DV_f_Vtnode_GND-((VT_Vtnode_GND*d00_logE0)+(VT*De_Vtnode_GND*d10_logE0)));
Dv_j1_Vbi_ci=(-(VT*De_Vbi_ci*d10_logE0));
#endif
Dv_j1=(DV_f-(VT*d00_logE0));
}
}
else
{
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=0.0;
De_1_Vtnode_GND=0.0;
De_1_Vbi_ci=0.0;
#endif
De_1=1.0;
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=0.0;
Dv_j1_Vbp_ci=0.0;
Dv_j1_Vtnode_GND=0.0;
Dv_j1_Vbi_ci=Vbici_Vbi_ci;
#endif
Dv_j1=Vbici;
}
#if defined(_DERIVATE)
Da_Vtnode_GND=((0.1*Dv_p_Vtnode_GND)+(4.0*VT_Vtnode_GND));
#endif
Da=((0.1*Dv_p)+(4.0*VT));
#if defined(_DERIVATE)
Dv_r_Vsi_ci=0.0;
Dv_r_Vb_ci=0.0;
Dv_r_Vbp_ci=0.0;
Dv_r_Vtnode_GND=(((Dv_p_Vtnode_GND+Dv_j1_Vtnode_GND)*Da-(Dv_p+Dv_j1)*Da_Vtnode_GND)/Da/Da);
Dv_r_Vbi_ci=(Dv_j1_Vbi_ci/Da);
#endif
Dv_r=((Dv_p+Dv_j1)/Da);
if
(Dv_r<80.0)
{
{
double m00_exp(d00_exp0,Dv_r)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=0.0;
De_Vbp_ci=0.0;
De_Vtnode_GND=Dv_r_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_r_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=0.0;
De_2_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_2_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_2=(De/(1.0+De));
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=0.0;
Dv_j2_Vbp_ci=0.0;
Dv_j2_Vtnode_GND=((-Dv_p_Vtnode_GND)+((Da_Vtnode_GND*d00_logE0)+(Da*De_Vtnode_GND*d10_logE0)));
Dv_j2_Vbi_ci=(Da*De_Vbi_ci*d10_logE0);
#endif
Dv_j2=((-Dv_p)+(Da*d00_logE0));
}
}
else
{
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=0.0;
De_2_Vtnode_GND=0.0;
De_2_Vbi_ci=0.0;
#endif
De_2=1.0;
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=0.0;
Dv_j2_Vbp_ci=0.0;
Dv_j2_Vtnode_GND=Dv_j1_Vtnode_GND;
Dv_j2_Vbi_ci=Dv_j1_Vbi_ci;
#endif
Dv_j2=Dv_j1;
}
#if defined(_DERIVATE)
Dv_j4_Vsi_ci=0.0;
Dv_j4_Vb_ci=0.0;
Dv_j4_Vbp_ci=0.0;
Dv_j4_Vbi_ci=(Vbici_Vbi_ci-Dv_j1_Vbi_ci);
Dv_j4_Vtnode_GND=(-Dv_j1_Vtnode_GND);
#endif
Dv_j4=(Vbici-Dv_j1);
{
double m00_logE(d00_logE0,(1.0-(Dv_j1/vdci_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j1/vdci_t)))
#endif
#if defined(_DERIVATE)
DCln1_Vsi_ci=0.0;
DCln1_Vb_ci=0.0;
DCln1_Vbp_ci=0.0;
DCln1_Vtnode_GND=(-((Dv_j1_Vtnode_GND*vdci_t-Dv_j1*vdci_t_Vtnode_GND)/vdci_t/vdci_t))*d10_logE0;
DCln1_Vbi_ci=(-(Dv_j1_Vbi_ci/vdci_t))*d10_logE0;
#endif
DCln1=d00_logE0;
}
{
double m00_logE(d00_logE0,(1.0-(Dv_j2/vdci_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j2/vdci_t)))
#endif
#if defined(_DERIVATE)
DCln2_Vsi_ci=0.0;
DCln2_Vb_ci=0.0;
DCln2_Vbp_ci=0.0;
DCln2_Vtnode_GND=(-((Dv_j2_Vtnode_GND*vdci_t-Dv_j2*vdci_t_Vtnode_GND)/vdci_t/vdci_t))*d10_logE0;
DCln2_Vbi_ci=(-(Dv_j2_Vbi_ci/vdci_t))*d10_logE0;
#endif
DCln2=d00_logE0;
}
Dz1=(1.0-zci);
Dzr1=(1.0-Dz_r);
{
double m00_exp(d00_exp0,(DCln2*(-zci)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j1_Vsi_ci=0.0;
DC_j1_Vb_ci=0.0;
DC_j1_Vbp_ci=0.0;
DC_j1_Vtnode_GND=((((((cjci0_t_Vtnode_GND*d00_exp0)+(cjci0_t*DCln2_Vtnode_GND*(-zci)*d10_exp0))*De_1)+((cjci0_t*d00_exp0)*De_1_Vtnode_GND))*De_2)+(((cjci0_t*d00_exp0)*De_1)*De_2_Vtnode_GND));
DC_j1_Vbi_ci=(((((cjci0_t*DCln2_Vbi_ci*(-zci)*d10_exp0)*De_1)+((cjci0_t*d00_exp0)*De_1_Vbi_ci))*De_2)+(((cjci0_t*d00_exp0)*De_1)*De_2_Vbi_ci));
#endif
DC_j1=(((cjci0_t*d00_exp0)*De_1)*De_2);
}
{
double m00_exp(d00_exp0,(DCln1*(-Dz_r)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j2_Vsi_ci=0.0;
DC_j2_Vb_ci=0.0;
DC_j2_Vbp_ci=0.0;
DC_j2_Vtnode_GND=((((DC_c_Vtnode_GND*d00_exp0)+(DC_c*DCln1_Vtnode_GND*(-Dz_r)*d10_exp0))*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vtnode_GND)));
DC_j2_Vbi_ci=(((DC_c*DCln1_Vbi_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbi_ci)));
#endif
DC_j2=((DC_c*d00_exp0)*(1.0-De_2));
}
#if defined(_DERIVATE)
DC_j3_Vsi_ci=0.0;
DC_j3_Vb_ci=0.0;
DC_j3_Vbp_ci=0.0;
DC_j3_Vtnode_GND=((DC_max_Vtnode_GND*(1.0-De_1))+(DC_max*(-De_1_Vtnode_GND)));
DC_j3_Vbi_ci=(DC_max*(-De_1_Vbi_ci));
#endif
DC_j3=(DC_max*(1.0-De_1));
#if defined(_DERIVATE)
Cjci_Vbi_ei=0.0;
Cjci_Vtnode_GND=((DC_j1_Vtnode_GND+DC_j2_Vtnode_GND)+DC_j3_Vtnode_GND);
Cjci_Vbi_ci=((DC_j1_Vbi_ci+DC_j2_Vbi_ci)+DC_j3_Vbi_ci);
#endif
Cjci=((DC_j1+DC_j2)+DC_j3);
{
double m00_exp(d00_exp0,(DCln2*Dz1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j1_Vsi_ci=0.0;
DQ_j1_Vb_ci=0.0;
DQ_j1_Vbp_ci=0.0;
DQ_j1_Vtnode_GND=(((cjci0_t_Vtnode_GND*(1.0-d00_exp0))+(cjci0_t*(-DCln2_Vtnode_GND*Dz1*d10_exp0)))/Dz1);
DQ_j1_Vbi_ci=((cjci0_t*(-DCln2_Vbi_ci*Dz1*d10_exp0))/Dz1);
#endif
DQ_j1=((cjci0_t*(1.0-d00_exp0))/Dz1);
}
{
double m00_exp(d00_exp0,(DCln1*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j2_Vsi_ci=0.0;
DQ_j2_Vb_ci=0.0;
DQ_j2_Vbp_ci=0.0;
DQ_j2_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln1_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j2_Vbi_ci=((DC_c*(-DCln1_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j2=((DC_c*(1.0-d00_exp0))/Dzr1);
}
{
double m00_exp(d00_exp0,(DCln2*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j3_Vsi_ci=0.0;
DQ_j3_Vb_ci=0.0;
DQ_j3_Vbp_ci=0.0;
DQ_j3_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln2_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j3_Vbi_ci=((DC_c*(-DCln2_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j3=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#if defined(_DERIVATE)
Qjci_Vbi_ei=0.0;
Qjci_Vtnode_GND=(((((DQ_j1_Vtnode_GND+DQ_j2_Vtnode_GND)-DQ_j3_Vtnode_GND)*vdci_t)+(((DQ_j1+DQ_j2)-DQ_j3)*vdci_t_Vtnode_GND))+((DC_max_Vtnode_GND*Dv_j4)+(DC_max*Dv_j4_Vtnode_GND)));
Qjci_Vbi_ci=(((DQ_j1_Vbi_ci+DQ_j2_Vbi_ci)-DQ_j3_Vbi_ci)*vdci_t+(DC_max*Dv_j4_Vbi_ci));
#endif
Qjci=((((DQ_j1+DQ_j2)-DQ_j3)*vdci_t)+(DC_max*Dv_j4));
}
else
{
#if defined(_DERIVATE)
Cjci_Vbi_ei=0.0;
Cjci_Vtnode_GND=0.0;
Cjci_Vbi_ci=0.0;
#endif
Cjci=0.0;
#if defined(_DERIVATE)
Qjci_Vbi_ei=0.0;
Qjci_Vtnode_GND=0.0;
Qjci_Vbi_ci=0.0;
#endif
Qjci=0.0;
}
}
else
{
if
(cjci0_t>0.0)
{
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zci))
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=vdci_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DFV_f=(vdci_t*(1.0-d00_exp1));
}
#if defined(_DERIVATE)
DFv_e_Vsi_ci=0.0;
DFv_e_Vb_ci=0.0;
DFv_e_Vbp_ci=0.0;
DFv_e_Vbp_ei=0.0;
DFv_e_Vbi_ci=((-Vbici_Vbi_ci)/VT);
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vbici)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=0.0;
#endif
DFv_e=((DFV_f-Vbici)/VT);
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=0.0;
DFs_q_Vb_ci=0.0;
DFs_q_Vbp_ci=0.0;
DFs_q_Vbp_ei=0.0;
DFs_q_Vbi_ci=((DFv_e_Vbi_ci*DFv_e)+(DFv_e*DFv_e_Vbi_ci))*d10_sqrt0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=0.0;
DFs_q2_Vb_ci=0.0;
DFs_q2_Vbp_ci=0.0;
DFs_q2_Vbp_ei=0.0;
DFs_q2_Vbi_ci=(DFv_e_Vbi_ci+DFs_q_Vbi_ci)*0.5;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#if defined(_DERIVATE)
DFv_j_Vsi_ci=0.0;
DFv_j_Vb_ci=0.0;
DFv_j_Vbp_ci=0.0;
DFv_j_Vbp_ei=0.0;
DFv_j_Vbi_ci=(-(VT*DFs_q2_Vbi_ci));
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=0.0;
DFdvj_dv_Vb_ci=0.0;
DFdvj_dv_Vbp_ci=0.0;
DFdvj_dv_Vbp_ei=0.0;
DFdvj_dv_Vbi_ci=((DFs_q2_Vbi_ci*DFs_q-DFs_q2*DFs_q_Vbi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vdci_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vdci_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=0.0;
DFb_Vb_ci=0.0;
DFb_Vbp_ci=0.0;
DFb_Vbp_ei=0.0;
DFb_Vbi_ci=(-(DFv_j_Vbi_ci/vdci_t))*d10_logE0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vdci_t-DFv_j*vdci_t_Vtnode_GND)/vdci_t/vdci_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vdci_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
{
double m00_exp(d00_exp0,((-zci)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=0.0;
DFC_j1_Vb_ci=0.0;
DFC_j1_Vbp_ci=0.0;
DFC_j1_Vbp_ei=0.0;
DFC_j1_Vbi_ci=(((cjci0_t*((-zci)*DFb_Vbi_ci)*d10_exp0)*DFdvj_dv)+((cjci0_t*d00_exp0)*DFdvj_dv_Vbi_ci));
DFC_j1_Vtnode_GND=((((cjci0_t_Vtnode_GND*d00_exp0)+(cjci0_t*((-zci)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjci0_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjci0_t*((-zci)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjci0_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjci0_t*d00_exp0)*DFdvj_dv);
}
#if defined(_DERIVATE)
Cjci_Vbi_ei=(DFC_j1_Vbi_ei+((2.4*cjci0_t)*(-DFdvj_dv_Vbi_ei)));
Cjci_Vtnode_GND=(DFC_j1_Vtnode_GND+(((2.4*cjci0_t_Vtnode_GND)*(1.0-DFdvj_dv))+((2.4*cjci0_t)*(-DFdvj_dv_Vtnode_GND))));
Cjci_Vbi_ci=(DFC_j1_Vbi_ci+((2.4*cjci0_t)*(-DFdvj_dv_Vbi_ci)));
#endif
Cjci=(DFC_j1+((2.4*cjci0_t)*(1.0-DFdvj_dv)));
{
double m00_exp(d00_exp0,(DFb*(1.0-zci)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=0.0;
DFQ_j_Vb_ci=0.0;
DFQ_j_Vbp_ci=0.0;
DFQ_j_Vbp_ei=0.0;
DFQ_j_Vbi_ci=(((cjci0_t*vdci_t)*(-DFb_Vbi_ci*(1.0-zci)*d10_exp0))/(1.0-zci));
DFQ_j_Vtnode_GND=(((((cjci0_t_Vtnode_GND*vdci_t)+(cjci0_t*vdci_t_Vtnode_GND))*(1.0-d00_exp0))+((cjci0_t*vdci_t)*(-DFb_Vtnode_GND*(1.0-zci)*d10_exp0)))/(1.0-zci));
DFQ_j_Vbi_ei=(((cjci0_t*vdci_t)*(-DFb_Vbi_ei*(1.0-zci)*d10_exp0))/(1.0-zci));
#endif
DFQ_j=(((cjci0_t*vdci_t)*(1.0-d00_exp0))/(1.0-zci));
}
#if defined(_DERIVATE)
Qjci_Vbi_ei=(DFQ_j_Vbi_ei+((2.4*cjci0_t)*(-DFv_j_Vbi_ei)));
Qjci_Vtnode_GND=(DFQ_j_Vtnode_GND+(((2.4*cjci0_t_Vtnode_GND)*(Vbici-DFv_j))+((2.4*cjci0_t)*(-DFv_j_Vtnode_GND))));
Qjci_Vbi_ci=(DFQ_j_Vbi_ci+((2.4*cjci0_t)*(Vbici_Vbi_ci-DFv_j_Vbi_ci)));
#endif
Qjci=(DFQ_j+((2.4*cjci0_t)*(Vbici-DFv_j)));
}
else
{
#if defined(_DERIVATE)
Cjci_Vbi_ei=0.0;
Cjci_Vtnode_GND=0.0;
Cjci_Vbi_ci=0.0;
#endif
Cjci=0.0;
#if defined(_DERIVATE)
Qjci_Vbi_ei=0.0;
Qjci_Vtnode_GND=0.0;
Qjci_Vbi_ci=0.0;
#endif
Qjci=0.0;
}
}
a_bpt=0.05;
#if defined(_DERIVATE)
Q_0_Vtnode_GND=((qp0_t_Vtnode_GND+((hjei_vbe_Vtnode_GND*Qjei)+(hjei_vbe*Qjei_Vtnode_GND)))+(hjci*Qjci_Vtnode_GND));
Q_0_Vbi_ei=(((hjei_vbe_Vbi_ei*Qjei)+(hjei_vbe*Qjei_Vbi_ei))+(hjci*Qjci_Vbi_ei));
Q_0_Vbi_ci=(hjci*Qjci_Vbi_ci);
#endif
Q_0=((qp0_t+(hjei_vbe*Qjei))+(hjci*Qjci));
#if defined(_DERIVATE)
Q_bpt_Vtnode_GND=(a_bpt*qp0_t_Vtnode_GND);
#endif
Q_bpt=(a_bpt*qp0_t);
#if defined(_DERIVATE)
b_q_Vtnode_GND=((Q_0_Vtnode_GND*Q_bpt-Q_0*Q_bpt_Vtnode_GND)/Q_bpt/Q_bpt);
b_q_Vbi_ei=(Q_0_Vbi_ei/Q_bpt);
b_q_Vbi_ci=(Q_0_Vbi_ci/Q_bpt);
#endif
b_q=((Q_0/Q_bpt)-1);
{
double m00_sqrt(d00_sqrt0,((b_q*b_q)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((b_q*b_q)+1.921812))
#endif
#if defined(_DERIVATE)
Q_0_Vtnode_GND=((Q_bpt_Vtnode_GND*(1+((b_q+d00_sqrt0)/2)))+(Q_bpt*((b_q_Vtnode_GND+((b_q_Vtnode_GND*b_q)+(b_q*b_q_Vtnode_GND))*d10_sqrt0)/2)));
Q_0_Vbi_ei=(Q_bpt*((b_q_Vbi_ei+((b_q_Vbi_ei*b_q)+(b_q*b_q_Vbi_ei))*d10_sqrt0)/2));
Q_0_Vbi_ci=(Q_bpt*((b_q_Vbi_ci+((b_q_Vbi_ci*b_q)+(b_q*b_q_Vbi_ci))*d10_sqrt0)/2));
#endif
Q_0=(Q_bpt*(1+((b_q+d00_sqrt0)/2)));
}
if
(cjci0_t>0.0)
{
double cV_f;
double cv_e;
double cs_q;
double cs_q2;
double cv_j;
double cdvj_dv;
#if defined(_DERIVATE)
double cV_f_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cv_e_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cv_e_Vbi_ci;
#endif
#if defined(_DERIVATE)
double cs_q_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cs_q_Vbi_ci;
#endif
#if defined(_DERIVATE)
double cs_q2_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cs_q2_Vbi_ci;
#endif
#if defined(_DERIVATE)
double cv_j_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cv_j_Vbi_ci;
#endif
#if defined(_DERIVATE)
double cdvj_dv_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double cdvj_dv_Vbi_ci;
#endif
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zci))
#if defined(_DERIVATE)
cV_f_Vtnode_GND=vdci_t_Vtnode_GND*(1.0-d00_exp1);
#endif
cV_f=(vdci_t*(1.0-d00_exp1));
}
#if defined(_DERIVATE)
cv_e_Vtnode_GND=((cV_f_Vtnode_GND*VT-(cV_f-Vbici)*VT_Vtnode_GND)/VT/VT);
cv_e_Vbi_ci=((-Vbici_Vbi_ci)/VT);
#endif
cv_e=((cV_f-Vbici)/VT);
{
double m00_sqrt(d00_sqrt0,((cv_e*cv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((cv_e*cv_e)+1.921812))
#endif
#if defined(_DERIVATE)
cs_q_Vtnode_GND=((cv_e_Vtnode_GND*cv_e)+(cv_e*cv_e_Vtnode_GND))*d10_sqrt0;
cs_q_Vbi_ci=((cv_e_Vbi_ci*cv_e)+(cv_e*cv_e_Vbi_ci))*d10_sqrt0;
#endif
cs_q=d00_sqrt0;
}
#if defined(_DERIVATE)
cs_q2_Vtnode_GND=(cv_e_Vtnode_GND+cs_q_Vtnode_GND)*0.5;
cs_q2_Vbi_ci=(cv_e_Vbi_ci+cs_q_Vbi_ci)*0.5;
#endif
cs_q2=((cv_e+cs_q)*0.5);
#if defined(_DERIVATE)
cv_j_Vtnode_GND=(cV_f_Vtnode_GND-((VT_Vtnode_GND*cs_q2)+(VT*cs_q2_Vtnode_GND)));
cv_j_Vbi_ci=(-(VT*cs_q2_Vbi_ci));
#endif
cv_j=(cV_f-(VT*cs_q2));
#if defined(_DERIVATE)
cdvj_dv_Vtnode_GND=((cs_q2_Vtnode_GND*cs_q-cs_q2*cs_q_Vtnode_GND)/cs_q/cs_q);
cdvj_dv_Vbi_ci=((cs_q2_Vbi_ci*cs_q-cs_q2*cs_q_Vbi_ci)/cs_q/cs_q);
#endif
cdvj_dv=(cs_q2/cs_q);
{
double m00_logE(d00_logE0,(1.0-(cv_j/vdci_t)))
double m00_exp(d00_exp1,((-zci)*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(cv_j/vdci_t)))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
Cjcit_Vtnode_GND=(((((cjci0_t_Vtnode_GND*d00_exp1)+(cjci0_t*((-zci)*(-((cv_j_Vtnode_GND*vdci_t-cv_j*vdci_t_Vtnode_GND)/vdci_t/vdci_t))*d10_logE0)*d10_exp1))*cdvj_dv)+((cjci0_t*d00_exp1)*cdvj_dv_Vtnode_GND))+(((2.4*cjci0_t_Vtnode_GND)*(1.0-cdvj_dv))+((2.4*cjci0_t)*(-cdvj_dv_Vtnode_GND))));
Cjcit_Vbi_ci=((((cjci0_t*((-zci)*(-(cv_j_Vbi_ci/vdci_t))*d10_logE0)*d10_exp1)*cdvj_dv)+((cjci0_t*d00_exp1)*cdvj_dv_Vbi_ci))+((2.4*cjci0_t)*(-cdvj_dv_Vbi_ci)));
#endif
Cjcit=(((cjci0_t*d00_exp1)*cdvj_dv)+((2.4*cjci0_t)*(1.0-cdvj_dv)));
}
}
else
{
#if defined(_DERIVATE)
Cjcit_Vtnode_GND=0.0;
Cjcit_Vbi_ci=0.0;
#endif
Cjcit=0.0;
}
if
(Cjcit>0.0)
{
#if defined(_DERIVATE)
cc_Vtnode_GND=((cjci0_t_Vtnode_GND*Cjcit-cjci0_t*Cjcit_Vtnode_GND)/Cjcit/Cjcit);
cc_Vbi_ci=(-cjci0_t*Cjcit_Vbi_ci/Cjcit/Cjcit);
#endif
cc=(cjci0_t/Cjcit);
}
else
{
#if defined(_DERIVATE)
cc_Vtnode_GND=0.0;
cc_Vbi_ci=0.0;
#endif
cc=1.0;
}
#if defined(_DERIVATE)
T_f0_Vtnode_GND=((t0_t_Vtnode_GND+(dt0h*cc_Vtnode_GND))+(tbvl*(-1*cc_Vtnode_GND/cc/cc)));
T_f0_Vbi_ci=((dt0h*cc_Vbi_ci)+(tbvl*(-1*cc_Vbi_ci/cc/cc)));
#endif
T_f0=((t0_t+(dt0h*(cc-1.0)))+(tbvl*((1/cc)-1.0)));
#if defined(_DERIVATE)
vc_Vci_ei=Vciei_Vci_ei;
vc_Vtnode_GND=(-vces_t_Vtnode_GND);
#endif
vc=(Vciei-vces_t);
{
Ovpt=(1.0/vpt);
#if defined(_DERIVATE)
a_Vbi_ei=0.0;
a_Vbi_ci=0.0;
a_Vci_ei=(vc_Vci_ei/VT);
a_Vtnode_GND=((vc_Vtnode_GND*VT-vc*VT_Vtnode_GND)/VT/VT);
#endif
a=(vc/VT);
#if defined(_DERIVATE)
d1_Vci_ei=a_Vci_ei;
d1_Vtnode_GND=a_Vtnode_GND;
#endif
d1=(a-1);
{
double m00_sqrt(d00_sqrt0,((d1*d1)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((d1*d1)+1.921812))
#endif
#if defined(_DERIVATE)
vceff_Vci_ei=((d1_Vci_ei+((d1_Vci_ei*d1)+(d1*d1_Vci_ei))*d10_sqrt0)/2)*VT;
vceff_Vtnode_GND=((((d1_Vtnode_GND+((d1_Vtnode_GND*d1)+(d1*d1_Vtnode_GND))*d10_sqrt0)/2)*VT)+((1.0+((d1+d00_sqrt0)/2))*VT_Vtnode_GND));
#endif
vceff=((1.0+((d1+d00_sqrt0)/2))*VT);
}
#if defined(_DERIVATE)
a1_Vci_ei=(vceff_Vci_ei/vlim_t);
a1_Vtnode_GND=((vceff_Vtnode_GND*vlim_t-vceff*vlim_t_Vtnode_GND)/vlim_t/vlim_t);
#endif
a1=(vceff/vlim_t);
#if defined(_DERIVATE)
a11_Vci_ei=vceff_Vci_ei*Orci0_t;
a11_Vtnode_GND=((vceff_Vtnode_GND*Orci0_t)+(vceff*Orci0_t_Vtnode_GND));
#endif
a11=(vceff*Orci0_t);
Odelck=(1/delck);
{
double m00_logE(d00_logE0,a1)
double m00_exp(d00_exp1,(delck*d00_logE0))
double m00_logE(d00_logE2,(1+d00_exp1))
double m00_exp(d00_exp3,(Odelck*d00_logE2))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,a1)
#define d10_exp1 d00_exp1
double m10_logE(d10_logE2,d00_logE2,(1+d00_exp1))
#define d10_exp3 d00_exp3
#endif
#if defined(_DERIVATE)
ick1_Vci_ei=(Odelck*(delck*a1_Vci_ei*d10_logE0)*d10_exp1*d10_logE2)*d10_exp3;
ick1_Vtnode_GND=(Odelck*(delck*a1_Vtnode_GND*d10_logE0)*d10_exp1*d10_logE2)*d10_exp3;
#endif
ick1=d00_exp3;
}
#if defined(_DERIVATE)
ick2_Vci_ei=((a11_Vci_ei*ick1-a11*ick1_Vci_ei)/ick1/ick1);
ick2_Vtnode_GND=((a11_Vtnode_GND*ick1-a11*ick1_Vtnode_GND)/ick1/ick1);
#endif
ick2=(a11/ick1);
#if defined(_DERIVATE)
ICKa_Vci_ei=vceff_Vci_ei*Ovpt;
ICKa_Vtnode_GND=(vceff_Vtnode_GND-vlim_t_Vtnode_GND)*Ovpt;
#endif
ICKa=((vceff-vlim_t)*Ovpt);
{
double m00_sqrt(d00_sqrt0,((ICKa*ICKa)+1.0e-3))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((ICKa*ICKa)+1.0e-3))
#endif
#if defined(_DERIVATE)
ick_Vci_ei=((ick2_Vci_ei*(1.0+(0.5*(ICKa+d00_sqrt0))))+(ick2*(0.5*(ICKa_Vci_ei+((ICKa_Vci_ei*ICKa)+(ICKa*ICKa_Vci_ei))*d10_sqrt0))));
ick_Vtnode_GND=((ick2_Vtnode_GND*(1.0+(0.5*(ICKa+d00_sqrt0))))+(ick2*(0.5*(ICKa_Vtnode_GND+((ICKa_Vtnode_GND*ICKa)+(ICKa*ICKa_Vtnode_GND))*d10_sqrt0))));
#endif
ick=(ick2*(1.0+(0.5*(ICKa+d00_sqrt0))));
}
}
#if defined(_DERIVATE)
Q_p_Vtnode_GND=Q_0_Vtnode_GND;
Q_p_Vbi_ei=Q_0_Vbi_ei;
Q_p_Vbi_ci=Q_0_Vbi_ci;
#endif
Q_p=Q_0;
if
((T_f0>0.0)||(Tr>0.0))
{
#if defined(_DERIVATE)
A_Vtnode_GND=(0.5*Q_0_Vtnode_GND);
A_Vbi_ei=(0.5*Q_0_Vbi_ei);
A_Vbi_ci=(0.5*Q_0_Vbi_ci);
#endif
A=(0.5*Q_0);
{
double m00_sqrt(d00_sqrt0,(((A*A)+(T_f0*i_0f))+(Tr*i_0r)))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,(((A*A)+(T_f0*i_0f))+(Tr*i_0r)))
#endif
#if defined(_DERIVATE)
Q_p_Vtnode_GND=(A_Vtnode_GND+((((A_Vtnode_GND*A)+(A*A_Vtnode_GND))+((T_f0_Vtnode_GND*i_0f)+(T_f0*i_0f_Vtnode_GND)))+(Tr*i_0r_Vtnode_GND))*d10_sqrt0);
Q_p_Vbi_ei=(A_Vbi_ei+(((A_Vbi_ei*A)+(A*A_Vbi_ei))+(T_f0*i_0f_Vbi_ei))*d10_sqrt0);
Q_p_Vbi_ci=(A_Vbi_ci+((((A_Vbi_ci*A)+(A*A_Vbi_ci))+T_f0_Vbi_ci*i_0f)+(Tr*i_0r_Vbi_ci))*d10_sqrt0);
#endif
Q_p=(A+d00_sqrt0);
}
}
#if defined(_DERIVATE)
I_Tf1_Vci_ei=0.0;
I_Tf1_Vtnode_GND=((i_0f_Vtnode_GND*Q_p-i_0f*Q_p_Vtnode_GND)/Q_p/Q_p);
I_Tf1_Vbi_ei=((i_0f_Vbi_ei*Q_p-i_0f*Q_p_Vbi_ei)/Q_p/Q_p);
I_Tf1_Vbi_ci=(-i_0f*Q_p_Vbi_ci/Q_p/Q_p);
#endif
I_Tf1=(i_0f/Q_p);
#if defined(_DERIVATE)
a_h_Vci_ei=0.0;
a_h_Vtnode_GND=(Oich*I_Tf1_Vtnode_GND);
a_h_Vbi_ei=(Oich*I_Tf1_Vbi_ei);
a_h_Vbi_ci=(Oich*I_Tf1_Vbi_ci);
#endif
a_h=(Oich*I_Tf1);
#if defined(_DERIVATE)
itf_Vci_ei=0.0;
itf_Vtnode_GND=((I_Tf1_Vtnode_GND*(1.0+a_h))+(I_Tf1*a_h_Vtnode_GND));
itf_Vbi_ei=((I_Tf1_Vbi_ei*(1.0+a_h))+(I_Tf1*a_h_Vbi_ei));
itf_Vbi_ci=((I_Tf1_Vbi_ci*(1.0+a_h))+(I_Tf1*a_h_Vbi_ci));
#endif
itf=(I_Tf1*(1.0+a_h));
#if defined(_DERIVATE)
itr_Vci_ei=0.0;
itr_Vtnode_GND=((i_0r_Vtnode_GND*Q_p-i_0r*Q_p_Vtnode_GND)/Q_p/Q_p);
itr_Vbi_ci=((i_0r_Vbi_ci*Q_p-i_0r*Q_p_Vbi_ci)/Q_p/Q_p);
itr_Vbi_ei=(-i_0r*Q_p_Vbi_ei/Q_p/Q_p);
#endif
itr=(i_0r/Q_p);
#if defined(_DERIVATE)
Q_bf_Vtnode_GND=0.0;
Q_bf_Vbi_ei=0.0;
Q_bf_Vbi_ci=0.0;
Q_bf_Vci_ei=0.0;
#endif
Q_bf=0.0;
#if defined(_DERIVATE)
Tf_Vbi_ei=0.0;
Tf_Vci_ei=0.0;
Tf_Vtnode_GND=T_f0_Vtnode_GND;
Tf_Vbi_ci=T_f0_Vbi_ci;
#endif
Tf=T_f0;
#if defined(_DERIVATE)
Qf_Vci_ei=0.0;
Qf_Vtnode_GND=((T_f0_Vtnode_GND*itf)+(T_f0*itf_Vtnode_GND));
Qf_Vbi_ci=((T_f0_Vbi_ci*itf)+(T_f0*itf_Vbi_ci));
Qf_Vbi_ei=(T_f0*itf_Vbi_ei);
#endif
Qf=(T_f0*itf);
if
(itf<(1.0e-6*ick))
{
#if defined(_DERIVATE)
Q_fT_Vci_ei=0.0;
Q_fT_Vtnode_GND=Qf_Vtnode_GND;
Q_fT_Vbi_ci=Qf_Vbi_ci;
Q_fT_Vbi_ei=Qf_Vbi_ei;
#endif
Q_fT=Qf;
#if defined(_DERIVATE)
T_fT_Vbi_ei=0.0;
T_fT_Vci_ei=0.0;
T_fT_Vtnode_GND=Tf_Vtnode_GND;
T_fT_Vbi_ci=Tf_Vbi_ci;
#endif
T_fT=Tf;
}
else
{
#if defined(_DERIVATE)
FFitf_ick_Vtnode_GND=((itf_Vtnode_GND*ick-itf*ick_Vtnode_GND)/ick/ick);
FFitf_ick_Vbi_ei=(itf_Vbi_ei/ick);
FFitf_ick_Vbi_ci=(itf_Vbi_ci/ick);
FFitf_ick_Vci_ei=((itf_Vci_ei*ick-itf*ick_Vci_ei)/ick/ick);
#endif
FFitf_ick=(itf/ick);
{
double m00_logE(d00_logE0,FFitf_ick)
double m00_exp(d00_exp1,(gtfe*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FFitf_ick)
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
FFdTef_Vtnode_GND=((tef0_t_Vtnode_GND*d00_exp1)+(tef0_t*(gtfe*FFitf_ick_Vtnode_GND*d10_logE0)*d10_exp1));
FFdTef_Vbi_ei=(tef0_t*(gtfe*FFitf_ick_Vbi_ei*d10_logE0)*d10_exp1);
FFdTef_Vbi_ci=(tef0_t*(gtfe*FFitf_ick_Vbi_ci*d10_logE0)*d10_exp1);
FFdTef_Vci_ei=(tef0_t*(gtfe*FFitf_ick_Vci_ei*d10_logE0)*d10_exp1);
#endif
FFdTef=(tef0_t*d00_exp1);
}
#if defined(_DERIVATE)
FFdQef_Vtnode_GND=(((FFdTef_Vtnode_GND*itf)+(FFdTef*itf_Vtnode_GND))/(1+gtfe));
FFdQef_Vbi_ei=(((FFdTef_Vbi_ei*itf)+(FFdTef*itf_Vbi_ei))/(1+gtfe));
FFdQef_Vbi_ci=(((FFdTef_Vbi_ci*itf)+(FFdTef*itf_Vbi_ci))/(1+gtfe));
FFdQef_Vci_ei=(((FFdTef_Vci_ei*itf)+(FFdTef*itf_Vci_ei))/(1+gtfe));
#endif
FFdQef=((FFdTef*itf)/(1+gtfe));
if
(icbar<1e-10)
{
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=0.0;
FFdVc_Vbi_ei=0.0;
FFdVc_Vbi_ci=0.0;
FFdVc_Vci_ei=0.0;
#endif
FFdVc=0;
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=0.0;
FFdVc_ditf_Vbi_ei=0.0;
FFdVc_ditf_Vbi_ci=0.0;
FFdVc_ditf_Vci_ei=0.0;
#endif
FFdVc_ditf=0;
}
else
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=((itf_Vtnode_GND-ick_Vtnode_GND)/icbar);
FFib_Vbi_ei=(itf_Vbi_ei/icbar);
FFib_Vbi_ci=(itf_Vbi_ci/icbar);
FFib_Vci_ei=((itf_Vci_ei-ick_Vci_ei)/icbar);
#endif
FFib=((itf-ick)/icbar);
if
(FFib<(-1.0e10))
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=0.0;
FFib_Vbi_ei=0.0;
FFib_Vbi_ci=0.0;
FFib_Vci_ei=0.0;
#endif
FFib=(-1.0e10);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFfcbar_Vtnode_GND=((FFib_Vtnode_GND+((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ei=((FFib_Vbi_ei+((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ci=((FFib_Vbi_ci+((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/2.0);
FFfcbar_Vci_ei=((FFib_Vci_ei+((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/2.0);
#endif
FFfcbar=((FFib+d00_sqrt0)/2.0);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFdib_ditf_Vtnode_GND=(((FFfcbar_Vtnode_GND*d00_sqrt0-FFfcbar*((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ei=(((FFfcbar_Vbi_ei*d00_sqrt0-FFfcbar*((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ci=(((FFfcbar_Vbi_ci*d00_sqrt0-FFfcbar*((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vci_ei=(((FFfcbar_Vci_ei*d00_sqrt0-FFfcbar*((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
#endif
FFdib_ditf=((FFfcbar/d00_sqrt0)/icbar);
}
{
double m00_exp(d00_exp0,((-1.0)/FFfcbar))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=(vcbar*(-(-1.0)*FFfcbar_Vtnode_GND/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ei=(vcbar*(-(-1.0)*FFfcbar_Vbi_ei/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ci=(vcbar*(-(-1.0)*FFfcbar_Vbi_ci/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vci_ei=(vcbar*(-(-1.0)*FFfcbar_Vci_ei/FFfcbar/FFfcbar)*d10_exp0);
#endif
FFdVc=(vcbar*d00_exp0);
}
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=((((FFdVc_Vtnode_GND*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vtnode_GND*FFfcbar)+(FFfcbar*FFfcbar_Vtnode_GND)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vtnode_GND));
FFdVc_ditf_Vbi_ei=((((FFdVc_Vbi_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ei*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ei));
FFdVc_ditf_Vbi_ci=((((FFdVc_Vbi_ci*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ci*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ci)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ci));
FFdVc_ditf_Vci_ei=((((FFdVc_Vci_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vci_ei*FFfcbar)+(FFfcbar*FFfcbar_Vci_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vci_ei));
#endif
FFdVc_ditf=((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf);
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQbfb_Vtnode_GND=((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQbfb_Vbi_ei=(((((1-fthc)*thcs_t)*itf_Vbi_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQbfb_Vbi_ci=(((((1-fthc)*thcs_t)*itf_Vbi_ci)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQbfb_Vci_ei=(((((1-fthc)*thcs_t)*itf_Vci_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQbfb=((((1-fthc)*thcs_t)*itf)*(d00_exp0-1));
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTbfb_Vtnode_GND=(((FFdQbfb_Vtnode_GND*itf-FFdQbfb*itf_Vtnode_GND)/itf/itf)+((((((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*d00_exp0)+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0))*VT-((((1-fthc)*thcs_t)*itf)*d00_exp0)*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTbfb_Vbi_ei=(((FFdQbfb_Vbi_ei*itf-FFdQbfb*itf_Vbi_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ei)));
FFdTbfb_Vbi_ci=(((FFdQbfb_Vbi_ci*itf-FFdQbfb*itf_Vbi_ci)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ci)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ci)));
FFdTbfb_Vci_ei=(((FFdQbfb_Vci_ei*itf-FFdQbfb*itf_Vci_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vci_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTbfb=((FFdQbfb/itf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf));
}
#if defined(_DERIVATE)
FFic_Vtnode_GND=(-(-FFitf_ick_Vtnode_GND/FFitf_ick/FFitf_ick));
FFic_Vbi_ei=(-(-FFitf_ick_Vbi_ei/FFitf_ick/FFitf_ick));
FFic_Vbi_ci=(-(-FFitf_ick_Vbi_ci/FFitf_ick/FFitf_ick));
FFic_Vci_ei=(-(-FFitf_ick_Vci_ei/FFitf_ick/FFitf_ick));
#endif
FFic=(1-(1.0/FFitf_ick));
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
double m00_sqrt(d00_sqrt1,(1+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFw_Vtnode_GND=((FFic_Vtnode_GND+((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ei=((FFic_Vbi_ei+((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ci=((FFic_Vbi_ci+((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vci_ei=((FFic_Vci_ei+((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0)/(1+d00_sqrt1));
#endif
FFw=((FFic+d00_sqrt0)/(1+d00_sqrt1));
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQfhc_Vtnode_GND=((((((((thcs_t_Vtnode_GND*itf)+(thcs_t*itf_Vtnode_GND))*FFw)+((thcs_t*itf)*FFw_Vtnode_GND))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vtnode_GND))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQfhc_Vbi_ei=(((((((thcs_t*itf_Vbi_ei)*FFw)+((thcs_t*itf)*FFw_Vbi_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQfhc_Vbi_ci=(((((((thcs_t*itf_Vbi_ci)*FFw)+((thcs_t*itf)*FFw_Vbi_ci))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ci))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQfhc_Vci_ei=(((((((thcs_t*itf_Vci_ei)*FFw)+((thcs_t*itf)*FFw_Vci_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vci_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQfhc=((((thcs_t*itf)*FFw)*FFw)*d00_exp0);
}
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFdTfhc_Vtnode_GND=((FFdQfhc_Vtnode_GND*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vtnode_GND/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vtnode_GND*d00_sqrt0)+(FFitf_ick*((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+(((-VT_Vtnode_GND/VT/VT)*FFdVc_ditf)+((1.0/VT)*FFdVc_ditf_Vtnode_GND)))));
FFdTfhc_Vbi_ei=((FFdQfhc_Vbi_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ei))));
FFdTfhc_Vbi_ci=((FFdQfhc_Vbi_ci*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ci/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ci*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ci))));
FFdTfhc_Vci_ei=((FFdQfhc_Vci_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vci_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vci_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vci_ei))));
#endif
FFdTfhc=(FFdQfhc*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)));
}
if
((latb<=0.0)&&(latl<=0.0))
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(fthc*FFdQfhc_Vtnode_GND);
FFdQcfc_Vbi_ei=(fthc*FFdQfhc_Vbi_ei);
FFdQcfc_Vbi_ci=(fthc*FFdQfhc_Vbi_ci);
FFdQcfc_Vci_ei=(fthc*FFdQfhc_Vci_ei);
#endif
FFdQcfc=(fthc*FFdQfhc);
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=(fthc*FFdTfhc_Vtnode_GND);
FFdTcfc_Vbi_ei=(fthc*FFdTfhc_Vbi_ei);
FFdTcfc_Vbi_ci=(fthc*FFdTfhc_Vbi_ci);
FFdTcfc_Vci_ei=(fthc*FFdTfhc_Vci_ei);
#endif
FFdTcfc=(fthc*FFdTfhc);
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=FFdQcfc_Vtnode_GND;
FFdQcfcT_Vbi_ei=FFdQcfc_Vbi_ei;
FFdQcfcT_Vbi_ci=FFdQcfc_Vbi_ci;
FFdQcfcT_Vci_ei=FFdQcfc_Vci_ei;
#endif
FFdQcfcT=FFdQcfc;
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=FFdTcfc_Vtnode_GND;
FFdTcfcT_Vbi_ei=FFdTcfc_Vbi_ei;
FFdTcfcT_Vbi_ci=FFdTcfc_Vbi_ci;
FFdTcfcT_Vci_ei=FFdTcfc_Vci_ei;
#endif
FFdTcfcT=FFdTcfc;
}
else
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(((fthc*thcs_t_Vtnode_GND)*itf)+((fthc*thcs_t)*itf_Vtnode_GND));
FFdQcfc_Vbi_ei=((fthc*thcs_t)*itf_Vbi_ei);
FFdQcfc_Vbi_ci=((fthc*thcs_t)*itf_Vbi_ci);
FFdQcfc_Vci_ei=0.0;
#endif
FFdQcfc=((fthc*thcs_t)*itf);
#if defined(_DERIVATE)
FCa_Vci_ei=(-((ick_Vci_ei*itf-ick*itf_Vci_ei)/itf/itf));
FCa_Vtnode_GND=(-((ick_Vtnode_GND*itf-ick*itf_Vtnode_GND)/itf/itf));
FCa_Vbi_ei=(-(-ick*itf_Vbi_ei/itf/itf));
FCa_Vbi_ci=(-(-ick*itf_Vbi_ci/itf/itf));
#endif
FCa=(1.0-(ick/itf));
{
double m00_sqrt(d00_sqrt0,((FCa*FCa)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FCa*FCa)+ahc))
#endif
#if defined(_DERIVATE)
FCrt_Vci_ei=((FCa_Vci_ei*FCa)+(FCa*FCa_Vci_ei))*d10_sqrt0;
FCrt_Vtnode_GND=((FCa_Vtnode_GND*FCa)+(FCa*FCa_Vtnode_GND))*d10_sqrt0;
FCrt_Vbi_ei=((FCa_Vbi_ei*FCa)+(FCa*FCa_Vbi_ei))*d10_sqrt0;
FCrt_Vbi_ci=((FCa_Vbi_ci*FCa)+(FCa*FCa_Vbi_ci))*d10_sqrt0;
#endif
FCrt=d00_sqrt0;
}
{
double m00_sqrt(d00_sqrt0,(1.0+ahc))
#if defined(_DERIVATE)
FCa_ck_Vci_ei=(-((FCa_Vci_ei+FCrt_Vci_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vtnode_GND=(-((FCa_Vtnode_GND+FCrt_Vtnode_GND)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ei=(-((FCa_Vbi_ei+FCrt_Vbi_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ci=(-((FCa_Vbi_ci+FCrt_Vbi_ci)/(1.0+d00_sqrt0)));
#endif
FCa_ck=(1.0-((FCa+FCrt)/(1.0+d00_sqrt0)));
}
#if defined(_DERIVATE)
FCdaick_ditf_Vci_ei=((((FCa_ck_Vci_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vci_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vci_ei*itf)+(FCrt*itf_Vci_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vtnode_GND=((((FCa_ck_Vtnode_GND*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vtnode_GND)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vtnode_GND*itf)+(FCrt*itf_Vtnode_GND)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ei=((((FCa_ck_Vbi_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ei*itf)+(FCrt*itf_Vbi_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ci=((((FCa_ck_Vbi_ci*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ci)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ci*itf)+(FCrt*itf_Vbi_ci)))/(FCrt*itf)/(FCrt*itf));
#endif
FCdaick_ditf=(((FCa_ck-1.0)*(1-FCa))/(FCrt*itf));
if
(latb>latl)
{
#if defined(_DERIVATE)
FCz_Vci_ei=0.0;
FCz_Vtnode_GND=0.0;
FCz_Vbi_ei=0.0;
FCz_Vbi_ci=0.0;
#endif
FCz=(latb-latl);
FCxl=(1.0+latl);
FCxb=(1.0+latb);
if
(latb>0.01)
{
{
double m00_logE(d00_logE0,(FCxb/FCxl))
FCln=d00_logE0;
}
{
double m00_exp(d00_exp0,((FCa_ck-1.0)*FCln))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=FCa_ck_Vci_ei*FCln*d10_exp0;
FCa1_Vtnode_GND=FCa_ck_Vtnode_GND*FCln*d10_exp0;
FCa1_Vbi_ei=FCa_ck_Vbi_ei*FCln*d10_exp0;
FCa1_Vbi_ci=FCa_ck_Vbi_ci*FCln*d10_exp0;
#endif
FCa1=d00_exp0;
}
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-(-FCa1_Vci_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vtnode_GND=(-(-FCa1_Vtnode_GND*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ei=(-(-FCa1_Vbi_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ci=(-(-FCa1_Vbi_ci*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
#endif
FCd_a=(1.0/(latl-(FCa1*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCa1_Vci_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCa1_Vtnode_GND*FCd_a)+((FCa1-1.0)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCa1_Vbi_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCa1_Vbi_ci*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ci));
#endif
FCw=((FCa1-1.0)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=(((((((-FCz_Vci_ei)*FCa1)+((-FCz)*FCa1_Vci_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vci_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=(((((((-FCz_Vtnode_GND)*FCa1)+((-FCz)*FCa1_Vtnode_GND))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vtnode_GND))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=(((((((-FCz_Vbi_ei)*FCa1)+((-FCz)*FCa1_Vbi_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=(((((((-FCz_Vbi_ci)*FCa1)+((-FCz)*FCa1_Vbi_ci))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ci))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a);
{
double m00_logE(d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=(((latb*FCw_Vci_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vci_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vtnode_GND=(((latb*FCw_Vtnode_GND)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vtnode_GND))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ei=(((latb*FCw_Vbi_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ci=(((latb*FCw_Vbi_ci)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ci))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
#endif
FCa1=d00_logE0;
}
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=((-latb*(latb*FCw_Vci_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vci_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vtnode_GND=((-latb*(latb*FCw_Vtnode_GND)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vtnode_GND)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ei=((-latb*(latb*FCw_Vbi_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ci=((-latb*(latb*FCw_Vbi_ci)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ci)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
#endif
FCda1_dw=((latb/(1.0+(latb*FCw)))-(latl/(1.0+(latl*FCw))));
}
else
{
#if defined(_DERIVATE)
FCf1_Vci_ei=(-FCa_ck_Vci_ei);
FCf1_Vtnode_GND=(-FCa_ck_Vtnode_GND);
FCf1_Vbi_ei=(-FCa_ck_Vbi_ei);
FCf1_Vbi_ci=(-FCa_ck_Vbi_ci);
#endif
FCf1=(1.0-FCa_ck);
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCf1_Vci_ei*FCd_a)+(FCf1*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCf1_Vtnode_GND*FCd_a)+(FCf1*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCf1_Vbi_ei*FCd_a)+(FCf1*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCf1_Vbi_ci*FCd_a)+(FCf1*FCd_a_Vbi_ci));
#endif
FCw=(FCf1*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((((-1.0)*FCd_a_Vci_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vci_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((((-1.0)*FCd_a_Vtnode_GND)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vtnode_GND))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((((-1.0)*FCd_a_Vbi_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((((-1.0)*FCd_a_Vbi_ci)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ci))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a);
#if defined(_DERIVATE)
FCa1_Vci_ei=((FCz_Vci_ei*FCw)+(FCz*FCw_Vci_ei));
FCa1_Vtnode_GND=((FCz_Vtnode_GND*FCw)+(FCz*FCw_Vtnode_GND));
FCa1_Vbi_ei=((FCz_Vbi_ei*FCw)+(FCz*FCw_Vbi_ei));
FCa1_Vbi_ci=((FCz_Vbi_ci*FCw)+(FCz*FCw_Vbi_ci));
#endif
FCa1=(FCz*FCw);
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=0.0;
FCda1_dw_Vtnode_GND=0.0;
FCda1_dw_Vbi_ei=0.0;
FCda1_dw_Vbi_ci=0.0;
#endif
FCda1_dw=FCz;
}
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=0.0;
FCf_CT_Vbi_ci=0.0;
FCf_CT_Vci_ei=0.0;
FCf_CT_Vtnode_GND=0.0;
#endif
FCf_CT=(2.0/FCz);
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCf1_Vci_ei=(((((((latb*latl)*FCw_Vci_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vci_ei))/3.0)+(((latb+latl)*FCw2_Vci_ei)/2.0))+FCw_Vci_ei);
FCf1_Vtnode_GND=(((((((latb*latl)*FCw_Vtnode_GND)*FCw2)+(((latb*latl)*FCw)*FCw2_Vtnode_GND))/3.0)+(((latb+latl)*FCw2_Vtnode_GND)/2.0))+FCw_Vtnode_GND);
FCf1_Vbi_ei=(((((((latb*latl)*FCw_Vbi_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ei))/3.0)+(((latb+latl)*FCw2_Vbi_ei)/2.0))+FCw_Vbi_ei);
FCf1_Vbi_ci=(((((((latb*latl)*FCw_Vbi_ci)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ci))/3.0)+(((latb+latl)*FCw2_Vbi_ci)/2.0))+FCw_Vbi_ci);
#endif
FCf1=((((((latb*latl)*FCw)*FCw2)/3.0)+(((latb+latl)*FCw2)/2.0))+FCw);
#if defined(_DERIVATE)
FCdf1_dw_Vci_ei=(((latb*latl)*FCw2_Vci_ei)+((latb+latl)*FCw_Vci_ei));
FCdf1_dw_Vtnode_GND=(((latb*latl)*FCw2_Vtnode_GND)+((latb+latl)*FCw_Vtnode_GND));
FCdf1_dw_Vbi_ei=(((latb*latl)*FCw2_Vbi_ei)+((latb+latl)*FCw_Vbi_ei));
FCdf1_dw_Vbi_ci=(((latb*latl)*FCw2_Vbi_ci)+((latb+latl)*FCw_Vbi_ci));
#endif
FCdf1_dw=((((latb*latl)*FCw2)+((latb+latl)*FCw))+1.0);
#if defined(_DERIVATE)
z_Vci_ei=(latb*FCw_Vci_ei);
z_Vtnode_GND=(latb*FCw_Vtnode_GND);
z_Vbi_ei=(latb*FCw_Vbi_ei);
z_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
z=(latb*FCw);
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latl/latb);
#if defined(_DERIVATE)
FCf2_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latb);
FCf2_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latb);
FCf2_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latb);
FCf2_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latb);
#endif
FCf2=((((1.0-r)*a2)+(r*a3))/latb);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf2_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((((((latb*a2_Vbi_ei)+(latl*a3_Vbi_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf2_Vbi_ci=(((((((latb*a2_Vbi_ci)+(latl*a3_Vbi_ci))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ci))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf2_Vci_ei=(((((((latb*a2_Vci_ei)+(latl*a3_Vci_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vci_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf2_Vtnode_GND=(((((((latb*a2_Vtnode_GND)+(latl*a3_Vtnode_GND))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vtnode_GND))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf2=(((((latb*a2)+(latl*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((latl*FCw_Vci_ei)*(1+z))+((1+(latl*FCw))*z_Vci_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((latl*FCw_Vtnode_GND)*(1+z))+((1+(latl*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((latl*FCw_Vbi_ei)*(1+z))+((1+(latl*FCw))*z_Vbi_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((latl*FCw_Vbi_ci)*(1+z))+((1+(latl*FCw))*z_Vbi_ci))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf2_dw=(((1+(latl*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
z_Vci_ei=(latl*FCw_Vci_ei);
z_Vtnode_GND=(latl*FCw_Vtnode_GND);
z_Vbi_ei=(latl*FCw_Vbi_ei);
z_Vbi_ci=(latl*FCw_Vbi_ci);
#endif
z=(latl*FCw);
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latb/latl);
#if defined(_DERIVATE)
FCf3_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latl);
FCf3_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latl);
FCf3_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latl);
FCf3_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latl);
#endif
FCf3=((((1.0-r)*a2)+(r*a3))/latl);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf3_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((((((latl*a2_Vbi_ei)+(latb*a3_Vbi_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf3_Vbi_ci=(((((((latl*a2_Vbi_ci)+(latb*a3_Vbi_ci))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ci))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf3_Vci_ei=(((((((latl*a2_Vci_ei)+(latb*a3_Vci_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vci_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf3_Vtnode_GND=(((((((latl*a2_Vtnode_GND)+(latb*a3_Vtnode_GND))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vtnode_GND))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf3=(((((latl*a2)+(latb*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((latb*FCw_Vci_ei)*(1+z))+((1+(latb*FCw))*z_Vci_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((latb*FCw_Vtnode_GND)*(1+z))+((1+(latb*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((latb*FCw_Vbi_ei)*(1+z))+((1+(latb*FCw))*z_Vbi_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((latb*FCw_Vbi_ci)*(1+z))+((1+(latb*FCw))*z_Vbi_ci))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf3_dw=(((1+(latb*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((FCf_CT_Vci_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vci_ei*FCf1)+(FCa1*FCf1_Vci_ei))-FCf2_Vci_ei)+FCf3_Vci_ei)));
FCf_ci_Vtnode_GND=((FCf_CT_Vtnode_GND*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vtnode_GND*FCf1)+(FCa1*FCf1_Vtnode_GND))-FCf2_Vtnode_GND)+FCf3_Vtnode_GND)));
FCf_ci_Vbi_ei=((FCf_CT_Vbi_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ei*FCf1)+(FCa1*FCf1_Vbi_ei))-FCf2_Vbi_ei)+FCf3_Vbi_ei)));
FCf_ci_Vbi_ci=((FCf_CT_Vbi_ci*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ci*FCf1)+(FCa1*FCf1_Vbi_ci))-FCf2_Vbi_ci)+FCf3_Vbi_ci)));
#endif
FCf_ci=(FCf_CT*(((FCa1*FCf1)-FCf2)+FCf3));
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=((FCf_CT_Vci_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vci_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vci_ei))+((FCda1_dw_Vci_ei*FCf1)+(FCda1_dw*FCf1_Vci_ei)))-FCdf2_dw_Vci_ei)+FCdf3_dw_Vci_ei)));
FCdfc_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vtnode_GND*FCdf1_dw)+(FCa1*FCdf1_dw_Vtnode_GND))+((FCda1_dw_Vtnode_GND*FCf1)+(FCda1_dw*FCf1_Vtnode_GND)))-FCdf2_dw_Vtnode_GND)+FCdf3_dw_Vtnode_GND)));
FCdfc_dw_Vbi_ei=((FCf_CT_Vbi_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ei))+((FCda1_dw_Vbi_ei*FCf1)+(FCda1_dw*FCf1_Vbi_ei)))-FCdf2_dw_Vbi_ei)+FCdf3_dw_Vbi_ei)));
FCdfc_dw_Vbi_ci=((FCf_CT_Vbi_ci*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ci*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ci))+((FCda1_dw_Vbi_ci*FCf1)+(FCda1_dw*FCf1_Vbi_ci)))-FCdf2_dw_Vbi_ci)+FCdf3_dw_Vbi_ci)));
#endif
FCdfc_dw=(FCf_CT*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw));
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
if
((flcomp==0.0)||(flcomp==2.1))
{
#if defined(_DERIVATE)
a_Vbi_ei=(latb*FCw_Vbi_ei);
a_Vbi_ci=(latb*FCw_Vbi_ci);
a_Vci_ei=(latb*FCw_Vci_ei);
a_Vtnode_GND=(latb*FCw_Vtnode_GND);
#endif
a=(latb*FCw);
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
if
(a>1.0e-6)
{
#if defined(_DERIVATE)
FCf2_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latb);
FCf2_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latb);
FCf2_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latb);
FCf2_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latb);
#endif
FCf2=((a-lnz)/latb);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf2_dw=(a/(1.0+a));
}
else
{
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf2_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf2_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf2_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf2=((0.5*a)*FCw);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=a_Vci_ei;
FCdf2_dw_Vtnode_GND=a_Vtnode_GND;
FCdf2_dw_Vbi_ei=a_Vbi_ei;
FCdf2_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf2_dw=a;
}
#if defined(_DERIVATE)
a_Vbi_ei=(latl*FCw_Vbi_ei);
a_Vbi_ci=(latl*FCw_Vbi_ci);
a_Vci_ei=(latl*FCw_Vci_ei);
a_Vtnode_GND=(latl*FCw_Vtnode_GND);
#endif
a=(latl*FCw);
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
if
(a>1.0e-6)
{
#if defined(_DERIVATE)
FCf3_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latl);
FCf3_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latl);
FCf3_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latl);
FCf3_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latl);
#endif
FCf3=((a-lnz)/latl);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf3_dw=(a/(1.0+a));
}
else
{
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf3_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf3_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf3_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf3=((0.5*a)*FCw);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=a_Vci_ei;
FCdf3_dw_Vtnode_GND=a_Vtnode_GND;
FCdf3_dw_Vbi_ei=a_Vbi_ei;
FCdf3_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf3_dw=a;
}
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((FCf_CT_Vbi_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ei-FCf3_Vbi_ei)));
FCf_CT_Vbi_ci=((FCf_CT_Vbi_ci*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ci-FCf3_Vbi_ci)));
FCf_CT_Vci_ei=((FCf_CT_Vci_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vci_ei-FCf3_Vci_ei)));
FCf_CT_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCf2-FCf3))+(FCf_CT*(FCf2_Vtnode_GND-FCf3_Vtnode_GND)));
#endif
FCf_CT=(FCf_CT*(FCf2-FCf3));
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=((FCf_CT_Vbi_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ei-FCdf3_dw_Vbi_ei)));
FCdfCT_dw_Vbi_ci=((FCf_CT_Vbi_ci*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ci-FCdf3_dw_Vbi_ci)));
FCdfCT_dw_Vci_ei=((FCf_CT_Vci_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vci_ei-FCdf3_dw_Vci_ei)));
FCdfCT_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vtnode_GND-FCdf3_dw_Vtnode_GND)));
#endif
FCdfCT_dw=(FCf_CT*(FCdf2_dw-FCdf3_dw));
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
}
}
else
{
if
(latb>0.01)
{
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=(((-FCa_ck_Vci_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vci_ei));
FCw_Vtnode_GND=(((-FCa_ck_Vtnode_GND)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=(((-FCa_ck_Vbi_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ei));
FCw_Vbi_ci=(((-FCa_ck_Vbi_ci)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ci));
#endif
FCw=((1.0-FCa_ck)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((-(1.0+latb))*FCd_a_Vci_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((-(1.0+latb))*FCd_a_Vtnode_GND)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((-(1.0+latb))*FCd_a_Vbi_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((-(1.0+latb))*FCd_a_Vbi_ci)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((-(1.0+latb))*FCd_a)*FCd_a);
}
else
{
#if defined(_DERIVATE)
FCw_Vci_ei=((-FCa_ck_Vci_ei)-FCa_ck_Vci_ei*latb);
FCw_Vtnode_GND=((-FCa_ck_Vtnode_GND)-FCa_ck_Vtnode_GND*latb);
FCw_Vbi_ei=((-FCa_ck_Vbi_ei)-FCa_ck_Vbi_ei*latb);
FCw_Vbi_ci=((-FCa_ck_Vbi_ci)-FCa_ck_Vbi_ci*latb);
#endif
FCw=((1.0-FCa_ck)-(FCa_ck*latb));
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=0.0;
FCdw_daick_Vtnode_GND=0.0;
FCdw_daick_Vbi_ei=0.0;
FCdw_daick_Vbi_ci=0.0;
#endif
FCdw_daick=(-(1.0+latb));
}
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCz_Vci_ei=(latb*FCw_Vci_ei);
FCz_Vtnode_GND=(latb*FCw_Vtnode_GND);
FCz_Vbi_ei=(latb*FCw_Vbi_ei);
FCz_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
FCz=(latb*FCw);
#if defined(_DERIVATE)
FCz_1_Vci_ei=FCz_Vci_ei;
FCz_1_Vtnode_GND=FCz_Vtnode_GND;
FCz_1_Vbi_ei=FCz_Vbi_ei;
FCz_1_Vbi_ci=FCz_Vbi_ci;
#endif
FCz_1=(1.0+FCz);
#if defined(_DERIVATE)
FCd_f_Vci_ei=(-FCz_1_Vci_ei/FCz_1/FCz_1);
FCd_f_Vtnode_GND=(-FCz_1_Vtnode_GND/FCz_1/FCz_1);
FCd_f_Vbi_ei=(-FCz_1_Vbi_ei/FCz_1/FCz_1);
FCd_f_Vbi_ci=(-FCz_1_Vbi_ci/FCz_1/FCz_1);
#endif
FCd_f=(1.0/FCz_1);
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((((FCw2_Vci_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vci_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vci_ei));
FCf_ci_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vtnode_GND/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vtnode_GND));
FCf_ci_Vbi_ei=((((FCw2_Vbi_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_ci_Vbi_ci=((((FCw2_Vbi_ci*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ci/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ci));
#endif
FCf_ci=((FCw2*(1.0+(FCz/3.0)))*FCd_f);
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vci_ei+(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfc_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vtnode_GND+(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
FCdfc_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ei+(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfc_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ci+(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
#endif
FCdfc_dw=((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
if
((flcomp==0.0)||(flcomp==2.1))
{
if
(FCz>0.001)
{
{
double m00_logE(d00_logE0,FCz_1)
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FCz_1)
#endif
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=(((2.0*(((FCz_1_Vbi_ei*d00_logE0)+(FCz_1*FCz_1_Vbi_ei*d10_logE0))-FCz_Vbi_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vbi_ci=(((2.0*(((FCz_1_Vbi_ci*d00_logE0)+(FCz_1*FCz_1_Vbi_ci*d10_logE0))-FCz_Vbi_ci))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ci))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vci_ei=(((2.0*(((FCz_1_Vci_ei*d00_logE0)+(FCz_1*FCz_1_Vci_ei*d10_logE0))-FCz_Vci_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vci_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vtnode_GND=(((2.0*(((FCz_1_Vtnode_GND*d00_logE0)+(FCz_1*FCz_1_Vtnode_GND*d10_logE0))-FCz_Vtnode_GND))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vtnode_GND))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
#endif
FCf_CT=((2.0*((FCz_1*d00_logE0)-FCz))/((latb*latb)*FCz_1));
}
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((2.0*FCw_Vbi_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((2.0*FCw_Vbi_ci)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ci))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((2.0*FCw_Vci_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vci_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((2.0*FCw_Vtnode_GND)*FCd_f)+((2.0*FCw)*FCd_f_Vtnode_GND))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=(((2.0*FCw)*FCd_f)*FCd_f);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((((FCw2_Vbi_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_CT_Vbi_ci=((((FCw2_Vbi_ci*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ci/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ci));
FCf_CT_Vci_ei=((((FCw2_Vci_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vci_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vci_ei));
FCf_CT_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vtnode_GND/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vtnode_GND));
#endif
FCf_CT=((FCw2*(1.0-(FCz/3.0)))*FCd_f);
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
}
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
}
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_CT)+(FFdQcfc*FCf_CT_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_CT)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfcT_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfcT_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfcT_Vci_ei=((((FFdQcfc_Vci_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfcT=((FFdQcfc*FCf_CT)*d00_exp0);
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_ci)+(FFdQcfc*FCf_ci_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_ci)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfc_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfc_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfc_Vci_ei=((((FFdQcfc_Vci_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfc=((FFdQcfc*FCf_ci)*d00_exp0);
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vtnode_GND+((itf_Vtnode_GND*FCdfc_ditf)+(itf*FCdfc_ditf_Vtnode_GND)))))+((((FFdQcfc_Vtnode_GND*VT-FFdQcfc*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfc_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ei+((itf_Vbi_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ei)))))+(((FFdQcfc_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfc_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ci+((itf_Vbi_ci*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ci)))))+(((FFdQcfc_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfc_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vci_ei+((itf_Vci_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vci_ei)))))+(((FFdQcfc_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfc=((((fthc*thcs_t)*d00_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+((FFdQcfc/VT)*FFdVc_ditf));
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vtnode_GND+((itf_Vtnode_GND*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vtnode_GND)))))+((((FFdQcfcT_Vtnode_GND*VT-FFdQcfcT*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfcT_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ei+((itf_Vbi_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ei)))))+(((FFdQcfcT_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfcT_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ci+((itf_Vbi_ci*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ci)))))+(((FFdQcfcT_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfcT_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vci_ei+((itf_Vci_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vci_ei)))))+(((FFdQcfcT_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfcT=((((fthc*thcs_t)*d00_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+((FFdQcfcT/VT)*FFdVc_ditf));
}
}
#if defined(_DERIVATE)
FFdQbfc_Vtnode_GND=((1-fthc)*FFdQfhc_Vtnode_GND);
FFdQbfc_Vbi_ei=((1-fthc)*FFdQfhc_Vbi_ei);
FFdQbfc_Vbi_ci=((1-fthc)*FFdQfhc_Vbi_ci);
FFdQbfc_Vci_ei=((1-fthc)*FFdQfhc_Vci_ei);
#endif
FFdQbfc=((1-fthc)*FFdQfhc);
#if defined(_DERIVATE)
FFdTbfc_Vtnode_GND=((1-fthc)*FFdTfhc_Vtnode_GND);
FFdTbfc_Vbi_ei=((1-fthc)*FFdTfhc_Vbi_ei);
FFdTbfc_Vbi_ci=((1-fthc)*FFdTfhc_Vbi_ci);
FFdTbfc_Vci_ei=((1-fthc)*FFdTfhc_Vci_ei);
#endif
FFdTbfc=((1-fthc)*FFdTfhc);
#if defined(_DERIVATE)
Q_fT_Vci_ei=(((((hf0_t*Qf_Vci_ei)+FFdQbfb_Vci_ei)+FFdQbfc_Vci_ei)+(hfe_t*FFdQef_Vci_ei))+(hfc_t*FFdQcfcT_Vci_ei));
Q_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Qf)+(hf0_t*Qf_Vtnode_GND))+FFdQbfb_Vtnode_GND)+FFdQbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdQef)+(hfe_t*FFdQef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdQcfcT)+(hfc_t*FFdQcfcT_Vtnode_GND)));
Q_fT_Vbi_ci=(((((hf0_t*Qf_Vbi_ci)+FFdQbfb_Vbi_ci)+FFdQbfc_Vbi_ci)+(hfe_t*FFdQef_Vbi_ci))+(hfc_t*FFdQcfcT_Vbi_ci));
Q_fT_Vbi_ei=(((((hf0_t*Qf_Vbi_ei)+FFdQbfb_Vbi_ei)+FFdQbfc_Vbi_ei)+(hfe_t*FFdQef_Vbi_ei))+(hfc_t*FFdQcfcT_Vbi_ei));
#endif
Q_fT=(((((hf0_t*Qf)+FFdQbfb)+FFdQbfc)+(hfe_t*FFdQef))+(hfc_t*FFdQcfcT));
#if defined(_DERIVATE)
T_fT_Vbi_ei=(((((hf0_t*Tf_Vbi_ei)+FFdTbfb_Vbi_ei)+FFdTbfc_Vbi_ei)+(hfe_t*FFdTef_Vbi_ei))+(hfc_t*FFdTcfcT_Vbi_ei));
T_fT_Vci_ei=(((((hf0_t*Tf_Vci_ei)+FFdTbfb_Vci_ei)+FFdTbfc_Vci_ei)+(hfe_t*FFdTef_Vci_ei))+(hfc_t*FFdTcfcT_Vci_ei));
T_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Tf)+(hf0_t*Tf_Vtnode_GND))+FFdTbfb_Vtnode_GND)+FFdTbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdTef)+(hfe_t*FFdTef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdTcfcT)+(hfc_t*FFdTcfcT_Vtnode_GND)));
T_fT_Vbi_ci=(((((hf0_t*Tf_Vbi_ci)+FFdTbfb_Vbi_ci)+FFdTbfc_Vbi_ci)+(hfe_t*FFdTef_Vbi_ci))+(hfc_t*FFdTcfcT_Vbi_ci));
#endif
T_fT=(((((hf0_t*Tf)+FFdTbfb)+FFdTbfc)+(hfe_t*FFdTef))+(hfc_t*FFdTcfcT));
#if defined(_DERIVATE)
Qf_Vci_ei=(((Qf_Vci_ei+(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei))+FFdQef_Vci_ei)+FFdQcfc_Vci_ei);
Qf_Vtnode_GND=(((Qf_Vtnode_GND+(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND))+FFdQef_Vtnode_GND)+FFdQcfc_Vtnode_GND);
Qf_Vbi_ci=(((Qf_Vbi_ci+(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci))+FFdQef_Vbi_ci)+FFdQcfc_Vbi_ci);
Qf_Vbi_ei=(((Qf_Vbi_ei+(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei))+FFdQef_Vbi_ei)+FFdQcfc_Vbi_ei);
#endif
Qf=(((Qf+(FFdQbfb+FFdQbfc))+FFdQef)+FFdQcfc);
#if defined(_DERIVATE)
Tf_Vbi_ei=(((Tf_Vbi_ei+(FFdTbfb_Vbi_ei+FFdTbfc_Vbi_ei))+FFdTef_Vbi_ei)+FFdTcfc_Vbi_ei);
Tf_Vci_ei=(((Tf_Vci_ei+(FFdTbfb_Vci_ei+FFdTbfc_Vci_ei))+FFdTef_Vci_ei)+FFdTcfc_Vci_ei);
Tf_Vtnode_GND=(((Tf_Vtnode_GND+(FFdTbfb_Vtnode_GND+FFdTbfc_Vtnode_GND))+FFdTef_Vtnode_GND)+FFdTcfc_Vtnode_GND);
Tf_Vbi_ci=(((Tf_Vbi_ci+(FFdTbfb_Vbi_ci+FFdTbfc_Vbi_ci))+FFdTef_Vbi_ci)+FFdTcfc_Vbi_ci);
#endif
Tf=(((Tf+(FFdTbfb+FFdTbfc))+FFdTef)+FFdTcfc);
#if defined(_DERIVATE)
Q_bf_Vtnode_GND=(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND);
Q_bf_Vbi_ei=(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei);
Q_bf_Vbi_ci=(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci);
Q_bf_Vci_ei=(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei);
#endif
Q_bf=(FFdQbfb+FFdQbfc);
}
#if defined(_DERIVATE)
Qr_Vci_ei=0.0;
Qr_Vtnode_GND=(Tr*itr_Vtnode_GND);
Qr_Vbi_ci=(Tr*itr_Vbi_ci);
Qr_Vbi_ei=(Tr*itr_Vbi_ei);
#endif
Qr=(Tr*itr);
l_it=0;
if
((Qf>(1.0e-5*Q_p))||(a_h>1.0e-5))
{
{
double m00_sqrt(d00_sqrt0,((T_f0*itf)*Q_fT))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((T_f0*itf)*Q_fT))
#endif
#if defined(_DERIVATE)
Qf_Vci_ei=(((T_f0*itf_Vci_ei)*Q_fT)+((T_f0*itf)*Q_fT_Vci_ei))*d10_sqrt0;
Qf_Vtnode_GND=((((T_f0_Vtnode_GND*itf)+(T_f0*itf_Vtnode_GND))*Q_fT)+((T_f0*itf)*Q_fT_Vtnode_GND))*d10_sqrt0;
Qf_Vbi_ci=((((T_f0_Vbi_ci*itf)+(T_f0*itf_Vbi_ci))*Q_fT)+((T_f0*itf)*Q_fT_Vbi_ci))*d10_sqrt0;
Qf_Vbi_ei=(((T_f0*itf_Vbi_ei)*Q_fT)+((T_f0*itf)*Q_fT_Vbi_ei))*d10_sqrt0;
#endif
Qf=d00_sqrt0;
}
#if defined(_DERIVATE)
Q_pT_Vtnode_GND=((Q_0_Vtnode_GND+Qf_Vtnode_GND)+Qr_Vtnode_GND);
Q_pT_Vbi_ei=((Q_0_Vbi_ei+Qf_Vbi_ei)+Qr_Vbi_ei);
Q_pT_Vbi_ci=((Q_0_Vbi_ci+Qf_Vbi_ci)+Qr_Vbi_ci);
Q_pT_Vci_ei=(Qf_Vci_ei+Qr_Vci_ei);
#endif
Q_pT=((Q_0+Qf)+Qr);
#if defined(_DERIVATE)
d_Q_Vtnode_GND=Q_pT_Vtnode_GND;
d_Q_Vbi_ei=Q_pT_Vbi_ei;
d_Q_Vbi_ci=Q_pT_Vbi_ci;
d_Q_Vci_ei=Q_pT_Vci_ei;
#endif
d_Q=Q_pT;
while
(((abs(d_Q)>=(1.0e-5*abs(Q_pT)))&&(l_it<=100)))
{
#if defined(_DYNAMIC)
d_Q0=d_Q;
#endif
#if defined(_DERIVATE)
I_Tf1_Vci_ei=(-i_0f*Q_pT_Vci_ei/Q_pT/Q_pT);
I_Tf1_Vtnode_GND=((i_0f_Vtnode_GND*Q_pT-i_0f*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
I_Tf1_Vbi_ei=((i_0f_Vbi_ei*Q_pT-i_0f*Q_pT_Vbi_ei)/Q_pT/Q_pT);
I_Tf1_Vbi_ci=(-i_0f*Q_pT_Vbi_ci/Q_pT/Q_pT);
#endif
I_Tf1=(i_0f/Q_pT);
#if defined(_DERIVATE)
a_h_Vci_ei=(Oich*I_Tf1_Vci_ei);
a_h_Vtnode_GND=(Oich*I_Tf1_Vtnode_GND);
a_h_Vbi_ei=(Oich*I_Tf1_Vbi_ei);
a_h_Vbi_ci=(Oich*I_Tf1_Vbi_ci);
#endif
a_h=(Oich*I_Tf1);
#if defined(_DERIVATE)
itf_Vci_ei=((I_Tf1_Vci_ei*(1.0+a_h))+(I_Tf1*a_h_Vci_ei));
itf_Vtnode_GND=((I_Tf1_Vtnode_GND*(1.0+a_h))+(I_Tf1*a_h_Vtnode_GND));
itf_Vbi_ei=((I_Tf1_Vbi_ei*(1.0+a_h))+(I_Tf1*a_h_Vbi_ei));
itf_Vbi_ci=((I_Tf1_Vbi_ci*(1.0+a_h))+(I_Tf1*a_h_Vbi_ci));
#endif
itf=(I_Tf1*(1.0+a_h));
#if defined(_DERIVATE)
itr_Vci_ei=(-i_0r*Q_pT_Vci_ei/Q_pT/Q_pT);
itr_Vtnode_GND=((i_0r_Vtnode_GND*Q_pT-i_0r*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
itr_Vbi_ci=((i_0r_Vbi_ci*Q_pT-i_0r*Q_pT_Vbi_ci)/Q_pT/Q_pT);
itr_Vbi_ei=(-i_0r*Q_pT_Vbi_ei/Q_pT/Q_pT);
#endif
itr=(i_0r/Q_pT);
#if defined(_DERIVATE)
Tf_Vbi_ei=0.0;
Tf_Vci_ei=0.0;
Tf_Vtnode_GND=T_f0_Vtnode_GND;
Tf_Vbi_ci=T_f0_Vbi_ci;
#endif
Tf=T_f0;
#if defined(_DERIVATE)
Qf_Vci_ei=(T_f0*itf_Vci_ei);
Qf_Vtnode_GND=((T_f0_Vtnode_GND*itf)+(T_f0*itf_Vtnode_GND));
Qf_Vbi_ci=((T_f0_Vbi_ci*itf)+(T_f0*itf_Vbi_ci));
Qf_Vbi_ei=(T_f0*itf_Vbi_ei);
#endif
Qf=(T_f0*itf);
if
(itf<(1.0e-6*ick))
{
#if defined(_DERIVATE)
Q_fT_Vci_ei=Qf_Vci_ei;
Q_fT_Vtnode_GND=Qf_Vtnode_GND;
Q_fT_Vbi_ci=Qf_Vbi_ci;
Q_fT_Vbi_ei=Qf_Vbi_ei;
#endif
Q_fT=Qf;
#if defined(_DERIVATE)
T_fT_Vbi_ei=Tf_Vbi_ei;
T_fT_Vci_ei=Tf_Vci_ei;
T_fT_Vtnode_GND=Tf_Vtnode_GND;
T_fT_Vbi_ci=Tf_Vbi_ci;
#endif
T_fT=Tf;
}
else
{
#if defined(_DERIVATE)
FFitf_ick_Vtnode_GND=((itf_Vtnode_GND*ick-itf*ick_Vtnode_GND)/ick/ick);
FFitf_ick_Vbi_ei=(itf_Vbi_ei/ick);
FFitf_ick_Vbi_ci=(itf_Vbi_ci/ick);
FFitf_ick_Vci_ei=((itf_Vci_ei*ick-itf*ick_Vci_ei)/ick/ick);
#endif
FFitf_ick=(itf/ick);
{
double m00_logE(d00_logE0,FFitf_ick)
double m00_exp(d00_exp1,(gtfe*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FFitf_ick)
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
FFdTef_Vtnode_GND=((tef0_t_Vtnode_GND*d00_exp1)+(tef0_t*(gtfe*FFitf_ick_Vtnode_GND*d10_logE0)*d10_exp1));
FFdTef_Vbi_ei=(tef0_t*(gtfe*FFitf_ick_Vbi_ei*d10_logE0)*d10_exp1);
FFdTef_Vbi_ci=(tef0_t*(gtfe*FFitf_ick_Vbi_ci*d10_logE0)*d10_exp1);
FFdTef_Vci_ei=(tef0_t*(gtfe*FFitf_ick_Vci_ei*d10_logE0)*d10_exp1);
#endif
FFdTef=(tef0_t*d00_exp1);
}
#if defined(_DERIVATE)
FFdQef_Vtnode_GND=(((FFdTef_Vtnode_GND*itf)+(FFdTef*itf_Vtnode_GND))/(1+gtfe));
FFdQef_Vbi_ei=(((FFdTef_Vbi_ei*itf)+(FFdTef*itf_Vbi_ei))/(1+gtfe));
FFdQef_Vbi_ci=(((FFdTef_Vbi_ci*itf)+(FFdTef*itf_Vbi_ci))/(1+gtfe));
FFdQef_Vci_ei=(((FFdTef_Vci_ei*itf)+(FFdTef*itf_Vci_ei))/(1+gtfe));
#endif
FFdQef=((FFdTef*itf)/(1+gtfe));
if
(icbar<1e-10)
{
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=0.0;
FFdVc_Vbi_ei=0.0;
FFdVc_Vbi_ci=0.0;
FFdVc_Vci_ei=0.0;
#endif
FFdVc=0;
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=0.0;
FFdVc_ditf_Vbi_ei=0.0;
FFdVc_ditf_Vbi_ci=0.0;
FFdVc_ditf_Vci_ei=0.0;
#endif
FFdVc_ditf=0;
}
else
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=((itf_Vtnode_GND-ick_Vtnode_GND)/icbar);
FFib_Vbi_ei=(itf_Vbi_ei/icbar);
FFib_Vbi_ci=(itf_Vbi_ci/icbar);
FFib_Vci_ei=((itf_Vci_ei-ick_Vci_ei)/icbar);
#endif
FFib=((itf-ick)/icbar);
if
(FFib<(-1.0e10))
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=0.0;
FFib_Vbi_ei=0.0;
FFib_Vbi_ci=0.0;
FFib_Vci_ei=0.0;
#endif
FFib=(-1.0e10);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFfcbar_Vtnode_GND=((FFib_Vtnode_GND+((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ei=((FFib_Vbi_ei+((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ci=((FFib_Vbi_ci+((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/2.0);
FFfcbar_Vci_ei=((FFib_Vci_ei+((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/2.0);
#endif
FFfcbar=((FFib+d00_sqrt0)/2.0);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFdib_ditf_Vtnode_GND=(((FFfcbar_Vtnode_GND*d00_sqrt0-FFfcbar*((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ei=(((FFfcbar_Vbi_ei*d00_sqrt0-FFfcbar*((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ci=(((FFfcbar_Vbi_ci*d00_sqrt0-FFfcbar*((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vci_ei=(((FFfcbar_Vci_ei*d00_sqrt0-FFfcbar*((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
#endif
FFdib_ditf=((FFfcbar/d00_sqrt0)/icbar);
}
{
double m00_exp(d00_exp0,((-1.0)/FFfcbar))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=(vcbar*(-(-1.0)*FFfcbar_Vtnode_GND/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ei=(vcbar*(-(-1.0)*FFfcbar_Vbi_ei/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ci=(vcbar*(-(-1.0)*FFfcbar_Vbi_ci/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vci_ei=(vcbar*(-(-1.0)*FFfcbar_Vci_ei/FFfcbar/FFfcbar)*d10_exp0);
#endif
FFdVc=(vcbar*d00_exp0);
}
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=((((FFdVc_Vtnode_GND*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vtnode_GND*FFfcbar)+(FFfcbar*FFfcbar_Vtnode_GND)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vtnode_GND));
FFdVc_ditf_Vbi_ei=((((FFdVc_Vbi_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ei*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ei));
FFdVc_ditf_Vbi_ci=((((FFdVc_Vbi_ci*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ci*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ci)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ci));
FFdVc_ditf_Vci_ei=((((FFdVc_Vci_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vci_ei*FFfcbar)+(FFfcbar*FFfcbar_Vci_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vci_ei));
#endif
FFdVc_ditf=((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf);
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQbfb_Vtnode_GND=((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQbfb_Vbi_ei=(((((1-fthc)*thcs_t)*itf_Vbi_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQbfb_Vbi_ci=(((((1-fthc)*thcs_t)*itf_Vbi_ci)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQbfb_Vci_ei=(((((1-fthc)*thcs_t)*itf_Vci_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQbfb=((((1-fthc)*thcs_t)*itf)*(d00_exp0-1));
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTbfb_Vtnode_GND=(((FFdQbfb_Vtnode_GND*itf-FFdQbfb*itf_Vtnode_GND)/itf/itf)+((((((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*d00_exp0)+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0))*VT-((((1-fthc)*thcs_t)*itf)*d00_exp0)*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTbfb_Vbi_ei=(((FFdQbfb_Vbi_ei*itf-FFdQbfb*itf_Vbi_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ei)));
FFdTbfb_Vbi_ci=(((FFdQbfb_Vbi_ci*itf-FFdQbfb*itf_Vbi_ci)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ci)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ci)));
FFdTbfb_Vci_ei=(((FFdQbfb_Vci_ei*itf-FFdQbfb*itf_Vci_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vci_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTbfb=((FFdQbfb/itf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf));
}
#if defined(_DERIVATE)
FFic_Vtnode_GND=(-(-FFitf_ick_Vtnode_GND/FFitf_ick/FFitf_ick));
FFic_Vbi_ei=(-(-FFitf_ick_Vbi_ei/FFitf_ick/FFitf_ick));
FFic_Vbi_ci=(-(-FFitf_ick_Vbi_ci/FFitf_ick/FFitf_ick));
FFic_Vci_ei=(-(-FFitf_ick_Vci_ei/FFitf_ick/FFitf_ick));
#endif
FFic=(1-(1.0/FFitf_ick));
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
double m00_sqrt(d00_sqrt1,(1+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFw_Vtnode_GND=((FFic_Vtnode_GND+((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ei=((FFic_Vbi_ei+((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ci=((FFic_Vbi_ci+((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vci_ei=((FFic_Vci_ei+((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0)/(1+d00_sqrt1));
#endif
FFw=((FFic+d00_sqrt0)/(1+d00_sqrt1));
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQfhc_Vtnode_GND=((((((((thcs_t_Vtnode_GND*itf)+(thcs_t*itf_Vtnode_GND))*FFw)+((thcs_t*itf)*FFw_Vtnode_GND))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vtnode_GND))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQfhc_Vbi_ei=(((((((thcs_t*itf_Vbi_ei)*FFw)+((thcs_t*itf)*FFw_Vbi_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQfhc_Vbi_ci=(((((((thcs_t*itf_Vbi_ci)*FFw)+((thcs_t*itf)*FFw_Vbi_ci))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ci))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQfhc_Vci_ei=(((((((thcs_t*itf_Vci_ei)*FFw)+((thcs_t*itf)*FFw_Vci_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vci_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQfhc=((((thcs_t*itf)*FFw)*FFw)*d00_exp0);
}
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFdTfhc_Vtnode_GND=((FFdQfhc_Vtnode_GND*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vtnode_GND/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vtnode_GND*d00_sqrt0)+(FFitf_ick*((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+(((-VT_Vtnode_GND/VT/VT)*FFdVc_ditf)+((1.0/VT)*FFdVc_ditf_Vtnode_GND)))));
FFdTfhc_Vbi_ei=((FFdQfhc_Vbi_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ei))));
FFdTfhc_Vbi_ci=((FFdQfhc_Vbi_ci*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ci/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ci*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ci))));
FFdTfhc_Vci_ei=((FFdQfhc_Vci_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vci_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vci_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vci_ei))));
#endif
FFdTfhc=(FFdQfhc*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)));
}
if
((latb<=0.0)&&(latl<=0.0))
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(fthc*FFdQfhc_Vtnode_GND);
FFdQcfc_Vbi_ei=(fthc*FFdQfhc_Vbi_ei);
FFdQcfc_Vbi_ci=(fthc*FFdQfhc_Vbi_ci);
FFdQcfc_Vci_ei=(fthc*FFdQfhc_Vci_ei);
#endif
FFdQcfc=(fthc*FFdQfhc);
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=(fthc*FFdTfhc_Vtnode_GND);
FFdTcfc_Vbi_ei=(fthc*FFdTfhc_Vbi_ei);
FFdTcfc_Vbi_ci=(fthc*FFdTfhc_Vbi_ci);
FFdTcfc_Vci_ei=(fthc*FFdTfhc_Vci_ei);
#endif
FFdTcfc=(fthc*FFdTfhc);
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=FFdQcfc_Vtnode_GND;
FFdQcfcT_Vbi_ei=FFdQcfc_Vbi_ei;
FFdQcfcT_Vbi_ci=FFdQcfc_Vbi_ci;
FFdQcfcT_Vci_ei=FFdQcfc_Vci_ei;
#endif
FFdQcfcT=FFdQcfc;
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=FFdTcfc_Vtnode_GND;
FFdTcfcT_Vbi_ei=FFdTcfc_Vbi_ei;
FFdTcfcT_Vbi_ci=FFdTcfc_Vbi_ci;
FFdTcfcT_Vci_ei=FFdTcfc_Vci_ei;
#endif
FFdTcfcT=FFdTcfc;
}
else
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(((fthc*thcs_t_Vtnode_GND)*itf)+((fthc*thcs_t)*itf_Vtnode_GND));
FFdQcfc_Vbi_ei=((fthc*thcs_t)*itf_Vbi_ei);
FFdQcfc_Vbi_ci=((fthc*thcs_t)*itf_Vbi_ci);
FFdQcfc_Vci_ei=((fthc*thcs_t)*itf_Vci_ei);
#endif
FFdQcfc=((fthc*thcs_t)*itf);
#if defined(_DERIVATE)
FCa_Vci_ei=(-((ick_Vci_ei*itf-ick*itf_Vci_ei)/itf/itf));
FCa_Vtnode_GND=(-((ick_Vtnode_GND*itf-ick*itf_Vtnode_GND)/itf/itf));
FCa_Vbi_ei=(-(-ick*itf_Vbi_ei/itf/itf));
FCa_Vbi_ci=(-(-ick*itf_Vbi_ci/itf/itf));
#endif
FCa=(1.0-(ick/itf));
{
double m00_sqrt(d00_sqrt0,((FCa*FCa)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FCa*FCa)+ahc))
#endif
#if defined(_DERIVATE)
FCrt_Vci_ei=((FCa_Vci_ei*FCa)+(FCa*FCa_Vci_ei))*d10_sqrt0;
FCrt_Vtnode_GND=((FCa_Vtnode_GND*FCa)+(FCa*FCa_Vtnode_GND))*d10_sqrt0;
FCrt_Vbi_ei=((FCa_Vbi_ei*FCa)+(FCa*FCa_Vbi_ei))*d10_sqrt0;
FCrt_Vbi_ci=((FCa_Vbi_ci*FCa)+(FCa*FCa_Vbi_ci))*d10_sqrt0;
#endif
FCrt=d00_sqrt0;
}
{
double m00_sqrt(d00_sqrt0,(1.0+ahc))
#if defined(_DERIVATE)
FCa_ck_Vci_ei=(-((FCa_Vci_ei+FCrt_Vci_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vtnode_GND=(-((FCa_Vtnode_GND+FCrt_Vtnode_GND)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ei=(-((FCa_Vbi_ei+FCrt_Vbi_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ci=(-((FCa_Vbi_ci+FCrt_Vbi_ci)/(1.0+d00_sqrt0)));
#endif
FCa_ck=(1.0-((FCa+FCrt)/(1.0+d00_sqrt0)));
}
#if defined(_DERIVATE)
FCdaick_ditf_Vci_ei=((((FCa_ck_Vci_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vci_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vci_ei*itf)+(FCrt*itf_Vci_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vtnode_GND=((((FCa_ck_Vtnode_GND*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vtnode_GND)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vtnode_GND*itf)+(FCrt*itf_Vtnode_GND)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ei=((((FCa_ck_Vbi_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ei*itf)+(FCrt*itf_Vbi_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ci=((((FCa_ck_Vbi_ci*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ci)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ci*itf)+(FCrt*itf_Vbi_ci)))/(FCrt*itf)/(FCrt*itf));
#endif
FCdaick_ditf=(((FCa_ck-1.0)*(1-FCa))/(FCrt*itf));
if
(latb>latl)
{
#if defined(_DERIVATE)
FCz_Vci_ei=0.0;
FCz_Vtnode_GND=0.0;
FCz_Vbi_ei=0.0;
FCz_Vbi_ci=0.0;
#endif
FCz=(latb-latl);
FCxl=(1.0+latl);
FCxb=(1.0+latb);
if
(latb>0.01)
{
{
double m00_logE(d00_logE0,(FCxb/FCxl))
FCln=d00_logE0;
}
{
double m00_exp(d00_exp0,((FCa_ck-1.0)*FCln))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=FCa_ck_Vci_ei*FCln*d10_exp0;
FCa1_Vtnode_GND=FCa_ck_Vtnode_GND*FCln*d10_exp0;
FCa1_Vbi_ei=FCa_ck_Vbi_ei*FCln*d10_exp0;
FCa1_Vbi_ci=FCa_ck_Vbi_ci*FCln*d10_exp0;
#endif
FCa1=d00_exp0;
}
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-(-FCa1_Vci_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vtnode_GND=(-(-FCa1_Vtnode_GND*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ei=(-(-FCa1_Vbi_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ci=(-(-FCa1_Vbi_ci*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
#endif
FCd_a=(1.0/(latl-(FCa1*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCa1_Vci_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCa1_Vtnode_GND*FCd_a)+((FCa1-1.0)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCa1_Vbi_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCa1_Vbi_ci*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ci));
#endif
FCw=((FCa1-1.0)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=(((((((-FCz_Vci_ei)*FCa1)+((-FCz)*FCa1_Vci_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vci_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=(((((((-FCz_Vtnode_GND)*FCa1)+((-FCz)*FCa1_Vtnode_GND))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vtnode_GND))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=(((((((-FCz_Vbi_ei)*FCa1)+((-FCz)*FCa1_Vbi_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=(((((((-FCz_Vbi_ci)*FCa1)+((-FCz)*FCa1_Vbi_ci))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ci))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a);
{
double m00_logE(d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=(((latb*FCw_Vci_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vci_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vtnode_GND=(((latb*FCw_Vtnode_GND)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vtnode_GND))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ei=(((latb*FCw_Vbi_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ci=(((latb*FCw_Vbi_ci)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ci))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
#endif
FCa1=d00_logE0;
}
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=((-latb*(latb*FCw_Vci_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vci_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vtnode_GND=((-latb*(latb*FCw_Vtnode_GND)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vtnode_GND)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ei=((-latb*(latb*FCw_Vbi_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ci=((-latb*(latb*FCw_Vbi_ci)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ci)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
#endif
FCda1_dw=((latb/(1.0+(latb*FCw)))-(latl/(1.0+(latl*FCw))));
}
else
{
#if defined(_DERIVATE)
FCf1_Vci_ei=(-FCa_ck_Vci_ei);
FCf1_Vtnode_GND=(-FCa_ck_Vtnode_GND);
FCf1_Vbi_ei=(-FCa_ck_Vbi_ei);
FCf1_Vbi_ci=(-FCa_ck_Vbi_ci);
#endif
FCf1=(1.0-FCa_ck);
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCf1_Vci_ei*FCd_a)+(FCf1*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCf1_Vtnode_GND*FCd_a)+(FCf1*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCf1_Vbi_ei*FCd_a)+(FCf1*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCf1_Vbi_ci*FCd_a)+(FCf1*FCd_a_Vbi_ci));
#endif
FCw=(FCf1*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((((-1.0)*FCd_a_Vci_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vci_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((((-1.0)*FCd_a_Vtnode_GND)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vtnode_GND))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((((-1.0)*FCd_a_Vbi_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((((-1.0)*FCd_a_Vbi_ci)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ci))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a);
#if defined(_DERIVATE)
FCa1_Vci_ei=((FCz_Vci_ei*FCw)+(FCz*FCw_Vci_ei));
FCa1_Vtnode_GND=((FCz_Vtnode_GND*FCw)+(FCz*FCw_Vtnode_GND));
FCa1_Vbi_ei=((FCz_Vbi_ei*FCw)+(FCz*FCw_Vbi_ei));
FCa1_Vbi_ci=((FCz_Vbi_ci*FCw)+(FCz*FCw_Vbi_ci));
#endif
FCa1=(FCz*FCw);
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=FCz_Vci_ei;
FCda1_dw_Vtnode_GND=FCz_Vtnode_GND;
FCda1_dw_Vbi_ei=FCz_Vbi_ei;
FCda1_dw_Vbi_ci=FCz_Vbi_ci;
#endif
FCda1_dw=FCz;
}
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=(-2.0*FCz_Vbi_ei/FCz/FCz);
FCf_CT_Vbi_ci=(-2.0*FCz_Vbi_ci/FCz/FCz);
FCf_CT_Vci_ei=(-2.0*FCz_Vci_ei/FCz/FCz);
FCf_CT_Vtnode_GND=(-2.0*FCz_Vtnode_GND/FCz/FCz);
#endif
FCf_CT=(2.0/FCz);
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCf1_Vci_ei=(((((((latb*latl)*FCw_Vci_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vci_ei))/3.0)+(((latb+latl)*FCw2_Vci_ei)/2.0))+FCw_Vci_ei);
FCf1_Vtnode_GND=(((((((latb*latl)*FCw_Vtnode_GND)*FCw2)+(((latb*latl)*FCw)*FCw2_Vtnode_GND))/3.0)+(((latb+latl)*FCw2_Vtnode_GND)/2.0))+FCw_Vtnode_GND);
FCf1_Vbi_ei=(((((((latb*latl)*FCw_Vbi_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ei))/3.0)+(((latb+latl)*FCw2_Vbi_ei)/2.0))+FCw_Vbi_ei);
FCf1_Vbi_ci=(((((((latb*latl)*FCw_Vbi_ci)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ci))/3.0)+(((latb+latl)*FCw2_Vbi_ci)/2.0))+FCw_Vbi_ci);
#endif
FCf1=((((((latb*latl)*FCw)*FCw2)/3.0)+(((latb+latl)*FCw2)/2.0))+FCw);
#if defined(_DERIVATE)
FCdf1_dw_Vci_ei=(((latb*latl)*FCw2_Vci_ei)+((latb+latl)*FCw_Vci_ei));
FCdf1_dw_Vtnode_GND=(((latb*latl)*FCw2_Vtnode_GND)+((latb+latl)*FCw_Vtnode_GND));
FCdf1_dw_Vbi_ei=(((latb*latl)*FCw2_Vbi_ei)+((latb+latl)*FCw_Vbi_ei));
FCdf1_dw_Vbi_ci=(((latb*latl)*FCw2_Vbi_ci)+((latb+latl)*FCw_Vbi_ci));
#endif
FCdf1_dw=((((latb*latl)*FCw2)+((latb+latl)*FCw))+1.0);
#if defined(_DERIVATE)
z_Vci_ei=(latb*FCw_Vci_ei);
z_Vtnode_GND=(latb*FCw_Vtnode_GND);
z_Vbi_ei=(latb*FCw_Vbi_ei);
z_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
z=(latb*FCw);
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latl/latb);
#if defined(_DERIVATE)
FCf2_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latb);
FCf2_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latb);
FCf2_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latb);
FCf2_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latb);
#endif
FCf2=((((1.0-r)*a2)+(r*a3))/latb);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf2_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((((((latb*a2_Vbi_ei)+(latl*a3_Vbi_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf2_Vbi_ci=(((((((latb*a2_Vbi_ci)+(latl*a3_Vbi_ci))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ci))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf2_Vci_ei=(((((((latb*a2_Vci_ei)+(latl*a3_Vci_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vci_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf2_Vtnode_GND=(((((((latb*a2_Vtnode_GND)+(latl*a3_Vtnode_GND))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vtnode_GND))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf2=(((((latb*a2)+(latl*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((latl*FCw_Vci_ei)*(1+z))+((1+(latl*FCw))*z_Vci_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((latl*FCw_Vtnode_GND)*(1+z))+((1+(latl*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((latl*FCw_Vbi_ei)*(1+z))+((1+(latl*FCw))*z_Vbi_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((latl*FCw_Vbi_ci)*(1+z))+((1+(latl*FCw))*z_Vbi_ci))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf2_dw=(((1+(latl*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
z_Vci_ei=(latl*FCw_Vci_ei);
z_Vtnode_GND=(latl*FCw_Vtnode_GND);
z_Vbi_ei=(latl*FCw_Vbi_ei);
z_Vbi_ci=(latl*FCw_Vbi_ci);
#endif
z=(latl*FCw);
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latb/latl);
#if defined(_DERIVATE)
FCf3_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latl);
FCf3_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latl);
FCf3_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latl);
FCf3_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latl);
#endif
FCf3=((((1.0-r)*a2)+(r*a3))/latl);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf3_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((((((latl*a2_Vbi_ei)+(latb*a3_Vbi_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf3_Vbi_ci=(((((((latl*a2_Vbi_ci)+(latb*a3_Vbi_ci))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ci))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf3_Vci_ei=(((((((latl*a2_Vci_ei)+(latb*a3_Vci_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vci_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf3_Vtnode_GND=(((((((latl*a2_Vtnode_GND)+(latb*a3_Vtnode_GND))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vtnode_GND))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf3=(((((latl*a2)+(latb*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((latb*FCw_Vci_ei)*(1+z))+((1+(latb*FCw))*z_Vci_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((latb*FCw_Vtnode_GND)*(1+z))+((1+(latb*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((latb*FCw_Vbi_ei)*(1+z))+((1+(latb*FCw))*z_Vbi_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((latb*FCw_Vbi_ci)*(1+z))+((1+(latb*FCw))*z_Vbi_ci))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf3_dw=(((1+(latb*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((FCf_CT_Vci_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vci_ei*FCf1)+(FCa1*FCf1_Vci_ei))-FCf2_Vci_ei)+FCf3_Vci_ei)));
FCf_ci_Vtnode_GND=((FCf_CT_Vtnode_GND*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vtnode_GND*FCf1)+(FCa1*FCf1_Vtnode_GND))-FCf2_Vtnode_GND)+FCf3_Vtnode_GND)));
FCf_ci_Vbi_ei=((FCf_CT_Vbi_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ei*FCf1)+(FCa1*FCf1_Vbi_ei))-FCf2_Vbi_ei)+FCf3_Vbi_ei)));
FCf_ci_Vbi_ci=((FCf_CT_Vbi_ci*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ci*FCf1)+(FCa1*FCf1_Vbi_ci))-FCf2_Vbi_ci)+FCf3_Vbi_ci)));
#endif
FCf_ci=(FCf_CT*(((FCa1*FCf1)-FCf2)+FCf3));
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=((FCf_CT_Vci_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vci_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vci_ei))+((FCda1_dw_Vci_ei*FCf1)+(FCda1_dw*FCf1_Vci_ei)))-FCdf2_dw_Vci_ei)+FCdf3_dw_Vci_ei)));
FCdfc_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vtnode_GND*FCdf1_dw)+(FCa1*FCdf1_dw_Vtnode_GND))+((FCda1_dw_Vtnode_GND*FCf1)+(FCda1_dw*FCf1_Vtnode_GND)))-FCdf2_dw_Vtnode_GND)+FCdf3_dw_Vtnode_GND)));
FCdfc_dw_Vbi_ei=((FCf_CT_Vbi_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ei))+((FCda1_dw_Vbi_ei*FCf1)+(FCda1_dw*FCf1_Vbi_ei)))-FCdf2_dw_Vbi_ei)+FCdf3_dw_Vbi_ei)));
FCdfc_dw_Vbi_ci=((FCf_CT_Vbi_ci*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ci*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ci))+((FCda1_dw_Vbi_ci*FCf1)+(FCda1_dw*FCf1_Vbi_ci)))-FCdf2_dw_Vbi_ci)+FCdf3_dw_Vbi_ci)));
#endif
FCdfc_dw=(FCf_CT*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw));
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
if
((flcomp==0.0)||(flcomp==2.1))
{
#if defined(_DERIVATE)
a_Vbi_ei=(latb*FCw_Vbi_ei);
a_Vbi_ci=(latb*FCw_Vbi_ci);
a_Vci_ei=(latb*FCw_Vci_ei);
a_Vtnode_GND=(latb*FCw_Vtnode_GND);
#endif
a=(latb*FCw);
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
if
(a>1.0e-6)
{
#if defined(_DERIVATE)
FCf2_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latb);
FCf2_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latb);
FCf2_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latb);
FCf2_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latb);
#endif
FCf2=((a-lnz)/latb);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf2_dw=(a/(1.0+a));
}
else
{
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf2_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf2_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf2_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf2=((0.5*a)*FCw);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=a_Vci_ei;
FCdf2_dw_Vtnode_GND=a_Vtnode_GND;
FCdf2_dw_Vbi_ei=a_Vbi_ei;
FCdf2_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf2_dw=a;
}
#if defined(_DERIVATE)
a_Vbi_ei=(latl*FCw_Vbi_ei);
a_Vbi_ci=(latl*FCw_Vbi_ci);
a_Vci_ei=(latl*FCw_Vci_ei);
a_Vtnode_GND=(latl*FCw_Vtnode_GND);
#endif
a=(latl*FCw);
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
if
(a>1.0e-6)
{
#if defined(_DERIVATE)
FCf3_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latl);
FCf3_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latl);
FCf3_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latl);
FCf3_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latl);
#endif
FCf3=((a-lnz)/latl);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf3_dw=(a/(1.0+a));
}
else
{
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf3_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf3_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf3_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf3=((0.5*a)*FCw);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=a_Vci_ei;
FCdf3_dw_Vtnode_GND=a_Vtnode_GND;
FCdf3_dw_Vbi_ei=a_Vbi_ei;
FCdf3_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf3_dw=a;
}
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((FCf_CT_Vbi_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ei-FCf3_Vbi_ei)));
FCf_CT_Vbi_ci=((FCf_CT_Vbi_ci*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ci-FCf3_Vbi_ci)));
FCf_CT_Vci_ei=((FCf_CT_Vci_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vci_ei-FCf3_Vci_ei)));
FCf_CT_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCf2-FCf3))+(FCf_CT*(FCf2_Vtnode_GND-FCf3_Vtnode_GND)));
#endif
FCf_CT=(FCf_CT*(FCf2-FCf3));
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=((FCf_CT_Vbi_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ei-FCdf3_dw_Vbi_ei)));
FCdfCT_dw_Vbi_ci=((FCf_CT_Vbi_ci*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ci-FCdf3_dw_Vbi_ci)));
FCdfCT_dw_Vci_ei=((FCf_CT_Vci_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vci_ei-FCdf3_dw_Vci_ei)));
FCdfCT_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vtnode_GND-FCdf3_dw_Vtnode_GND)));
#endif
FCdfCT_dw=(FCf_CT*(FCdf2_dw-FCdf3_dw));
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
}
}
else
{
if
(latb>0.01)
{
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=(((-FCa_ck_Vci_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vci_ei));
FCw_Vtnode_GND=(((-FCa_ck_Vtnode_GND)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=(((-FCa_ck_Vbi_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ei));
FCw_Vbi_ci=(((-FCa_ck_Vbi_ci)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ci));
#endif
FCw=((1.0-FCa_ck)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((-(1.0+latb))*FCd_a_Vci_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((-(1.0+latb))*FCd_a_Vtnode_GND)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((-(1.0+latb))*FCd_a_Vbi_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((-(1.0+latb))*FCd_a_Vbi_ci)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((-(1.0+latb))*FCd_a)*FCd_a);
}
else
{
#if defined(_DERIVATE)
FCw_Vci_ei=((-FCa_ck_Vci_ei)-FCa_ck_Vci_ei*latb);
FCw_Vtnode_GND=((-FCa_ck_Vtnode_GND)-FCa_ck_Vtnode_GND*latb);
FCw_Vbi_ei=((-FCa_ck_Vbi_ei)-FCa_ck_Vbi_ei*latb);
FCw_Vbi_ci=((-FCa_ck_Vbi_ci)-FCa_ck_Vbi_ci*latb);
#endif
FCw=((1.0-FCa_ck)-(FCa_ck*latb));
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=0.0;
FCdw_daick_Vtnode_GND=0.0;
FCdw_daick_Vbi_ei=0.0;
FCdw_daick_Vbi_ci=0.0;
#endif
FCdw_daick=(-(1.0+latb));
}
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCz_Vci_ei=(latb*FCw_Vci_ei);
FCz_Vtnode_GND=(latb*FCw_Vtnode_GND);
FCz_Vbi_ei=(latb*FCw_Vbi_ei);
FCz_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
FCz=(latb*FCw);
#if defined(_DERIVATE)
FCz_1_Vci_ei=FCz_Vci_ei;
FCz_1_Vtnode_GND=FCz_Vtnode_GND;
FCz_1_Vbi_ei=FCz_Vbi_ei;
FCz_1_Vbi_ci=FCz_Vbi_ci;
#endif
FCz_1=(1.0+FCz);
#if defined(_DERIVATE)
FCd_f_Vci_ei=(-FCz_1_Vci_ei/FCz_1/FCz_1);
FCd_f_Vtnode_GND=(-FCz_1_Vtnode_GND/FCz_1/FCz_1);
FCd_f_Vbi_ei=(-FCz_1_Vbi_ei/FCz_1/FCz_1);
FCd_f_Vbi_ci=(-FCz_1_Vbi_ci/FCz_1/FCz_1);
#endif
FCd_f=(1.0/FCz_1);
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((((FCw2_Vci_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vci_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vci_ei));
FCf_ci_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vtnode_GND/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vtnode_GND));
FCf_ci_Vbi_ei=((((FCw2_Vbi_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_ci_Vbi_ci=((((FCw2_Vbi_ci*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ci/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ci));
#endif
FCf_ci=((FCw2*(1.0+(FCz/3.0)))*FCd_f);
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vci_ei+(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfc_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vtnode_GND+(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
FCdfc_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ei+(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfc_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ci+(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
#endif
FCdfc_dw=((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
if
((flcomp==0.0)||(flcomp==2.1))
{
if
(FCz>0.001)
{
{
double m00_logE(d00_logE0,FCz_1)
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FCz_1)
#endif
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=(((2.0*(((FCz_1_Vbi_ei*d00_logE0)+(FCz_1*FCz_1_Vbi_ei*d10_logE0))-FCz_Vbi_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vbi_ci=(((2.0*(((FCz_1_Vbi_ci*d00_logE0)+(FCz_1*FCz_1_Vbi_ci*d10_logE0))-FCz_Vbi_ci))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ci))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vci_ei=(((2.0*(((FCz_1_Vci_ei*d00_logE0)+(FCz_1*FCz_1_Vci_ei*d10_logE0))-FCz_Vci_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vci_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vtnode_GND=(((2.0*(((FCz_1_Vtnode_GND*d00_logE0)+(FCz_1*FCz_1_Vtnode_GND*d10_logE0))-FCz_Vtnode_GND))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vtnode_GND))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
#endif
FCf_CT=((2.0*((FCz_1*d00_logE0)-FCz))/((latb*latb)*FCz_1));
}
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((2.0*FCw_Vbi_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((2.0*FCw_Vbi_ci)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ci))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((2.0*FCw_Vci_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vci_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((2.0*FCw_Vtnode_GND)*FCd_f)+((2.0*FCw)*FCd_f_Vtnode_GND))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=(((2.0*FCw)*FCd_f)*FCd_f);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((((FCw2_Vbi_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_CT_Vbi_ci=((((FCw2_Vbi_ci*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ci/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ci));
FCf_CT_Vci_ei=((((FCw2_Vci_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vci_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vci_ei));
FCf_CT_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vtnode_GND/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vtnode_GND));
#endif
FCf_CT=((FCw2*(1.0-(FCz/3.0)))*FCd_f);
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
}
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
}
else
{
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
}
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_CT)+(FFdQcfc*FCf_CT_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_CT)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfcT_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfcT_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfcT_Vci_ei=((((FFdQcfc_Vci_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfcT=((FFdQcfc*FCf_CT)*d00_exp0);
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_ci)+(FFdQcfc*FCf_ci_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_ci)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfc_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfc_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfc_Vci_ei=((((FFdQcfc_Vci_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfc=((FFdQcfc*FCf_ci)*d00_exp0);
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vtnode_GND+((itf_Vtnode_GND*FCdfc_ditf)+(itf*FCdfc_ditf_Vtnode_GND)))))+((((FFdQcfc_Vtnode_GND*VT-FFdQcfc*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfc_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ei+((itf_Vbi_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ei)))))+(((FFdQcfc_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfc_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ci+((itf_Vbi_ci*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ci)))))+(((FFdQcfc_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfc_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vci_ei+((itf_Vci_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vci_ei)))))+(((FFdQcfc_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfc=((((fthc*thcs_t)*d00_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+((FFdQcfc/VT)*FFdVc_ditf));
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vtnode_GND+((itf_Vtnode_GND*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vtnode_GND)))))+((((FFdQcfcT_Vtnode_GND*VT-FFdQcfcT*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfcT_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ei+((itf_Vbi_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ei)))))+(((FFdQcfcT_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfcT_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ci+((itf_Vbi_ci*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ci)))))+(((FFdQcfcT_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfcT_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vci_ei+((itf_Vci_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vci_ei)))))+(((FFdQcfcT_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfcT=((((fthc*thcs_t)*d00_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+((FFdQcfcT/VT)*FFdVc_ditf));
}
}
#if defined(_DERIVATE)
FFdQbfc_Vtnode_GND=((1-fthc)*FFdQfhc_Vtnode_GND);
FFdQbfc_Vbi_ei=((1-fthc)*FFdQfhc_Vbi_ei);
FFdQbfc_Vbi_ci=((1-fthc)*FFdQfhc_Vbi_ci);
FFdQbfc_Vci_ei=((1-fthc)*FFdQfhc_Vci_ei);
#endif
FFdQbfc=((1-fthc)*FFdQfhc);
#if defined(_DERIVATE)
FFdTbfc_Vtnode_GND=((1-fthc)*FFdTfhc_Vtnode_GND);
FFdTbfc_Vbi_ei=((1-fthc)*FFdTfhc_Vbi_ei);
FFdTbfc_Vbi_ci=((1-fthc)*FFdTfhc_Vbi_ci);
FFdTbfc_Vci_ei=((1-fthc)*FFdTfhc_Vci_ei);
#endif
FFdTbfc=((1-fthc)*FFdTfhc);
#if defined(_DERIVATE)
Q_fT_Vci_ei=(((((hf0_t*Qf_Vci_ei)+FFdQbfb_Vci_ei)+FFdQbfc_Vci_ei)+(hfe_t*FFdQef_Vci_ei))+(hfc_t*FFdQcfcT_Vci_ei));
Q_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Qf)+(hf0_t*Qf_Vtnode_GND))+FFdQbfb_Vtnode_GND)+FFdQbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdQef)+(hfe_t*FFdQef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdQcfcT)+(hfc_t*FFdQcfcT_Vtnode_GND)));
Q_fT_Vbi_ci=(((((hf0_t*Qf_Vbi_ci)+FFdQbfb_Vbi_ci)+FFdQbfc_Vbi_ci)+(hfe_t*FFdQef_Vbi_ci))+(hfc_t*FFdQcfcT_Vbi_ci));
Q_fT_Vbi_ei=(((((hf0_t*Qf_Vbi_ei)+FFdQbfb_Vbi_ei)+FFdQbfc_Vbi_ei)+(hfe_t*FFdQef_Vbi_ei))+(hfc_t*FFdQcfcT_Vbi_ei));
#endif
Q_fT=(((((hf0_t*Qf)+FFdQbfb)+FFdQbfc)+(hfe_t*FFdQef))+(hfc_t*FFdQcfcT));
#if defined(_DERIVATE)
T_fT_Vbi_ei=(((((hf0_t*Tf_Vbi_ei)+FFdTbfb_Vbi_ei)+FFdTbfc_Vbi_ei)+(hfe_t*FFdTef_Vbi_ei))+(hfc_t*FFdTcfcT_Vbi_ei));
T_fT_Vci_ei=(((((hf0_t*Tf_Vci_ei)+FFdTbfb_Vci_ei)+FFdTbfc_Vci_ei)+(hfe_t*FFdTef_Vci_ei))+(hfc_t*FFdTcfcT_Vci_ei));
T_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Tf)+(hf0_t*Tf_Vtnode_GND))+FFdTbfb_Vtnode_GND)+FFdTbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdTef)+(hfe_t*FFdTef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdTcfcT)+(hfc_t*FFdTcfcT_Vtnode_GND)));
T_fT_Vbi_ci=(((((hf0_t*Tf_Vbi_ci)+FFdTbfb_Vbi_ci)+FFdTbfc_Vbi_ci)+(hfe_t*FFdTef_Vbi_ci))+(hfc_t*FFdTcfcT_Vbi_ci));
#endif
T_fT=(((((hf0_t*Tf)+FFdTbfb)+FFdTbfc)+(hfe_t*FFdTef))+(hfc_t*FFdTcfcT));
#if defined(_DERIVATE)
Qf_Vci_ei=(((Qf_Vci_ei+(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei))+FFdQef_Vci_ei)+FFdQcfc_Vci_ei);
Qf_Vtnode_GND=(((Qf_Vtnode_GND+(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND))+FFdQef_Vtnode_GND)+FFdQcfc_Vtnode_GND);
Qf_Vbi_ci=(((Qf_Vbi_ci+(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci))+FFdQef_Vbi_ci)+FFdQcfc_Vbi_ci);
Qf_Vbi_ei=(((Qf_Vbi_ei+(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei))+FFdQef_Vbi_ei)+FFdQcfc_Vbi_ei);
#endif
Qf=(((Qf+(FFdQbfb+FFdQbfc))+FFdQef)+FFdQcfc);
#if defined(_DERIVATE)
Tf_Vbi_ei=(((Tf_Vbi_ei+(FFdTbfb_Vbi_ei+FFdTbfc_Vbi_ei))+FFdTef_Vbi_ei)+FFdTcfc_Vbi_ei);
Tf_Vci_ei=(((Tf_Vci_ei+(FFdTbfb_Vci_ei+FFdTbfc_Vci_ei))+FFdTef_Vci_ei)+FFdTcfc_Vci_ei);
Tf_Vtnode_GND=(((Tf_Vtnode_GND+(FFdTbfb_Vtnode_GND+FFdTbfc_Vtnode_GND))+FFdTef_Vtnode_GND)+FFdTcfc_Vtnode_GND);
Tf_Vbi_ci=(((Tf_Vbi_ci+(FFdTbfb_Vbi_ci+FFdTbfc_Vbi_ci))+FFdTef_Vbi_ci)+FFdTcfc_Vbi_ci);
#endif
Tf=(((Tf+(FFdTbfb+FFdTbfc))+FFdTef)+FFdTcfc);
#if defined(_DERIVATE)
Q_bf_Vtnode_GND=(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND);
Q_bf_Vbi_ei=(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei);
Q_bf_Vbi_ci=(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci);
Q_bf_Vci_ei=(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei);
#endif
Q_bf=(FFdQbfb+FFdQbfc);
}
#if defined(_DERIVATE)
Qr_Vci_ei=(Tr*itr_Vci_ei);
Qr_Vtnode_GND=(Tr*itr_Vtnode_GND);
Qr_Vbi_ci=(Tr*itr_Vbi_ci);
Qr_Vbi_ei=(Tr*itr_Vbi_ei);
#endif
Qr=(Tr*itr);
if
(Oich==0.0)
{
#if defined(_DERIVATE)
a_Vbi_ei=(((((T_fT_Vbi_ei*itf)+(T_fT*itf_Vbi_ei))+Qr_Vbi_ei)*Q_pT-((T_fT*itf)+Qr)*Q_pT_Vbi_ei)/Q_pT/Q_pT);
a_Vbi_ci=(((((T_fT_Vbi_ci*itf)+(T_fT*itf_Vbi_ci))+Qr_Vbi_ci)*Q_pT-((T_fT*itf)+Qr)*Q_pT_Vbi_ci)/Q_pT/Q_pT);
a_Vci_ei=(((((T_fT_Vci_ei*itf)+(T_fT*itf_Vci_ei))+Qr_Vci_ei)*Q_pT-((T_fT*itf)+Qr)*Q_pT_Vci_ei)/Q_pT/Q_pT);
a_Vtnode_GND=(((((T_fT_Vtnode_GND*itf)+(T_fT*itf_Vtnode_GND))+Qr_Vtnode_GND)*Q_pT-((T_fT*itf)+Qr)*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
#endif
a=(1.0+(((T_fT*itf)+Qr)/Q_pT));
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=(((((((T_fT_Vbi_ei*I_Tf1)+(T_fT*I_Tf1_Vbi_ei))*(1.0+(2.0*a_h)))+((T_fT*I_Tf1)*(2.0*a_h_Vbi_ei)))+Qr_Vbi_ei)*Q_pT-(((T_fT*I_Tf1)*(1.0+(2.0*a_h)))+Qr)*Q_pT_Vbi_ei)/Q_pT/Q_pT);
a_Vbi_ci=(((((((T_fT_Vbi_ci*I_Tf1)+(T_fT*I_Tf1_Vbi_ci))*(1.0+(2.0*a_h)))+((T_fT*I_Tf1)*(2.0*a_h_Vbi_ci)))+Qr_Vbi_ci)*Q_pT-(((T_fT*I_Tf1)*(1.0+(2.0*a_h)))+Qr)*Q_pT_Vbi_ci)/Q_pT/Q_pT);
a_Vci_ei=(((((((T_fT_Vci_ei*I_Tf1)+(T_fT*I_Tf1_Vci_ei))*(1.0+(2.0*a_h)))+((T_fT*I_Tf1)*(2.0*a_h_Vci_ei)))+Qr_Vci_ei)*Q_pT-(((T_fT*I_Tf1)*(1.0+(2.0*a_h)))+Qr)*Q_pT_Vci_ei)/Q_pT/Q_pT);
a_Vtnode_GND=(((((((T_fT_Vtnode_GND*I_Tf1)+(T_fT*I_Tf1_Vtnode_GND))*(1.0+(2.0*a_h)))+((T_fT*I_Tf1)*(2.0*a_h_Vtnode_GND)))+Qr_Vtnode_GND)*Q_pT-(((T_fT*I_Tf1)*(1.0+(2.0*a_h)))+Qr)*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
#endif
a=(1.0+((((T_fT*I_Tf1)*(1.0+(2.0*a_h)))+Qr)/Q_pT));
}
#if defined(_DERIVATE)
d_Q_Vtnode_GND=(((-(Q_pT_Vtnode_GND-((Q_0_Vtnode_GND+Q_fT_Vtnode_GND)+Qr_Vtnode_GND)))*a-(-(Q_pT-((Q_0+Q_fT)+Qr)))*a_Vtnode_GND)/a/a);
d_Q_Vbi_ei=(((-(Q_pT_Vbi_ei-((Q_0_Vbi_ei+Q_fT_Vbi_ei)+Qr_Vbi_ei)))*a-(-(Q_pT-((Q_0+Q_fT)+Qr)))*a_Vbi_ei)/a/a);
d_Q_Vbi_ci=(((-(Q_pT_Vbi_ci-((Q_0_Vbi_ci+Q_fT_Vbi_ci)+Qr_Vbi_ci)))*a-(-(Q_pT-((Q_0+Q_fT)+Qr)))*a_Vbi_ci)/a/a);
d_Q_Vci_ei=(((-(Q_pT_Vci_ei-(Q_fT_Vci_ei+Qr_Vci_ei)))*a-(-(Q_pT-((Q_0+Q_fT)+Qr)))*a_Vci_ei)/a/a);
#endif
d_Q=((-(Q_pT-((Q_0+Q_fT)+Qr)))/a);
{
double m00_abs(d00_abs0,(0.3*Q_pT))
#if defined(_DERIVATE)
double m10_abs(d10_abs0,d00_abs0,(0.3*Q_pT))
#endif
#if defined(_DERIVATE)
a_Vbi_ei=(0.3*Q_pT_Vbi_ei)*d10_abs0;
a_Vbi_ci=(0.3*Q_pT_Vbi_ci)*d10_abs0;
a_Vci_ei=(0.3*Q_pT_Vci_ei)*d10_abs0;
a_Vtnode_GND=(0.3*Q_pT_Vtnode_GND)*d10_abs0;
#endif
a=d00_abs0;
}
{
double m00_abs(d00_abs0,d_Q)
if
(d00_abs0>a)
{
if
(d_Q>=0)
{
#if defined(_DERIVATE)
d_Q_Vtnode_GND=a_Vtnode_GND;
d_Q_Vbi_ei=a_Vbi_ei;
d_Q_Vbi_ci=a_Vbi_ci;
d_Q_Vci_ei=a_Vci_ei;
#endif
d_Q=a;
}
else
{
#if defined(_DERIVATE)
d_Q_Vtnode_GND=(-a_Vtnode_GND);
d_Q_Vbi_ei=(-a_Vbi_ei);
d_Q_Vbi_ci=(-a_Vbi_ci);
d_Q_Vci_ei=(-a_Vci_ei);
#endif
d_Q=(-a);
}
}
}
#if defined(_DERIVATE)
Q_pT_Vtnode_GND=(Q_pT_Vtnode_GND+d_Q_Vtnode_GND);
Q_pT_Vbi_ei=(Q_pT_Vbi_ei+d_Q_Vbi_ei);
Q_pT_Vbi_ci=(Q_pT_Vbi_ci+d_Q_Vbi_ci);
Q_pT_Vci_ei=(Q_pT_Vci_ei+d_Q_Vci_ei);
#endif
Q_pT=(Q_pT+d_Q);
l_it=(l_it+1);
}
#if defined(_DERIVATE)
I_Tf1_Vci_ei=(-i_0f*Q_pT_Vci_ei/Q_pT/Q_pT);
I_Tf1_Vtnode_GND=((i_0f_Vtnode_GND*Q_pT-i_0f*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
I_Tf1_Vbi_ei=((i_0f_Vbi_ei*Q_pT-i_0f*Q_pT_Vbi_ei)/Q_pT/Q_pT);
I_Tf1_Vbi_ci=(-i_0f*Q_pT_Vbi_ci/Q_pT/Q_pT);
#endif
I_Tf1=(i_0f/Q_pT);
#if defined(_DERIVATE)
a_h_Vci_ei=(Oich*I_Tf1_Vci_ei);
a_h_Vtnode_GND=(Oich*I_Tf1_Vtnode_GND);
a_h_Vbi_ei=(Oich*I_Tf1_Vbi_ei);
a_h_Vbi_ci=(Oich*I_Tf1_Vbi_ci);
#endif
a_h=(Oich*I_Tf1);
#if defined(_DERIVATE)
itf_Vci_ei=((I_Tf1_Vci_ei*(1.0+a_h))+(I_Tf1*a_h_Vci_ei));
itf_Vtnode_GND=((I_Tf1_Vtnode_GND*(1.0+a_h))+(I_Tf1*a_h_Vtnode_GND));
itf_Vbi_ei=((I_Tf1_Vbi_ei*(1.0+a_h))+(I_Tf1*a_h_Vbi_ei));
itf_Vbi_ci=((I_Tf1_Vbi_ci*(1.0+a_h))+(I_Tf1*a_h_Vbi_ci));
#endif
itf=(I_Tf1*(1.0+a_h));
#if defined(_DERIVATE)
itr_Vci_ei=(-i_0r*Q_pT_Vci_ei/Q_pT/Q_pT);
itr_Vtnode_GND=((i_0r_Vtnode_GND*Q_pT-i_0r*Q_pT_Vtnode_GND)/Q_pT/Q_pT);
itr_Vbi_ci=((i_0r_Vbi_ci*Q_pT-i_0r*Q_pT_Vbi_ci)/Q_pT/Q_pT);
itr_Vbi_ei=(-i_0r*Q_pT_Vbi_ei/Q_pT/Q_pT);
#endif
itr=(i_0r/Q_pT);
#if defined(_DERIVATE)
Tf_Vbi_ei=0.0;
Tf_Vci_ei=0.0;
Tf_Vtnode_GND=T_f0_Vtnode_GND;
Tf_Vbi_ci=T_f0_Vbi_ci;
#endif
Tf=T_f0;
#if defined(_DERIVATE)
Qf_Vci_ei=(T_f0*itf_Vci_ei);
Qf_Vtnode_GND=((T_f0_Vtnode_GND*itf)+(T_f0*itf_Vtnode_GND));
Qf_Vbi_ci=((T_f0_Vbi_ci*itf)+(T_f0*itf_Vbi_ci));
Qf_Vbi_ei=(T_f0*itf_Vbi_ei);
#endif
Qf=(T_f0*itf);
if
(itf<(1.0e-6*ick))
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Q_fT_Vci_ei=Qf_Vci_ei;
Q_fT_Vtnode_GND=Qf_Vtnode_GND;
Q_fT_Vbi_ci=Qf_Vbi_ci;
Q_fT_Vbi_ei=Qf_Vbi_ei;
#endif
Q_fT=Qf;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
T_fT_Vbi_ei=Tf_Vbi_ei;
T_fT_Vci_ei=Tf_Vci_ei;
T_fT_Vtnode_GND=Tf_Vtnode_GND;
T_fT_Vbi_ci=Tf_Vbi_ci;
#endif
T_fT=Tf;
#endif
}
else
{
#if defined(_DERIVATE)
FFitf_ick_Vtnode_GND=((itf_Vtnode_GND*ick-itf*ick_Vtnode_GND)/ick/ick);
FFitf_ick_Vbi_ei=(itf_Vbi_ei/ick);
FFitf_ick_Vbi_ci=(itf_Vbi_ci/ick);
FFitf_ick_Vci_ei=((itf_Vci_ei*ick-itf*ick_Vci_ei)/ick/ick);
#endif
FFitf_ick=(itf/ick);
{
double m00_logE(d00_logE0,FFitf_ick)
double m00_exp(d00_exp1,(gtfe*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FFitf_ick)
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
FFdTef_Vtnode_GND=((tef0_t_Vtnode_GND*d00_exp1)+(tef0_t*(gtfe*FFitf_ick_Vtnode_GND*d10_logE0)*d10_exp1));
FFdTef_Vbi_ei=(tef0_t*(gtfe*FFitf_ick_Vbi_ei*d10_logE0)*d10_exp1);
FFdTef_Vbi_ci=(tef0_t*(gtfe*FFitf_ick_Vbi_ci*d10_logE0)*d10_exp1);
FFdTef_Vci_ei=(tef0_t*(gtfe*FFitf_ick_Vci_ei*d10_logE0)*d10_exp1);
#endif
FFdTef=(tef0_t*d00_exp1);
}
#if defined(_DERIVATE)
FFdQef_Vtnode_GND=(((FFdTef_Vtnode_GND*itf)+(FFdTef*itf_Vtnode_GND))/(1+gtfe));
FFdQef_Vbi_ei=(((FFdTef_Vbi_ei*itf)+(FFdTef*itf_Vbi_ei))/(1+gtfe));
FFdQef_Vbi_ci=(((FFdTef_Vbi_ci*itf)+(FFdTef*itf_Vbi_ci))/(1+gtfe));
FFdQef_Vci_ei=(((FFdTef_Vci_ei*itf)+(FFdTef*itf_Vci_ei))/(1+gtfe));
#endif
FFdQef=((FFdTef*itf)/(1+gtfe));
if
(icbar<1e-10)
{
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=0.0;
FFdVc_Vbi_ei=0.0;
FFdVc_Vbi_ci=0.0;
FFdVc_Vci_ei=0.0;
#endif
FFdVc=0;
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=0.0;
FFdVc_ditf_Vbi_ei=0.0;
FFdVc_ditf_Vbi_ci=0.0;
FFdVc_ditf_Vci_ei=0.0;
#endif
FFdVc_ditf=0;
}
else
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=((itf_Vtnode_GND-ick_Vtnode_GND)/icbar);
FFib_Vbi_ei=(itf_Vbi_ei/icbar);
FFib_Vbi_ci=(itf_Vbi_ci/icbar);
FFib_Vci_ei=((itf_Vci_ei-ick_Vci_ei)/icbar);
#endif
FFib=((itf-ick)/icbar);
if
(FFib<(-1.0e10))
{
#if defined(_DERIVATE)
FFib_Vtnode_GND=0.0;
FFib_Vbi_ei=0.0;
FFib_Vbi_ci=0.0;
FFib_Vci_ei=0.0;
#endif
FFib=(-1.0e10);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFfcbar_Vtnode_GND=((FFib_Vtnode_GND+((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ei=((FFib_Vbi_ei+((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/2.0);
FFfcbar_Vbi_ci=((FFib_Vbi_ci+((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/2.0);
FFfcbar_Vci_ei=((FFib_Vci_ei+((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/2.0);
#endif
FFfcbar=((FFib+d00_sqrt0)/2.0);
}
{
double m00_sqrt(d00_sqrt0,((FFib*FFib)+acbar))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFib*FFib)+acbar))
#endif
#if defined(_DERIVATE)
FFdib_ditf_Vtnode_GND=(((FFfcbar_Vtnode_GND*d00_sqrt0-FFfcbar*((FFib_Vtnode_GND*FFib)+(FFib*FFib_Vtnode_GND))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ei=(((FFfcbar_Vbi_ei*d00_sqrt0-FFfcbar*((FFib_Vbi_ei*FFib)+(FFib*FFib_Vbi_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vbi_ci=(((FFfcbar_Vbi_ci*d00_sqrt0-FFfcbar*((FFib_Vbi_ci*FFib)+(FFib*FFib_Vbi_ci))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
FFdib_ditf_Vci_ei=(((FFfcbar_Vci_ei*d00_sqrt0-FFfcbar*((FFib_Vci_ei*FFib)+(FFib*FFib_Vci_ei))*d10_sqrt0)/d00_sqrt0/d00_sqrt0)/icbar);
#endif
FFdib_ditf=((FFfcbar/d00_sqrt0)/icbar);
}
{
double m00_exp(d00_exp0,((-1.0)/FFfcbar))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdVc_Vtnode_GND=(vcbar*(-(-1.0)*FFfcbar_Vtnode_GND/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ei=(vcbar*(-(-1.0)*FFfcbar_Vbi_ei/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vbi_ci=(vcbar*(-(-1.0)*FFfcbar_Vbi_ci/FFfcbar/FFfcbar)*d10_exp0);
FFdVc_Vci_ei=(vcbar*(-(-1.0)*FFfcbar_Vci_ei/FFfcbar/FFfcbar)*d10_exp0);
#endif
FFdVc=(vcbar*d00_exp0);
}
#if defined(_DERIVATE)
FFdVc_ditf_Vtnode_GND=((((FFdVc_Vtnode_GND*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vtnode_GND*FFfcbar)+(FFfcbar*FFfcbar_Vtnode_GND)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vtnode_GND));
FFdVc_ditf_Vbi_ei=((((FFdVc_Vbi_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ei*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ei));
FFdVc_ditf_Vbi_ci=((((FFdVc_Vbi_ci*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vbi_ci*FFfcbar)+(FFfcbar*FFfcbar_Vbi_ci)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vbi_ci));
FFdVc_ditf_Vci_ei=((((FFdVc_Vci_ei*(FFfcbar*FFfcbar)-FFdVc*((FFfcbar_Vci_ei*FFfcbar)+(FFfcbar*FFfcbar_Vci_ei)))/(FFfcbar*FFfcbar)/(FFfcbar*FFfcbar))*FFdib_ditf)+((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf_Vci_ei));
#endif
FFdVc_ditf=((FFdVc/(FFfcbar*FFfcbar))*FFdib_ditf);
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQbfb_Vtnode_GND=((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQbfb_Vbi_ei=(((((1-fthc)*thcs_t)*itf_Vbi_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQbfb_Vbi_ci=(((((1-fthc)*thcs_t)*itf_Vbi_ci)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQbfb_Vci_ei=(((((1-fthc)*thcs_t)*itf_Vci_ei)*(d00_exp0-1))+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQbfb=((((1-fthc)*thcs_t)*itf)*(d00_exp0-1));
}
{
double m00_exp(d00_exp0,(FFdVc/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTbfb_Vtnode_GND=(((FFdQbfb_Vtnode_GND*itf-FFdQbfb*itf_Vtnode_GND)/itf/itf)+((((((((((1-fthc)*thcs_t_Vtnode_GND)*itf)+(((1-fthc)*thcs_t)*itf_Vtnode_GND))*d00_exp0)+((((1-fthc)*thcs_t)*itf)*((FFdVc_Vtnode_GND*VT-FFdVc*VT_Vtnode_GND)/VT/VT)*d10_exp0))*VT-((((1-fthc)*thcs_t)*itf)*d00_exp0)*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTbfb_Vbi_ei=(((FFdQbfb_Vbi_ei*itf-FFdQbfb*itf_Vbi_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ei)));
FFdTbfb_Vbi_ci=(((FFdQbfb_Vbi_ci*itf-FFdQbfb*itf_Vbi_ci)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vbi_ci)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vbi_ci/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vbi_ci)));
FFdTbfb_Vci_ei=(((FFdQbfb_Vci_ei*itf-FFdQbfb*itf_Vci_ei)/itf/itf)+((((((((1-fthc)*thcs_t)*itf_Vci_ei)*d00_exp0)+((((1-fthc)*thcs_t)*itf)*(FFdVc_Vci_ei/VT)*d10_exp0))/VT)*FFdVc_ditf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTbfb=((FFdQbfb/itf)+((((((1-fthc)*thcs_t)*itf)*d00_exp0)/VT)*FFdVc_ditf));
}
#if defined(_DERIVATE)
FFic_Vtnode_GND=(-(-FFitf_ick_Vtnode_GND/FFitf_ick/FFitf_ick));
FFic_Vbi_ei=(-(-FFitf_ick_Vbi_ei/FFitf_ick/FFitf_ick));
FFic_Vbi_ci=(-(-FFitf_ick_Vbi_ci/FFitf_ick/FFitf_ick));
FFic_Vci_ei=(-(-FFitf_ick_Vci_ei/FFitf_ick/FFitf_ick));
#endif
FFic=(1-(1.0/FFitf_ick));
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
double m00_sqrt(d00_sqrt1,(1+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFw_Vtnode_GND=((FFic_Vtnode_GND+((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ei=((FFic_Vbi_ei+((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vbi_ci=((FFic_Vbi_ci+((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0)/(1+d00_sqrt1));
FFw_Vci_ei=((FFic_Vci_ei+((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0)/(1+d00_sqrt1));
#endif
FFw=((FFic+d00_sqrt0)/(1+d00_sqrt1));
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQfhc_Vtnode_GND=((((((((thcs_t_Vtnode_GND*itf)+(thcs_t*itf_Vtnode_GND))*FFw)+((thcs_t*itf)*FFw_Vtnode_GND))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vtnode_GND))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQfhc_Vbi_ei=(((((((thcs_t*itf_Vbi_ei)*FFw)+((thcs_t*itf)*FFw_Vbi_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQfhc_Vbi_ci=(((((((thcs_t*itf_Vbi_ci)*FFw)+((thcs_t*itf)*FFw_Vbi_ci))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vbi_ci))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQfhc_Vci_ei=(((((((thcs_t*itf_Vci_ei)*FFw)+((thcs_t*itf)*FFw_Vci_ei))*FFw)+(((thcs_t*itf)*FFw)*FFw_Vci_ei))*d00_exp0)+((((thcs_t*itf)*FFw)*FFw)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQfhc=((((thcs_t*itf)*FFw)*FFw)*d00_exp0);
}
{
double m00_sqrt(d00_sqrt0,((FFic*FFic)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FFic*FFic)+ahc))
#endif
#if defined(_DERIVATE)
FFdTfhc_Vtnode_GND=((FFdQfhc_Vtnode_GND*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vtnode_GND/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vtnode_GND*d00_sqrt0)+(FFitf_ick*((FFic_Vtnode_GND*FFic)+(FFic*FFic_Vtnode_GND))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+(((-VT_Vtnode_GND/VT/VT)*FFdVc_ditf)+((1.0/VT)*FFdVc_ditf_Vtnode_GND)))));
FFdTfhc_Vbi_ei=((FFdQfhc_Vbi_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ei*FFic)+(FFic*FFic_Vbi_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ei))));
FFdTfhc_Vbi_ci=((FFdQfhc_Vbi_ci*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vbi_ci/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vbi_ci*d00_sqrt0)+(FFitf_ick*((FFic_Vbi_ci*FFic)+(FFic*FFic_Vbi_ci))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vbi_ci))));
FFdTfhc_Vci_ei=((FFdQfhc_Vci_ei*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)))+(FFdQfhc*((((-itf_Vci_ei/itf/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/itf)*(-2.0*((FFitf_ick_Vci_ei*d00_sqrt0)+(FFitf_ick*((FFic_Vci_ei*FFic)+(FFic*FFic_Vci_ei))*d10_sqrt0))/(FFitf_ick*d00_sqrt0)/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf_Vci_ei))));
#endif
FFdTfhc=(FFdQfhc*(((1.0/itf)*(1.0+(2.0/(FFitf_ick*d00_sqrt0))))+((1.0/VT)*FFdVc_ditf)));
}
if
((latb<=0.0)&&(latl<=0.0))
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(fthc*FFdQfhc_Vtnode_GND);
FFdQcfc_Vbi_ei=(fthc*FFdQfhc_Vbi_ei);
FFdQcfc_Vbi_ci=(fthc*FFdQfhc_Vbi_ci);
FFdQcfc_Vci_ei=(fthc*FFdQfhc_Vci_ei);
#endif
FFdQcfc=(fthc*FFdQfhc);
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=(fthc*FFdTfhc_Vtnode_GND);
FFdTcfc_Vbi_ei=(fthc*FFdTfhc_Vbi_ei);
FFdTcfc_Vbi_ci=(fthc*FFdTfhc_Vbi_ci);
FFdTcfc_Vci_ei=(fthc*FFdTfhc_Vci_ei);
#endif
FFdTcfc=(fthc*FFdTfhc);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=FFdQcfc_Vtnode_GND;
FFdQcfcT_Vbi_ei=FFdQcfc_Vbi_ei;
FFdQcfcT_Vbi_ci=FFdQcfc_Vbi_ci;
FFdQcfcT_Vci_ei=FFdQcfc_Vci_ei;
#endif
FFdQcfcT=FFdQcfc;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=FFdTcfc_Vtnode_GND;
FFdTcfcT_Vbi_ei=FFdTcfc_Vbi_ei;
FFdTcfcT_Vbi_ci=FFdTcfc_Vbi_ci;
FFdTcfcT_Vci_ei=FFdTcfc_Vci_ei;
#endif
FFdTcfcT=FFdTcfc;
#endif
}
else
{
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=(((fthc*thcs_t_Vtnode_GND)*itf)+((fthc*thcs_t)*itf_Vtnode_GND));
FFdQcfc_Vbi_ei=((fthc*thcs_t)*itf_Vbi_ei);
FFdQcfc_Vbi_ci=((fthc*thcs_t)*itf_Vbi_ci);
FFdQcfc_Vci_ei=((fthc*thcs_t)*itf_Vci_ei);
#endif
FFdQcfc=((fthc*thcs_t)*itf);
#if defined(_DERIVATE)
FCa_Vci_ei=(-((ick_Vci_ei*itf-ick*itf_Vci_ei)/itf/itf));
FCa_Vtnode_GND=(-((ick_Vtnode_GND*itf-ick*itf_Vtnode_GND)/itf/itf));
FCa_Vbi_ei=(-(-ick*itf_Vbi_ei/itf/itf));
FCa_Vbi_ci=(-(-ick*itf_Vbi_ci/itf/itf));
#endif
FCa=(1.0-(ick/itf));
{
double m00_sqrt(d00_sqrt0,((FCa*FCa)+ahc))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((FCa*FCa)+ahc))
#endif
#if defined(_DERIVATE)
FCrt_Vci_ei=((FCa_Vci_ei*FCa)+(FCa*FCa_Vci_ei))*d10_sqrt0;
FCrt_Vtnode_GND=((FCa_Vtnode_GND*FCa)+(FCa*FCa_Vtnode_GND))*d10_sqrt0;
FCrt_Vbi_ei=((FCa_Vbi_ei*FCa)+(FCa*FCa_Vbi_ei))*d10_sqrt0;
FCrt_Vbi_ci=((FCa_Vbi_ci*FCa)+(FCa*FCa_Vbi_ci))*d10_sqrt0;
#endif
FCrt=d00_sqrt0;
}
{
double m00_sqrt(d00_sqrt0,(1.0+ahc))
#if defined(_DERIVATE)
FCa_ck_Vci_ei=(-((FCa_Vci_ei+FCrt_Vci_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vtnode_GND=(-((FCa_Vtnode_GND+FCrt_Vtnode_GND)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ei=(-((FCa_Vbi_ei+FCrt_Vbi_ei)/(1.0+d00_sqrt0)));
FCa_ck_Vbi_ci=(-((FCa_Vbi_ci+FCrt_Vbi_ci)/(1.0+d00_sqrt0)));
#endif
FCa_ck=(1.0-((FCa+FCrt)/(1.0+d00_sqrt0)));
}
#if defined(_DERIVATE)
FCdaick_ditf_Vci_ei=((((FCa_ck_Vci_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vci_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vci_ei*itf)+(FCrt*itf_Vci_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vtnode_GND=((((FCa_ck_Vtnode_GND*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vtnode_GND)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vtnode_GND*itf)+(FCrt*itf_Vtnode_GND)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ei=((((FCa_ck_Vbi_ei*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ei)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ei*itf)+(FCrt*itf_Vbi_ei)))/(FCrt*itf)/(FCrt*itf));
FCdaick_ditf_Vbi_ci=((((FCa_ck_Vbi_ci*(1-FCa))+((FCa_ck-1.0)*(-FCa_Vbi_ci)))*(FCrt*itf)-((FCa_ck-1.0)*(1-FCa))*((FCrt_Vbi_ci*itf)+(FCrt*itf_Vbi_ci)))/(FCrt*itf)/(FCrt*itf));
#endif
FCdaick_ditf=(((FCa_ck-1.0)*(1-FCa))/(FCrt*itf));
if
(latb>latl)
{
#if defined(_DERIVATE)
FCz_Vci_ei=0.0;
FCz_Vtnode_GND=0.0;
FCz_Vbi_ei=0.0;
FCz_Vbi_ci=0.0;
#endif
FCz=(latb-latl);
FCxl=(1.0+latl);
FCxb=(1.0+latb);
if
(latb>0.01)
{
{
double m00_logE(d00_logE0,(FCxb/FCxl))
FCln=d00_logE0;
}
{
double m00_exp(d00_exp0,((FCa_ck-1.0)*FCln))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=FCa_ck_Vci_ei*FCln*d10_exp0;
FCa1_Vtnode_GND=FCa_ck_Vtnode_GND*FCln*d10_exp0;
FCa1_Vbi_ei=FCa_ck_Vbi_ei*FCln*d10_exp0;
FCa1_Vbi_ci=FCa_ck_Vbi_ci*FCln*d10_exp0;
#endif
FCa1=d00_exp0;
}
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-(-FCa1_Vci_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vtnode_GND=(-(-FCa1_Vtnode_GND*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ei=(-(-FCa1_Vbi_ei*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
FCd_a_Vbi_ci=(-(-FCa1_Vbi_ci*latb)/(latl-(FCa1*latb))/(latl-(FCa1*latb)));
#endif
FCd_a=(1.0/(latl-(FCa1*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCa1_Vci_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCa1_Vtnode_GND*FCd_a)+((FCa1-1.0)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCa1_Vbi_ei*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCa1_Vbi_ci*FCd_a)+((FCa1-1.0)*FCd_a_Vbi_ci));
#endif
FCw=((FCa1-1.0)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=(((((((-FCz_Vci_ei)*FCa1)+((-FCz)*FCa1_Vci_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vci_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=(((((((-FCz_Vtnode_GND)*FCa1)+((-FCz)*FCa1_Vtnode_GND))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vtnode_GND))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=(((((((-FCz_Vbi_ei)*FCa1)+((-FCz)*FCa1_Vbi_ei))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ei))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=(((((((-FCz_Vbi_ci)*FCa1)+((-FCz)*FCa1_Vbi_ci))*FCln*FCd_a)+((((-FCz)*FCa1)*FCln)*FCd_a_Vbi_ci))*FCd_a)+(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-FCz)*FCa1)*FCln)*FCd_a)*FCd_a);
{
double m00_logE(d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,((1.0+(latb*FCw))/(1.0+(latl*FCw))))
#endif
#if defined(_DERIVATE)
FCa1_Vci_ei=(((latb*FCw_Vci_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vci_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vtnode_GND=(((latb*FCw_Vtnode_GND)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vtnode_GND))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ei=(((latb*FCw_Vbi_ei)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ei))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
FCa1_Vbi_ci=(((latb*FCw_Vbi_ci)*(1.0+(latl*FCw))-(1.0+(latb*FCw))*(latl*FCw_Vbi_ci))/(1.0+(latl*FCw))/(1.0+(latl*FCw)))*d10_logE0;
#endif
FCa1=d00_logE0;
}
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=((-latb*(latb*FCw_Vci_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vci_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vtnode_GND=((-latb*(latb*FCw_Vtnode_GND)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vtnode_GND)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ei=((-latb*(latb*FCw_Vbi_ei)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ei)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
FCda1_dw_Vbi_ci=((-latb*(latb*FCw_Vbi_ci)/(1.0+(latb*FCw))/(1.0+(latb*FCw)))-(-latl*(latl*FCw_Vbi_ci)/(1.0+(latl*FCw))/(1.0+(latl*FCw))));
#endif
FCda1_dw=((latb/(1.0+(latb*FCw)))-(latl/(1.0+(latl*FCw))));
}
else
{
#if defined(_DERIVATE)
FCf1_Vci_ei=(-FCa_ck_Vci_ei);
FCf1_Vtnode_GND=(-FCa_ck_Vtnode_GND);
FCf1_Vbi_ei=(-FCa_ck_Vbi_ei);
FCf1_Vbi_ci=(-FCa_ck_Vbi_ci);
#endif
FCf1=(1.0-FCa_ck);
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=((FCf1_Vci_ei*FCd_a)+(FCf1*FCd_a_Vci_ei));
FCw_Vtnode_GND=((FCf1_Vtnode_GND*FCd_a)+(FCf1*FCd_a_Vtnode_GND));
FCw_Vbi_ei=((FCf1_Vbi_ei*FCd_a)+(FCf1*FCd_a_Vbi_ei));
FCw_Vbi_ci=((FCf1_Vbi_ci*FCd_a)+(FCf1*FCd_a_Vbi_ci));
#endif
FCw=(FCf1*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((((-1.0)*FCd_a_Vci_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vci_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((((-1.0)*FCd_a_Vtnode_GND)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vtnode_GND))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((((-1.0)*FCd_a_Vbi_ei)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ei))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((((-1.0)*FCd_a_Vbi_ci)*FCd_a)+(((-1.0)*FCd_a)*FCd_a_Vbi_ci))*FCxb*FCd_a)+(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((((-1.0)*FCd_a)*FCd_a)*FCxb)*FCd_a);
#if defined(_DERIVATE)
FCa1_Vci_ei=((FCz_Vci_ei*FCw)+(FCz*FCw_Vci_ei));
FCa1_Vtnode_GND=((FCz_Vtnode_GND*FCw)+(FCz*FCw_Vtnode_GND));
FCa1_Vbi_ei=((FCz_Vbi_ei*FCw)+(FCz*FCw_Vbi_ei));
FCa1_Vbi_ci=((FCz_Vbi_ci*FCw)+(FCz*FCw_Vbi_ci));
#endif
FCa1=(FCz*FCw);
#if defined(_DERIVATE)
FCda1_dw_Vci_ei=FCz_Vci_ei;
FCda1_dw_Vtnode_GND=FCz_Vtnode_GND;
FCda1_dw_Vbi_ei=FCz_Vbi_ei;
FCda1_dw_Vbi_ci=FCz_Vbi_ci;
#endif
FCda1_dw=FCz;
}
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=(-2.0*FCz_Vbi_ei/FCz/FCz);
FCf_CT_Vbi_ci=(-2.0*FCz_Vbi_ci/FCz/FCz);
FCf_CT_Vci_ei=(-2.0*FCz_Vci_ei/FCz/FCz);
FCf_CT_Vtnode_GND=(-2.0*FCz_Vtnode_GND/FCz/FCz);
#endif
FCf_CT=(2.0/FCz);
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCf1_Vci_ei=(((((((latb*latl)*FCw_Vci_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vci_ei))/3.0)+(((latb+latl)*FCw2_Vci_ei)/2.0))+FCw_Vci_ei);
FCf1_Vtnode_GND=(((((((latb*latl)*FCw_Vtnode_GND)*FCw2)+(((latb*latl)*FCw)*FCw2_Vtnode_GND))/3.0)+(((latb+latl)*FCw2_Vtnode_GND)/2.0))+FCw_Vtnode_GND);
FCf1_Vbi_ei=(((((((latb*latl)*FCw_Vbi_ei)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ei))/3.0)+(((latb+latl)*FCw2_Vbi_ei)/2.0))+FCw_Vbi_ei);
FCf1_Vbi_ci=(((((((latb*latl)*FCw_Vbi_ci)*FCw2)+(((latb*latl)*FCw)*FCw2_Vbi_ci))/3.0)+(((latb+latl)*FCw2_Vbi_ci)/2.0))+FCw_Vbi_ci);
#endif
FCf1=((((((latb*latl)*FCw)*FCw2)/3.0)+(((latb+latl)*FCw2)/2.0))+FCw);
#if defined(_DERIVATE)
FCdf1_dw_Vci_ei=(((latb*latl)*FCw2_Vci_ei)+((latb+latl)*FCw_Vci_ei));
FCdf1_dw_Vtnode_GND=(((latb*latl)*FCw2_Vtnode_GND)+((latb+latl)*FCw_Vtnode_GND));
FCdf1_dw_Vbi_ei=(((latb*latl)*FCw2_Vbi_ei)+((latb+latl)*FCw_Vbi_ei));
FCdf1_dw_Vbi_ci=(((latb*latl)*FCw2_Vbi_ci)+((latb+latl)*FCw_Vbi_ci));
#endif
FCdf1_dw=((((latb*latl)*FCw2)+((latb+latl)*FCw))+1.0);
#if defined(_DERIVATE)
z_Vci_ei=(latb*FCw_Vci_ei);
z_Vtnode_GND=(latb*FCw_Vtnode_GND);
z_Vbi_ei=(latb*FCw_Vbi_ei);
z_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
z=(latb*FCw);
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latl/latb);
#if defined(_DERIVATE)
FCf2_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latb);
FCf2_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latb);
FCf2_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latb);
FCf2_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latb);
#endif
FCf2=((((1.0-r)*a2)+(r*a3))/latb);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf2_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((((((latb*a2_Vbi_ei)+(latl*a3_Vbi_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf2_Vbi_ci=(((((((latb*a2_Vbi_ci)+(latl*a3_Vbi_ci))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vbi_ci))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf2_Vci_ei=(((((((latb*a2_Vci_ei)+(latl*a3_Vci_ei))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vci_ei))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf2_Vtnode_GND=(((((((latb*a2_Vtnode_GND)+(latl*a3_Vtnode_GND))*FCw)+(((latb*a2)+(latl*a3))*FCw_Vtnode_GND))*FCw)+((((latb*a2)+(latl*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf2=(((((latb*a2)+(latl*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=(((((latl*FCw_Vci_ei)*(1+z))+((1+(latl*FCw))*z_Vci_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vci_ei));
FCdf2_dw_Vtnode_GND=(((((latl*FCw_Vtnode_GND)*(1+z))+((1+(latl*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf2_dw_Vbi_ei=(((((latl*FCw_Vbi_ei)*(1+z))+((1+(latl*FCw))*z_Vbi_ei))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf2_dw_Vbi_ci=(((((latl*FCw_Vbi_ci)*(1+z))+((1+(latl*FCw))*z_Vbi_ci))*lnzb)+(((1+(latl*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf2_dw=(((1+(latl*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
z_Vci_ei=(latl*FCw_Vci_ei);
z_Vtnode_GND=(latl*FCw_Vtnode_GND);
z_Vbi_ei=(latl*FCw_Vbi_ei);
z_Vbi_ci=(latl*FCw_Vbi_ci);
#endif
z=(latl*FCw);
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnzb_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnzb_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnzb_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnzb_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnzb=d00_logE0;
}
if
(z>1.0e-6)
{
#if defined(_DERIVATE)
x_Vci_ei=z_Vci_ei;
x_Vtnode_GND=z_Vtnode_GND;
x_Vbi_ei=z_Vbi_ei;
x_Vbi_ci=z_Vbi_ci;
#endif
x=(1.0+z);
#if defined(_DERIVATE)
a_Vbi_ei=((x_Vbi_ei*x)+(x*x_Vbi_ei));
a_Vbi_ci=((x_Vbi_ci*x)+(x*x_Vbi_ci));
a_Vci_ei=((x_Vci_ei*x)+(x*x_Vci_ei));
a_Vtnode_GND=((x_Vtnode_GND*x)+(x*x_Vtnode_GND));
#endif
a=(x*x);
#if defined(_DERIVATE)
a2_Vbi_ei=(0.250*((a_Vbi_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ei))));
a2_Vbi_ci=(0.250*((a_Vbi_ci*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vbi_ci))));
a2_Vci_ei=(0.250*((a_Vci_ei*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vci_ei))));
a2_Vtnode_GND=(0.250*((a_Vtnode_GND*((2.0*lnzb)-1.0))+(a*(2.0*lnzb_Vtnode_GND))));
#endif
a2=(0.250*((a*((2.0*lnzb)-1.0))+1.0));
#if defined(_DERIVATE)
a3_Vbi_ei=(((((a_Vbi_ei*x)+(a*x_Vbi_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ei)))/9.0);
a3_Vbi_ci=(((((a_Vbi_ci*x)+(a*x_Vbi_ci))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vbi_ci)))/9.0);
a3_Vci_ei=(((((a_Vci_ei*x)+(a*x_Vci_ei))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vci_ei)))/9.0);
a3_Vtnode_GND=(((((a_Vtnode_GND*x)+(a*x_Vtnode_GND))*((3.0*lnzb)-1.0))+((a*x)*(3.0*lnzb_Vtnode_GND)))/9.0);
#endif
a3=((((a*x)*((3.0*lnzb)-1.0))+1.0)/9.0);
r=(latb/latl);
#if defined(_DERIVATE)
FCf3_Vbi_ei=((((1.0-r)*a2_Vbi_ei)+(r*a3_Vbi_ei))/latl);
FCf3_Vbi_ci=((((1.0-r)*a2_Vbi_ci)+(r*a3_Vbi_ci))/latl);
FCf3_Vci_ei=((((1.0-r)*a2_Vci_ei)+(r*a3_Vci_ei))/latl);
FCf3_Vtnode_GND=((((1.0-r)*a2_Vtnode_GND)+(r*a3_Vtnode_GND))/latl);
#endif
FCf3=((((1.0-r)*a2)+(r*a3))/latl);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((1.0-r)*x_Vci_ei)+(r*a_Vci_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((1.0-r)*x_Vtnode_GND)+(r*a_Vtnode_GND))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((1.0-r)*x_Vbi_ei)+(r*a_Vbi_ei))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((1.0-r)*x_Vbi_ci)+(r*a_Vbi_ci))*lnzb)+((((1.0-r)*x)+(r*a))*lnzb_Vbi_ci));
#endif
FCdf3_dw=((((1.0-r)*x)+(r*a))*lnzb);
}
else
{
#if defined(_DERIVATE)
a_Vbi_ei=((z_Vbi_ei*z)+(z*z_Vbi_ei));
a_Vbi_ci=((z_Vbi_ci*z)+(z*z_Vbi_ci));
a_Vci_ei=((z_Vci_ei*z)+(z*z_Vci_ei));
a_Vtnode_GND=((z_Vtnode_GND*z)+(z*z_Vtnode_GND));
#endif
a=(z*z);
#if defined(_DERIVATE)
a2_Vbi_ei=((z_Vbi_ei-(0.25*a_Vbi_ei))+(((0.10*z_Vbi_ei)*a)+((0.10*z)*a_Vbi_ei)));
a2_Vbi_ci=((z_Vbi_ci-(0.25*a_Vbi_ci))+(((0.10*z_Vbi_ci)*a)+((0.10*z)*a_Vbi_ci)));
a2_Vci_ei=((z_Vci_ei-(0.25*a_Vci_ei))+(((0.10*z_Vci_ei)*a)+((0.10*z)*a_Vci_ei)));
a2_Vtnode_GND=((z_Vtnode_GND-(0.25*a_Vtnode_GND))+(((0.10*z_Vtnode_GND)*a)+((0.10*z)*a_Vtnode_GND)));
#endif
a2=(((3.0+z)-(0.25*a))+((0.10*z)*a));
#if defined(_DERIVATE)
a3_Vbi_ei=(((2.0*z_Vbi_ei)+(0.75*a_Vbi_ei))-(((0.20*a_Vbi_ei)*z)+((0.20*a)*z_Vbi_ei)));
a3_Vbi_ci=(((2.0*z_Vbi_ci)+(0.75*a_Vbi_ci))-(((0.20*a_Vbi_ci)*z)+((0.20*a)*z_Vbi_ci)));
a3_Vci_ei=(((2.0*z_Vci_ei)+(0.75*a_Vci_ei))-(((0.20*a_Vci_ei)*z)+((0.20*a)*z_Vci_ei)));
a3_Vtnode_GND=(((2.0*z_Vtnode_GND)+(0.75*a_Vtnode_GND))-(((0.20*a_Vtnode_GND)*z)+((0.20*a)*z_Vtnode_GND)));
#endif
a3=(((2.0*z)+(0.75*a))-((0.20*a)*z));
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((((((latl*a2_Vbi_ei)+(latb*a3_Vbi_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ei))/6.0);
FCf3_Vbi_ci=(((((((latl*a2_Vbi_ci)+(latb*a3_Vbi_ci))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vbi_ci))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vbi_ci))/6.0);
FCf3_Vci_ei=(((((((latl*a2_Vci_ei)+(latb*a3_Vci_ei))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vci_ei))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vci_ei))/6.0);
FCf3_Vtnode_GND=(((((((latl*a2_Vtnode_GND)+(latb*a3_Vtnode_GND))*FCw)+(((latl*a2)+(latb*a3))*FCw_Vtnode_GND))*FCw)+((((latl*a2)+(latb*a3))*FCw)*FCw_Vtnode_GND))/6.0);
#endif
FCf3=(((((latl*a2)+(latb*a3))*FCw)*FCw)/6.0);
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=(((((latb*FCw_Vci_ei)*(1+z))+((1+(latb*FCw))*z_Vci_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vci_ei));
FCdf3_dw_Vtnode_GND=(((((latb*FCw_Vtnode_GND)*(1+z))+((1+(latb*FCw))*z_Vtnode_GND))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vtnode_GND));
FCdf3_dw_Vbi_ei=(((((latb*FCw_Vbi_ei)*(1+z))+((1+(latb*FCw))*z_Vbi_ei))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ei));
FCdf3_dw_Vbi_ci=(((((latb*FCw_Vbi_ci)*(1+z))+((1+(latb*FCw))*z_Vbi_ci))*lnzb)+(((1+(latb*FCw))*(1+z))*lnzb_Vbi_ci));
#endif
FCdf3_dw=(((1+(latb*FCw))*(1+z))*lnzb);
}
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((FCf_CT_Vci_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vci_ei*FCf1)+(FCa1*FCf1_Vci_ei))-FCf2_Vci_ei)+FCf3_Vci_ei)));
FCf_ci_Vtnode_GND=((FCf_CT_Vtnode_GND*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vtnode_GND*FCf1)+(FCa1*FCf1_Vtnode_GND))-FCf2_Vtnode_GND)+FCf3_Vtnode_GND)));
FCf_ci_Vbi_ei=((FCf_CT_Vbi_ei*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ei*FCf1)+(FCa1*FCf1_Vbi_ei))-FCf2_Vbi_ei)+FCf3_Vbi_ei)));
FCf_ci_Vbi_ci=((FCf_CT_Vbi_ci*(((FCa1*FCf1)-FCf2)+FCf3))+(FCf_CT*((((FCa1_Vbi_ci*FCf1)+(FCa1*FCf1_Vbi_ci))-FCf2_Vbi_ci)+FCf3_Vbi_ci)));
#endif
FCf_ci=(FCf_CT*(((FCa1*FCf1)-FCf2)+FCf3));
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=((FCf_CT_Vci_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vci_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vci_ei))+((FCda1_dw_Vci_ei*FCf1)+(FCda1_dw*FCf1_Vci_ei)))-FCdf2_dw_Vci_ei)+FCdf3_dw_Vci_ei)));
FCdfc_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vtnode_GND*FCdf1_dw)+(FCa1*FCdf1_dw_Vtnode_GND))+((FCda1_dw_Vtnode_GND*FCf1)+(FCda1_dw*FCf1_Vtnode_GND)))-FCdf2_dw_Vtnode_GND)+FCdf3_dw_Vtnode_GND)));
FCdfc_dw_Vbi_ei=((FCf_CT_Vbi_ei*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ei*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ei))+((FCda1_dw_Vbi_ei*FCf1)+(FCda1_dw*FCf1_Vbi_ei)))-FCdf2_dw_Vbi_ei)+FCdf3_dw_Vbi_ei)));
FCdfc_dw_Vbi_ci=((FCf_CT_Vbi_ci*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw))+(FCf_CT*(((((FCa1_Vbi_ci*FCdf1_dw)+(FCa1*FCdf1_dw_Vbi_ci))+((FCda1_dw_Vbi_ci*FCf1)+(FCda1_dw*FCf1_Vbi_ci)))-FCdf2_dw_Vbi_ci)+FCdf3_dw_Vbi_ci)));
#endif
FCdfc_dw=(FCf_CT*((((FCa1*FCdf1_dw)+(FCda1_dw*FCf1))-FCdf2_dw)+FCdf3_dw));
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
#ifdef _DYNAMIC
if
((flcomp==0.0)||(flcomp==2.1))
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
a_Vbi_ei=(latb*FCw_Vbi_ei);
a_Vbi_ci=(latb*FCw_Vbi_ci);
a_Vci_ei=(latb*FCw_Vci_ei);
a_Vtnode_GND=(latb*FCw_Vtnode_GND);
#endif
a=(latb*FCw);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1+(latb*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latb*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latb*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latb*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latb*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latb*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
#endif
#ifdef _DYNAMIC
if
(a>1.0e-6)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf2_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latb);
FCf2_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latb);
FCf2_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latb);
FCf2_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latb);
#endif
FCf2=((a-lnz)/latb);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf2_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf2_dw=(a/(1.0+a));
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf2_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf2_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf2_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf2_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf2=((0.5*a)*FCw);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdf2_dw_Vci_ei=a_Vci_ei;
FCdf2_dw_Vtnode_GND=a_Vtnode_GND;
FCdf2_dw_Vbi_ei=a_Vbi_ei;
FCdf2_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf2_dw=a;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
a_Vbi_ei=(latl*FCw_Vbi_ei);
a_Vbi_ci=(latl*FCw_Vbi_ci);
a_Vci_ei=(latl*FCw_Vci_ei);
a_Vtnode_GND=(latl*FCw_Vtnode_GND);
#endif
a=(latl*FCw);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1+(latl*FCw)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1+(latl*FCw)))
#endif
#if defined(_DERIVATE)
lnz_Vci_ei=(latl*FCw_Vci_ei)*d10_logE0;
lnz_Vtnode_GND=(latl*FCw_Vtnode_GND)*d10_logE0;
lnz_Vbi_ei=(latl*FCw_Vbi_ei)*d10_logE0;
lnz_Vbi_ci=(latl*FCw_Vbi_ci)*d10_logE0;
#endif
lnz=d00_logE0;
}
#endif
#ifdef _DYNAMIC
if
(a>1.0e-6)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf3_Vbi_ei=((a_Vbi_ei-lnz_Vbi_ei)/latl);
FCf3_Vbi_ci=((a_Vbi_ci-lnz_Vbi_ci)/latl);
FCf3_Vci_ei=((a_Vci_ei-lnz_Vci_ei)/latl);
FCf3_Vtnode_GND=((a_Vtnode_GND-lnz_Vtnode_GND)/latl);
#endif
FCf3=((a-lnz)/latl);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=((a_Vci_ei*(1.0+a)-a*a_Vci_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vtnode_GND=((a_Vtnode_GND*(1.0+a)-a*a_Vtnode_GND)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ei=((a_Vbi_ei*(1.0+a)-a*a_Vbi_ei)/(1.0+a)/(1.0+a));
FCdf3_dw_Vbi_ci=((a_Vbi_ci*(1.0+a)-a*a_Vbi_ci)/(1.0+a)/(1.0+a));
#endif
FCdf3_dw=(a/(1.0+a));
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf3_Vbi_ei=(((0.5*a_Vbi_ei)*FCw)+((0.5*a)*FCw_Vbi_ei));
FCf3_Vbi_ci=(((0.5*a_Vbi_ci)*FCw)+((0.5*a)*FCw_Vbi_ci));
FCf3_Vci_ei=(((0.5*a_Vci_ei)*FCw)+((0.5*a)*FCw_Vci_ei));
FCf3_Vtnode_GND=(((0.5*a_Vtnode_GND)*FCw)+((0.5*a)*FCw_Vtnode_GND));
#endif
FCf3=((0.5*a)*FCw);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdf3_dw_Vci_ei=a_Vci_ei;
FCdf3_dw_Vtnode_GND=a_Vtnode_GND;
FCdf3_dw_Vbi_ei=a_Vbi_ei;
FCdf3_dw_Vbi_ci=a_Vbi_ci;
#endif
FCdf3_dw=a;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((FCf_CT_Vbi_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ei-FCf3_Vbi_ei)));
FCf_CT_Vbi_ci=((FCf_CT_Vbi_ci*(FCf2-FCf3))+(FCf_CT*(FCf2_Vbi_ci-FCf3_Vbi_ci)));
FCf_CT_Vci_ei=((FCf_CT_Vci_ei*(FCf2-FCf3))+(FCf_CT*(FCf2_Vci_ei-FCf3_Vci_ei)));
FCf_CT_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCf2-FCf3))+(FCf_CT*(FCf2_Vtnode_GND-FCf3_Vtnode_GND)));
#endif
FCf_CT=(FCf_CT*(FCf2-FCf3));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=((FCf_CT_Vbi_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ei-FCdf3_dw_Vbi_ei)));
FCdfCT_dw_Vbi_ci=((FCf_CT_Vbi_ci*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vbi_ci-FCdf3_dw_Vbi_ci)));
FCdfCT_dw_Vci_ei=((FCf_CT_Vci_ei*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vci_ei-FCdf3_dw_Vci_ei)));
FCdfCT_dw_Vtnode_GND=((FCf_CT_Vtnode_GND*(FCdf2_dw-FCdf3_dw))+(FCf_CT*(FCdf2_dw_Vtnode_GND-FCdf3_dw_Vtnode_GND)));
#endif
FCdfCT_dw=(FCf_CT*(FCdf2_dw-FCdf3_dw));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
#endif
}
#endif /* if (...) */
}
else
{
if
(latb>0.01)
{
#if defined(_DERIVATE)
FCd_a_Vci_ei=(-FCa_ck_Vci_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vtnode_GND=(-FCa_ck_Vtnode_GND*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ei=(-FCa_ck_Vbi_ei*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
FCd_a_Vbi_ci=(-FCa_ck_Vbi_ci*latb/(1.0+(FCa_ck*latb))/(1.0+(FCa_ck*latb)));
#endif
FCd_a=(1.0/(1.0+(FCa_ck*latb)));
#if defined(_DERIVATE)
FCw_Vci_ei=(((-FCa_ck_Vci_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vci_ei));
FCw_Vtnode_GND=(((-FCa_ck_Vtnode_GND)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vtnode_GND));
FCw_Vbi_ei=(((-FCa_ck_Vbi_ei)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ei));
FCw_Vbi_ci=(((-FCa_ck_Vbi_ci)*FCd_a)+((1.0-FCa_ck)*FCd_a_Vbi_ci));
#endif
FCw=((1.0-FCa_ck)*FCd_a);
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=((((-(1.0+latb))*FCd_a_Vci_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vci_ei));
FCdw_daick_Vtnode_GND=((((-(1.0+latb))*FCd_a_Vtnode_GND)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vtnode_GND));
FCdw_daick_Vbi_ei=((((-(1.0+latb))*FCd_a_Vbi_ei)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ei));
FCdw_daick_Vbi_ci=((((-(1.0+latb))*FCd_a_Vbi_ci)*FCd_a)+(((-(1.0+latb))*FCd_a)*FCd_a_Vbi_ci));
#endif
FCdw_daick=(((-(1.0+latb))*FCd_a)*FCd_a);
}
else
{
#if defined(_DERIVATE)
FCw_Vci_ei=((-FCa_ck_Vci_ei)-FCa_ck_Vci_ei*latb);
FCw_Vtnode_GND=((-FCa_ck_Vtnode_GND)-FCa_ck_Vtnode_GND*latb);
FCw_Vbi_ei=((-FCa_ck_Vbi_ei)-FCa_ck_Vbi_ei*latb);
FCw_Vbi_ci=((-FCa_ck_Vbi_ci)-FCa_ck_Vbi_ci*latb);
#endif
FCw=((1.0-FCa_ck)-(FCa_ck*latb));
#if defined(_DERIVATE)
FCdw_daick_Vci_ei=0.0;
FCdw_daick_Vtnode_GND=0.0;
FCdw_daick_Vbi_ei=0.0;
FCdw_daick_Vbi_ci=0.0;
#endif
FCdw_daick=(-(1.0+latb));
}
#if defined(_DERIVATE)
FCw2_Vci_ei=((FCw_Vci_ei*FCw)+(FCw*FCw_Vci_ei));
FCw2_Vtnode_GND=((FCw_Vtnode_GND*FCw)+(FCw*FCw_Vtnode_GND));
FCw2_Vbi_ei=((FCw_Vbi_ei*FCw)+(FCw*FCw_Vbi_ei));
FCw2_Vbi_ci=((FCw_Vbi_ci*FCw)+(FCw*FCw_Vbi_ci));
#endif
FCw2=(FCw*FCw);
#if defined(_DERIVATE)
FCz_Vci_ei=(latb*FCw_Vci_ei);
FCz_Vtnode_GND=(latb*FCw_Vtnode_GND);
FCz_Vbi_ei=(latb*FCw_Vbi_ei);
FCz_Vbi_ci=(latb*FCw_Vbi_ci);
#endif
FCz=(latb*FCw);
#if defined(_DERIVATE)
FCz_1_Vci_ei=FCz_Vci_ei;
FCz_1_Vtnode_GND=FCz_Vtnode_GND;
FCz_1_Vbi_ei=FCz_Vbi_ei;
FCz_1_Vbi_ci=FCz_Vbi_ci;
#endif
FCz_1=(1.0+FCz);
#if defined(_DERIVATE)
FCd_f_Vci_ei=(-FCz_1_Vci_ei/FCz_1/FCz_1);
FCd_f_Vtnode_GND=(-FCz_1_Vtnode_GND/FCz_1/FCz_1);
FCd_f_Vbi_ei=(-FCz_1_Vbi_ei/FCz_1/FCz_1);
FCd_f_Vbi_ci=(-FCz_1_Vbi_ci/FCz_1/FCz_1);
#endif
FCd_f=(1.0/FCz_1);
#if defined(_DERIVATE)
FCf_ci_Vci_ei=((((FCw2_Vci_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vci_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vci_ei));
FCf_ci_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vtnode_GND/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vtnode_GND));
FCf_ci_Vbi_ei=((((FCw2_Vbi_ei*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ei/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_ci_Vbi_ci=((((FCw2_Vbi_ci*(1.0+(FCz/3.0)))+(FCw2*(FCz_Vbi_ci/3.0)))*FCd_f)+((FCw2*(1.0+(FCz/3.0)))*FCd_f_Vbi_ci));
#endif
FCf_ci=((FCw2*(1.0+(FCz/3.0)))*FCd_f);
#if defined(_DERIVATE)
FCdfc_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vci_ei+(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfc_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vtnode_GND+(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
FCdfc_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ei+(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfc_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(FCz_1+((FCz*FCz)/3.0)))+((2.0*FCw)*(FCz_1_Vbi_ci+(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
#endif
FCdfc_dw=((((2.0*FCw)*(FCz_1+((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
#if defined(_DERIVATE)
FCdw_ditf_Vci_ei=((FCdw_daick_Vci_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vci_ei));
FCdw_ditf_Vtnode_GND=((FCdw_daick_Vtnode_GND*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vtnode_GND));
FCdw_ditf_Vbi_ei=((FCdw_daick_Vbi_ei*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ei));
FCdw_ditf_Vbi_ci=((FCdw_daick_Vbi_ci*FCdaick_ditf)+(FCdw_daick*FCdaick_ditf_Vbi_ci));
#endif
FCdw_ditf=(FCdw_daick*FCdaick_ditf);
#if defined(_DERIVATE)
FCdfc_ditf_Vci_ei=((FCdfc_dw_Vci_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vci_ei));
FCdfc_ditf_Vtnode_GND=((FCdfc_dw_Vtnode_GND*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vtnode_GND));
FCdfc_ditf_Vbi_ei=((FCdfc_dw_Vbi_ei*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ei));
FCdfc_ditf_Vbi_ci=((FCdfc_dw_Vbi_ci*FCdw_ditf)+(FCdfc_dw*FCdw_ditf_Vbi_ci));
#endif
FCdfc_ditf=(FCdfc_dw*FCdw_ditf);
#ifdef _DYNAMIC
if
((flcomp==0.0)||(flcomp==2.1))
{
#ifdef _DYNAMIC
if
(FCz>0.001)
{
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,FCz_1)
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,FCz_1)
#endif
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=(((2.0*(((FCz_1_Vbi_ei*d00_logE0)+(FCz_1*FCz_1_Vbi_ei*d10_logE0))-FCz_Vbi_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vbi_ci=(((2.0*(((FCz_1_Vbi_ci*d00_logE0)+(FCz_1*FCz_1_Vbi_ci*d10_logE0))-FCz_Vbi_ci))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vbi_ci))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vci_ei=(((2.0*(((FCz_1_Vci_ei*d00_logE0)+(FCz_1*FCz_1_Vci_ei*d10_logE0))-FCz_Vci_ei))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vci_ei))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
FCf_CT_Vtnode_GND=(((2.0*(((FCz_1_Vtnode_GND*d00_logE0)+(FCz_1*FCz_1_Vtnode_GND*d10_logE0))-FCz_Vtnode_GND))*((latb*latb)*FCz_1)-(2.0*((FCz_1*d00_logE0)-FCz))*((latb*latb)*FCz_1_Vtnode_GND))/((latb*latb)*FCz_1)/((latb*latb)*FCz_1));
#endif
FCf_CT=((2.0*((FCz_1*d00_logE0)-FCz))/((latb*latb)*FCz_1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((2.0*FCw_Vbi_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((2.0*FCw_Vbi_ci)*FCd_f)+((2.0*FCw)*FCd_f_Vbi_ci))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((2.0*FCw_Vci_ei)*FCd_f)+((2.0*FCw)*FCd_f_Vci_ei))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((2.0*FCw_Vtnode_GND)*FCd_f)+((2.0*FCw)*FCd_f_Vtnode_GND))*FCd_f)+(((2.0*FCw)*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=(((2.0*FCw)*FCd_f)*FCd_f);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=((((FCw2_Vbi_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ei));
FCf_CT_Vbi_ci=((((FCw2_Vbi_ci*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vbi_ci/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vbi_ci));
FCf_CT_Vci_ei=((((FCw2_Vci_ei*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vci_ei/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vci_ei));
FCf_CT_Vtnode_GND=((((FCw2_Vtnode_GND*(1.0-(FCz/3.0)))+(FCw2*(-(FCz_Vtnode_GND/3.0))))*FCd_f)+((FCw2*(1.0-(FCz/3.0)))*FCd_f_Vtnode_GND));
#endif
FCf_CT=((FCw2*(1.0-(FCz/3.0)))*FCd_f);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_dw_Vbi_ei=(((((((2.0*FCw_Vbi_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ei*FCz)+(FCz*FCz_Vbi_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ei));
FCdfCT_dw_Vbi_ci=(((((((2.0*FCw_Vbi_ci)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vbi_ci*FCz)+(FCz*FCz_Vbi_ci))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vbi_ci))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vbi_ci));
FCdfCT_dw_Vci_ei=(((((((2.0*FCw_Vci_ei)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vci_ei*FCz)+(FCz*FCz_Vci_ei))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vci_ei))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vci_ei));
FCdfCT_dw_Vtnode_GND=(((((((2.0*FCw_Vtnode_GND)*(1.0-((FCz*FCz)/3.0)))+((2.0*FCw)*(-(((FCz_Vtnode_GND*FCz)+(FCz*FCz_Vtnode_GND))/3.0))))*FCd_f)+(((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f_Vtnode_GND))*FCd_f)+((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f_Vtnode_GND));
#endif
FCdfCT_dw=((((2.0*FCw)*(1.0-((FCz*FCz)/3.0)))*FCd_f)*FCd_f);
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=((FCdfCT_dw_Vbi_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ei));
FCdfCT_ditf_Vbi_ci=((FCdfCT_dw_Vbi_ci*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vbi_ci));
FCdfCT_ditf_Vci_ei=((FCdfCT_dw_Vci_ei*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vci_ei));
FCdfCT_ditf_Vtnode_GND=((FCdfCT_dw_Vtnode_GND*FCdw_ditf)+(FCdfCT_dw*FCdw_ditf_Vtnode_GND));
#endif
FCdfCT_ditf=(FCdfCT_dw*FCdw_ditf);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCf_CT_Vbi_ei=FCf_ci_Vbi_ei;
FCf_CT_Vbi_ci=FCf_ci_Vbi_ci;
FCf_CT_Vci_ei=FCf_ci_Vci_ei;
FCf_CT_Vtnode_GND=FCf_ci_Vtnode_GND;
#endif
FCf_CT=FCf_ci;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
FCdfCT_ditf_Vbi_ei=FCdfc_ditf_Vbi_ei;
FCdfCT_ditf_Vbi_ci=FCdfc_ditf_Vbi_ci;
FCdfCT_ditf_Vci_ei=FCdfc_ditf_Vci_ei;
FCdfCT_ditf_Vtnode_GND=FCdfc_ditf_Vtnode_GND;
#endif
FCdfCT_ditf=FCdfc_ditf;
#endif
}
#endif /* if (...) */
}
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfcT_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_CT)+(FFdQcfc*FCf_CT_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_CT)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfcT_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfcT_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_CT)+(FFdQcfc*FCf_CT_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfcT_Vci_ei=((((FFdQcfc_Vci_ei*FCf_CT)+(FFdQcfc*FCf_CT_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_CT)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfcT=((FFdQcfc*FCf_CT)*d00_exp0);
}
#endif
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdQcfc_Vtnode_GND=((((FFdQcfc_Vtnode_GND*FCf_ci)+(FFdQcfc*FCf_ci_Vtnode_GND))*d00_exp0)+((FFdQcfc*FCf_ci)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0));
FFdQcfc_Vbi_ei=((((FFdQcfc_Vbi_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ei/VT)*d10_exp0));
FFdQcfc_Vbi_ci=((((FFdQcfc_Vbi_ci*FCf_ci)+(FFdQcfc*FCf_ci_Vbi_ci))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vbi_ci/VT)*d10_exp0));
FFdQcfc_Vci_ei=((((FFdQcfc_Vci_ei*FCf_ci)+(FFdQcfc*FCf_ci_Vci_ei))*d00_exp0)+((FFdQcfc*FCf_ci)*(FFdVc_Vci_ei/VT)*d10_exp0));
#endif
FFdQcfc=((FFdQcfc*FCf_ci)*d00_exp0);
}
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfc_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vtnode_GND+((itf_Vtnode_GND*FCdfc_ditf)+(itf*FCdfc_ditf_Vtnode_GND)))))+((((FFdQcfc_Vtnode_GND*VT-FFdQcfc*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfc_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ei+((itf_Vbi_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ei)))))+(((FFdQcfc_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfc_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vbi_ci+((itf_Vbi_ci*FCdfc_ditf)+(itf*FCdfc_ditf_Vbi_ci)))))+(((FFdQcfc_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfc_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_ci_Vci_ei+((itf_Vci_ei*FCdfc_ditf)+(itf*FCdfc_ditf_Vci_ei)))))+(((FFdQcfc_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfc/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfc=((((fthc*thcs_t)*d00_exp0)*(FCf_ci+(itf*FCdfc_ditf)))+((FFdQcfc/VT)*FFdVc_ditf));
}
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((FFdVc-vcbar)/VT))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
FFdTcfcT_Vtnode_GND=((((((fthc*thcs_t_Vtnode_GND)*d00_exp0)+((fthc*thcs_t)*((FFdVc_Vtnode_GND*VT-(FFdVc-vcbar)*VT_Vtnode_GND)/VT/VT)*d10_exp0))*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vtnode_GND+((itf_Vtnode_GND*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vtnode_GND)))))+((((FFdQcfcT_Vtnode_GND*VT-FFdQcfcT*VT_Vtnode_GND)/VT/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vtnode_GND)));
FFdTcfcT_Vbi_ei=(((((fthc*thcs_t)*(FFdVc_Vbi_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ei+((itf_Vbi_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ei)))))+(((FFdQcfcT_Vbi_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ei)));
FFdTcfcT_Vbi_ci=(((((fthc*thcs_t)*(FFdVc_Vbi_ci/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vbi_ci+((itf_Vbi_ci*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vbi_ci)))))+(((FFdQcfcT_Vbi_ci/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vbi_ci)));
FFdTcfcT_Vci_ei=(((((fthc*thcs_t)*(FFdVc_Vci_ei/VT)*d10_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+(((fthc*thcs_t)*d00_exp0)*(FCf_CT_Vci_ei+((itf_Vci_ei*FCdfCT_ditf)+(itf*FCdfCT_ditf_Vci_ei)))))+(((FFdQcfcT_Vci_ei/VT)*FFdVc_ditf)+((FFdQcfcT/VT)*FFdVc_ditf_Vci_ei)));
#endif
FFdTcfcT=((((fthc*thcs_t)*d00_exp0)*(FCf_CT+(itf*FCdfCT_ditf)))+((FFdQcfcT/VT)*FFdVc_ditf));
}
#endif
}
#if defined(_DERIVATE)
FFdQbfc_Vtnode_GND=((1-fthc)*FFdQfhc_Vtnode_GND);
FFdQbfc_Vbi_ei=((1-fthc)*FFdQfhc_Vbi_ei);
FFdQbfc_Vbi_ci=((1-fthc)*FFdQfhc_Vbi_ci);
FFdQbfc_Vci_ei=((1-fthc)*FFdQfhc_Vci_ei);
#endif
FFdQbfc=((1-fthc)*FFdQfhc);
#if defined(_DERIVATE)
FFdTbfc_Vtnode_GND=((1-fthc)*FFdTfhc_Vtnode_GND);
FFdTbfc_Vbi_ei=((1-fthc)*FFdTfhc_Vbi_ei);
FFdTbfc_Vbi_ci=((1-fthc)*FFdTfhc_Vbi_ci);
FFdTbfc_Vci_ei=((1-fthc)*FFdTfhc_Vci_ei);
#endif
FFdTbfc=((1-fthc)*FFdTfhc);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Q_fT_Vci_ei=(((((hf0_t*Qf_Vci_ei)+FFdQbfb_Vci_ei)+FFdQbfc_Vci_ei)+(hfe_t*FFdQef_Vci_ei))+(hfc_t*FFdQcfcT_Vci_ei));
Q_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Qf)+(hf0_t*Qf_Vtnode_GND))+FFdQbfb_Vtnode_GND)+FFdQbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdQef)+(hfe_t*FFdQef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdQcfcT)+(hfc_t*FFdQcfcT_Vtnode_GND)));
Q_fT_Vbi_ci=(((((hf0_t*Qf_Vbi_ci)+FFdQbfb_Vbi_ci)+FFdQbfc_Vbi_ci)+(hfe_t*FFdQef_Vbi_ci))+(hfc_t*FFdQcfcT_Vbi_ci));
Q_fT_Vbi_ei=(((((hf0_t*Qf_Vbi_ei)+FFdQbfb_Vbi_ei)+FFdQbfc_Vbi_ei)+(hfe_t*FFdQef_Vbi_ei))+(hfc_t*FFdQcfcT_Vbi_ei));
#endif
Q_fT=(((((hf0_t*Qf)+FFdQbfb)+FFdQbfc)+(hfe_t*FFdQef))+(hfc_t*FFdQcfcT));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
T_fT_Vbi_ei=(((((hf0_t*Tf_Vbi_ei)+FFdTbfb_Vbi_ei)+FFdTbfc_Vbi_ei)+(hfe_t*FFdTef_Vbi_ei))+(hfc_t*FFdTcfcT_Vbi_ei));
T_fT_Vci_ei=(((((hf0_t*Tf_Vci_ei)+FFdTbfb_Vci_ei)+FFdTbfc_Vci_ei)+(hfe_t*FFdTef_Vci_ei))+(hfc_t*FFdTcfcT_Vci_ei));
T_fT_Vtnode_GND=((((((hf0_t_Vtnode_GND*Tf)+(hf0_t*Tf_Vtnode_GND))+FFdTbfb_Vtnode_GND)+FFdTbfc_Vtnode_GND)+((hfe_t_Vtnode_GND*FFdTef)+(hfe_t*FFdTef_Vtnode_GND)))+((hfc_t_Vtnode_GND*FFdTcfcT)+(hfc_t*FFdTcfcT_Vtnode_GND)));
T_fT_Vbi_ci=(((((hf0_t*Tf_Vbi_ci)+FFdTbfb_Vbi_ci)+FFdTbfc_Vbi_ci)+(hfe_t*FFdTef_Vbi_ci))+(hfc_t*FFdTcfcT_Vbi_ci));
#endif
T_fT=(((((hf0_t*Tf)+FFdTbfb)+FFdTbfc)+(hfe_t*FFdTef))+(hfc_t*FFdTcfcT));
#endif
#if defined(_DERIVATE)
Qf_Vci_ei=(((Qf_Vci_ei+(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei))+FFdQef_Vci_ei)+FFdQcfc_Vci_ei);
Qf_Vtnode_GND=(((Qf_Vtnode_GND+(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND))+FFdQef_Vtnode_GND)+FFdQcfc_Vtnode_GND);
Qf_Vbi_ci=(((Qf_Vbi_ci+(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci))+FFdQef_Vbi_ci)+FFdQcfc_Vbi_ci);
Qf_Vbi_ei=(((Qf_Vbi_ei+(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei))+FFdQef_Vbi_ei)+FFdQcfc_Vbi_ei);
#endif
Qf=(((Qf+(FFdQbfb+FFdQbfc))+FFdQef)+FFdQcfc);
#if defined(_DERIVATE)
Tf_Vbi_ei=(((Tf_Vbi_ei+(FFdTbfb_Vbi_ei+FFdTbfc_Vbi_ei))+FFdTef_Vbi_ei)+FFdTcfc_Vbi_ei);
Tf_Vci_ei=(((Tf_Vci_ei+(FFdTbfb_Vci_ei+FFdTbfc_Vci_ei))+FFdTef_Vci_ei)+FFdTcfc_Vci_ei);
Tf_Vtnode_GND=(((Tf_Vtnode_GND+(FFdTbfb_Vtnode_GND+FFdTbfc_Vtnode_GND))+FFdTef_Vtnode_GND)+FFdTcfc_Vtnode_GND);
Tf_Vbi_ci=(((Tf_Vbi_ci+(FFdTbfb_Vbi_ci+FFdTbfc_Vbi_ci))+FFdTef_Vbi_ci)+FFdTcfc_Vbi_ci);
#endif
Tf=(((Tf+(FFdTbfb+FFdTbfc))+FFdTef)+FFdTcfc);
#if defined(_DERIVATE)
Q_bf_Vtnode_GND=(FFdQbfb_Vtnode_GND+FFdQbfc_Vtnode_GND);
Q_bf_Vbi_ei=(FFdQbfb_Vbi_ei+FFdQbfc_Vbi_ei);
Q_bf_Vbi_ci=(FFdQbfb_Vbi_ci+FFdQbfc_Vbi_ci);
Q_bf_Vci_ei=(FFdQbfb_Vci_ei+FFdQbfc_Vci_ei);
#endif
Q_bf=(FFdQbfb+FFdQbfc);
}
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qr_Vci_ei=(Tr*itr_Vci_ei);
Qr_Vtnode_GND=(Tr*itr_Vtnode_GND);
Qr_Vbi_ci=(Tr*itr_Vbi_ci);
Qr_Vbi_ei=(Tr*itr_Vbi_ei);
#endif
Qr=(Tr*itr);
#endif
}
#if defined(_DERIVATE)
it_Vci_ei=(itf_Vci_ei-itr_Vci_ei);
it_Vtnode_GND=(itf_Vtnode_GND-itr_Vtnode_GND);
it_Vbi_ei=(itf_Vbi_ei-itr_Vbi_ei);
it_Vbi_ci=(itf_Vbi_ci-itr_Vbi_ci);
#endif
it=(itf-itr);
#if defined(_DERIVATE)
Qdei_Vci_ei=Qf_Vci_ei;
Qdei_Vtnode_GND=Qf_Vtnode_GND;
Qdei_Vbi_ci=Qf_Vbi_ci;
Qdei_Vbi_ei=Qf_Vbi_ei;
#endif
Qdei=Qf;
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdci_Vci_ei=Qr_Vci_ei;
Qdci_Vtnode_GND=Qr_Vtnode_GND;
Qdci_Vbi_ci=Qr_Vbi_ci;
Qdci_Vbi_ei=Qr_Vbi_ei;
#endif
Qdci=Qr;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Cdei_Vtnode_GND=((((T_f0_Vtnode_GND*itf)+(T_f0*itf_Vtnode_GND))*VT-(T_f0*itf)*VT_Vtnode_GND)/VT/VT);
Cdei_Vbi_ci=(((T_f0_Vbi_ci*itf)+(T_f0*itf_Vbi_ci))/VT);
Cdei_Vci_ei=((T_f0*itf_Vci_ei)/VT);
Cdei_Vbi_ei=((T_f0*itf_Vbi_ei)/VT);
#endif
Cdei=((T_f0*itf)/VT);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Cdci_Vci_ei=((tr*itr_Vci_ei)/VT);
Cdci_Vtnode_GND=(((tr*itr_Vtnode_GND)*VT-(tr*itr)*VT_Vtnode_GND)/VT/VT);
Cdci_Vbi_ci=((tr*itr_Vbi_ci)/VT);
Cdci_Vbi_ei=((tr*itr_Vbi_ei)/VT);
#endif
Cdci=((tr*itr)/VT);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Crbi_Vtnode_GND=(fcrbi*(((Cjei_Vtnode_GND+Cjci_Vtnode_GND)+Cdei_Vtnode_GND)+Cdci_Vtnode_GND));
Crbi_Vbi_ei=(fcrbi*(((Cjei_Vbi_ei+Cjci_Vbi_ei)+Cdei_Vbi_ei)+Cdci_Vbi_ei));
Crbi_Vbi_ci=(fcrbi*((Cjci_Vbi_ci+Cdei_Vbi_ci)+Cdci_Vbi_ci));
Crbi_Vci_ei=(fcrbi*(Cdei_Vci_ei+Cdci_Vci_ei));
#endif
Crbi=(fcrbi*(((Cjei+Cjci)+Cdei)+Cdci));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qrbi_Vtnode_GND=Crbi_Vtnode_GND*BP(bp,bi);
qrbi_Vbi_ei=Crbi_Vbi_ei*BP(bp,bi);
qrbi_Vbi_ci=Crbi_Vbi_ci*BP(bp,bi);
qrbi_Vci_ei=Crbi_Vci_ei*BP(bp,bi);
qrbi_Vbp_bi=(Crbi);
#endif
qrbi=(Crbi*BP(bp,bi));
#endif
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=(Vbici_Vbi_ci/(mbci*VT));
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=(-Vbici*(mbci*VT_Vtnode_GND)/(mbci*VT)/(mbci*VT));
#endif
DIOY=(Vbici/(mbci*VT));
if
(ibcis>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=DIOY_Vbi_ci;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=((le_Vbi_ci*d00_limexp0)+(le*DIOY_Vbi_ci*d10_limexp0));
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
ibci_Vtnode_GND=((ibcis_t_Vtnode_GND*(le-1.0))+(ibcis_t*le_Vtnode_GND));
ibci_Vbi_ci=(ibcis_t*le_Vbi_ci);
ibci_Vbi_ei=(ibcis_t*le_Vbi_ei);
#endif
ibci=(ibcis_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
ibci_Vtnode_GND=(-ibcis_t_Vtnode_GND);
ibci_Vbi_ci=0.0;
ibci_Vbi_ei=0.0;
#endif
ibci=(-ibcis_t);
}
}
else
{
#if defined(_DERIVATE)
ibci_Vtnode_GND=0.0;
ibci_Vbi_ci=0.0;
ibci_Vbi_ei=0.0;
#endif
ibci=0.0;
}
if
(((Vbici<0.0)&&(favl_t>0.0))&&(cjci0_t>0.0))
{
double v_bord;
double v_q;
double U0;
double av;
double avl;
#if defined(_DERIVATE)
double v_bord_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double v_bord_Vbi_ci;
#endif
#if defined(_DERIVATE)
double v_q_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double v_q_Vbi_ei;
#endif
#if defined(_DERIVATE)
double v_q_Vbi_ci;
#endif
#if defined(_DERIVATE)
double U0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double av_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double av_Vbi_ei;
#endif
#if defined(_DERIVATE)
double av_Vbi_ci;
#endif
#if defined(_DERIVATE)
double avl_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double avl_Vbi_ei;
#endif
#if defined(_DERIVATE)
double avl_Vbi_ci;
#endif
#if defined(_DERIVATE)
v_bord_Vtnode_GND=vdci_t_Vtnode_GND;
v_bord_Vbi_ci=(-Vbici_Vbi_ci);
#endif
v_bord=(vdci_t-Vbici);
#if defined(_DERIVATE)
v_q_Vtnode_GND=((qavl_t_Vtnode_GND*Cjci-qavl_t*Cjci_Vtnode_GND)/Cjci/Cjci);
v_q_Vbi_ei=(-qavl_t*Cjci_Vbi_ei/Cjci/Cjci);
v_q_Vbi_ci=(-qavl_t*Cjci_Vbi_ci/Cjci/Cjci);
#endif
v_q=(qavl_t/Cjci);
#if defined(_DERIVATE)
U0_Vtnode_GND=((qavl_t_Vtnode_GND*cjci0_t-qavl_t*cjci0_t_Vtnode_GND)/cjci0_t/cjci0_t);
#endif
U0=(qavl_t/cjci0_t);
if
(v_bord>U0)
{
{
double m00_exp(d00_exp0,((-v_q)/U0))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
av_Vtnode_GND=((favl_t_Vtnode_GND*d00_exp0)+(favl_t*(((-v_q_Vtnode_GND)*U0-(-v_q)*U0_Vtnode_GND)/U0/U0)*d10_exp0));
av_Vbi_ei=(favl_t*((-v_q_Vbi_ei)/U0)*d10_exp0);
av_Vbi_ci=(favl_t*((-v_q_Vbi_ci)/U0)*d10_exp0);
#endif
av=(favl_t*d00_exp0);
}
#if defined(_DERIVATE)
avl_Vtnode_GND=((av_Vtnode_GND*(U0+((1.0+(v_q/U0))*(v_bord-U0))))+(av*(U0_Vtnode_GND+((((v_q_Vtnode_GND*U0-v_q*U0_Vtnode_GND)/U0/U0)*(v_bord-U0))+((1.0+(v_q/U0))*(v_bord_Vtnode_GND-U0_Vtnode_GND))))));
avl_Vbi_ei=((av_Vbi_ei*(U0+((1.0+(v_q/U0))*(v_bord-U0))))+(av*(v_q_Vbi_ei/U0)*(v_bord-U0)));
avl_Vbi_ci=((av_Vbi_ci*(U0+((1.0+(v_q/U0))*(v_bord-U0))))+(av*(((v_q_Vbi_ci/U0)*(v_bord-U0))+((1.0+(v_q/U0))*v_bord_Vbi_ci))));
#endif
avl=(av*(U0+((1.0+(v_q/U0))*(v_bord-U0))));
}
else
{
{
double m00_exp(d00_exp0,((-v_q)/v_bord))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
avl_Vtnode_GND=((((favl_t_Vtnode_GND*v_bord)+(favl_t*v_bord_Vtnode_GND))*d00_exp0)+((favl_t*v_bord)*(((-v_q_Vtnode_GND)*v_bord-(-v_q)*v_bord_Vtnode_GND)/v_bord/v_bord)*d10_exp0));
avl_Vbi_ei=((favl_t*v_bord)*((-v_q_Vbi_ei)/v_bord)*d10_exp0);
avl_Vbi_ci=(((favl_t*v_bord_Vbi_ci)*d00_exp0)+((favl_t*v_bord)*(((-v_q_Vbi_ci)*v_bord-(-v_q)*v_bord_Vbi_ci)/v_bord/v_bord)*d10_exp0));
#endif
avl=((favl_t*v_bord)*d00_exp0);
}
}
#if defined(_DERIVATE)
iavl_Vci_ei=itf_Vci_ei*avl;
iavl_Vtnode_GND=((itf_Vtnode_GND*avl)+(itf*avl_Vtnode_GND));
iavl_Vbi_ei=((itf_Vbi_ei*avl)+(itf*avl_Vbi_ei));
iavl_Vbi_ci=((itf_Vbi_ci*avl)+(itf*avl_Vbi_ci));
#endif
iavl=(itf*avl);
}
else
{
#if defined(_DERIVATE)
iavl_Vci_ei=0.0;
iavl_Vtnode_GND=0.0;
iavl_Vbi_ei=0.0;
iavl_Vbi_ci=0.0;
#endif
iavl=0.0;
}
#if defined(_DERIVATE)
ibh_rec_Vtnode_GND=Q_bf_Vtnode_GND*Otbhrec;
ibh_rec_Vbi_ei=Q_bf_Vbi_ei*Otbhrec;
ibh_rec_Vbi_ci=Q_bf_Vbi_ci*Otbhrec;
ibh_rec_Vci_ei=Q_bf_Vci_ei*Otbhrec;
#endif
ibh_rec=(Q_bf*Otbhrec);
if
(rbi0_t>0.0)
{
double Qz_nom;
double f_QR;
double ETA;
double Qz0;
double fQz;
#if defined(_DERIVATE)
double Qz_nom_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double Qz_nom_Vbi_ei;
#endif
#if defined(_DERIVATE)
double Qz_nom_Vbi_ci;
#endif
#if defined(_DERIVATE)
double Qz_nom_Vci_ei;
#endif
#if defined(_DERIVATE)
double f_QR_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ETA_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double ETA_Vbi_ei;
#endif
#if defined(_DERIVATE)
double ETA_Vbi_ci;
#endif
#if defined(_DERIVATE)
double ETA_Vci_ei;
#endif
#if defined(_DERIVATE)
double Qz0_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double Qz0_Vbi_ei;
#endif
#if defined(_DERIVATE)
double Qz0_Vbi_ci;
#endif
#if defined(_DERIVATE)
double Qz0_Vci_ei;
#endif
#if defined(_DERIVATE)
double fQz_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double fQz_Vbi_ei;
#endif
#if defined(_DERIVATE)
double fQz_Vbi_ci;
#endif
#if defined(_DERIVATE)
double fQz_Vci_ei;
#endif
#if defined(_DERIVATE)
f_QR_Vtnode_GND=((1+fdqr0)*qp0_t_Vtnode_GND);
#endif
f_QR=((1+fdqr0)*qp0_t);
#if defined(_DERIVATE)
Qz0_Vtnode_GND=((Qjei_Vtnode_GND+Qjci_Vtnode_GND)+Qf_Vtnode_GND);
Qz0_Vbi_ei=((Qjei_Vbi_ei+Qjci_Vbi_ei)+Qf_Vbi_ei);
Qz0_Vbi_ci=(Qjci_Vbi_ci+Qf_Vbi_ci);
Qz0_Vci_ei=Qf_Vci_ei;
#endif
Qz0=((Qjei+Qjci)+Qf);
#if defined(_DERIVATE)
Qz_nom_Vtnode_GND=((Qz0_Vtnode_GND*f_QR-Qz0*f_QR_Vtnode_GND)/f_QR/f_QR);
Qz_nom_Vbi_ei=(Qz0_Vbi_ei/f_QR);
Qz_nom_Vbi_ci=(Qz0_Vbi_ci/f_QR);
Qz_nom_Vci_ei=(Qz0_Vci_ei/f_QR);
#endif
Qz_nom=(1+(Qz0/f_QR));
{
double m00_sqrt(d00_sqrt0,((Qz_nom*Qz_nom)+0.01))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((Qz_nom*Qz_nom)+0.01))
#endif
#if defined(_DERIVATE)
fQz_Vtnode_GND=(0.5*(Qz_nom_Vtnode_GND+((Qz_nom_Vtnode_GND*Qz_nom)+(Qz_nom*Qz_nom_Vtnode_GND))*d10_sqrt0));
fQz_Vbi_ei=(0.5*(Qz_nom_Vbi_ei+((Qz_nom_Vbi_ei*Qz_nom)+(Qz_nom*Qz_nom_Vbi_ei))*d10_sqrt0));
fQz_Vbi_ci=(0.5*(Qz_nom_Vbi_ci+((Qz_nom_Vbi_ci*Qz_nom)+(Qz_nom*Qz_nom_Vbi_ci))*d10_sqrt0));
fQz_Vci_ei=(0.5*(Qz_nom_Vci_ei+((Qz_nom_Vci_ei*Qz_nom)+(Qz_nom*Qz_nom_Vci_ei))*d10_sqrt0));
#endif
fQz=(0.5*(Qz_nom+d00_sqrt0));
}
#if defined(_DERIVATE)
rbi_Vtnode_GND=((rbi0_t_Vtnode_GND*fQz-rbi0_t*fQz_Vtnode_GND)/fQz/fQz);
rbi_Vbi_ei=(-rbi0_t*fQz_Vbi_ei/fQz/fQz);
rbi_Vbi_ci=(-rbi0_t*fQz_Vbi_ci/fQz/fQz);
rbi_Vci_ei=(-rbi0_t*fQz_Vci_ei/fQz/fQz);
#endif
rbi=(rbi0_t/fQz);
if
(ibei>0.0)
{
#if defined(_DERIVATE)
ETA_Vtnode_GND=((((rbi_Vtnode_GND*ibei)+(rbi*ibei_Vtnode_GND))*fgeo*VT-((rbi*ibei)*fgeo)*VT_Vtnode_GND)/VT/VT);
ETA_Vbi_ei=(((rbi_Vbi_ei*ibei)+(rbi*ibei_Vbi_ei))*fgeo/VT);
ETA_Vbi_ci=(rbi_Vbi_ci*ibei*fgeo/VT);
ETA_Vci_ei=(rbi_Vci_ei*ibei*fgeo/VT);
#endif
ETA=(((rbi*ibei)*fgeo)/VT);
if
(ETA<1.0e-6)
{
#if defined(_DERIVATE)
rbi_Vtnode_GND=((rbi_Vtnode_GND*(1.0-(0.5*ETA)))+(rbi*(-(0.5*ETA_Vtnode_GND))));
rbi_Vbi_ei=((rbi_Vbi_ei*(1.0-(0.5*ETA)))+(rbi*(-(0.5*ETA_Vbi_ei))));
rbi_Vbi_ci=((rbi_Vbi_ci*(1.0-(0.5*ETA)))+(rbi*(-(0.5*ETA_Vbi_ci))));
rbi_Vci_ei=((rbi_Vci_ei*(1.0-(0.5*ETA)))+(rbi*(-(0.5*ETA_Vci_ei))));
#endif
rbi=(rbi*(1.0-(0.5*ETA)));
}
else
{
{
double m00_logE(d00_logE0,(1.0+ETA))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+ETA))
#endif
#if defined(_DERIVATE)
rbi_Vtnode_GND=((((rbi_Vtnode_GND*d00_logE0)+(rbi*ETA_Vtnode_GND*d10_logE0))*ETA-(rbi*d00_logE0)*ETA_Vtnode_GND)/ETA/ETA);
rbi_Vbi_ei=((((rbi_Vbi_ei*d00_logE0)+(rbi*ETA_Vbi_ei*d10_logE0))*ETA-(rbi*d00_logE0)*ETA_Vbi_ei)/ETA/ETA);
rbi_Vbi_ci=((((rbi_Vbi_ci*d00_logE0)+(rbi*ETA_Vbi_ci*d10_logE0))*ETA-(rbi*d00_logE0)*ETA_Vbi_ci)/ETA/ETA);
rbi_Vci_ei=((((rbi_Vci_ei*d00_logE0)+(rbi*ETA_Vci_ei*d10_logE0))*ETA-(rbi*d00_logE0)*ETA_Vci_ei)/ETA/ETA);
#endif
rbi=((rbi*d00_logE0)/ETA);
}
}
}
if
(Qf>0.0)
{
#if defined(_DERIVATE)
rbi_Vtnode_GND=((((rbi_Vtnode_GND*(Qjei+(Qf*fqi)))+(rbi*(Qjei_Vtnode_GND+Qf_Vtnode_GND*fqi)))*(Qjei+Qf)-(rbi*(Qjei+(Qf*fqi)))*(Qjei_Vtnode_GND+Qf_Vtnode_GND))/(Qjei+Qf)/(Qjei+Qf));
rbi_Vbi_ei=((((rbi_Vbi_ei*(Qjei+(Qf*fqi)))+(rbi*(Qjei_Vbi_ei+Qf_Vbi_ei*fqi)))*(Qjei+Qf)-(rbi*(Qjei+(Qf*fqi)))*(Qjei_Vbi_ei+Qf_Vbi_ei))/(Qjei+Qf)/(Qjei+Qf));
rbi_Vbi_ci=((((rbi_Vbi_ci*(Qjei+(Qf*fqi)))+(rbi*Qf_Vbi_ci*fqi))*(Qjei+Qf)-(rbi*(Qjei+(Qf*fqi)))*Qf_Vbi_ci)/(Qjei+Qf)/(Qjei+Qf));
rbi_Vci_ei=((((rbi_Vci_ei*(Qjei+(Qf*fqi)))+(rbi*Qf_Vci_ei*fqi))*(Qjei+Qf)-(rbi*(Qjei+(Qf*fqi)))*Qf_Vci_ei)/(Qjei+Qf)/(Qjei+Qf));
#endif
rbi=((rbi*(Qjei+(Qf*fqi)))/(Qjei+Qf));
}
}
else
{
#if defined(_DERIVATE)
rbi_Vtnode_GND=0.0;
rbi_Vbi_ei=0.0;
rbi_Vbi_ci=0.0;
rbi_Vci_ei=0.0;
#endif
rbi=0.0;
}
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=(Vbpei_Vbp_ei/(mbep*VT));
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=(-Vbpei*(mbep*VT_Vtnode_GND)/(mbep*VT)/(mbep*VT));
#endif
DIOY=(Vbpei/(mbep*VT));
if
(ibeps>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=DIOY_Vbp_ei;
le_Vbi_ci=DIOY_Vbi_ci;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=((le_Vbp_ei*d00_limexp0)+(le*DIOY_Vbp_ei*d10_limexp0));
le_Vbi_ci=((le_Vbi_ci*d00_limexp0)+(le*DIOY_Vbi_ci*d10_limexp0));
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
ibep_Vtnode_GND=((ibeps_t_Vtnode_GND*(le-1.0))+(ibeps_t*le_Vtnode_GND));
ibep_Vbp_ei=(ibeps_t*le_Vbp_ei);
ibep_Vbi_ci=(ibeps_t*le_Vbi_ci);
ibep_Vbi_ei=(ibeps_t*le_Vbi_ei);
#endif
ibep=(ibeps_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
ibep_Vtnode_GND=(-ibeps_t_Vtnode_GND);
ibep_Vbp_ei=0.0;
ibep_Vbi_ci=0.0;
ibep_Vbi_ei=0.0;
#endif
ibep=(-ibeps_t);
}
}
else
{
#if defined(_DERIVATE)
ibep_Vtnode_GND=0.0;
ibep_Vbp_ei=0.0;
ibep_Vbi_ci=0.0;
ibep_Vbi_ei=0.0;
#endif
ibep=0.0;
}
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=(Vbpei_Vbp_ei/(mrep*VT));
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=(-Vbpei*(mrep*VT_Vtnode_GND)/(mrep*VT)/(mrep*VT));
#endif
DIOY=(Vbpei/(mrep*VT));
if
(ireps>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=DIOY_Vbp_ei;
le_Vbi_ci=DIOY_Vbi_ci;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=((le_Vbp_ei*d00_limexp0)+(le*DIOY_Vbp_ei*d10_limexp0));
le_Vbi_ci=((le_Vbi_ci*d00_limexp0)+(le*DIOY_Vbi_ci*d10_limexp0));
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
irep_Vtnode_GND=((ireps_t_Vtnode_GND*(le-1.0))+(ireps_t*le_Vtnode_GND));
irep_Vbp_ei=(ireps_t*le_Vbp_ei);
irep_Vbi_ci=(ireps_t*le_Vbi_ci);
irep_Vbi_ei=(ireps_t*le_Vbi_ei);
#endif
irep=(ireps_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
irep_Vtnode_GND=(-ireps_t_Vtnode_GND);
irep_Vbp_ei=0.0;
irep_Vbi_ci=0.0;
irep_Vbi_ei=0.0;
#endif
irep=(-ireps_t);
}
}
else
{
#if defined(_DERIVATE)
irep_Vtnode_GND=0.0;
irep_Vbp_ei=0.0;
irep_Vbi_ci=0.0;
irep_Vbi_ei=0.0;
#endif
irep=0.0;
}
if
(cjep0_t>0.0)
{
{
double m00_logE(d00_logE0,ajep_t)
double m00_exp(d00_exp1,((-d00_logE0)/zep))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,ajep_t)
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=((vdep_t_Vtnode_GND*(1.0-d00_exp1))+(vdep_t*(-((-ajep_t_Vtnode_GND*d10_logE0)/zep)*d10_exp1)));
#endif
DFV_f=(vdep_t*(1.0-d00_exp1));
}
#if defined(_DERIVATE)
DFv_e_Vsi_ci=0.0;
DFv_e_Vb_ci=0.0;
DFv_e_Vbp_ci=0.0;
DFv_e_Vbp_ei=((-Vbpei_Vbp_ei)/VT);
DFv_e_Vbi_ci=0.0;
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vbpei)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=0.0;
#endif
DFv_e=((DFV_f-Vbpei)/VT);
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=0.0;
DFs_q_Vb_ci=0.0;
DFs_q_Vbp_ci=0.0;
DFs_q_Vbp_ei=((DFv_e_Vbp_ei*DFv_e)+(DFv_e*DFv_e_Vbp_ei))*d10_sqrt0;
DFs_q_Vbi_ci=((DFv_e_Vbi_ci*DFv_e)+(DFv_e*DFv_e_Vbi_ci))*d10_sqrt0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=0.0;
DFs_q2_Vb_ci=0.0;
DFs_q2_Vbp_ci=0.0;
DFs_q2_Vbp_ei=(DFv_e_Vbp_ei+DFs_q_Vbp_ei)*0.5;
DFs_q2_Vbi_ci=(DFv_e_Vbi_ci+DFs_q_Vbi_ci)*0.5;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#if defined(_DERIVATE)
DFv_j_Vsi_ci=0.0;
DFv_j_Vb_ci=0.0;
DFv_j_Vbp_ci=0.0;
DFv_j_Vbp_ei=(-(VT*DFs_q2_Vbp_ei));
DFv_j_Vbi_ci=(-(VT*DFs_q2_Vbi_ci));
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=0.0;
DFdvj_dv_Vb_ci=0.0;
DFdvj_dv_Vbp_ci=0.0;
DFdvj_dv_Vbp_ei=((DFs_q2_Vbp_ei*DFs_q-DFs_q2*DFs_q_Vbp_ei)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ci=((DFs_q2_Vbi_ci*DFs_q-DFs_q2*DFs_q_Vbi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vdep_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vdep_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=0.0;
DFb_Vb_ci=0.0;
DFb_Vbp_ci=0.0;
DFb_Vbp_ei=(-(DFv_j_Vbp_ei/vdep_t))*d10_logE0;
DFb_Vbi_ci=(-(DFv_j_Vbi_ci/vdep_t))*d10_logE0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vdep_t-DFv_j*vdep_t_Vtnode_GND)/vdep_t/vdep_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vdep_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
{
double m00_exp(d00_exp0,((-zep)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=0.0;
DFC_j1_Vb_ci=0.0;
DFC_j1_Vbp_ci=0.0;
DFC_j1_Vbp_ei=(((cjep0_t*((-zep)*DFb_Vbp_ei)*d10_exp0)*DFdvj_dv)+((cjep0_t*d00_exp0)*DFdvj_dv_Vbp_ei));
DFC_j1_Vbi_ci=(((cjep0_t*((-zep)*DFb_Vbi_ci)*d10_exp0)*DFdvj_dv)+((cjep0_t*d00_exp0)*DFdvj_dv_Vbi_ci));
DFC_j1_Vtnode_GND=((((cjep0_t_Vtnode_GND*d00_exp0)+(cjep0_t*((-zep)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjep0_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjep0_t*((-zep)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjep0_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjep0_t*d00_exp0)*DFdvj_dv);
}
#if defined(_DERIVATE)
Cjep_Vbp_ei=(DFC_j1_Vbp_ei+((ajep_t*cjep0_t)*(-DFdvj_dv_Vbp_ei)));
Cjep_Vbi_ci=(DFC_j1_Vbi_ci+((ajep_t*cjep0_t)*(-DFdvj_dv_Vbi_ci)));
Cjep_Vtnode_GND=(DFC_j1_Vtnode_GND+((((ajep_t_Vtnode_GND*cjep0_t)+(ajep_t*cjep0_t_Vtnode_GND))*(1.0-DFdvj_dv))+((ajep_t*cjep0_t)*(-DFdvj_dv_Vtnode_GND))));
Cjep_Vbi_ei=(DFC_j1_Vbi_ei+((ajep_t*cjep0_t)*(-DFdvj_dv_Vbi_ei)));
#endif
Cjep=(DFC_j1+((ajep_t*cjep0_t)*(1.0-DFdvj_dv)));
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DFb*(1.0-zep)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=0.0;
DFQ_j_Vb_ci=0.0;
DFQ_j_Vbp_ci=0.0;
DFQ_j_Vbp_ei=(((cjep0_t*vdep_t)*(-DFb_Vbp_ei*(1.0-zep)*d10_exp0))/(1.0-zep));
DFQ_j_Vbi_ci=(((cjep0_t*vdep_t)*(-DFb_Vbi_ci*(1.0-zep)*d10_exp0))/(1.0-zep));
DFQ_j_Vtnode_GND=(((((cjep0_t_Vtnode_GND*vdep_t)+(cjep0_t*vdep_t_Vtnode_GND))*(1.0-d00_exp0))+((cjep0_t*vdep_t)*(-DFb_Vtnode_GND*(1.0-zep)*d10_exp0)))/(1.0-zep));
DFQ_j_Vbi_ei=(((cjep0_t*vdep_t)*(-DFb_Vbi_ei*(1.0-zep)*d10_exp0))/(1.0-zep));
#endif
DFQ_j=(((cjep0_t*vdep_t)*(1.0-d00_exp0))/(1.0-zep));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjep_Vbp_ei=(DFQ_j_Vbp_ei+((ajep_t*cjep0_t)*(Vbpei_Vbp_ei-DFv_j_Vbp_ei)));
Qjep_Vbi_ci=(DFQ_j_Vbi_ci+((ajep_t*cjep0_t)*(-DFv_j_Vbi_ci)));
Qjep_Vtnode_GND=(DFQ_j_Vtnode_GND+((((ajep_t_Vtnode_GND*cjep0_t)+(ajep_t*cjep0_t_Vtnode_GND))*(Vbpei-DFv_j))+((ajep_t*cjep0_t)*(-DFv_j_Vtnode_GND))));
Qjep_Vbi_ei=(DFQ_j_Vbi_ei+((ajep_t*cjep0_t)*(-DFv_j_Vbi_ei)));
#endif
Qjep=(DFQ_j+((ajep_t*cjep0_t)*(Vbpei-DFv_j)));
#endif
}
else
{
#if defined(_DERIVATE)
Cjep_Vbp_ei=0.0;
Cjep_Vbi_ci=0.0;
Cjep_Vtnode_GND=0.0;
Cjep_Vbi_ei=0.0;
#endif
Cjep=0.0;
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjep_Vbp_ei=0.0;
Qjep_Vbi_ci=0.0;
Qjep_Vtnode_GND=0.0;
Qjep_Vbi_ei=0.0;
#endif
Qjep=0.0;
#endif
}
if
((Vbpei<0.0)||(Vbiei<0.0))
{
double pocce;
double czz;
#if defined(_DERIVATE)
double pocce_Vbp_ei;
#endif
#if defined(_DERIVATE)
double pocce_Vbi_ci;
#endif
#if defined(_DERIVATE)
double pocce_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double pocce_Vbi_ei;
#endif
#if defined(_DERIVATE)
double czz_Vbp_ei;
#endif
#if defined(_DERIVATE)
double czz_Vtnode_GND;
#endif
#if defined(_DERIVATE)
double czz_Vbi_ci;
#endif
#if defined(_DERIVATE)
double czz_Vbi_ei;
#endif
if
(((tunode==1)&&(cjep0_t>0.0))&&(vdep_t>0.0))
{
{
double m00_logE(d00_logE0,(Cjep/cjep0_t))
double m00_exp(d00_exp1,((1-(1/zep))*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(Cjep/cjep0_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
pocce_Vbp_ei=((1-(1/zep))*(Cjep_Vbp_ei/cjep0_t)*d10_logE0)*d10_exp1;
pocce_Vbi_ci=((1-(1/zep))*(Cjep_Vbi_ci/cjep0_t)*d10_logE0)*d10_exp1;
pocce_Vtnode_GND=((1-(1/zep))*((Cjep_Vtnode_GND*cjep0_t-Cjep*cjep0_t_Vtnode_GND)/cjep0_t/cjep0_t)*d10_logE0)*d10_exp1;
pocce_Vbi_ei=((1-(1/zep))*(Cjep_Vbi_ei/cjep0_t)*d10_logE0)*d10_exp1;
#endif
pocce=d00_exp1;
}
#if defined(_DERIVATE)
czz_Vbp_ei=(((-(Vbpei_Vbp_ei/vdep_t))*ibets_t*pocce)+(((-(Vbpei/vdep_t))*ibets_t)*pocce_Vbp_ei));
czz_Vtnode_GND=(((((-(-Vbpei*vdep_t_Vtnode_GND/vdep_t/vdep_t))*ibets_t)+((-(Vbpei/vdep_t))*ibets_t_Vtnode_GND))*pocce)+(((-(Vbpei/vdep_t))*ibets_t)*pocce_Vtnode_GND));
czz_Vbi_ci=(((-(Vbpei/vdep_t))*ibets_t)*pocce_Vbi_ci);
czz_Vbi_ei=(((-(Vbpei/vdep_t))*ibets_t)*pocce_Vbi_ei);
#endif
czz=(((-(Vbpei/vdep_t))*ibets_t)*pocce);
{
double m00_exp(d00_exp0,((-abet_t)/pocce))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibet_Vbp_ei=((czz_Vbp_ei*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbp_ei/pocce/pocce)*d10_exp0));
ibet_Vtnode_GND=((czz_Vtnode_GND*d00_exp0)+(czz*(((-abet_t_Vtnode_GND)*pocce-(-abet_t)*pocce_Vtnode_GND)/pocce/pocce)*d10_exp0));
ibet_Vbi_ci=((czz_Vbi_ci*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbi_ci/pocce/pocce)*d10_exp0));
ibet_Vbi_ei=((czz_Vbi_ei*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbi_ei/pocce/pocce)*d10_exp0));
#endif
ibet=(czz*d00_exp0);
}
}
else
{
if
(((tunode==0)&&(cjei0_t>0.0))&&(vdei_t>0.0))
{
{
double m00_logE(d00_logE0,(Cjei/cjei0_t))
double m00_exp(d00_exp1,((1-(1/zei))*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(Cjei/cjei0_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
pocce_Vbp_ei=0.0;
pocce_Vbi_ci=0.0;
pocce_Vtnode_GND=((1-(1/zei))*((Cjei_Vtnode_GND*cjei0_t-Cjei*cjei0_t_Vtnode_GND)/cjei0_t/cjei0_t)*d10_logE0)*d10_exp1;
pocce_Vbi_ei=((1-(1/zei))*(Cjei_Vbi_ei/cjei0_t)*d10_logE0)*d10_exp1;
#endif
pocce=d00_exp1;
}
#if defined(_DERIVATE)
czz_Vbp_ei=(((-(Vbiei/vdei_t))*ibets_t)*pocce_Vbp_ei);
czz_Vtnode_GND=(((((-(-Vbiei*vdei_t_Vtnode_GND/vdei_t/vdei_t))*ibets_t)+((-(Vbiei/vdei_t))*ibets_t_Vtnode_GND))*pocce)+(((-(Vbiei/vdei_t))*ibets_t)*pocce_Vtnode_GND));
czz_Vbi_ci=(((-(Vbiei/vdei_t))*ibets_t)*pocce_Vbi_ci);
czz_Vbi_ei=(((-(Vbiei_Vbi_ei/vdei_t))*ibets_t*pocce)+(((-(Vbiei/vdei_t))*ibets_t)*pocce_Vbi_ei));
#endif
czz=(((-(Vbiei/vdei_t))*ibets_t)*pocce);
{
double m00_exp(d00_exp0,((-abet_t)/pocce))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
ibet_Vbp_ei=((czz_Vbp_ei*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbp_ei/pocce/pocce)*d10_exp0));
ibet_Vtnode_GND=((czz_Vtnode_GND*d00_exp0)+(czz*(((-abet_t_Vtnode_GND)*pocce-(-abet_t)*pocce_Vtnode_GND)/pocce/pocce)*d10_exp0));
ibet_Vbi_ci=((czz_Vbi_ci*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbi_ci/pocce/pocce)*d10_exp0));
ibet_Vbi_ei=((czz_Vbi_ei*d00_exp0)+(czz*(-(-abet_t)*pocce_Vbi_ei/pocce/pocce)*d10_exp0));
#endif
ibet=(czz*d00_exp0);
}
}
else
{
#if defined(_DERIVATE)
ibet_Vbp_ei=0.0;
ibet_Vtnode_GND=0.0;
ibet_Vbi_ci=0.0;
ibet_Vbi_ei=0.0;
#endif
ibet=0.0;
}
}
}
else
{
#if defined(_DERIVATE)
ibet_Vbp_ei=0.0;
ibet_Vtnode_GND=0.0;
ibet_Vbi_ci=0.0;
ibet_Vbi_ei=0.0;
#endif
ibet=0.0;
}
#ifdef _DYNAMIC
if
(vptcx_t<1.0e2)
{
#ifdef _DYNAMIC
if
(cjcx02_t>0.0)
{
#if defined(_DYNAMIC)
Dz_r=(zcx/4.0);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_p_Vtnode_GND=(vptcx_t_Vtnode_GND-vdcx_t_Vtnode_GND);
#endif
Dv_p=(vptcx_t-vdcx_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zcx))
#if defined(_DERIVATE)
DV_f_Vtnode_GND=vdcx_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DV_f=(vdcx_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_max_Vtnode_GND=(2.4*cjcx02_t_Vtnode_GND);
#endif
DC_max=(2.4*cjcx02_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vptcx_t/vdcx_t))
double m00_exp(d00_exp1,((Dz_r-zcx)*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vptcx_t/vdcx_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DC_c_Vtnode_GND=((cjcx02_t_Vtnode_GND*d00_exp1)+(cjcx02_t*((Dz_r-zcx)*((vptcx_t_Vtnode_GND*vdcx_t-vptcx_t*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t)*d10_logE0)*d10_exp1));
#endif
DC_c=(cjcx02_t*d00_exp1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_e_Vsi_ci=0.0;
Dv_e_Vb_ci=0.0;
Dv_e_Vbp_ci=((-Vbpci_Vbp_ci)/VT);
Dv_e_Vtnode_GND=((DV_f_Vtnode_GND*VT-(DV_f-Vbpci)*VT_Vtnode_GND)/VT/VT);
Dv_e_Vbi_ci=0.0;
#endif
Dv_e=((DV_f-Vbpci)/VT);
#endif
#ifdef _DYNAMIC
if
(Dv_e<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_e)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=0.0;
De_Vbp_ci=Dv_e_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_e_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_e_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_1_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_1_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_1=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=0.0;
Dv_j1_Vbp_ci=(-(VT*De_Vbp_ci*d10_logE0));
Dv_j1_Vtnode_GND=(DV_f_Vtnode_GND-((VT_Vtnode_GND*d00_logE0)+(VT*De_Vtnode_GND*d10_logE0)));
Dv_j1_Vbi_ci=(-(VT*De_Vbi_ci*d10_logE0));
#endif
Dv_j1=(DV_f-(VT*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=0.0;
De_1_Vtnode_GND=0.0;
De_1_Vbi_ci=0.0;
#endif
De_1=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=0.0;
Dv_j1_Vbp_ci=Vbpci_Vbp_ci;
Dv_j1_Vtnode_GND=0.0;
Dv_j1_Vbi_ci=0.0;
#endif
Dv_j1=Vbpci;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Da_Vtnode_GND=((0.1*Dv_p_Vtnode_GND)+(4.0*VT_Vtnode_GND));
#endif
Da=((0.1*Dv_p)+(4.0*VT));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_r_Vsi_ci=0.0;
Dv_r_Vb_ci=0.0;
Dv_r_Vbp_ci=(Dv_j1_Vbp_ci/Da);
Dv_r_Vtnode_GND=(((Dv_p_Vtnode_GND+Dv_j1_Vtnode_GND)*Da-(Dv_p+Dv_j1)*Da_Vtnode_GND)/Da/Da);
Dv_r_Vbi_ci=(Dv_j1_Vbi_ci/Da);
#endif
Dv_r=((Dv_p+Dv_j1)/Da);
#endif
#ifdef _DYNAMIC
if
(Dv_r<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_r)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=0.0;
De_Vbp_ci=Dv_r_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_r_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_r_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_2_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_2_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_2=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=0.0;
Dv_j2_Vbp_ci=(Da*De_Vbp_ci*d10_logE0);
Dv_j2_Vtnode_GND=((-Dv_p_Vtnode_GND)+((Da_Vtnode_GND*d00_logE0)+(Da*De_Vtnode_GND*d10_logE0)));
Dv_j2_Vbi_ci=(Da*De_Vbi_ci*d10_logE0);
#endif
Dv_j2=((-Dv_p)+(Da*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=0.0;
De_2_Vtnode_GND=0.0;
De_2_Vbi_ci=0.0;
#endif
De_2=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=0.0;
Dv_j2_Vbp_ci=Dv_j1_Vbp_ci;
Dv_j2_Vtnode_GND=Dv_j1_Vtnode_GND;
Dv_j2_Vbi_ci=Dv_j1_Vbi_ci;
#endif
Dv_j2=Dv_j1;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j4_Vsi_ci=0.0;
Dv_j4_Vb_ci=0.0;
Dv_j4_Vbp_ci=(Vbpci_Vbp_ci-Dv_j1_Vbp_ci);
Dv_j4_Vbi_ci=(-Dv_j1_Vbi_ci);
Dv_j4_Vtnode_GND=(-Dv_j1_Vtnode_GND);
#endif
Dv_j4=(Vbpci-Dv_j1);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j1/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j1/vdcx_t)))
#endif
#if defined(_DERIVATE)
DCln1_Vsi_ci=0.0;
DCln1_Vb_ci=0.0;
DCln1_Vbp_ci=(-(Dv_j1_Vbp_ci/vdcx_t))*d10_logE0;
DCln1_Vtnode_GND=(-((Dv_j1_Vtnode_GND*vdcx_t-Dv_j1*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DCln1_Vbi_ci=(-(Dv_j1_Vbi_ci/vdcx_t))*d10_logE0;
#endif
DCln1=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j2/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j2/vdcx_t)))
#endif
#if defined(_DERIVATE)
DCln2_Vsi_ci=0.0;
DCln2_Vb_ci=0.0;
DCln2_Vbp_ci=(-(Dv_j2_Vbp_ci/vdcx_t))*d10_logE0;
DCln2_Vtnode_GND=(-((Dv_j2_Vtnode_GND*vdcx_t-Dv_j2*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DCln2_Vbi_ci=(-(Dv_j2_Vbi_ci/vdcx_t))*d10_logE0;
#endif
DCln2=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
Dz1=(1.0-zcx);
#endif
#if defined(_DYNAMIC)
Dzr1=(1.0-Dz_r);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*(-zcx)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j1_Vsi_ci=0.0;
DC_j1_Vb_ci=0.0;
DC_j1_Vbp_ci=(((((cjcx02_t*DCln2_Vbp_ci*(-zcx)*d10_exp0)*De_1)+((cjcx02_t*d00_exp0)*De_1_Vbp_ci))*De_2)+(((cjcx02_t*d00_exp0)*De_1)*De_2_Vbp_ci));
DC_j1_Vtnode_GND=((((((cjcx02_t_Vtnode_GND*d00_exp0)+(cjcx02_t*DCln2_Vtnode_GND*(-zcx)*d10_exp0))*De_1)+((cjcx02_t*d00_exp0)*De_1_Vtnode_GND))*De_2)+(((cjcx02_t*d00_exp0)*De_1)*De_2_Vtnode_GND));
DC_j1_Vbi_ci=(((((cjcx02_t*DCln2_Vbi_ci*(-zcx)*d10_exp0)*De_1)+((cjcx02_t*d00_exp0)*De_1_Vbi_ci))*De_2)+(((cjcx02_t*d00_exp0)*De_1)*De_2_Vbi_ci));
#endif
DC_j1=(((cjcx02_t*d00_exp0)*De_1)*De_2);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*(-Dz_r)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j2_Vsi_ci=0.0;
DC_j2_Vb_ci=0.0;
DC_j2_Vbp_ci=(((DC_c*DCln1_Vbp_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbp_ci)));
DC_j2_Vtnode_GND=((((DC_c_Vtnode_GND*d00_exp0)+(DC_c*DCln1_Vtnode_GND*(-Dz_r)*d10_exp0))*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vtnode_GND)));
DC_j2_Vbi_ci=(((DC_c*DCln1_Vbi_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbi_ci)));
#endif
DC_j2=((DC_c*d00_exp0)*(1.0-De_2));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_j3_Vsi_ci=0.0;
DC_j3_Vb_ci=0.0;
DC_j3_Vbp_ci=(DC_max*(-De_1_Vbp_ci));
DC_j3_Vtnode_GND=((DC_max_Vtnode_GND*(1.0-De_1))+(DC_max*(-De_1_Vtnode_GND)));
DC_j3_Vbi_ci=(DC_max*(-De_1_Vbi_ci));
#endif
DC_j3=(DC_max*(1.0-De_1));
#endif
#if defined(_DYNAMIC)
c_dummy=((DC_j1+DC_j2)+DC_j3);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dz1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j1_Vsi_ci=0.0;
DQ_j1_Vb_ci=0.0;
DQ_j1_Vbp_ci=((cjcx02_t*(-DCln2_Vbp_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vtnode_GND=(((cjcx02_t_Vtnode_GND*(1.0-d00_exp0))+(cjcx02_t*(-DCln2_Vtnode_GND*Dz1*d10_exp0)))/Dz1);
DQ_j1_Vbi_ci=((cjcx02_t*(-DCln2_Vbi_ci*Dz1*d10_exp0))/Dz1);
#endif
DQ_j1=((cjcx02_t*(1.0-d00_exp0))/Dz1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j2_Vsi_ci=0.0;
DQ_j2_Vb_ci=0.0;
DQ_j2_Vbp_ci=((DC_c*(-DCln1_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln1_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j2_Vbi_ci=((DC_c*(-DCln1_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j2=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j3_Vsi_ci=0.0;
DQ_j3_Vb_ci=0.0;
DQ_j3_Vbp_ci=((DC_c*(-DCln2_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln2_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j3_Vbi_ci=((DC_c*(-DCln2_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j3=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_ii_Vbp_ei=0.0;
qjcx0_t_ii_Vbi_ei=0.0;
qjcx0_t_ii_Vbp_ci=(((DQ_j1_Vbp_ci+DQ_j2_Vbp_ci)-DQ_j3_Vbp_ci)*vdcx_t+(DC_max*Dv_j4_Vbp_ci));
qjcx0_t_ii_Vtnode_GND=(((((DQ_j1_Vtnode_GND+DQ_j2_Vtnode_GND)-DQ_j3_Vtnode_GND)*vdcx_t)+(((DQ_j1+DQ_j2)-DQ_j3)*vdcx_t_Vtnode_GND))+((DC_max_Vtnode_GND*Dv_j4)+(DC_max*Dv_j4_Vtnode_GND)));
qjcx0_t_ii_Vbi_ci=(((DQ_j1_Vbi_ci+DQ_j2_Vbi_ci)-DQ_j3_Vbi_ci)*vdcx_t+(DC_max*Dv_j4_Vbi_ci));
#endif
qjcx0_t_ii=((((DQ_j1+DQ_j2)-DQ_j3)*vdcx_t)+(DC_max*Dv_j4));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_ii_Vbp_ei=0.0;
qjcx0_t_ii_Vbi_ei=0.0;
qjcx0_t_ii_Vbp_ci=0.0;
qjcx0_t_ii_Vtnode_GND=0.0;
qjcx0_t_ii_Vbi_ci=0.0;
#endif
qjcx0_t_ii=0.0;
#endif
}
#endif /* if (...) */
}
else
{
#ifdef _DYNAMIC
if
(cjcx02_t>0.0)
{
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zcx))
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=vdcx_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DFV_f=(vdcx_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_e_Vsi_ci=0.0;
DFv_e_Vb_ci=0.0;
DFv_e_Vbp_ci=((-Vbpci_Vbp_ci)/VT);
DFv_e_Vbp_ei=0.0;
DFv_e_Vbi_ci=0.0;
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vbpci)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=0.0;
#endif
DFv_e=((DFV_f-Vbpci)/VT);
#endif
#if defined(_DYNAMIC)
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=0.0;
DFs_q_Vb_ci=0.0;
DFs_q_Vbp_ci=((DFv_e_Vbp_ci*DFv_e)+(DFv_e*DFv_e_Vbp_ci))*d10_sqrt0;
DFs_q_Vbp_ei=((DFv_e_Vbp_ei*DFv_e)+(DFv_e*DFv_e_Vbp_ei))*d10_sqrt0;
DFs_q_Vbi_ci=((DFv_e_Vbi_ci*DFv_e)+(DFv_e*DFv_e_Vbi_ci))*d10_sqrt0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=0.0;
DFs_q2_Vb_ci=0.0;
DFs_q2_Vbp_ci=(DFv_e_Vbp_ci+DFs_q_Vbp_ci)*0.5;
DFs_q2_Vbp_ei=(DFv_e_Vbp_ei+DFs_q_Vbp_ei)*0.5;
DFs_q2_Vbi_ci=(DFv_e_Vbi_ci+DFs_q_Vbi_ci)*0.5;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_j_Vsi_ci=0.0;
DFv_j_Vb_ci=0.0;
DFv_j_Vbp_ci=(-(VT*DFs_q2_Vbp_ci));
DFv_j_Vbp_ei=(-(VT*DFs_q2_Vbp_ei));
DFv_j_Vbi_ci=(-(VT*DFs_q2_Vbi_ci));
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=0.0;
DFdvj_dv_Vb_ci=0.0;
DFdvj_dv_Vbp_ci=((DFs_q2_Vbp_ci*DFs_q-DFs_q2*DFs_q_Vbp_ci)/DFs_q/DFs_q);
DFdvj_dv_Vbp_ei=((DFs_q2_Vbp_ei*DFs_q-DFs_q2*DFs_q_Vbp_ei)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ci=((DFs_q2_Vbi_ci*DFs_q-DFs_q2*DFs_q_Vbi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vdcx_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=0.0;
DFb_Vb_ci=0.0;
DFb_Vbp_ci=(-(DFv_j_Vbp_ci/vdcx_t))*d10_logE0;
DFb_Vbp_ei=(-(DFv_j_Vbp_ei/vdcx_t))*d10_logE0;
DFb_Vbi_ci=(-(DFv_j_Vbi_ci/vdcx_t))*d10_logE0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vdcx_t-DFv_j*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vdcx_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((-zcx)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=0.0;
DFC_j1_Vb_ci=0.0;
DFC_j1_Vbp_ci=(((cjcx02_t*((-zcx)*DFb_Vbp_ci)*d10_exp0)*DFdvj_dv)+((cjcx02_t*d00_exp0)*DFdvj_dv_Vbp_ci));
DFC_j1_Vbp_ei=(((cjcx02_t*((-zcx)*DFb_Vbp_ei)*d10_exp0)*DFdvj_dv)+((cjcx02_t*d00_exp0)*DFdvj_dv_Vbp_ei));
DFC_j1_Vbi_ci=(((cjcx02_t*((-zcx)*DFb_Vbi_ci)*d10_exp0)*DFdvj_dv)+((cjcx02_t*d00_exp0)*DFdvj_dv_Vbi_ci));
DFC_j1_Vtnode_GND=((((cjcx02_t_Vtnode_GND*d00_exp0)+(cjcx02_t*((-zcx)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjcx02_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjcx02_t*((-zcx)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjcx02_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjcx02_t*d00_exp0)*DFdvj_dv);
}
#endif
#if defined(_DYNAMIC)
c_dummy=(DFC_j1+((2.4*cjcx02_t)*(1.0-DFdvj_dv)));
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DFb*(1.0-zcx)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=0.0;
DFQ_j_Vb_ci=0.0;
DFQ_j_Vbp_ci=(((cjcx02_t*vdcx_t)*(-DFb_Vbp_ci*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vbp_ei=(((cjcx02_t*vdcx_t)*(-DFb_Vbp_ei*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vbi_ci=(((cjcx02_t*vdcx_t)*(-DFb_Vbi_ci*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vtnode_GND=(((((cjcx02_t_Vtnode_GND*vdcx_t)+(cjcx02_t*vdcx_t_Vtnode_GND))*(1.0-d00_exp0))+((cjcx02_t*vdcx_t)*(-DFb_Vtnode_GND*(1.0-zcx)*d10_exp0)))/(1.0-zcx));
DFQ_j_Vbi_ei=(((cjcx02_t*vdcx_t)*(-DFb_Vbi_ei*(1.0-zcx)*d10_exp0))/(1.0-zcx));
#endif
DFQ_j=(((cjcx02_t*vdcx_t)*(1.0-d00_exp0))/(1.0-zcx));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_ii_Vbp_ei=(DFQ_j_Vbp_ei+((2.4*cjcx02_t)*(-DFv_j_Vbp_ei)));
qjcx0_t_ii_Vbi_ei=(DFQ_j_Vbi_ei+((2.4*cjcx02_t)*(-DFv_j_Vbi_ei)));
qjcx0_t_ii_Vbp_ci=(DFQ_j_Vbp_ci+((2.4*cjcx02_t)*(Vbpci_Vbp_ci-DFv_j_Vbp_ci)));
qjcx0_t_ii_Vtnode_GND=(DFQ_j_Vtnode_GND+(((2.4*cjcx02_t_Vtnode_GND)*(Vbpci-DFv_j))+((2.4*cjcx02_t)*(-DFv_j_Vtnode_GND))));
qjcx0_t_ii_Vbi_ci=(DFQ_j_Vbi_ci+((2.4*cjcx02_t)*(-DFv_j_Vbi_ci)));
#endif
qjcx0_t_ii=(DFQ_j+((2.4*cjcx02_t)*(Vbpci-DFv_j)));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_ii_Vbp_ei=0.0;
qjcx0_t_ii_Vbi_ei=0.0;
qjcx0_t_ii_Vbp_ci=0.0;
qjcx0_t_ii_Vtnode_GND=0.0;
qjcx0_t_ii_Vbi_ci=0.0;
#endif
qjcx0_t_ii=0.0;
#endif
}
#endif /* if (...) */
}
#endif /* if (...) */
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=(Vbpci_Vbp_ci/(mbcx*VT));
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=(-Vbpci*(mbcx*VT_Vtnode_GND)/(mbcx*VT)/(mbcx*VT));
#endif
DIOY=(Vbpci/(mbcx*VT));
if
(ibcxs>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=DIOY_Vbp_ci;
le_Vbp_ei=DIOY_Vbp_ei;
le_Vbi_ci=DIOY_Vbi_ci;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=((le_Vbp_ci*d00_limexp0)+(le*DIOY_Vbp_ci*d10_limexp0));
le_Vbp_ei=((le_Vbp_ei*d00_limexp0)+(le*DIOY_Vbp_ei*d10_limexp0));
le_Vbi_ci=((le_Vbi_ci*d00_limexp0)+(le*DIOY_Vbi_ci*d10_limexp0));
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
ijbcx_Vtnode_GND=((ibcxs_t_Vtnode_GND*(le-1.0))+(ibcxs_t*le_Vtnode_GND));
ijbcx_Vbp_ci=(ibcxs_t*le_Vbp_ci);
ijbcx_Vbp_ei=(ibcxs_t*le_Vbp_ei);
ijbcx_Vbi_ci=(ibcxs_t*le_Vbi_ci);
ijbcx_Vbi_ei=(ibcxs_t*le_Vbi_ei);
#endif
ijbcx=(ibcxs_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
ijbcx_Vtnode_GND=(-ibcxs_t_Vtnode_GND);
ijbcx_Vbp_ci=0.0;
ijbcx_Vbp_ei=0.0;
ijbcx_Vbi_ci=0.0;
ijbcx_Vbi_ei=0.0;
#endif
ijbcx=(-ibcxs_t);
}
}
else
{
#if defined(_DERIVATE)
ijbcx_Vtnode_GND=0.0;
ijbcx_Vbp_ci=0.0;
ijbcx_Vbp_ei=0.0;
ijbcx_Vbi_ci=0.0;
ijbcx_Vbi_ei=0.0;
#endif
ijbcx=0.0;
}
#ifdef _DYNAMIC
if
(vptcx_t<1.0e2)
{
#ifdef _DYNAMIC
if
(cjcx01_t>0.0)
{
#if defined(_DYNAMIC)
Dz_r=(zcx/4.0);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_p_Vtnode_GND=(vptcx_t_Vtnode_GND-vdcx_t_Vtnode_GND);
#endif
Dv_p=(vptcx_t-vdcx_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zcx))
#if defined(_DERIVATE)
DV_f_Vtnode_GND=vdcx_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DV_f=(vdcx_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_max_Vtnode_GND=(2.4*cjcx01_t_Vtnode_GND);
#endif
DC_max=(2.4*cjcx01_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vptcx_t/vdcx_t))
double m00_exp(d00_exp1,((Dz_r-zcx)*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vptcx_t/vdcx_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DC_c_Vtnode_GND=((cjcx01_t_Vtnode_GND*d00_exp1)+(cjcx01_t*((Dz_r-zcx)*((vptcx_t_Vtnode_GND*vdcx_t-vptcx_t*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t)*d10_logE0)*d10_exp1));
#endif
DC_c=(cjcx01_t*d00_exp1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_e_Vsi_ci=0.0;
Dv_e_Vb_ci=((-Vbci_Vb_ci)/VT);
Dv_e_Vbp_ci=0.0;
Dv_e_Vtnode_GND=((DV_f_Vtnode_GND*VT-(DV_f-Vbci)*VT_Vtnode_GND)/VT/VT);
Dv_e_Vbi_ci=0.0;
#endif
Dv_e=((DV_f-Vbci)/VT);
#endif
#ifdef _DYNAMIC
if
(Dv_e<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_e)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=Dv_e_Vb_ci*d10_exp0;
De_Vbp_ci=Dv_e_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_e_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_e_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=((De_Vb_ci*(1.0+De)-De*De_Vb_ci)/(1.0+De)/(1.0+De));
De_1_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_1_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_1_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_1=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=(-(VT*De_Vb_ci*d10_logE0));
Dv_j1_Vbp_ci=(-(VT*De_Vbp_ci*d10_logE0));
Dv_j1_Vtnode_GND=(DV_f_Vtnode_GND-((VT_Vtnode_GND*d00_logE0)+(VT*De_Vtnode_GND*d10_logE0)));
Dv_j1_Vbi_ci=(-(VT*De_Vbi_ci*d10_logE0));
#endif
Dv_j1=(DV_f-(VT*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=0.0;
De_1_Vtnode_GND=0.0;
De_1_Vbi_ci=0.0;
#endif
De_1=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=0.0;
Dv_j1_Vb_ci=Vbci_Vb_ci;
Dv_j1_Vbp_ci=0.0;
Dv_j1_Vtnode_GND=0.0;
Dv_j1_Vbi_ci=0.0;
#endif
Dv_j1=Vbci;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Da_Vtnode_GND=((0.1*Dv_p_Vtnode_GND)+(4.0*VT_Vtnode_GND));
#endif
Da=((0.1*Dv_p)+(4.0*VT));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_r_Vsi_ci=0.0;
Dv_r_Vb_ci=(Dv_j1_Vb_ci/Da);
Dv_r_Vbp_ci=(Dv_j1_Vbp_ci/Da);
Dv_r_Vtnode_GND=(((Dv_p_Vtnode_GND+Dv_j1_Vtnode_GND)*Da-(Dv_p+Dv_j1)*Da_Vtnode_GND)/Da/Da);
Dv_r_Vbi_ci=(Dv_j1_Vbi_ci/Da);
#endif
Dv_r=((Dv_p+Dv_j1)/Da);
#endif
#ifdef _DYNAMIC
if
(Dv_r<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_r)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=0.0;
De_Vb_ci=Dv_r_Vb_ci*d10_exp0;
De_Vbp_ci=Dv_r_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_r_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_r_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=((De_Vb_ci*(1.0+De)-De*De_Vb_ci)/(1.0+De)/(1.0+De));
De_2_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_2_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_2_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_2=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=(Da*De_Vb_ci*d10_logE0);
Dv_j2_Vbp_ci=(Da*De_Vbp_ci*d10_logE0);
Dv_j2_Vtnode_GND=((-Dv_p_Vtnode_GND)+((Da_Vtnode_GND*d00_logE0)+(Da*De_Vtnode_GND*d10_logE0)));
Dv_j2_Vbi_ci=(Da*De_Vbi_ci*d10_logE0);
#endif
Dv_j2=((-Dv_p)+(Da*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=0.0;
De_2_Vtnode_GND=0.0;
De_2_Vbi_ci=0.0;
#endif
De_2=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=0.0;
Dv_j2_Vb_ci=Dv_j1_Vb_ci;
Dv_j2_Vbp_ci=Dv_j1_Vbp_ci;
Dv_j2_Vtnode_GND=Dv_j1_Vtnode_GND;
Dv_j2_Vbi_ci=Dv_j1_Vbi_ci;
#endif
Dv_j2=Dv_j1;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j4_Vsi_ci=0.0;
Dv_j4_Vb_ci=(Vbci_Vb_ci-Dv_j1_Vb_ci);
Dv_j4_Vbp_ci=(-Dv_j1_Vbp_ci);
Dv_j4_Vbi_ci=(-Dv_j1_Vbi_ci);
Dv_j4_Vtnode_GND=(-Dv_j1_Vtnode_GND);
#endif
Dv_j4=(Vbci-Dv_j1);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j1/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j1/vdcx_t)))
#endif
#if defined(_DERIVATE)
DCln1_Vsi_ci=0.0;
DCln1_Vb_ci=(-(Dv_j1_Vb_ci/vdcx_t))*d10_logE0;
DCln1_Vbp_ci=(-(Dv_j1_Vbp_ci/vdcx_t))*d10_logE0;
DCln1_Vtnode_GND=(-((Dv_j1_Vtnode_GND*vdcx_t-Dv_j1*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DCln1_Vbi_ci=(-(Dv_j1_Vbi_ci/vdcx_t))*d10_logE0;
#endif
DCln1=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j2/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j2/vdcx_t)))
#endif
#if defined(_DERIVATE)
DCln2_Vsi_ci=0.0;
DCln2_Vb_ci=(-(Dv_j2_Vb_ci/vdcx_t))*d10_logE0;
DCln2_Vbp_ci=(-(Dv_j2_Vbp_ci/vdcx_t))*d10_logE0;
DCln2_Vtnode_GND=(-((Dv_j2_Vtnode_GND*vdcx_t-Dv_j2*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DCln2_Vbi_ci=(-(Dv_j2_Vbi_ci/vdcx_t))*d10_logE0;
#endif
DCln2=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
Dz1=(1.0-zcx);
#endif
#if defined(_DYNAMIC)
Dzr1=(1.0-Dz_r);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*(-zcx)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j1_Vsi_ci=0.0;
DC_j1_Vb_ci=(((((cjcx01_t*DCln2_Vb_ci*(-zcx)*d10_exp0)*De_1)+((cjcx01_t*d00_exp0)*De_1_Vb_ci))*De_2)+(((cjcx01_t*d00_exp0)*De_1)*De_2_Vb_ci));
DC_j1_Vbp_ci=(((((cjcx01_t*DCln2_Vbp_ci*(-zcx)*d10_exp0)*De_1)+((cjcx01_t*d00_exp0)*De_1_Vbp_ci))*De_2)+(((cjcx01_t*d00_exp0)*De_1)*De_2_Vbp_ci));
DC_j1_Vtnode_GND=((((((cjcx01_t_Vtnode_GND*d00_exp0)+(cjcx01_t*DCln2_Vtnode_GND*(-zcx)*d10_exp0))*De_1)+((cjcx01_t*d00_exp0)*De_1_Vtnode_GND))*De_2)+(((cjcx01_t*d00_exp0)*De_1)*De_2_Vtnode_GND));
DC_j1_Vbi_ci=(((((cjcx01_t*DCln2_Vbi_ci*(-zcx)*d10_exp0)*De_1)+((cjcx01_t*d00_exp0)*De_1_Vbi_ci))*De_2)+(((cjcx01_t*d00_exp0)*De_1)*De_2_Vbi_ci));
#endif
DC_j1=(((cjcx01_t*d00_exp0)*De_1)*De_2);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*(-Dz_r)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j2_Vsi_ci=0.0;
DC_j2_Vb_ci=(((DC_c*DCln1_Vb_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vb_ci)));
DC_j2_Vbp_ci=(((DC_c*DCln1_Vbp_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbp_ci)));
DC_j2_Vtnode_GND=((((DC_c_Vtnode_GND*d00_exp0)+(DC_c*DCln1_Vtnode_GND*(-Dz_r)*d10_exp0))*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vtnode_GND)));
DC_j2_Vbi_ci=(((DC_c*DCln1_Vbi_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbi_ci)));
#endif
DC_j2=((DC_c*d00_exp0)*(1.0-De_2));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_j3_Vsi_ci=0.0;
DC_j3_Vb_ci=(DC_max*(-De_1_Vb_ci));
DC_j3_Vbp_ci=(DC_max*(-De_1_Vbp_ci));
DC_j3_Vtnode_GND=((DC_max_Vtnode_GND*(1.0-De_1))+(DC_max*(-De_1_Vtnode_GND)));
DC_j3_Vbi_ci=(DC_max*(-De_1_Vbi_ci));
#endif
DC_j3=(DC_max*(1.0-De_1));
#endif
#if defined(_DYNAMIC)
c_dummy=((DC_j1+DC_j2)+DC_j3);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dz1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j1_Vsi_ci=0.0;
DQ_j1_Vb_ci=((cjcx01_t*(-DCln2_Vb_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vbp_ci=((cjcx01_t*(-DCln2_Vbp_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vtnode_GND=(((cjcx01_t_Vtnode_GND*(1.0-d00_exp0))+(cjcx01_t*(-DCln2_Vtnode_GND*Dz1*d10_exp0)))/Dz1);
DQ_j1_Vbi_ci=((cjcx01_t*(-DCln2_Vbi_ci*Dz1*d10_exp0))/Dz1);
#endif
DQ_j1=((cjcx01_t*(1.0-d00_exp0))/Dz1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j2_Vsi_ci=0.0;
DQ_j2_Vb_ci=((DC_c*(-DCln1_Vb_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vbp_ci=((DC_c*(-DCln1_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln1_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j2_Vbi_ci=((DC_c*(-DCln1_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j2=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j3_Vsi_ci=0.0;
DQ_j3_Vb_ci=((DC_c*(-DCln2_Vb_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vbp_ci=((DC_c*(-DCln2_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln2_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j3_Vbi_ci=((DC_c*(-DCln2_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j3=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_i_Vbp_ei=0.0;
qjcx0_t_i_Vbi_ei=0.0;
qjcx0_t_i_Vb_ci=(((DQ_j1_Vb_ci+DQ_j2_Vb_ci)-DQ_j3_Vb_ci)*vdcx_t+(DC_max*Dv_j4_Vb_ci));
qjcx0_t_i_Vbp_ci=(((DQ_j1_Vbp_ci+DQ_j2_Vbp_ci)-DQ_j3_Vbp_ci)*vdcx_t+(DC_max*Dv_j4_Vbp_ci));
qjcx0_t_i_Vtnode_GND=(((((DQ_j1_Vtnode_GND+DQ_j2_Vtnode_GND)-DQ_j3_Vtnode_GND)*vdcx_t)+(((DQ_j1+DQ_j2)-DQ_j3)*vdcx_t_Vtnode_GND))+((DC_max_Vtnode_GND*Dv_j4)+(DC_max*Dv_j4_Vtnode_GND)));
qjcx0_t_i_Vbi_ci=(((DQ_j1_Vbi_ci+DQ_j2_Vbi_ci)-DQ_j3_Vbi_ci)*vdcx_t+(DC_max*Dv_j4_Vbi_ci));
#endif
qjcx0_t_i=((((DQ_j1+DQ_j2)-DQ_j3)*vdcx_t)+(DC_max*Dv_j4));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_i_Vbp_ei=0.0;
qjcx0_t_i_Vbi_ei=0.0;
qjcx0_t_i_Vb_ci=0.0;
qjcx0_t_i_Vbp_ci=0.0;
qjcx0_t_i_Vtnode_GND=0.0;
qjcx0_t_i_Vbi_ci=0.0;
#endif
qjcx0_t_i=0.0;
#endif
}
#endif /* if (...) */
}
else
{
#ifdef _DYNAMIC
if
(cjcx01_t>0.0)
{
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zcx))
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=vdcx_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DFV_f=(vdcx_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_e_Vsi_ci=0.0;
DFv_e_Vb_ci=((-Vbci_Vb_ci)/VT);
DFv_e_Vbp_ci=0.0;
DFv_e_Vbp_ei=0.0;
DFv_e_Vbi_ci=0.0;
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vbci)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=0.0;
#endif
DFv_e=((DFV_f-Vbci)/VT);
#endif
#if defined(_DYNAMIC)
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=0.0;
DFs_q_Vb_ci=((DFv_e_Vb_ci*DFv_e)+(DFv_e*DFv_e_Vb_ci))*d10_sqrt0;
DFs_q_Vbp_ci=((DFv_e_Vbp_ci*DFv_e)+(DFv_e*DFv_e_Vbp_ci))*d10_sqrt0;
DFs_q_Vbp_ei=((DFv_e_Vbp_ei*DFv_e)+(DFv_e*DFv_e_Vbp_ei))*d10_sqrt0;
DFs_q_Vbi_ci=((DFv_e_Vbi_ci*DFv_e)+(DFv_e*DFv_e_Vbi_ci))*d10_sqrt0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=0.0;
DFs_q2_Vb_ci=(DFv_e_Vb_ci+DFs_q_Vb_ci)*0.5;
DFs_q2_Vbp_ci=(DFv_e_Vbp_ci+DFs_q_Vbp_ci)*0.5;
DFs_q2_Vbp_ei=(DFv_e_Vbp_ei+DFs_q_Vbp_ei)*0.5;
DFs_q2_Vbi_ci=(DFv_e_Vbi_ci+DFs_q_Vbi_ci)*0.5;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_j_Vsi_ci=0.0;
DFv_j_Vb_ci=(-(VT*DFs_q2_Vb_ci));
DFv_j_Vbp_ci=(-(VT*DFs_q2_Vbp_ci));
DFv_j_Vbp_ei=(-(VT*DFs_q2_Vbp_ei));
DFv_j_Vbi_ci=(-(VT*DFs_q2_Vbi_ci));
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=0.0;
DFdvj_dv_Vb_ci=((DFs_q2_Vb_ci*DFs_q-DFs_q2*DFs_q_Vb_ci)/DFs_q/DFs_q);
DFdvj_dv_Vbp_ci=((DFs_q2_Vbp_ci*DFs_q-DFs_q2*DFs_q_Vbp_ci)/DFs_q/DFs_q);
DFdvj_dv_Vbp_ei=((DFs_q2_Vbp_ei*DFs_q-DFs_q2*DFs_q_Vbp_ei)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ci=((DFs_q2_Vbi_ci*DFs_q-DFs_q2*DFs_q_Vbi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vdcx_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vdcx_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=0.0;
DFb_Vb_ci=(-(DFv_j_Vb_ci/vdcx_t))*d10_logE0;
DFb_Vbp_ci=(-(DFv_j_Vbp_ci/vdcx_t))*d10_logE0;
DFb_Vbp_ei=(-(DFv_j_Vbp_ei/vdcx_t))*d10_logE0;
DFb_Vbi_ci=(-(DFv_j_Vbi_ci/vdcx_t))*d10_logE0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vdcx_t-DFv_j*vdcx_t_Vtnode_GND)/vdcx_t/vdcx_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vdcx_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((-zcx)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=0.0;
DFC_j1_Vb_ci=(((cjcx01_t*((-zcx)*DFb_Vb_ci)*d10_exp0)*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vb_ci));
DFC_j1_Vbp_ci=(((cjcx01_t*((-zcx)*DFb_Vbp_ci)*d10_exp0)*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vbp_ci));
DFC_j1_Vbp_ei=(((cjcx01_t*((-zcx)*DFb_Vbp_ei)*d10_exp0)*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vbp_ei));
DFC_j1_Vbi_ci=(((cjcx01_t*((-zcx)*DFb_Vbi_ci)*d10_exp0)*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vbi_ci));
DFC_j1_Vtnode_GND=((((cjcx01_t_Vtnode_GND*d00_exp0)+(cjcx01_t*((-zcx)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjcx01_t*((-zcx)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjcx01_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjcx01_t*d00_exp0)*DFdvj_dv);
}
#endif
#if defined(_DYNAMIC)
c_dummy=(DFC_j1+((2.4*cjcx01_t)*(1.0-DFdvj_dv)));
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DFb*(1.0-zcx)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=0.0;
DFQ_j_Vb_ci=(((cjcx01_t*vdcx_t)*(-DFb_Vb_ci*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vbp_ci=(((cjcx01_t*vdcx_t)*(-DFb_Vbp_ci*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vbp_ei=(((cjcx01_t*vdcx_t)*(-DFb_Vbp_ei*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vbi_ci=(((cjcx01_t*vdcx_t)*(-DFb_Vbi_ci*(1.0-zcx)*d10_exp0))/(1.0-zcx));
DFQ_j_Vtnode_GND=(((((cjcx01_t_Vtnode_GND*vdcx_t)+(cjcx01_t*vdcx_t_Vtnode_GND))*(1.0-d00_exp0))+((cjcx01_t*vdcx_t)*(-DFb_Vtnode_GND*(1.0-zcx)*d10_exp0)))/(1.0-zcx));
DFQ_j_Vbi_ei=(((cjcx01_t*vdcx_t)*(-DFb_Vbi_ei*(1.0-zcx)*d10_exp0))/(1.0-zcx));
#endif
DFQ_j=(((cjcx01_t*vdcx_t)*(1.0-d00_exp0))/(1.0-zcx));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_i_Vbp_ei=(DFQ_j_Vbp_ei+((2.4*cjcx01_t)*(-DFv_j_Vbp_ei)));
qjcx0_t_i_Vbi_ei=(DFQ_j_Vbi_ei+((2.4*cjcx01_t)*(-DFv_j_Vbi_ei)));
qjcx0_t_i_Vb_ci=(DFQ_j_Vb_ci+((2.4*cjcx01_t)*(Vbci_Vb_ci-DFv_j_Vb_ci)));
qjcx0_t_i_Vbp_ci=(DFQ_j_Vbp_ci+((2.4*cjcx01_t)*(-DFv_j_Vbp_ci)));
qjcx0_t_i_Vtnode_GND=(DFQ_j_Vtnode_GND+(((2.4*cjcx01_t_Vtnode_GND)*(Vbci-DFv_j))+((2.4*cjcx01_t)*(-DFv_j_Vtnode_GND))));
qjcx0_t_i_Vbi_ci=(DFQ_j_Vbi_ci+((2.4*cjcx01_t)*(-DFv_j_Vbi_ci)));
#endif
qjcx0_t_i=(DFQ_j+((2.4*cjcx01_t)*(Vbci-DFv_j)));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
qjcx0_t_i_Vbp_ei=0.0;
qjcx0_t_i_Vbi_ei=0.0;
qjcx0_t_i_Vb_ci=0.0;
qjcx0_t_i_Vbp_ci=0.0;
qjcx0_t_i_Vtnode_GND=0.0;
qjcx0_t_i_Vbi_ci=0.0;
#endif
qjcx0_t_i=0.0;
#endif
}
#endif /* if (...) */
}
#endif /* if (...) */
#ifdef _DYNAMIC
if
(vpts_t<1.0e2)
{
#ifdef _DYNAMIC
if
(cjs0_t>0.0)
{
#if defined(_DYNAMIC)
Dz_r=(zs/4.0);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_p_Vtnode_GND=(vpts_t_Vtnode_GND-vds_t_Vtnode_GND);
#endif
Dv_p=(vpts_t-vds_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zs))
#if defined(_DERIVATE)
DV_f_Vtnode_GND=vds_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DV_f=(vds_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_max_Vtnode_GND=(2.4*cjs0_t_Vtnode_GND);
#endif
DC_max=(2.4*cjs0_t);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(vpts_t/vds_t))
double m00_exp(d00_exp1,((Dz_r-zs)*d00_logE0))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(vpts_t/vds_t))
#define d10_exp1 d00_exp1
#endif
#if defined(_DERIVATE)
DC_c_Vtnode_GND=((cjs0_t_Vtnode_GND*d00_exp1)+(cjs0_t*((Dz_r-zs)*((vpts_t_Vtnode_GND*vds_t-vpts_t*vds_t_Vtnode_GND)/vds_t/vds_t)*d10_logE0)*d10_exp1));
#endif
DC_c=(cjs0_t*d00_exp1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_e_Vsi_ci=((-Vsici_Vsi_ci)/VT);
Dv_e_Vb_ci=0.0;
Dv_e_Vbp_ci=0.0;
Dv_e_Vtnode_GND=((DV_f_Vtnode_GND*VT-(DV_f-Vsici)*VT_Vtnode_GND)/VT/VT);
Dv_e_Vbi_ci=0.0;
#endif
Dv_e=((DV_f-Vsici)/VT);
#endif
#ifdef _DYNAMIC
if
(Dv_e<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_e)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=Dv_e_Vsi_ci*d10_exp0;
De_Vb_ci=Dv_e_Vb_ci*d10_exp0;
De_Vbp_ci=Dv_e_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_e_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_e_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=((De_Vsi_ci*(1.0+De)-De*De_Vsi_ci)/(1.0+De)/(1.0+De));
De_1_Vb_ci=((De_Vb_ci*(1.0+De)-De*De_Vb_ci)/(1.0+De)/(1.0+De));
De_1_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_1_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_1_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_1=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=(-(VT*De_Vsi_ci*d10_logE0));
Dv_j1_Vb_ci=(-(VT*De_Vb_ci*d10_logE0));
Dv_j1_Vbp_ci=(-(VT*De_Vbp_ci*d10_logE0));
Dv_j1_Vtnode_GND=(DV_f_Vtnode_GND-((VT_Vtnode_GND*d00_logE0)+(VT*De_Vtnode_GND*d10_logE0)));
Dv_j1_Vbi_ci=(-(VT*De_Vbi_ci*d10_logE0));
#endif
Dv_j1=(DV_f-(VT*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_1_Vsi_ci=0.0;
De_1_Vb_ci=0.0;
De_1_Vbp_ci=0.0;
De_1_Vtnode_GND=0.0;
De_1_Vbi_ci=0.0;
#endif
De_1=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j1_Vsi_ci=Vsici_Vsi_ci;
Dv_j1_Vb_ci=0.0;
Dv_j1_Vbp_ci=0.0;
Dv_j1_Vtnode_GND=0.0;
Dv_j1_Vbi_ci=0.0;
#endif
Dv_j1=Vsici;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Da_Vtnode_GND=((0.1*Dv_p_Vtnode_GND)+(4.0*VT_Vtnode_GND));
#endif
Da=((0.1*Dv_p)+(4.0*VT));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_r_Vsi_ci=(Dv_j1_Vsi_ci/Da);
Dv_r_Vb_ci=(Dv_j1_Vb_ci/Da);
Dv_r_Vbp_ci=(Dv_j1_Vbp_ci/Da);
Dv_r_Vtnode_GND=(((Dv_p_Vtnode_GND+Dv_j1_Vtnode_GND)*Da-(Dv_p+Dv_j1)*Da_Vtnode_GND)/Da/Da);
Dv_r_Vbi_ci=(Dv_j1_Vbi_ci/Da);
#endif
Dv_r=((Dv_p+Dv_j1)/Da);
#endif
#ifdef _DYNAMIC
if
(Dv_r<80.0)
{
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,Dv_r)
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
De_Vsi_ci=Dv_r_Vsi_ci*d10_exp0;
De_Vb_ci=Dv_r_Vb_ci*d10_exp0;
De_Vbp_ci=Dv_r_Vbp_ci*d10_exp0;
De_Vtnode_GND=Dv_r_Vtnode_GND*d10_exp0;
De_Vbi_ci=Dv_r_Vbi_ci*d10_exp0;
#endif
De=d00_exp0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=((De_Vsi_ci*(1.0+De)-De*De_Vsi_ci)/(1.0+De)/(1.0+De));
De_2_Vb_ci=((De_Vb_ci*(1.0+De)-De*De_Vb_ci)/(1.0+De)/(1.0+De));
De_2_Vbp_ci=((De_Vbp_ci*(1.0+De)-De*De_Vbp_ci)/(1.0+De)/(1.0+De));
De_2_Vtnode_GND=((De_Vtnode_GND*(1.0+De)-De*De_Vtnode_GND)/(1.0+De)/(1.0+De));
De_2_Vbi_ci=((De_Vbi_ci*(1.0+De)-De*De_Vbi_ci)/(1.0+De)/(1.0+De));
#endif
De_2=(De/(1.0+De));
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0+De))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0+De))
#endif
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=(Da*De_Vsi_ci*d10_logE0);
Dv_j2_Vb_ci=(Da*De_Vb_ci*d10_logE0);
Dv_j2_Vbp_ci=(Da*De_Vbp_ci*d10_logE0);
Dv_j2_Vtnode_GND=((-Dv_p_Vtnode_GND)+((Da_Vtnode_GND*d00_logE0)+(Da*De_Vtnode_GND*d10_logE0)));
Dv_j2_Vbi_ci=(Da*De_Vbi_ci*d10_logE0);
#endif
Dv_j2=((-Dv_p)+(Da*d00_logE0));
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
De_2_Vsi_ci=0.0;
De_2_Vb_ci=0.0;
De_2_Vbp_ci=0.0;
De_2_Vtnode_GND=0.0;
De_2_Vbi_ci=0.0;
#endif
De_2=1.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j2_Vsi_ci=Dv_j1_Vsi_ci;
Dv_j2_Vb_ci=Dv_j1_Vb_ci;
Dv_j2_Vbp_ci=Dv_j1_Vbp_ci;
Dv_j2_Vtnode_GND=Dv_j1_Vtnode_GND;
Dv_j2_Vbi_ci=Dv_j1_Vbi_ci;
#endif
Dv_j2=Dv_j1;
#endif
}
#endif /* if (...) */
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Dv_j4_Vsi_ci=(Vsici_Vsi_ci-Dv_j1_Vsi_ci);
Dv_j4_Vb_ci=(-Dv_j1_Vb_ci);
Dv_j4_Vbp_ci=(-Dv_j1_Vbp_ci);
Dv_j4_Vbi_ci=(-Dv_j1_Vbi_ci);
Dv_j4_Vtnode_GND=(-Dv_j1_Vtnode_GND);
#endif
Dv_j4=(Vsici-Dv_j1);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j1/vds_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j1/vds_t)))
#endif
#if defined(_DERIVATE)
DCln1_Vsi_ci=(-(Dv_j1_Vsi_ci/vds_t))*d10_logE0;
DCln1_Vb_ci=(-(Dv_j1_Vb_ci/vds_t))*d10_logE0;
DCln1_Vbp_ci=(-(Dv_j1_Vbp_ci/vds_t))*d10_logE0;
DCln1_Vtnode_GND=(-((Dv_j1_Vtnode_GND*vds_t-Dv_j1*vds_t_Vtnode_GND)/vds_t/vds_t))*d10_logE0;
DCln1_Vbi_ci=(-(Dv_j1_Vbi_ci/vds_t))*d10_logE0;
#endif
DCln1=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(Dv_j2/vds_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(Dv_j2/vds_t)))
#endif
#if defined(_DERIVATE)
DCln2_Vsi_ci=(-(Dv_j2_Vsi_ci/vds_t))*d10_logE0;
DCln2_Vb_ci=(-(Dv_j2_Vb_ci/vds_t))*d10_logE0;
DCln2_Vbp_ci=(-(Dv_j2_Vbp_ci/vds_t))*d10_logE0;
DCln2_Vtnode_GND=(-((Dv_j2_Vtnode_GND*vds_t-Dv_j2*vds_t_Vtnode_GND)/vds_t/vds_t))*d10_logE0;
DCln2_Vbi_ci=(-(Dv_j2_Vbi_ci/vds_t))*d10_logE0;
#endif
DCln2=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
Dz1=(1.0-zs);
#endif
#if defined(_DYNAMIC)
Dzr1=(1.0-Dz_r);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*(-zs)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j1_Vsi_ci=(((((cjs0_t*DCln2_Vsi_ci*(-zs)*d10_exp0)*De_1)+((cjs0_t*d00_exp0)*De_1_Vsi_ci))*De_2)+(((cjs0_t*d00_exp0)*De_1)*De_2_Vsi_ci));
DC_j1_Vb_ci=(((((cjs0_t*DCln2_Vb_ci*(-zs)*d10_exp0)*De_1)+((cjs0_t*d00_exp0)*De_1_Vb_ci))*De_2)+(((cjs0_t*d00_exp0)*De_1)*De_2_Vb_ci));
DC_j1_Vbp_ci=(((((cjs0_t*DCln2_Vbp_ci*(-zs)*d10_exp0)*De_1)+((cjs0_t*d00_exp0)*De_1_Vbp_ci))*De_2)+(((cjs0_t*d00_exp0)*De_1)*De_2_Vbp_ci));
DC_j1_Vtnode_GND=((((((cjs0_t_Vtnode_GND*d00_exp0)+(cjs0_t*DCln2_Vtnode_GND*(-zs)*d10_exp0))*De_1)+((cjs0_t*d00_exp0)*De_1_Vtnode_GND))*De_2)+(((cjs0_t*d00_exp0)*De_1)*De_2_Vtnode_GND));
DC_j1_Vbi_ci=(((((cjs0_t*DCln2_Vbi_ci*(-zs)*d10_exp0)*De_1)+((cjs0_t*d00_exp0)*De_1_Vbi_ci))*De_2)+(((cjs0_t*d00_exp0)*De_1)*De_2_Vbi_ci));
#endif
DC_j1=(((cjs0_t*d00_exp0)*De_1)*De_2);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*(-Dz_r)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DC_j2_Vsi_ci=(((DC_c*DCln1_Vsi_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vsi_ci)));
DC_j2_Vb_ci=(((DC_c*DCln1_Vb_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vb_ci)));
DC_j2_Vbp_ci=(((DC_c*DCln1_Vbp_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbp_ci)));
DC_j2_Vtnode_GND=((((DC_c_Vtnode_GND*d00_exp0)+(DC_c*DCln1_Vtnode_GND*(-Dz_r)*d10_exp0))*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vtnode_GND)));
DC_j2_Vbi_ci=(((DC_c*DCln1_Vbi_ci*(-Dz_r)*d10_exp0)*(1.0-De_2))+((DC_c*d00_exp0)*(-De_2_Vbi_ci)));
#endif
DC_j2=((DC_c*d00_exp0)*(1.0-De_2));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DC_j3_Vsi_ci=(DC_max*(-De_1_Vsi_ci));
DC_j3_Vb_ci=(DC_max*(-De_1_Vb_ci));
DC_j3_Vbp_ci=(DC_max*(-De_1_Vbp_ci));
DC_j3_Vtnode_GND=((DC_max_Vtnode_GND*(1.0-De_1))+(DC_max*(-De_1_Vtnode_GND)));
DC_j3_Vbi_ci=(DC_max*(-De_1_Vbi_ci));
#endif
DC_j3=(DC_max*(1.0-De_1));
#endif
#if defined(_DYNAMIC)
c_dummy=((DC_j1+DC_j2)+DC_j3);
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dz1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j1_Vsi_ci=((cjs0_t*(-DCln2_Vsi_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vb_ci=((cjs0_t*(-DCln2_Vb_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vbp_ci=((cjs0_t*(-DCln2_Vbp_ci*Dz1*d10_exp0))/Dz1);
DQ_j1_Vtnode_GND=(((cjs0_t_Vtnode_GND*(1.0-d00_exp0))+(cjs0_t*(-DCln2_Vtnode_GND*Dz1*d10_exp0)))/Dz1);
DQ_j1_Vbi_ci=((cjs0_t*(-DCln2_Vbi_ci*Dz1*d10_exp0))/Dz1);
#endif
DQ_j1=((cjs0_t*(1.0-d00_exp0))/Dz1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln1*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j2_Vsi_ci=((DC_c*(-DCln1_Vsi_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vb_ci=((DC_c*(-DCln1_Vb_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vbp_ci=((DC_c*(-DCln1_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j2_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln1_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j2_Vbi_ci=((DC_c*(-DCln1_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j2=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DCln2*Dzr1))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DQ_j3_Vsi_ci=((DC_c*(-DCln2_Vsi_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vb_ci=((DC_c*(-DCln2_Vb_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vbp_ci=((DC_c*(-DCln2_Vbp_ci*Dzr1*d10_exp0))/Dzr1);
DQ_j3_Vtnode_GND=(((DC_c_Vtnode_GND*(1.0-d00_exp0))+(DC_c*(-DCln2_Vtnode_GND*Dzr1*d10_exp0)))/Dzr1);
DQ_j3_Vbi_ci=((DC_c*(-DCln2_Vbi_ci*Dzr1*d10_exp0))/Dzr1);
#endif
DQ_j3=((DC_c*(1.0-d00_exp0))/Dzr1);
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjs_Vbp_ei=0.0;
Qjs_Vbi_ei=0.0;
Qjs_Vsi_ci=(((DQ_j1_Vsi_ci+DQ_j2_Vsi_ci)-DQ_j3_Vsi_ci)*vds_t+(DC_max*Dv_j4_Vsi_ci));
Qjs_Vb_ci=(((DQ_j1_Vb_ci+DQ_j2_Vb_ci)-DQ_j3_Vb_ci)*vds_t+(DC_max*Dv_j4_Vb_ci));
Qjs_Vbp_ci=(((DQ_j1_Vbp_ci+DQ_j2_Vbp_ci)-DQ_j3_Vbp_ci)*vds_t+(DC_max*Dv_j4_Vbp_ci));
Qjs_Vtnode_GND=(((((DQ_j1_Vtnode_GND+DQ_j2_Vtnode_GND)-DQ_j3_Vtnode_GND)*vds_t)+(((DQ_j1+DQ_j2)-DQ_j3)*vds_t_Vtnode_GND))+((DC_max_Vtnode_GND*Dv_j4)+(DC_max*Dv_j4_Vtnode_GND)));
Qjs_Vbi_ci=(((DQ_j1_Vbi_ci+DQ_j2_Vbi_ci)-DQ_j3_Vbi_ci)*vds_t+(DC_max*Dv_j4_Vbi_ci));
#endif
Qjs=((((DQ_j1+DQ_j2)-DQ_j3)*vds_t)+(DC_max*Dv_j4));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjs_Vbp_ei=0.0;
Qjs_Vbi_ei=0.0;
Qjs_Vsi_ci=0.0;
Qjs_Vb_ci=0.0;
Qjs_Vbp_ci=0.0;
Qjs_Vtnode_GND=0.0;
Qjs_Vbi_ci=0.0;
#endif
Qjs=0.0;
#endif
}
#endif /* if (...) */
}
else
{
#ifdef _DYNAMIC
if
(cjs0_t>0.0)
{
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,2.4)
double m00_exp(d00_exp1,((-d00_logE0)/zs))
#if defined(_DERIVATE)
DFV_f_Vtnode_GND=vds_t_Vtnode_GND*(1.0-d00_exp1);
#endif
DFV_f=(vds_t*(1.0-d00_exp1));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_e_Vsi_ci=((-Vsici_Vsi_ci)/VT);
DFv_e_Vb_ci=0.0;
DFv_e_Vbp_ci=0.0;
DFv_e_Vbp_ei=0.0;
DFv_e_Vbi_ci=0.0;
DFv_e_Vtnode_GND=((DFV_f_Vtnode_GND*VT-(DFV_f-Vsici)*VT_Vtnode_GND)/VT/VT);
DFv_e_Vbi_ei=0.0;
#endif
DFv_e=((DFV_f-Vsici)/VT);
#endif
#if defined(_DYNAMIC)
{
double m00_sqrt(d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,((DFv_e*DFv_e)+1.921812))
#endif
#if defined(_DERIVATE)
DFs_q_Vsi_ci=((DFv_e_Vsi_ci*DFv_e)+(DFv_e*DFv_e_Vsi_ci))*d10_sqrt0;
DFs_q_Vb_ci=((DFv_e_Vb_ci*DFv_e)+(DFv_e*DFv_e_Vb_ci))*d10_sqrt0;
DFs_q_Vbp_ci=((DFv_e_Vbp_ci*DFv_e)+(DFv_e*DFv_e_Vbp_ci))*d10_sqrt0;
DFs_q_Vbp_ei=((DFv_e_Vbp_ei*DFv_e)+(DFv_e*DFv_e_Vbp_ei))*d10_sqrt0;
DFs_q_Vbi_ci=((DFv_e_Vbi_ci*DFv_e)+(DFv_e*DFv_e_Vbi_ci))*d10_sqrt0;
DFs_q_Vtnode_GND=((DFv_e_Vtnode_GND*DFv_e)+(DFv_e*DFv_e_Vtnode_GND))*d10_sqrt0;
DFs_q_Vbi_ei=((DFv_e_Vbi_ei*DFv_e)+(DFv_e*DFv_e_Vbi_ei))*d10_sqrt0;
#endif
DFs_q=d00_sqrt0;
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFs_q2_Vsi_ci=(DFv_e_Vsi_ci+DFs_q_Vsi_ci)*0.5;
DFs_q2_Vb_ci=(DFv_e_Vb_ci+DFs_q_Vb_ci)*0.5;
DFs_q2_Vbp_ci=(DFv_e_Vbp_ci+DFs_q_Vbp_ci)*0.5;
DFs_q2_Vbp_ei=(DFv_e_Vbp_ei+DFs_q_Vbp_ei)*0.5;
DFs_q2_Vbi_ci=(DFv_e_Vbi_ci+DFs_q_Vbi_ci)*0.5;
DFs_q2_Vtnode_GND=(DFv_e_Vtnode_GND+DFs_q_Vtnode_GND)*0.5;
DFs_q2_Vbi_ei=(DFv_e_Vbi_ei+DFs_q_Vbi_ei)*0.5;
#endif
DFs_q2=((DFv_e+DFs_q)*0.5);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFv_j_Vsi_ci=(-(VT*DFs_q2_Vsi_ci));
DFv_j_Vb_ci=(-(VT*DFs_q2_Vb_ci));
DFv_j_Vbp_ci=(-(VT*DFs_q2_Vbp_ci));
DFv_j_Vbp_ei=(-(VT*DFs_q2_Vbp_ei));
DFv_j_Vbi_ci=(-(VT*DFs_q2_Vbi_ci));
DFv_j_Vtnode_GND=(DFV_f_Vtnode_GND-((VT_Vtnode_GND*DFs_q2)+(VT*DFs_q2_Vtnode_GND)));
DFv_j_Vbi_ei=(-(VT*DFs_q2_Vbi_ei));
#endif
DFv_j=(DFV_f-(VT*DFs_q2));
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
DFdvj_dv_Vsi_ci=((DFs_q2_Vsi_ci*DFs_q-DFs_q2*DFs_q_Vsi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vb_ci=((DFs_q2_Vb_ci*DFs_q-DFs_q2*DFs_q_Vb_ci)/DFs_q/DFs_q);
DFdvj_dv_Vbp_ci=((DFs_q2_Vbp_ci*DFs_q-DFs_q2*DFs_q_Vbp_ci)/DFs_q/DFs_q);
DFdvj_dv_Vbp_ei=((DFs_q2_Vbp_ei*DFs_q-DFs_q2*DFs_q_Vbp_ei)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ci=((DFs_q2_Vbi_ci*DFs_q-DFs_q2*DFs_q_Vbi_ci)/DFs_q/DFs_q);
DFdvj_dv_Vtnode_GND=((DFs_q2_Vtnode_GND*DFs_q-DFs_q2*DFs_q_Vtnode_GND)/DFs_q/DFs_q);
DFdvj_dv_Vbi_ei=((DFs_q2_Vbi_ei*DFs_q-DFs_q2*DFs_q_Vbi_ei)/DFs_q/DFs_q);
#endif
DFdvj_dv=(DFs_q2/DFs_q);
#endif
#if defined(_DYNAMIC)
{
double m00_logE(d00_logE0,(1.0-(DFv_j/vds_t)))
#if defined(_DERIVATE)
double m10_logE(d10_logE0,d00_logE0,(1.0-(DFv_j/vds_t)))
#endif
#if defined(_DERIVATE)
DFb_Vsi_ci=(-(DFv_j_Vsi_ci/vds_t))*d10_logE0;
DFb_Vb_ci=(-(DFv_j_Vb_ci/vds_t))*d10_logE0;
DFb_Vbp_ci=(-(DFv_j_Vbp_ci/vds_t))*d10_logE0;
DFb_Vbp_ei=(-(DFv_j_Vbp_ei/vds_t))*d10_logE0;
DFb_Vbi_ci=(-(DFv_j_Vbi_ci/vds_t))*d10_logE0;
DFb_Vtnode_GND=(-((DFv_j_Vtnode_GND*vds_t-DFv_j*vds_t_Vtnode_GND)/vds_t/vds_t))*d10_logE0;
DFb_Vbi_ei=(-(DFv_j_Vbi_ei/vds_t))*d10_logE0;
#endif
DFb=d00_logE0;
}
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,((-zs)*DFb))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFC_j1_Vsi_ci=(((cjs0_t*((-zs)*DFb_Vsi_ci)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vsi_ci));
DFC_j1_Vb_ci=(((cjs0_t*((-zs)*DFb_Vb_ci)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vb_ci));
DFC_j1_Vbp_ci=(((cjs0_t*((-zs)*DFb_Vbp_ci)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vbp_ci));
DFC_j1_Vbp_ei=(((cjs0_t*((-zs)*DFb_Vbp_ei)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vbp_ei));
DFC_j1_Vbi_ci=(((cjs0_t*((-zs)*DFb_Vbi_ci)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vbi_ci));
DFC_j1_Vtnode_GND=((((cjs0_t_Vtnode_GND*d00_exp0)+(cjs0_t*((-zs)*DFb_Vtnode_GND)*d10_exp0))*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vtnode_GND));
DFC_j1_Vbi_ei=(((cjs0_t*((-zs)*DFb_Vbi_ei)*d10_exp0)*DFdvj_dv)+((cjs0_t*d00_exp0)*DFdvj_dv_Vbi_ei));
#endif
DFC_j1=((cjs0_t*d00_exp0)*DFdvj_dv);
}
#endif
#if defined(_DYNAMIC)
c_dummy=(DFC_j1+((2.4*cjs0_t)*(1.0-DFdvj_dv)));
#endif
#if defined(_DYNAMIC)
{
double m00_exp(d00_exp0,(DFb*(1.0-zs)))
#if defined(_DERIVATE)
#define d10_exp0 d00_exp0
#endif
#if defined(_DERIVATE)
DFQ_j_Vsi_ci=(((cjs0_t*vds_t)*(-DFb_Vsi_ci*(1.0-zs)*d10_exp0))/(1.0-zs));
DFQ_j_Vb_ci=(((cjs0_t*vds_t)*(-DFb_Vb_ci*(1.0-zs)*d10_exp0))/(1.0-zs));
DFQ_j_Vbp_ci=(((cjs0_t*vds_t)*(-DFb_Vbp_ci*(1.0-zs)*d10_exp0))/(1.0-zs));
DFQ_j_Vbp_ei=(((cjs0_t*vds_t)*(-DFb_Vbp_ei*(1.0-zs)*d10_exp0))/(1.0-zs));
DFQ_j_Vbi_ci=(((cjs0_t*vds_t)*(-DFb_Vbi_ci*(1.0-zs)*d10_exp0))/(1.0-zs));
DFQ_j_Vtnode_GND=(((((cjs0_t_Vtnode_GND*vds_t)+(cjs0_t*vds_t_Vtnode_GND))*(1.0-d00_exp0))+((cjs0_t*vds_t)*(-DFb_Vtnode_GND*(1.0-zs)*d10_exp0)))/(1.0-zs));
DFQ_j_Vbi_ei=(((cjs0_t*vds_t)*(-DFb_Vbi_ei*(1.0-zs)*d10_exp0))/(1.0-zs));
#endif
DFQ_j=(((cjs0_t*vds_t)*(1.0-d00_exp0))/(1.0-zs));
}
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjs_Vbp_ei=(DFQ_j_Vbp_ei+((2.4*cjs0_t)*(-DFv_j_Vbp_ei)));
Qjs_Vbi_ei=(DFQ_j_Vbi_ei+((2.4*cjs0_t)*(-DFv_j_Vbi_ei)));
Qjs_Vsi_ci=(DFQ_j_Vsi_ci+((2.4*cjs0_t)*(Vsici_Vsi_ci-DFv_j_Vsi_ci)));
Qjs_Vb_ci=(DFQ_j_Vb_ci+((2.4*cjs0_t)*(-DFv_j_Vb_ci)));
Qjs_Vbp_ci=(DFQ_j_Vbp_ci+((2.4*cjs0_t)*(-DFv_j_Vbp_ci)));
Qjs_Vtnode_GND=(DFQ_j_Vtnode_GND+(((2.4*cjs0_t_Vtnode_GND)*(Vsici-DFv_j))+((2.4*cjs0_t)*(-DFv_j_Vtnode_GND))));
Qjs_Vbi_ci=(DFQ_j_Vbi_ci+((2.4*cjs0_t)*(-DFv_j_Vbi_ci)));
#endif
Qjs=(DFQ_j+((2.4*cjs0_t)*(Vsici-DFv_j)));
#endif
}
else
{
#if defined(_DYNAMIC)
c_dummy=0.0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qjs_Vbp_ei=0.0;
Qjs_Vbi_ei=0.0;
Qjs_Vsi_ci=0.0;
Qjs_Vb_ci=0.0;
Qjs_Vbp_ci=0.0;
Qjs_Vtnode_GND=0.0;
Qjs_Vbi_ci=0.0;
#endif
Qjs=0.0;
#endif
}
#endif /* if (...) */
}
#endif /* if (...) */
if
(itss>0.0)
{
#if defined(_DERIVATE)
HSUM_Vtnode_GND=(msf*VT_Vtnode_GND);
#endif
HSUM=(msf*VT);
{
double m00_limexp(d00_limexp0,(Vbpci/HSUM))
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,(Vbpci/HSUM))
#endif
#if defined(_DERIVATE)
HSa_Vbp_ci=(Vbpci_Vbp_ci/HSUM)*d10_limexp0;
HSa_Vtnode_GND=(-Vbpci*HSUM_Vtnode_GND/HSUM/HSUM)*d10_limexp0;
#endif
HSa=d00_limexp0;
}
{
double m00_limexp(d00_limexp0,(Vsici/HSUM))
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,(Vsici/HSUM))
#endif
#if defined(_DERIVATE)
HSb_Vsi_ci=(Vsici_Vsi_ci/HSUM)*d10_limexp0;
HSb_Vtnode_GND=(-Vsici*HSUM_Vtnode_GND/HSUM/HSUM)*d10_limexp0;
#endif
HSb=d00_limexp0;
}
#if defined(_DERIVATE)
HSI_Tsu_Vtnode_GND=((itss_t_Vtnode_GND*(HSa-HSb))+(itss_t*(HSa_Vtnode_GND-HSb_Vtnode_GND)));
HSI_Tsu_Vbp_ci=(itss_t*HSa_Vbp_ci);
HSI_Tsu_Vsi_ci=(itss_t*(-HSb_Vsi_ci));
#endif
HSI_Tsu=(itss_t*(HSa-HSb));
#ifdef _DYNAMIC
if
(tsf>0.0)
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdsu_Vtnode_GND=((((tsf_t_Vtnode_GND*itss_t)+(tsf_t*itss_t_Vtnode_GND))*HSa)+((tsf_t*itss_t)*HSa_Vtnode_GND));
Qdsu_Vbp_ci=((tsf_t*itss_t)*HSa_Vbp_ci);
#endif
Qdsu=((tsf_t*itss_t)*HSa);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdsu_Vtnode_GND=0.0;
Qdsu_Vbp_ci=0.0;
#endif
Qdsu=0.0;
#endif
}
#endif /* if (...) */
}
else
{
#if defined(_DERIVATE)
HSI_Tsu_Vtnode_GND=0.0;
HSI_Tsu_Vbp_ci=0.0;
HSI_Tsu_Vsi_ci=0.0;
#endif
HSI_Tsu=0.0;
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdsu_Vtnode_GND=0.0;
Qdsu_Vbp_ci=0.0;
#endif
Qdsu=0.0;
#endif
}
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
betad_Vtnode_GND=ibei_Vtnode_GND;
betad_Vbi_ei=ibei_Vbi_ei;
#endif
betad=ibei;
#endif
#ifdef _DYNAMIC
if
(betad>0.0)
{
#if defined(_DYNAMIC)
betadin=betad;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
betan_Vci_ei=it_Vci_ei;
betan_Vtnode_GND=it_Vtnode_GND;
betan_Vbi_ei=it_Vbi_ei;
betan_Vbi_ci=it_Vbi_ci;
#endif
betan=it;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
betadc_Vci_ei=(betan_Vci_ei/betad);
betadc_Vtnode_GND=((betan_Vtnode_GND*betad-betan*betad_Vtnode_GND)/betad/betad);
betadc_Vbi_ei=((betan_Vbi_ei*betad-betan*betad_Vbi_ei)/betad/betad);
betadc_Vbi_ci=(betan_Vbi_ci/betad);
#endif
betadc=(betan/betad);
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
betadc_Vci_ei=0.0;
betadc_Vtnode_GND=0.0;
betadc_Vbi_ei=0.0;
betadc_Vbi_ci=0.0;
#endif
betadc=0.0;
#endif
}
#endif /* if (...) */
#if defined(_DERIVATE)
DIOY_Vsi_ci=(Vsici_Vsi_ci/(msc*VT));
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=(-Vsici*(msc*VT_Vtnode_GND)/(msc*VT)/(msc*VT));
#endif
DIOY=(Vsici/(msc*VT));
if
(iscs>0.0)
{
if
(DIOY>80.0)
{
#if defined(_DERIVATE)
le_Vsi_ci=DIOY_Vsi_ci;
le_Vbp_ci=DIOY_Vbp_ci;
le_Vbp_ei=DIOY_Vbp_ei;
le_Vbi_ci=DIOY_Vbi_ci;
le_Vbi_ei=DIOY_Vbi_ei;
le_Vtnode_GND=DIOY_Vtnode_GND;
#endif
le=(1+(DIOY-80.0));
#if defined(_DERIVATE)
DIOY_Vsi_ci=0.0;
DIOY_Vbp_ci=0.0;
DIOY_Vbp_ei=0.0;
DIOY_Vbi_ci=0.0;
DIOY_Vbi_ei=0.0;
DIOY_Vtnode_GND=0.0;
#endif
DIOY=80.0;
}
else
{
#if defined(_DERIVATE)
le_Vsi_ci=0.0;
le_Vbp_ci=0.0;
le_Vbp_ei=0.0;
le_Vbi_ci=0.0;
le_Vbi_ei=0.0;
le_Vtnode_GND=0.0;
#endif
le=1;
}
{
double m00_limexp(d00_limexp0,DIOY)
#if defined(_DERIVATE)
double m10_limexp(d10_limexp0,d00_limexp0,DIOY)
#endif
#if defined(_DERIVATE)
le_Vsi_ci=((le_Vsi_ci*d00_limexp0)+(le*DIOY_Vsi_ci*d10_limexp0));
le_Vbp_ci=((le_Vbp_ci*d00_limexp0)+(le*DIOY_Vbp_ci*d10_limexp0));
le_Vbp_ei=((le_Vbp_ei*d00_limexp0)+(le*DIOY_Vbp_ei*d10_limexp0));
le_Vbi_ci=((le_Vbi_ci*d00_limexp0)+(le*DIOY_Vbi_ci*d10_limexp0));
le_Vbi_ei=((le_Vbi_ei*d00_limexp0)+(le*DIOY_Vbi_ei*d10_limexp0));
le_Vtnode_GND=((le_Vtnode_GND*d00_limexp0)+(le*DIOY_Vtnode_GND*d10_limexp0));
#endif
le=(le*d00_limexp0);
}
#if defined(_DERIVATE)
ijsc_Vtnode_GND=((iscs_t_Vtnode_GND*(le-1.0))+(iscs_t*le_Vtnode_GND));
ijsc_Vsi_ci=(iscs_t*le_Vsi_ci);
ijsc_Vbp_ci=(iscs_t*le_Vbp_ci);
ijsc_Vbp_ei=(iscs_t*le_Vbp_ei);
ijsc_Vbi_ci=(iscs_t*le_Vbi_ci);
ijsc_Vbi_ei=(iscs_t*le_Vbi_ei);
#endif
ijsc=(iscs_t*(le-1.0));
if
(DIOY<=(-14.0))
{
#if defined(_DERIVATE)
ijsc_Vtnode_GND=(-iscs_t_Vtnode_GND);
ijsc_Vsi_ci=0.0;
ijsc_Vbp_ci=0.0;
ijsc_Vbp_ei=0.0;
ijsc_Vbi_ci=0.0;
ijsc_Vbi_ei=0.0;
#endif
ijsc=(-iscs_t);
}
}
else
{
#if defined(_DERIVATE)
ijsc_Vtnode_GND=0.0;
ijsc_Vsi_ci=0.0;
ijsc_Vbp_ci=0.0;
ijsc_Vbp_ei=0.0;
ijsc_Vbi_ci=0.0;
ijsc_Vbi_ei=0.0;
#endif
ijsc=0.0;
}
if
((flsh==1)&&(rth>=0.001))
{
#if defined(_DERIVATE)
pterm_Vb_bp=0.0;
pterm_Vci_c=0.0;
pterm_Vei_e=0.0;
pterm_Vbp_bi=0.0;
pterm_Vbp_ei=0.0;
pterm_Vbp_ci=0.0;
pterm_Vsi_ci=0.0;
pterm_Vci_ei=(((Vciei_Vci_ei*it)+(Vciei*it_Vci_ei))+((vdci_t-Vbici)*iavl_Vci_ei));
pterm_Vtnode_GND=((Vciei*it_Vtnode_GND)+((vdci_t_Vtnode_GND*iavl)+((vdci_t-Vbici)*iavl_Vtnode_GND)));
pterm_Vbi_ei=((Vciei*it_Vbi_ei)+((vdci_t-Vbici)*iavl_Vbi_ei));
pterm_Vbi_ci=((Vciei*it_Vbi_ci)+(((-Vbici_Vbi_ci)*iavl)+((vdci_t-Vbici)*iavl_Vbi_ci)));
#endif
pterm=((Vciei*it)+((vdci_t-Vbici)*iavl));
}
else
{
if
((flsh==2)&&(rth>=0.001))
{
#if defined(_DERIVATE)
pterm_Vb_bp=0.0;
pterm_Vci_c=0.0;
pterm_Vei_e=0.0;
pterm_Vbp_bi=0.0;
pterm_Vbp_ei=((((ibep_Vbp_ei*Vbpei)+(ibep*Vbpei_Vbp_ei))+ijbcx_Vbp_ei*Vbpci)+ijsc_Vbp_ei*Vsici);
pterm_Vbp_ci=(((ijbcx_Vbp_ci*Vbpci)+(ijbcx*Vbpci_Vbp_ci))+ijsc_Vbp_ci*Vsici);
pterm_Vsi_ci=((ijsc_Vsi_ci*Vsici)+(ijsc*Vsici_Vsi_ci));
pterm_Vci_ei=(((Vciei_Vci_ei*it)+(Vciei*it_Vci_ei))+((vdci_t-Vbici)*iavl_Vci_ei));
pterm_Vtnode_GND=(((((((Vciei*it_Vtnode_GND)+((vdci_t_Vtnode_GND*iavl)+((vdci_t-Vbici)*iavl_Vtnode_GND)))+ibei_Vtnode_GND*Vbiei)+ibci_Vtnode_GND*Vbici)+ibep_Vtnode_GND*Vbpei)+ijbcx_Vtnode_GND*Vbpci)+ijsc_Vtnode_GND*Vsici);
pterm_Vbi_ei=(((((((Vciei*it_Vbi_ei)+((vdci_t-Vbici)*iavl_Vbi_ei))+((ibei_Vbi_ei*Vbiei)+(ibei*Vbiei_Vbi_ei)))+ibci_Vbi_ei*Vbici)+ibep_Vbi_ei*Vbpei)+ijbcx_Vbi_ei*Vbpci)+ijsc_Vbi_ei*Vsici);
pterm_Vbi_ci=((((((Vciei*it_Vbi_ci)+(((-Vbici_Vbi_ci)*iavl)+((vdci_t-Vbici)*iavl_Vbi_ci)))+((ibci_Vbi_ci*Vbici)+(ibci*Vbici_Vbi_ci)))+ibep_Vbi_ci*Vbpei)+ijbcx_Vbi_ci*Vbpci)+ijsc_Vbi_ci*Vsici);
#endif
pterm=(((((((Vciei*it)+((vdci_t-Vbici)*iavl))+(ibei*Vbiei))+(ibci*Vbici))+(ibep*Vbpei))+(ijbcx*Vbpci))+(ijsc*Vsici));
if
(rbi>=0.001)
{
#if defined(_DERIVATE)
pterm_Vb_bp=0.0;
pterm_Vci_c=0.0;
pterm_Vei_e=0.0;
pterm_Vbp_bi=(pterm_Vbp_bi+((BP(bp,bi)+BP(bp,bi))/rbi));
pterm_Vbp_ei=pterm_Vbp_ei;
pterm_Vbp_ci=pterm_Vbp_ci;
pterm_Vsi_ci=pterm_Vsi_ci;
pterm_Vci_ei=(pterm_Vci_ei+(-(BP(bp,bi)*BP(bp,bi))*rbi_Vci_ei/rbi/rbi));
pterm_Vtnode_GND=(pterm_Vtnode_GND+(-(BP(bp,bi)*BP(bp,bi))*rbi_Vtnode_GND/rbi/rbi));
pterm_Vbi_ei=(pterm_Vbi_ei+(-(BP(bp,bi)*BP(bp,bi))*rbi_Vbi_ei/rbi/rbi));
pterm_Vbi_ci=(pterm_Vbi_ci+(-(BP(bp,bi)*BP(bp,bi))*rbi_Vbi_ci/rbi/rbi));
#endif
pterm=(pterm+((BP(bp,bi)*BP(bp,bi))/rbi));
}
if
(re_t>=0.001)
{
#if defined(_DERIVATE)
pterm_Vb_bp=0.0;
pterm_Vci_c=0.0;
pterm_Vei_e=(pterm_Vei_e+((BP(ei,e)+BP(ei,e))/re_t));
pterm_Vbp_bi=pterm_Vbp_bi;
pterm_Vbp_ei=pterm_Vbp_ei;
pterm_Vbp_ci=pterm_Vbp_ci;
pterm_Vsi_ci=pterm_Vsi_ci;
pterm_Vci_ei=pterm_Vci_ei;
pterm_Vtnode_GND=(pterm_Vtnode_GND+(-(BP(ei,e)*BP(ei,e))*re_t_Vtnode_GND/re_t/re_t));
pterm_Vbi_ei=pterm_Vbi_ei;
pterm_Vbi_ci=pterm_Vbi_ci;
#endif
pterm=(pterm+((BP(ei,e)*BP(ei,e))/re_t));
}
if
(rcx_t>=0.001)
{
#if defined(_DERIVATE)
pterm_Vb_bp=0.0;
pterm_Vci_c=(pterm_Vci_c+((BP(ci,c)+BP(ci,c))/rcx_t));
pterm_Vei_e=pterm_Vei_e;
pterm_Vbp_bi=pterm_Vbp_bi;
pterm_Vbp_ei=pterm_Vbp_ei;
pterm_Vbp_ci=pterm_Vbp_ci;
pterm_Vsi_ci=pterm_Vsi_ci;
pterm_Vci_ei=pterm_Vci_ei;
pterm_Vtnode_GND=(pterm_Vtnode_GND+(-(BP(ci,c)*BP(ci,c))*rcx_t_Vtnode_GND/rcx_t/rcx_t));
pterm_Vbi_ei=pterm_Vbi_ei;
pterm_Vbi_ci=pterm_Vbi_ci;
#endif
pterm=(pterm+((BP(ci,c)*BP(ci,c))/rcx_t));
}
if
(rbx_t>=0.001)
{
#if defined(_DERIVATE)
pterm_Vb_bp=(pterm_Vb_bp+((BP(b,bp)+BP(b,bp))/rbx_t));
pterm_Vci_c=pterm_Vci_c;
pterm_Vei_e=pterm_Vei_e;
pterm_Vbp_bi=pterm_Vbp_bi;
pterm_Vbp_ei=pterm_Vbp_ei;
pterm_Vbp_ci=pterm_Vbp_ci;
pterm_Vsi_ci=pterm_Vsi_ci;
pterm_Vci_ei=pterm_Vci_ei;
pterm_Vtnode_GND=(pterm_Vtnode_GND+(-(BP(b,bp)*BP(b,bp))*rbx_t_Vtnode_GND/rbx_t/rbx_t));
pterm_Vbi_ei=pterm_Vbi_ei;
pterm_Vbi_ci=pterm_Vbi_ci;
#endif
pterm=(pterm+((BP(b,bp)*BP(b,bp))/rbx_t));
}
}
}
#if defined(_DERIVATE)
Itxf_Vxf2_GND=0.0;
Itxf_Vci_ei=itf_Vci_ei;
Itxf_Vtnode_GND=itf_Vtnode_GND;
Itxf_Vbi_ei=itf_Vbi_ei;
Itxf_Vbi_ci=itf_Vbi_ci;
#endif
Itxf=itf;
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdeix_Vxf_GND=0.0;
Qdeix_Vci_ei=Qdei_Vci_ei;
Qdeix_Vtnode_GND=Qdei_Vtnode_GND;
Qdeix_Vbi_ci=Qdei_Vbi_ci;
Qdeix_Vbi_ei=Qdei_Vbi_ei;
#endif
Qdeix=Qdei;
#endif
if
((flnqs!=0)&&(Tf!=0))
{
#if defined(_DERIVATE)
Vxf1_Vxf1_GND=1.0;
#endif
Vxf1=NP(xf1);
#if defined(_DERIVATE)
Vxf2_Vxf2_GND=1.0;
#endif
Vxf2=NP(xf2);
#if defined(_DERIVATE)
Ixf1_Vxf1_GND=0.0;
Ixf1_Vxf2_GND=(Vxf2_Vxf2_GND/Tf)*t0;
Ixf1_Vci_ei=(((-itf_Vci_ei)*Tf-(Vxf2-itf)*Tf_Vci_ei)/Tf/Tf)*t0;
Ixf1_Vtnode_GND=(((-itf_Vtnode_GND)*Tf-(Vxf2-itf)*Tf_Vtnode_GND)/Tf/Tf)*t0;
Ixf1_Vbi_ei=(((-itf_Vbi_ei)*Tf-(Vxf2-itf)*Tf_Vbi_ei)/Tf/Tf)*t0;
Ixf1_Vbi_ci=(((-itf_Vbi_ci)*Tf-(Vxf2-itf)*Tf_Vbi_ci)/Tf/Tf)*t0;
#endif
Ixf1=(((Vxf2-itf)/Tf)*t0);
#if defined(_DERIVATE)
Ixf2_Vxf2_GND=(Vxf2_Vxf2_GND/Tf)*t0;
Ixf2_Vxf1_GND=((-Vxf1_Vxf1_GND)/Tf)*t0;
Ixf2_Vbi_ei=(-(Vxf2-Vxf1)*Tf_Vbi_ei/Tf/Tf)*t0;
Ixf2_Vci_ei=(-(Vxf2-Vxf1)*Tf_Vci_ei/Tf/Tf)*t0;
Ixf2_Vtnode_GND=(-(Vxf2-Vxf1)*Tf_Vtnode_GND/Tf/Tf)*t0;
Ixf2_Vbi_ci=(-(Vxf2-Vxf1)*Tf_Vbi_ci/Tf/Tf)*t0;
#endif
Ixf2=(((Vxf2-Vxf1)/Tf)*t0);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf1_Vxf1_GND=(alit*Vxf1_Vxf1_GND)*t0;
#endif
Qxf1=((alit*Vxf1)*t0);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf2_Vxf2_GND=((alit*Vxf2_Vxf2_GND)/3)*t0;
#endif
Qxf2=(((alit*Vxf2)/3)*t0);
#endif
#if defined(_DERIVATE)
Itxf_Vxf2_GND=Vxf2_Vxf2_GND;
Itxf_Vci_ei=0.0;
Itxf_Vtnode_GND=0.0;
Itxf_Vbi_ei=0.0;
Itxf_Vbi_ci=0.0;
#endif
Itxf=Vxf2;
#if defined(_DERIVATE)
Vxf_Vxf_GND=1.0;
#endif
Vxf=NP(xf);
#if defined(_DERIVATE)
fact_Vbi_ei=(-t0*Tf_Vbi_ei/Tf/Tf);
fact_Vci_ei=(-t0*Tf_Vci_ei/Tf/Tf);
fact_Vtnode_GND=(-t0*Tf_Vtnode_GND/Tf/Tf);
fact_Vbi_ci=(-t0*Tf_Vbi_ci/Tf/Tf);
#endif
fact=(t0/Tf);
#if defined(_DERIVATE)
Ixf_Vxf_GND=Vxf_Vxf_GND*fact;
Ixf_Vci_ei=(((-Qdei_Vci_ei)*fact)+((Vxf-Qdei)*fact_Vci_ei));
Ixf_Vtnode_GND=(((-Qdei_Vtnode_GND)*fact)+((Vxf-Qdei)*fact_Vtnode_GND));
Ixf_Vbi_ci=(((-Qdei_Vbi_ci)*fact)+((Vxf-Qdei)*fact_Vbi_ci));
Ixf_Vbi_ei=(((-Qdei_Vbi_ei)*fact)+((Vxf-Qdei)*fact_Vbi_ei));
#endif
Ixf=((Vxf-Qdei)*fact);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf_Vxf_GND=(alqf*Vxf_Vxf_GND)*t0;
#endif
Qxf=((alqf*Vxf)*t0);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qdeix_Vxf_GND=Vxf_Vxf_GND;
Qdeix_Vci_ei=0.0;
Qdeix_Vtnode_GND=0.0;
Qdeix_Vbi_ci=0.0;
Qdeix_Vbi_ei=0.0;
#endif
Qdeix=Vxf;
#endif
}
else
{
#if defined(_DERIVATE)
Ixf1_Vxf1_GND=1.0;
Ixf1_Vxf2_GND=0.0;
Ixf1_Vci_ei=0.0;
Ixf1_Vtnode_GND=0.0;
Ixf1_Vbi_ei=0.0;
Ixf1_Vbi_ci=0.0;
#endif
Ixf1=NP(xf1);
#if defined(_DERIVATE)
Ixf2_Vxf2_GND=1.0;
Ixf2_Vxf1_GND=0.0;
Ixf2_Vbi_ei=0.0;
Ixf2_Vci_ei=0.0;
Ixf2_Vtnode_GND=0.0;
Ixf2_Vbi_ci=0.0;
#endif
Ixf2=NP(xf2);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf1_Vxf1_GND=0.0;
#endif
Qxf1=0;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf2_Vxf2_GND=0.0;
#endif
Qxf2=0;
#endif
#if defined(_DERIVATE)
Ixf_Vxf_GND=1.0;
Ixf_Vci_ei=0.0;
Ixf_Vtnode_GND=0.0;
Ixf_Vbi_ci=0.0;
Ixf_Vbi_ei=0.0;
#endif
Ixf=NP(xf);
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
Qxf_Vxf_GND=0.0;
#endif
Qxf=0;
#endif
}
}
{
_load_static_residual2(bi,ei,(1.0e-12*BP(bi,ei)));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ei,bi,ei,(1.0e-12));
#endif
_load_static_residual2(bi,ci,(1.0e-12*BP(bi,ci)));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ci,bi,ci,(1.0e-12));
#endif
_load_static_residual2(ci,ei,(1.0e-12*BP(ci,ei)));
#if defined(_DERIVATE)
_load_static_jacobian4(ci,ei,ci,ei,(1.0e-12));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(b,ci,_DDT((type*qjcx0_t_i)));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(b,ci,bi,ci,(type*qjcx0_t_i_Vbi_ci));
_load_dynamic_jacobian2s(b,ci,tnode,(type*qjcx0_t_i_Vtnode_GND));
_load_dynamic_jacobian4(b,ci,bp,ci,(type*qjcx0_t_i_Vbp_ci));
_load_dynamic_jacobian4(b,ci,b,ci,(type*qjcx0_t_i_Vb_ci));
_load_dynamic_jacobian4(b,ci,bi,ei,(type*qjcx0_t_i_Vbi_ei));
_load_dynamic_jacobian4(b,ci,bp,ei,(type*qjcx0_t_i_Vbp_ei));
#endif
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(b,ci,_DDT((cbcpar1*BP(b,ci))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(b,ci,b,ci,(cbcpar1));
#endif
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bp,ci,_DDT((cbcpar2*BP(bp,ci))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bp,ci,bp,ci,(cbcpar2));
#endif
#endif
if
(rbx>=0.001)
{
_load_static_residual2(b,bp,(BP(b,bp)/rbx_t));
#if defined(_DERIVATE)
_load_static_jacobian2s(b,bp,tnode,(-BP(b,bp)*rbx_t_Vtnode_GND/rbx_t/rbx_t));
_load_static_jacobian4(b,bp,b,bp,(1/rbx_t));
#endif
}
else
{
_load_static_residual2(b,bp,(BP(b,bp)/1e-6));
#if defined(_DERIVATE)
_load_static_jacobian4(b,bp,b,bp,(1/1e-6));
#endif
}
if
(rbi0>=0.001)
{
_load_static_residual2(bp,bi,(BP(bp,bi)/rbi));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,bi,ci,ei,(-BP(bp,bi)*rbi_Vci_ei/rbi/rbi));
_load_static_jacobian4(bp,bi,bi,ci,(-BP(bp,bi)*rbi_Vbi_ci/rbi/rbi));
_load_static_jacobian4(bp,bi,bi,ei,(-BP(bp,bi)*rbi_Vbi_ei/rbi/rbi));
_load_static_jacobian2s(bp,bi,tnode,(-BP(bp,bi)*rbi_Vtnode_GND/rbi/rbi));
_load_static_jacobian4(bp,bi,bp,bi,(1/rbi));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bp,bi,_DDT(qrbi));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bp,bi,bp,bi,qrbi_Vbp_bi);
_load_dynamic_jacobian4(bp,bi,ci,ei,qrbi_Vci_ei);
_load_dynamic_jacobian4(bp,bi,bi,ci,qrbi_Vbi_ci);
_load_dynamic_jacobian4(bp,bi,bi,ei,qrbi_Vbi_ei);
_load_dynamic_jacobian2s(bp,bi,tnode,qrbi_Vtnode_GND);
#endif
#endif
}
else
{
_load_static_residual2(bp,bi,(BP(bp,bi)/1e-6));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,bi,bp,bi,(1/1e-6));
#endif
}
if
(tunode==1.0)
{
_load_static_residual2(bp,ei,((-type)*ibet));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,ei,bi,ei,((-type)*ibet_Vbi_ei));
_load_static_jacobian4(bp,ei,bi,ci,((-type)*ibet_Vbi_ci));
_load_static_jacobian2s(bp,ei,tnode,((-type)*ibet_Vtnode_GND));
_load_static_jacobian4(bp,ei,bp,ei,((-type)*ibet_Vbp_ei));
#endif
}
else
{
_load_static_residual2(bi,ei,((-type)*ibet));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ei,bi,ei,((-type)*ibet_Vbi_ei));
_load_static_jacobian4(bi,ei,bi,ci,((-type)*ibet_Vbi_ci));
_load_static_jacobian2s(bi,ei,tnode,((-type)*ibet_Vtnode_GND));
_load_static_jacobian4(bi,ei,bp,ei,((-type)*ibet_Vbp_ei));
#endif
}
_load_static_residual2(bp,ei,(type*ibep));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,ei,bi,ei,(type*ibep_Vbi_ei));
_load_static_jacobian4(bp,ei,bi,ci,(type*ibep_Vbi_ci));
_load_static_jacobian4(bp,ei,bp,ei,(type*ibep_Vbp_ei));
_load_static_jacobian2s(bp,ei,tnode,(type*ibep_Vtnode_GND));
#endif
_load_static_residual2(bp,ei,(type*irep));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,ei,bi,ei,(type*irep_Vbi_ei));
_load_static_jacobian4(bp,ei,bi,ci,(type*irep_Vbi_ci));
_load_static_jacobian4(bp,ei,bp,ei,(type*irep_Vbp_ei));
_load_static_jacobian2s(bp,ei,tnode,(type*irep_Vtnode_GND));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bp,ei,_DDT((type*Qjep)));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bp,ei,bi,ei,(type*Qjep_Vbi_ei));
_load_dynamic_jacobian2s(bp,ei,tnode,(type*Qjep_Vtnode_GND));
_load_dynamic_jacobian4(bp,ei,bi,ci,(type*Qjep_Vbi_ci));
_load_dynamic_jacobian4(bp,ei,bp,ei,(type*Qjep_Vbp_ei));
#endif
#endif
_load_static_residual2(bi,ei,(type*ibei));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ei,bi,ei,(type*ibei_Vbi_ei));
_load_static_jacobian2s(bi,ei,tnode,(type*ibei_Vtnode_GND));
#endif
_load_static_residual2(bi,ei,(type*irei));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ei,bi,ei,(type*irei_Vbi_ei));
_load_static_jacobian2s(bi,ei,tnode,(type*irei_Vtnode_GND));
#endif
_load_static_residual2(bi,ei,(type*ibh_rec));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ei,ci,ei,(type*ibh_rec_Vci_ei));
_load_static_jacobian4(bi,ei,bi,ci,(type*ibh_rec_Vbi_ci));
_load_static_jacobian4(bi,ei,bi,ei,(type*ibh_rec_Vbi_ei));
_load_static_jacobian2s(bi,ei,tnode,(type*ibh_rec_Vtnode_GND));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bi,ei,_DDT((type*(Qdeix+Qjei))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bi,ei,bi,ei,(type*(Qdeix_Vbi_ei+Qjei_Vbi_ei)));
_load_dynamic_jacobian4(bi,ei,bi,ci,(type*Qdeix_Vbi_ci));
_load_dynamic_jacobian2s(bi,ei,tnode,(type*(Qdeix_Vtnode_GND+Qjei_Vtnode_GND)));
_load_dynamic_jacobian4(bi,ei,ci,ei,(type*Qdeix_Vci_ei));
_load_dynamic_jacobian2s(bi,ei,xf,(type*Qdeix_Vxf_GND));
#endif
#endif
_load_static_residual2(bp,si,(type*HSI_Tsu));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,si,si,ci,(type*HSI_Tsu_Vsi_ci));
_load_static_jacobian4(bp,si,bp,ci,(type*HSI_Tsu_Vbp_ci));
_load_static_jacobian2s(bp,si,tnode,(type*HSI_Tsu_Vtnode_GND));
#endif
_load_static_residual2(bp,ci,(type*ijbcx));
#if defined(_DERIVATE)
_load_static_jacobian4(bp,ci,bi,ei,(type*ijbcx_Vbi_ei));
_load_static_jacobian4(bp,ci,bi,ci,(type*ijbcx_Vbi_ci));
_load_static_jacobian4(bp,ci,bp,ei,(type*ijbcx_Vbp_ei));
_load_static_jacobian4(bp,ci,bp,ci,(type*ijbcx_Vbp_ci));
_load_static_jacobian2s(bp,ci,tnode,(type*ijbcx_Vtnode_GND));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bp,ci,_DDT((type*(qjcx0_t_ii+Qdsu))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bp,ci,bi,ci,(type*qjcx0_t_ii_Vbi_ci));
_load_dynamic_jacobian2s(bp,ci,tnode,(type*(qjcx0_t_ii_Vtnode_GND+Qdsu_Vtnode_GND)));
_load_dynamic_jacobian4(bp,ci,bp,ci,(type*(qjcx0_t_ii_Vbp_ci+Qdsu_Vbp_ci)));
_load_dynamic_jacobian4(bp,ci,bi,ei,(type*qjcx0_t_ii_Vbi_ei));
_load_dynamic_jacobian4(bp,ci,bp,ei,(type*qjcx0_t_ii_Vbp_ei));
#endif
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(b,e,_DDT((cbepar1*BP(b,e))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(b,e,b,e,(cbepar1));
#endif
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bp,e,_DDT((cbepar2*BP(bp,e))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bp,e,bp,e,(cbepar2));
#endif
#endif
_load_static_residual2(bi,ci,(type*(ibci-iavl)));
#if defined(_DERIVATE)
_load_static_jacobian4(bi,ci,ci,ei,(type*(-iavl_Vci_ei)));
_load_static_jacobian4(bi,ci,bi,ei,(type*(ibci_Vbi_ei-iavl_Vbi_ei)));
_load_static_jacobian4(bi,ci,bi,ci,(type*(ibci_Vbi_ci-iavl_Vbi_ci)));
_load_static_jacobian2s(bi,ci,tnode,(type*(ibci_Vtnode_GND-iavl_Vtnode_GND)));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bi,ci,_DDT((type*(Qdci+Qjci))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bi,ci,bi,ei,(type*(Qdci_Vbi_ei+Qjci_Vbi_ei)));
_load_dynamic_jacobian4(bi,ci,bi,ci,(type*(Qdci_Vbi_ci+Qjci_Vbi_ci)));
_load_dynamic_jacobian2s(bi,ci,tnode,(type*(Qdci_Vtnode_GND+Qjci_Vtnode_GND)));
_load_dynamic_jacobian4(bi,ci,ci,ei,(type*Qdci_Vci_ei));
#endif
#endif
_load_static_residual2(si,ci,(type*ijsc));
#if defined(_DERIVATE)
_load_static_jacobian4(si,ci,bi,ei,(type*ijsc_Vbi_ei));
_load_static_jacobian4(si,ci,bi,ci,(type*ijsc_Vbi_ci));
_load_static_jacobian4(si,ci,bp,ei,(type*ijsc_Vbp_ei));
_load_static_jacobian4(si,ci,bp,ci,(type*ijsc_Vbp_ci));
_load_static_jacobian4(si,ci,si,ci,(type*ijsc_Vsi_ci));
_load_static_jacobian2s(si,ci,tnode,(type*ijsc_Vtnode_GND));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(si,ci,_DDT((type*Qjs)));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(si,ci,bi,ci,(type*Qjs_Vbi_ci));
_load_dynamic_jacobian2s(si,ci,tnode,(type*Qjs_Vtnode_GND));
_load_dynamic_jacobian4(si,ci,bp,ci,(type*Qjs_Vbp_ci));
_load_dynamic_jacobian4(si,ci,b,ci,(type*Qjs_Vb_ci));
_load_dynamic_jacobian4(si,ci,si,ci,(type*Qjs_Vsi_ci));
_load_dynamic_jacobian4(si,ci,bi,ei,(type*Qjs_Vbi_ei));
_load_dynamic_jacobian4(si,ci,bp,ei,(type*Qjs_Vbp_ei));
#endif
#endif
_load_static_residual2(ci,ei,(type*Itxf));
#if defined(_DERIVATE)
_load_static_jacobian4(ci,ei,bi,ci,(type*Itxf_Vbi_ci));
_load_static_jacobian4(ci,ei,bi,ei,(type*Itxf_Vbi_ei));
_load_static_jacobian2s(ci,ei,tnode,(type*Itxf_Vtnode_GND));
_load_static_jacobian4(ci,ei,ci,ei,(type*Itxf_Vci_ei));
_load_static_jacobian2s(ci,ei,xf2,(type*Itxf_Vxf2_GND));
#endif
_load_static_residual2(ei,ci,(type*itr));
#if defined(_DERIVATE)
_load_static_jacobian4(ei,ci,bi,ei,(type*itr_Vbi_ei));
_load_static_jacobian4(ei,ci,bi,ci,(type*itr_Vbi_ci));
_load_static_jacobian2s(ei,ci,tnode,(type*itr_Vtnode_GND));
_load_static_jacobian4(ei,ci,ci,ei,(type*itr_Vci_ei));
#endif
if
(rcx>=0.001)
{
_load_static_residual2(ci,c,(BP(ci,c)/rcx_t));
#if defined(_DERIVATE)
_load_static_jacobian2s(ci,c,tnode,(-BP(ci,c)*rcx_t_Vtnode_GND/rcx_t/rcx_t));
_load_static_jacobian4(ci,c,ci,c,(1/rcx_t));
#endif
}
else
{
_load_static_residual2(ci,c,(BP(ci,c)/1e-6));
#if defined(_DERIVATE)
_load_static_jacobian4(ci,c,ci,c,(1/1e-6));
#endif
}
if
(re>=0.001)
{
_load_static_residual2(ei,e,(BP(ei,e)/re_t));
#if defined(_DERIVATE)
_load_static_jacobian2s(ei,e,tnode,(-BP(ei,e)*re_t_Vtnode_GND/re_t/re_t));
_load_static_jacobian4(ei,e,ei,e,(1/re_t));
#endif
}
else
{
_load_static_residual2(ei,e,(BP(ei,e)/1e-6));
#if defined(_DERIVATE)
_load_static_jacobian4(ei,e,ei,e,(1/1e-6));
#endif
}
if
(rsu>=0.001)
{
_load_static_residual2(si,s,(BP(si,s)/rsu));
#if defined(_DERIVATE)
_load_static_jacobian4(si,s,si,s,(1/rsu));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(si,s,_DDT((csu*BP(si,s))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(si,s,si,s,(csu));
#endif
#endif
}
else
{
_load_static_residual2(si,s,(BP(si,s)/1e-6));
#if defined(_DERIVATE)
_load_static_jacobian4(si,s,si,s,(1/1e-6));
#endif
}
if
((flsh==0)||(rth<0.001))
{
_load_static_residual1(tnode,(NP(tnode)/0.001));
#if defined(_DERIVATE)
_load_static_jacobian1(tnode,tnode,(1/0.001));
#endif
}
else
{
_load_static_residual1(tnode,((NP(tnode)/rth_t)-pterm));
#if defined(_DERIVATE)
_load_static_jacobian2p(tnode,bi,ci,(-pterm_Vbi_ci));
_load_static_jacobian2p(tnode,bi,ei,(-pterm_Vbi_ei));
_load_static_jacobian2p(tnode,ci,ei,(-pterm_Vci_ei));
_load_static_jacobian2p(tnode,si,ci,(-pterm_Vsi_ci));
_load_static_jacobian2p(tnode,bp,ci,(-pterm_Vbp_ci));
_load_static_jacobian2p(tnode,bp,ei,(-pterm_Vbp_ei));
_load_static_jacobian2p(tnode,bp,bi,(-pterm_Vbp_bi));
_load_static_jacobian2p(tnode,ei,e,(-pterm_Vei_e));
_load_static_jacobian2p(tnode,ci,c,(-pterm_Vci_c));
_load_static_jacobian2p(tnode,b,bp,(-pterm_Vb_bp));
_load_static_jacobian1(tnode,tnode,(((rth_t-(NP(tnode)*rth_t_Vtnode_GND))/rth_t/rth_t)-pterm_Vtnode_GND));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual1(tnode,_DDT((cth*NP(tnode))));
#if defined(_DERIVATE)
_load_dynamic_jacobian1(tnode,tnode,(cth));
#endif
#endif
}
_load_static_residual1(xf1,Ixf1);
#if defined(_DERIVATE)
_load_static_jacobian2p(xf1,bi,ci,Ixf1_Vbi_ci);
_load_static_jacobian2p(xf1,bi,ei,Ixf1_Vbi_ei);
_load_static_jacobian1(xf1,tnode,Ixf1_Vtnode_GND);
_load_static_jacobian2p(xf1,ci,ei,Ixf1_Vci_ei);
_load_static_jacobian1(xf1,xf2,Ixf1_Vxf2_GND);
_load_static_jacobian1(xf1,xf1,Ixf1_Vxf1_GND);
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual1(xf1,_DDT(Qxf1));
#if defined(_DERIVATE)
_load_dynamic_jacobian1(xf1,xf1,Qxf1_Vxf1_GND);
#endif
#endif
_load_static_residual1(xf2,Ixf2);
#if defined(_DERIVATE)
_load_static_jacobian2p(xf2,bi,ci,Ixf2_Vbi_ci);
_load_static_jacobian1(xf2,tnode,Ixf2_Vtnode_GND);
_load_static_jacobian2p(xf2,ci,ei,Ixf2_Vci_ei);
_load_static_jacobian2p(xf2,bi,ei,Ixf2_Vbi_ei);
_load_static_jacobian1(xf2,xf1,Ixf2_Vxf1_GND);
_load_static_jacobian1(xf2,xf2,Ixf2_Vxf2_GND);
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual1(xf2,_DDT(Qxf2));
#if defined(_DERIVATE)
_load_dynamic_jacobian1(xf2,xf2,Qxf2_Vxf2_GND);
#endif
#endif
_load_static_residual1(xf,Ixf);
#if defined(_DERIVATE)
_load_static_jacobian2p(xf,bi,ei,Ixf_Vbi_ei);
_load_static_jacobian2p(xf,bi,ci,Ixf_Vbi_ci);
_load_static_jacobian1(xf,tnode,Ixf_Vtnode_GND);
_load_static_jacobian2p(xf,ci,ei,Ixf_Vci_ei);
_load_static_jacobian1(xf,xf,Ixf_Vxf_GND);
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual1(xf,_DDT(Qxf));
#if defined(_DERIVATE)
_load_dynamic_jacobian1(xf,xf,Qxf_Vxf_GND);
#endif
#endif
}

/* ------------------ end of verilog analog equations --------------------- */

/* ------------------ evaluate verilog noise equations -------------------- */
#if defined(_DYNAMIC)
double n_2;
#if defined(_DERIVATE)
double n_2_Vbi_ei;
double n_2_Vci_ei;
double n_2_Vtnode_GND;
double n_2_Vbi_ci;
#endif
#endif
#if defined(_DYNAMIC)
double sqrt_n2;
#if defined(_DERIVATE)
double sqrt_n2_Vci_ei;
double sqrt_n2_Vtnode_GND;
double sqrt_n2_Vbi_ei;
double sqrt_n2_Vbi_ci;
#endif
#endif
#if defined(_DYNAMIC)
double n_1;
#if defined(_DERIVATE)
double n_1_Vbi_ei;
double n_1_Vci_ei;
double n_1_Vtnode_GND;
double n_1_Vbi_ci;
#endif
#endif
#if defined(_DYNAMIC)
double n_w;
#endif
double twoq;
double i_re;
#if defined(_DERIVATE)
double i_re_Vei_e;
double i_re_Vtnode_GND;
#endif
double flicker_Pwr;
#if defined(_DERIVATE)
double flicker_Pwr_Vei_e;
double flicker_Pwr_Vtnode_GND;
double flicker_Pwr_Vbi_ei;
double flicker_Pwr_Vbp_ei;
double flicker_Pwr_Vbi_ci;
#endif
double fourkt;
#if defined(_DERIVATE)
double fourkt_Vtnode_GND;
#endif
{
#if defined(_DERIVATE)
fourkt_Vtnode_GND=((4.0*1.3806503e-23)*Tdev_Vtnode_GND);
#endif
fourkt=((4.0*1.3806503e-23)*Tdev);
if
(rbx>=0.001)
{
_save_whitenoise2(b,bp,(fourkt/rbx_t),"thermal");
}
if
(rbi0>=0.001)
{
_save_whitenoise2(bp,bi,(fourkt/rbi),"thermal");
}
if
(rcx>=0.001)
{
_save_whitenoise2(ci,c,(fourkt/rcx_t),"thermal");
}
if
(re>=0.001)
{
_save_whitenoise2(ei,e,(fourkt/re_t),"thermal");
}
if
(rsu>=0.001)
{
_save_whitenoise2(si,s,(fourkt/rsu),"thermal");
}
{
double m00_pow(d00_pow0,(ibei+ibep),af)
#if defined(_DERIVATE)
double m10_pow(d10_pow0,d00_pow0,(ibei+ibep),af)
#endif
#if defined(_DERIVATE)
flicker_Pwr_Vei_e=0.0;
flicker_Pwr_Vtnode_GND=(kf*(d10_pow0*(ibei_Vtnode_GND+ibep_Vtnode_GND)));
flicker_Pwr_Vbi_ei=(kf*(d10_pow0*(ibei_Vbi_ei+ibep_Vbi_ei)));
flicker_Pwr_Vbp_ei=(kf*(d10_pow0*ibep_Vbp_ei));
flicker_Pwr_Vbi_ci=(kf*(d10_pow0*ibep_Vbi_ci));
#endif
flicker_Pwr=(kf*d00_pow0);
}
if
(cfbe==(-1))
{
_save_flickernoise2(bi,ei,flicker_Pwr,1.0,NULL);
}
else
{
_save_flickernoise2(bp,ei,flicker_Pwr,1.0,NULL);
}
if
(re>=0.001)
{
#if defined(_DERIVATE)
i_re_Vei_e=(1/re_t);
i_re_Vtnode_GND=(-BP(ei,e)*re_t_Vtnode_GND/re_t/re_t);
#endif
i_re=(BP(ei,e)/re_t);
{
double m00_abs(d00_abs0,i_re)
double m00_pow(d00_pow1,d00_abs0,afre)
#if defined(_DERIVATE)
double m10_abs(d10_abs0,d00_abs0,i_re)
double m10_pow(d10_pow1,d00_pow1,d00_abs0,afre)
#endif
#if defined(_DERIVATE)
flicker_Pwr_Vei_e=(kfre*(d10_pow1*i_re_Vei_e*d10_abs0));
flicker_Pwr_Vtnode_GND=(kfre*(d10_pow1*i_re_Vtnode_GND*d10_abs0));
flicker_Pwr_Vbi_ei=0.0;
flicker_Pwr_Vbp_ei=0.0;
flicker_Pwr_Vbi_ci=0.0;
#endif
flicker_Pwr=(kfre*d00_pow1);
}
_save_flickernoise2(ei,e,flicker_Pwr,1.0,NULL);
}
twoq=(2.0*1.602176462e-19);
_save_whitenoise2(ci,bi,(twoq*iavl),"shot");
_save_whitenoise2(bi,ci,(twoq*abs(ibci)),"shot");
_save_whitenoise2(bp,ei,(twoq*ibep),"shot");
_save_whitenoise2(bp,ci,(twoq*abs(ijbcx)),"shot");
_save_whitenoise2(si,ci,(twoq*abs(ijsc)),"shot");
if
(flcono==1)
{
#if defined(_DYNAMIC)
n_w=1;
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
n_1_Vbi_ei=Tf_Vbi_ei*alit;
n_1_Vci_ei=Tf_Vci_ei*alit;
n_1_Vtnode_GND=Tf_Vtnode_GND*alit;
n_1_Vbi_ci=Tf_Vbi_ci*alit;
#endif
n_1=(Tf*alit);
#endif
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
sqrt_n2_Vci_ei=betadc_Vci_ei*((2*alqf)-(alit*alit));
sqrt_n2_Vtnode_GND=betadc_Vtnode_GND*((2*alqf)-(alit*alit));
sqrt_n2_Vbi_ei=betadc_Vbi_ei*((2*alqf)-(alit*alit));
sqrt_n2_Vbi_ci=betadc_Vbi_ci*((2*alqf)-(alit*alit));
#endif
sqrt_n2=(betadc*((2*alqf)-(alit*alit)));
#endif
#ifdef _DYNAMIC
if
(sqrt_n2>0.0)
{
#if defined(_DYNAMIC)
{
double m00_sqrt(d00_sqrt0,sqrt_n2)
#if defined(_DERIVATE)
double m10_sqrt(d10_sqrt0,d00_sqrt0,sqrt_n2)
#endif
#if defined(_DERIVATE)
n_2_Vbi_ei=((Tf_Vbi_ei*d00_sqrt0)+(Tf*sqrt_n2_Vbi_ei*d10_sqrt0));
n_2_Vci_ei=((Tf_Vci_ei*d00_sqrt0)+(Tf*sqrt_n2_Vci_ei*d10_sqrt0));
n_2_Vtnode_GND=((Tf_Vtnode_GND*d00_sqrt0)+(Tf*sqrt_n2_Vtnode_GND*d10_sqrt0));
n_2_Vbi_ci=((Tf_Vbi_ci*d00_sqrt0)+(Tf*sqrt_n2_Vbi_ci*d10_sqrt0));
#endif
n_2=(Tf*d00_sqrt0);
}
#endif
}
else
{
#if defined(_DYNAMIC)
#if defined(_DERIVATE)
n_2_Vbi_ei=0.0;
n_2_Vci_ei=0.0;
n_2_Vtnode_GND=0.0;
n_2_Vbi_ci=0.0;
#endif
n_2=0;
#endif
}
#endif /* if (...) */
_save_whitenoise1(n1,((2*1.602176462e-19)*ibei),"shot");
_load_static_residual1(n1,(-NP(n1)));
#if defined(_DERIVATE)
_load_static_jacobian1(n1,n1,(-1.0));
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bi,ei,(NP(n1)+((n_2/n_w)*_DDT((n_w*NP(n1))))));
#if defined(_DERIVATE)
_load_dynamic_jacobian4(bi,ei,bi,ci,(n_2_Vbi_ci/n_w)*_DDT((n_w*NP(n1))));
_load_dynamic_jacobian2s(bi,ei,tnode,(n_2_Vtnode_GND/n_w)*_DDT((n_w*NP(n1))));
_load_dynamic_jacobian4(bi,ei,ci,ei,(n_2_Vci_ei/n_w)*_DDT((n_w*NP(n1))));
_load_dynamic_jacobian4(bi,ei,bi,ei,(n_2_Vbi_ei/n_w)*_DDT((n_w*NP(n1))));
_load_dynamic_jacobian2s(bi,ei,n1,(1.0+((n_2/n_w)*(n_w))));
#endif
#endif
#if defined(_DYNAMIC)
_load_dynamic_residual2(bi,ei,((n_1/n_w)*_DDT((n_w*NP(n2)))));
#if defined(_DERIVATE)
_load_dynamic_jacobian2s(bi,ei,n2,((n_1/n_w)*(n_w)));
_load_dynamic_jacobian4(bi,ei,bi,ci,(n_1_Vbi_ci/n_w)*_DDT((n_w*NP(n2))));
_load_dynamic_jacobian2s(bi,ei,tnode,(n_1_Vtnode_GND/n_w)*_DDT((n_w*NP(n2))));
_load_dynamic_jacobian4(bi,ei,ci,ei,(n_1_Vci_ei/n_w)*_DDT((n_w*NP(n2))));
_load_dynamic_jacobian4(bi,ei,bi,ei,(n_1_Vbi_ei/n_w)*_DDT((n_w*NP(n2))));
#endif
#endif
_save_whitenoise1(n2,((2*1.602176462e-19)*it),"shot");
_load_static_residual1(n2,(-NP(n2)));
#if defined(_DERIVATE)
_load_static_jacobian1(n2,n2,(-1.0));
#endif
_load_static_residual2(ci,ei,NP(n2));
#if defined(_DERIVATE)
_load_static_jacobian2s(ci,ei,n2,1.0);
#endif
}
else
{
_save_whitenoise2(ci,ei,(twoq*it),"shot");
_save_whitenoise2(bi,ei,(twoq*ibei),"shot");
_load_static_residual1(n1,NP(n1));
#if defined(_DERIVATE)
_load_static_jacobian1(n1,n1,1.0);
#endif
_load_static_residual1(n2,NP(n2));
#if defined(_DERIVATE)
_load_static_jacobian1(n2,n2,1.0);
#endif
}
}

/* ------------------- end of verilog noise equations --------------------- */
}

/* Perform DC iteration. */
void hicumL2V2p31n::calcDC (void)
{
  // evaluate Verilog code
  initVerilog ();
  calcVerilog ();

  // fill right hand side and static jacobian
  for (int i1 = 0; i1 < 15; i1++) {
    setI (i1, _rhs[i1]);
    for (int i2 = 0; i2 < 15; i2++) {
      setY (i1, i2, _jstat[i1][i2]);
    }
  }
}

/* Save operating points. */
void hicumL2V2p31n::saveOperatingPoints (void)
{
  // save global instance operating points
  setOperatingPoint ("it", it);
  setOperatingPoint ("ibei", ibei);
  setOperatingPoint ("ibci", ibci);
  setOperatingPoint ("ibep", ibep);
  setOperatingPoint ("iavl", iavl);
  setOperatingPoint ("ijbcx", ijbcx);
  setOperatingPoint ("ijsc", ijsc);
  setOperatingPoint ("rbi", rbi);
  setOperatingPoint ("Tf", Tf);
  setOperatingPoint ("betadc", betadc);
}

/* Load operating points. */
void hicumL2V2p31n::loadOperatingPoints (void)
{
}

/* Calculate operating points. */
void hicumL2V2p31n::calcOperatingPoints (void)
{
}

/* Initialization of AC analysis. */
void hicumL2V2p31n::initAC (void)
{
  allocMatrixMNA ();
}

/* Perform AC calculations. */
void hicumL2V2p31n::calcAC (nr_double_t frequency)
{
  setMatrixY (calcMatrixY (frequency));
}

/* Compute Y-matrix for AC analysis. */
matrix hicumL2V2p31n::calcMatrixY (nr_double_t frequency)
{
  _freq = frequency;
  saveOperatingPoints ();
  matrix y (15);

  for (int i1 = 0; i1 < 15; i1++) {
    for (int i2 = 0; i2 < 15; i2++) {
      y (i1,i2) = nr_complex_t (_jstat[i1][i2], _jdyna[i1][i2] * 2 * M_PI * _freq);
    }
  }

  return y;
}

/* Initialization of S-parameter analysis. */
void hicumL2V2p31n::initSP (void)
{
  allocMatrixS ();
}

/* Perform S-parameter calculations. */
void hicumL2V2p31n::calcSP (nr_double_t frequency)
{
  setMatrixS (ytos (calcMatrixY (frequency)));
}

/* Initialization of transient analysis. */
void hicumL2V2p31n::initTR (void)
{
  setStates (2 * 15 * 15);
  initDC ();
}

/* Perform transient analysis iteration step. */
void hicumL2V2p31n::calcTR (nr_double_t)
{
  doHB = 0;
  doAC = 1;
  doTR = 1;
  calcDC ();

  int i1, i2, i3, i4, state;

  // 2-node charge integrations
  for (i1 = 0; i1 < 15; i1++) {
  for (i2 = 0; i2 < 15; i2++) {
    state = 2 * (i2 + 15 * i1);
    if (i1 != i2)
    if (_charges[i1][i2] != 0.0)
      transientCapacitanceQ (state, i1, i2, _charges[i1][i2]);
  } }

  // 1-node charge integrations
  for (i1 = 0; i1 < 15; i1++) {
    state = 2 * (i1 + 15 * i1);
    if (_charges[i1][i1] != 0.0)
      transientCapacitanceQ (state, i1, _charges[i1][i1]);
  }

  // charge: 2-node, voltage: 2-node
  for (i1 = 0; i1 < 15; i1++) {
  for (i2 = 0; i2 < 15; i2++) {
  if (i1 != i2)
  for (i3 = 0; i3 < 15; i3++) {
  for (i4 = 0; i4 < 15; i4++) {
    if (i3 != i4)
    if (_caps[i1][i2][i3][i4] != 0.0)
      transientCapacitanceC (i1, i2, i3, i4, _caps[i1][i2][i3][i4], BP(i3,i4));
  } } } }

  // charge: 2-node, voltage: 1-node
  for (i1 = 0; i1 < 15; i1++) {
  for (i2 = 0; i2 < 15; i2++) {
  if (i1 != i2)
  for (i3 = 0; i3 < 15; i3++) {
    if (_caps[i1][i2][i3][i3] != 0.0)
      transientCapacitanceC2Q (i1, i2, i3, _caps[i1][i2][i3][i3], NP(i3));
  } } }

  // charge: 1-node, voltage: 2-node
  for (i1 = 0; i1 < 15; i1++) {
  for (i3 = 0; i3 < 15; i3++) {
  for (i4 = 0; i4 < 15; i4++) {
    if (i3 != i4)
    if (_caps[i1][i1][i3][i4] != 0.0)
      transientCapacitanceC2V (i1, i3, i4, _caps[i1][i1][i3][i4], BP(i3,i4));
  } } }

  // charge: 1-node, voltage: 1-node
  for (i1 = 0; i1 < 15; i1++) {
  for (i3 = 0; i3 < 15; i3++) {
    if (_caps[i1][i1][i3][i3] != 0.0)
      transientCapacitanceC (i1, i3, _caps[i1][i1][i3][i3], NP(i3));
  } }
}

/* Compute Cy-matrix for AC noise analysis. */
matrix hicumL2V2p31n::calcMatrixCy (nr_double_t frequency) 
{
  _freq = frequency;
  matrix cy (15);

  _load_flickernoise2 (ei,e,_flicker_pwr[ei][e],_flicker_exp[ei][e]);
  _load_flickernoise2 (bp,ei,_flicker_pwr[bp][ei],_flicker_exp[bp][ei]);
  _load_flickernoise2 (bi,ei,_flicker_pwr[bi][ei],_flicker_exp[bi][ei]);
  _load_whitenoise1 (n2,_white_pwr[n2][n2]);
  _load_whitenoise1 (n1,_white_pwr[n1][n1]);
  _load_whitenoise2 (ci,bi,_white_pwr[ci][bi]);
  _load_whitenoise2 (si,s,_white_pwr[si][s]);
  _load_whitenoise2 (ei,e,_white_pwr[ei][e]);
  _load_whitenoise2 (ci,c,_white_pwr[ci][c]);
  _load_whitenoise2 (si,ci,_white_pwr[si][ci]);
  _load_whitenoise2 (bp,ei,_white_pwr[bp][ei]);
  _load_whitenoise2 (bp,bi,_white_pwr[bp][bi]);
  _load_whitenoise2 (b,bp,_white_pwr[b][bp]);
  _load_whitenoise2 (bp,ci,_white_pwr[bp][ci]);
  _load_whitenoise2 (ci,ei,_white_pwr[ci][ei]);
  _load_whitenoise2 (bi,ci,_white_pwr[bi][ci]);
  _load_whitenoise2 (bi,ei,_white_pwr[bi][ei]);

  return cy;
}

/* Perform AC noise computations. */
void hicumL2V2p31n::calcNoiseAC (nr_double_t frequency) 
{
  setMatrixN (calcMatrixCy (frequency));
}

/* Perform S-parameter noise computations. */
void hicumL2V2p31n::calcNoiseSP (nr_double_t frequency) 
{
  setMatrixN (cytocs (calcMatrixCy (frequency) * z0, getMatrixS ()));
}

/* Initialization of HB analysis. */
void hicumL2V2p31n::initHB (int)
{
  initDC ();
  allocMatrixHB ();
}

/* Perform HB analysis. */
void hicumL2V2p31n::calcHB (int)
{
  doHB = 1;
  doAC = 1;
  doTR = 0;

  // jacobian dI/dV and currents get filled
  calcDC ();
  saveOperatingPoints ();

  // fill in HB matrices
  for (int i1 = 0; i1 < 15; i1++) {
    setQ  (i1, _qhs[i1]); // charges
    setCV (i1, _chs[i1]); // jacobian dQ/dV * V
    setGV (i1, _ghs[i1]); // jacobian dI/dV * V
    for (int i2 = 0; i2 < 15; i2++) {
      setQV (i1, i2, _jdyna[i1][i2]); // jacobian dQ/dV
    }
  }
}

#include "hicumL2V2p31n.defs.h"

