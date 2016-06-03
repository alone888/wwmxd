/*
 * dff_SR.core.h - device class definitions for dff_SR module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 */

#ifndef __dff_SR_H__
#define __dff_SR_H__

class dff_SR : public qucs::circuit
{
 public:
  CREATOR (dff_SR);

  void initDC (void);
  void restartDC (void);
  void calcDC (void);
  void saveOperatingPoints (void);
  void loadOperatingPoints (void);
  void calcOperatingPoints (void);
  void initAC (void);
  void calcAC (nr_double_t);
  void initSP (void);
  void calcSP (nr_double_t);
  void calcNoiseAC (nr_double_t);
  void calcNoiseSP (nr_double_t);
  void initTR (void);
  void calcTR (nr_double_t);
  void initHB (int);
  void calcHB (int);

 private:
  void initVerilog (void);
  void calcVerilog (void);
  void initModel (void);
  void loadVariables (void);
  void initializeModel (void);
  void initializeInstance (void);
  void initialStep (void);
  void finalStep (void);
  qucs::matrix calcMatrixY (nr_double_t);
  qucs::matrix calcMatrixCy (nr_double_t);

 private:
  int doHB;
  int doAC;
  int doTR;

  nr_double_t _freq;
  nr_double_t _rhs[13];
  nr_double_t _qhs[13];
  nr_double_t _ghs[13];
  nr_double_t _chs[13];
  nr_double_t _jstat[13][13];
  nr_double_t _jdyna[13][13];
  nr_double_t _white_pwr[13][13];
  nr_double_t _flicker_pwr[13][13];
  nr_double_t _flicker_exp[13][13];
  nr_double_t _charges[13][13];
  nr_double_t _caps[13][13][13][13];

  // device model parameters
  nr_double_t TR_H;
  nr_double_t TR_L;
  nr_double_t Delay;

  // global model variables
  double Rd;
  double Ccc;



};

#endif /* __dff_SR_H__ */