/*
 *  ofxSunrise.h
 *
 *  Created by mar Canet sola on 7/15/11.
 *  Copyright 2011 student. All rights reserved.
 *
 */
#ifndef _OFXSUNRISE_H
#define _OFXSUNRISE_H

#include "ofMain.h"

class ofxSunrise
{
public:
	ofxSunrise();
	~ofxSunrise();
	void setData( double latit, double longit, double tzone);
	double getSunrise();
	double getSunset();
private:
	double FNday (int y, int m, int d, float h);
	double FNrange (double x);
	double f0(double lat, double declin);
	double f1(double lat, double declin);
	double FNsun (double d);
	void showhrmn(double dhr);
private:
	double settm;
	double riset;
	double tPI;
	double degs;
	double rads;
	double L,g,daylen;
	double SunDia;  // Sunradius degrees
	double AirRefr; // athmospheric refraction degrees //
};

#endif