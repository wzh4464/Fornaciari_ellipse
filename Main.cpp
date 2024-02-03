#include <opencv2/opencv.hpp>

#include "EllipseDetectorYaed.h"
#include <fstream>


using namespace std;
using namespace cv;




//void OnVideo()
//{
//	VideoCapture cap(0);
//	if(!cap.isOpened()) return;
//
//	CEllipseDetectorYaed yaed;
//
//	Mat1b gray;
//	while(true)
//	{	
//		Mat3b image;
//		cap >> image;
//		cvtColor(image, gray, CV_BGR2GRAY);	
//
//		vector<CEllipse> ellipses;
//
//		//Find Ellipses		
//		yaed.Detect(gray, ellipses);
//		yaed.DrawDetectedEllipses(image,ellipses);
//		imshow("Output", image);
//
//			
//		if(waitKey(10) >= 0) break;
//	}
//}

void OnImage()
{
	string imageName = "D:/Dataset/img/ellipse0.jpg";

	Mat3b image = imread(imageName);
	Size sz = image.size();

	// To grayscale
	Mat1b gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	

	// Parameters Settings
	Size	szPreProcessingGaussKernelSize	= Size(5,5);
	double	dPreProcessingGaussSigma		= 1.0;
	float	fThPosition						= 1.0f;
	float	fCenterDistance					= 0.05f;
	float	fMaxCenterDistance				= sqrt(float(sz.width*sz.width + sz.height*sz.height)) * fCenterDistance;
	int		iMinEdgeLength					= 16;
	float	fMinOrientedRectSide			= 3.0f;
	float	fDistanceToEllipseContour		= 0.1f;
	float	fMinScore						= 0.4f;
	float	fMinReliability					= 0.4f;
	int 	iNs								= 16;

			
	CEllipseDetectorYaed yaed;
	yaed.SetParameters	(	szPreProcessingGaussKernelSize,	
							dPreProcessingGaussSigma,		
							fThPosition,
							fMaxCenterDistance,
							iMinEdgeLength,		
							fMinOrientedRectSide,	
							fDistanceToEllipseContour,		
							fMinScore,		
							fMinReliability,		
							iNs
						);
		

	vector<CEllipse> ellsYaed;

	cout << "Yaed : ";

	// define a gray.clone as left
	Mat1b gray_yaed = gray.clone();

	yaed.Detect(gray_yaed, ellsYaed);
	double exec_time_yaed = yaed.GetExecTime();
	cout  << exec_time_yaed << endl;
	
	Mat3b imageYaed = image.clone();
	yaed.DrawDetectedEllipses(imageYaed, ellsYaed);
	imshow("Yaed", imageYaed);	 
	waitKey();
}

void SaveEllipses(const string& workingDir, const string& method, const string& imgName, vector<CEllipse>& ellipses, double alghTime, double runTime)
{
	string path(workingDir + method + "/" + imgName + ".txt");
	ofstream out(path, ofstream::out | ofstream::trunc);
	if(!out.good())
	{
		cout << "Error saving: " << path << endl;
		return;
	}

	unsigned n = ellipses.size();
	//first line is the execution time
	out << alghTime << "\t" << runTime << "\n";
	//second line is the number of ellipses
	out << n << "\n";
	for(unsigned i=0; i<n; ++i)
	{
		CEllipse& e = ellipses[i];
		out << e._xc << "\t" << e._yc << "\t" << e._A << "\t" << e._B << "\t" << e._rho << "\t" << e._score << "\t" << e._rel << "\n";
	}
	out.close();
}

void LoadGT(vector<CEllipse>& gt, const string& gtFileName, bool bIsAngleInRadians=true)
{
	ifstream in(gtFileName);
	if(!in.good())
	{
		cout << "Error opening: " << gtFileName << endl;
		return;
	}

	unsigned n;
	in >> n;

	gt.clear();
	gt.reserve(n);
	
	while(in.good() && n--)
	{
		CEllipse e;
		in >> e._xc >> e._yc >> e._A >> e._B >> e._rho;

		if(!bIsAngleInRadians)
		{
			e._rho = float(e._rho * CV_PI / 180.0);
		}

		if(e._A < e._B)
		{
			float temp = e._A;
			e._A = e._B;
			e._B = temp;

			e._rho = e._rho + float(0.5*CV_PI);
		}

		e._rho = fmod(float(e._rho + 2.f*CV_PI), float(CV_PI));
		e._score = 1.f;
		e._rel   = 1.f;
		gt.push_back(e);
	}
	in.close();	
}

void LoadTest(const string& sTestName, vector<CEllipse>& ellipses, double& dTimeAlg, double& dTimeTot, bool isRadians = true)
{
	ifstream in(sTestName);
	if(!in.good())
	{
		cout << "Error opening: " << sTestName << endl;
		return;
	}

	in >> dTimeAlg >> dTimeTot;

	unsigned n;
	in >> n;

	ellipses.clear();
	ellipses.reserve(n);	
				
	while(in.good() && n--)
	{
		CEllipse e;
		in >> e._xc >> e._yc >> e._A >> e._B >> e._rho >> e._score >> e._rel;

		if(!isRadians)
		{
			e._rho = e._rho * float(CV_PI / 180.0);
		}

		e._rho = fmod(float(e._rho + 2.0*CV_PI), float(CV_PI));

		if(e._A>0 && e._B>0 && e._rho>=0)
		{
			ellipses.push_back(e);
		}
	}
	in.close();
}






int main(int argc, char** argv)
{
	//OnVideo();
	OnImage();

	return 0;	   
}
