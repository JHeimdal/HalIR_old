#pragma once

template<class T>
inline T SIGN(const T &a, const T &b)
	{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

inline float SIGN(const float &a, const double &b)
	{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

inline float SIGN(const double &a, const float &b)
	{return (float)(b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a));}
