/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper:

Michele Fornaciari, Andrea Prati,
Very Fast Ellipse Detecetion for Embedded Vision Applications
in Proceedings of 6th ACM/IEEE International Conference on Distributed Smart Cameras, 
Hong Kong, Oct. 30 - Nov 2, 2012


The comments in the code refer to a paper currently under review, where the algorithm is presented in detail.
Until the paper is (hopefully) accepted, please refer to the above mentioned paper for the algorithm details.
If you need further details about the code or the algorithm, please contact me at:

michele.fornaciari@unimore.it

last update: 11/07/2013
*/


/*
Data structure for an ellipse.

	float _xc		= center x-coordinate 
	float _yc		= center y-coordinate
	float _A		= major semi-axis length
	float _B		= minor semi-axis length
	float _rho		= orientation (in radians)
	float _score	= score, in interval [0,1]. Computed as formula [] in the paper.
	float _rel		= auxiliary measure of detection quality
*/


#pragma once

#include <opencv2/core/types_c.h>

class CEllipse
{

public:

	float _xc;
	float _yc;
	float _A;
	float _B;
	float _rho;
	float _score;
	float _rel;

	//Constructors
	CEllipse(void);
	CEllipse(const CEllipse& other);
	CEllipse(float xc, float yc, float A, float B, float rho);
	CEllipse(float xc, float yc, float A, float B, float rho, float score, float rel);
	~CEllipse(void);
	

	//Used for sorting ellipses. 
	bool operator<(const CEllipse& other) const;
	

	//Approximation of ellipse perimeter.
	float GetPerimeter() const;
};

