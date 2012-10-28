#include "ofxSunrise.h"

ofxSunrise::ofxSunrise()
{
	tPI = 2 * PI;
	degs = 180.0/PI;
	rads = PI/180.0;
	SunDia = 0.53;  // Sunradius degrees
	AirRefr = 34.0/60.0; // athmospheric refraction degrees //
}

ofxSunrise::~ofxSunrise()
{

}

double ofxSunrise::FNday (int y, int m, int d, float h) {
	long int luku = - 7 * (y + (m + 9)/12)/4 + 275*m/9 + d;
	// Typecasting needed for TClite on PC DOS at least, to avoid product overflow
	luku+= (long int)y*367;
	return (double)luku - 730531.5 + h/24.0;
}

// the function below returns an angle in the range
// 0 to 2*PI

double ofxSunrise::FNrange (double x) {
    double b = x / tPI;
    double a = tPI * (b - (long)(b));
    if (a < 0) a = tPI + a;
    return a;
}

// Calculating the hourangle
double ofxSunrise::f0(double lat, double declin) {
	double fo,dfo;
	// Correction: different sign at S HS
	dfo = rads*(0.5*SunDia + AirRefr); if (lat < 0.0) dfo = -dfo;
	fo = tan(declin + dfo) * tan(lat*rads);

	if (fo > 0.99999) fo=1.0; // to avoid overflow //
	fo = asin(fo) + PI/2.0;
	return fo;
}

// Calculating the hourangle for twilight times
//
double ofxSunrise::f1(double lat, double declin) {
	double fi,df1;
	// Correction: different sign at S HS
	df1 = rads * 6.0; if (lat < 0.0) df1 = -df1;
	fi = tan(declin + df1) * tan(lat*rads);

	if (fi > 0.99999) fi=1.0; // to avoid overflow //
	fi = asin(fi) + PI/2.0;
	return fi;
}

// Find the ecliptic longitude of the Sun
double ofxSunrise::FNsun (double d) {

	// mean longitude of the Sun
	L = FNrange(280.461 * rads + .9856474 * rads * d);

	// mean anomaly of the Sun
	g = FNrange(357.528 * rads + .9856003 * rads * d);

	// Ecliptic longitude of the Sun
	return FNrange(L + 1.915 * rads * sin(g) + .02 * rads * sin(2 * g));
}

// Display decimal hours in hours and minutes
void ofxSunrise::showhrmn(double dhr) {
	int hr,mn;
	hr = (int) dhr;
	mn = (dhr - (double) hr)*60;
	if (hr < 10) cout << '0';
	cout << hr << ':';
	if (mn < 10) cout << '0';
	cout << mn;
}



//riset;
//settm


void ofxSunrise::setData( double latit, double longit, double tzone){
	double y,m,day,h;//,latit,longit,tzone;

	time_t sekunnit;
	struct tm *p;

	// get the date and time from the user
	// read system date and extract the year

	/** First get current time **/
	time(&sekunnit);

	/** Next get localtime **/

	p=localtime(&sekunnit);
	// this is Y2K compliant algorithm
	y = 1900 + p->tm_year;

	m = p->tm_mon + 1;
	day = p->tm_mday;
	h = 12;

	cout << "Input latitude, longitude and timezone\n";
	double d = FNday(y, m, day, h);

	// Use FNsun to find the ecliptic longitude of the
	// Sun
	double lambda = FNsun(d);

	// Obliquity of the ecliptic
	double obliq = 23.439 * rads - .0000004 * rads * d;

	// Find the RA and DEC of the Sun
	double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
	double delta = asin(sin(obliq) * sin(lambda));

	// Find the Equation of Time in minutes
	// Correction suggested by David Smith
	double LL = L - alpha;
	if (L < PI) LL += tPI;
	double equation = 1440.0 * (1.0 - LL / tPI);
	double ha = f0(latit,delta);
	double hb = f1(latit,delta);
	double twx = hb - ha;   // length of twilight in radians
	twx = 12.0*twx/PI;      // length of twilight in degrees

	// Conversion of angle to hours and minutes //
	daylen = degs * ha / 7.5;
	if (daylen<0.0001) {daylen = 0.0;}
	// arctic winter   //

	riset = 12.0 - 12.0 * ha/PI + tzone - longit/15.0 + equation/60.0;
	settm = 12.0 + 12.0 * ha/PI + tzone - longit/15.0 + equation/60.0;
	double noont = riset + 12.0 * ha/PI;
	double altmax = 90.0 + delta * degs - latit;
	// Correction suggested by David Smith
	// to express as degrees from the N horizon

	if (delta * degs > latit ) altmax = 90.0 + latit - delta * degs;

	double twam = riset - twx;      // morning twilight begin
	double twpm = settm + twx;      // evening twilight end

	if (riset > 24.0) riset-= 24.0;
	if (settm > 24.0) settm-= 24.0;

	cout << "\n Sunrise and set\n";
	cout << "===============\n";

	cout.setf(ios::fixed);
	cout.precision(0);

	cout << "  year  : " << y << '\n';
	cout << "  month : " << m << '\n';
	cout << "  day   : " << day << "\n\n";
	cout << "Days until Y2K :  " << -d << '\n';

	cout.precision(2);
	cout << "Latitude :  " << latit << ", longitude:  " << longit << '\n';
	cout << "Timezone :  " << tzone << "\n\n";
	cout << "Declination : " << delta * degs << '\n';
	cout << "Daylength   : "; showhrmn(daylen); cout << " hours \n";

	cout << "Begin civil twilight: ";
	showhrmn(twam); cout << '\n';

	cout << "Sunrise     : ";
	showhrmn(riset); cout << '\n';

	cout << "Sun altitude at noontime ";

	// Correction by D. Smith
	showhrmn(noont); cout << " = " << altmax << " degrees"
    << (latit>= delta * degs ? "S" : "N")<< endl;

	cout << "Sunset      : ";
	showhrmn(settm); cout << '\n';
	cout << "Civil twilight: ";
}


double ofxSunrise::getSunrise()
{
	return riset;
}

double ofxSunrise::getSunset()
{
	return settm;
}