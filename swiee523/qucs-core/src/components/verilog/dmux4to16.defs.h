/*
 * dmux4to16.defs.h - device definition header
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 */

#ifndef __dmux4to16_DEFS_H__
#define __dmux4to16_DEFS_H__


// required properties
PROP_REQ [] = {
  { "TR", PROP_REAL, { 6, PROP_NO_STR }, PROP_RNGII (1.0, 20.0) },
  { "Delay", PROP_REAL, { (1*1.0e-9), PROP_NO_STR }, PROP_MIN_VAL (0) },
  PROP_NO_PROP };

// optional properties
PROP_OPT [] = {
  { "Temp", PROP_REAL, { 26.85, PROP_NO_STR }, PROP_MIN_VAL (K) },
  PROP_NO_PROP };

// device definition
struct define_t dmux4to16::cirdef =
  { "dmux4to16",
    21, PROP_COMPONENT, PROP_NO_SUBSTRATE, PROP_NONLINEAR, PROP_DEF };

#endif /* __dmux4to16_DEFS_H__ */
