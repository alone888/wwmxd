/*
 * vcresistor.defs.h - device definition header
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 */

#ifndef __vcresistor_DEFS_H__
#define __vcresistor_DEFS_H__


// required properties
PROP_REQ [] = {
  { "gain", PROP_REAL, { 1, PROP_NO_STR }, PROP_MIN_VAL (1e-6) },
  PROP_NO_PROP };

// optional properties
PROP_OPT [] = {
  { "Temp", PROP_REAL, { 26.85, PROP_NO_STR }, PROP_MIN_VAL (K) },
  PROP_NO_PROP };

// device definition
struct define_t vcresistor::cirdef =
  { "vcresistor",
    4, PROP_COMPONENT, PROP_NO_SUBSTRATE, PROP_NONLINEAR, PROP_DEF };

#endif /* __vcresistor_DEFS_H__ */
