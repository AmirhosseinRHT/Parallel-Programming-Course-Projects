#include <opencv2/opencv.hpp> 
#include <stdio.h> 
#include <intrin.h>
#include <iostream>
#include <chrono>
using namespace cv; 

uint64_t micros()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

long long serial(std::string filename){
	long long start, end;
    VideoCapture capture(filename);
    Mat frame1;
	Mat frame2;
    if( !capture.isOpened())
        throw "Error when reading steam_avi";

	Mat out_frame;

	int frame_width = int(capture.get(3));
	int frame_height = int(capture.get(4));
	double fps =  capture.get(CAP_PROP_FPS);  
	std::cout << "fps: " << fps<< std::endl;
	VideoWriter out_video("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/codes/Q4/Q4output.avi",VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width,frame_height),false);
	
	start = micros();
	int count = 0;
    for( ; ; )
    {
        capture >> frame1;
        capture >> frame2;
        if(frame1.empty() || frame2.empty()){
            break;
		}
		cvtColor(frame1, frame1, COLOR_BGR2GRAY);
		cvtColor(frame2, frame2, COLOR_BGR2GRAY);

		uchar* f1 = frame1.ptr<uchar>();
		uchar* f2 = frame2.ptr<uchar>();
		out_frame.create(frame1.size(), frame1.type());
		uchar* out = out_frame.ptr<uchar>(0);
		int width = frame1.cols;
    	int height = frame1.rows;
		for (int i = 0; i < height; i++) { 
			for (int j = 0; j < width; j++) { 
				int index = i * width + j;
				short val1 = static_cast<short>(*(f2+index));
				short val2 = -static_cast<short>(*(f1 + index));
				short value = saturate_cast<short>(static_cast<short>(val1 + val2));
				uchar finalVal =  std::abs(value);
				out[index] =  finalVal;
				} 
		}
		out_video.write(out_frame);
	}
	end = micros();
	capture.release();
	out_video.release();
	return end-start;
}



long parallel(Mat image1,Mat paddedImage){
	return 0;
}

int main(int argc, char** argv) 
{ 
	double ser_time = serial("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/assets/Q4/Q4.mp4");
	std::cout << "Serial Time: " <<ser_time <<std::endl;
	// std::cout << "parallel Time: " <<par_time <<std::endl;
	// std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
 	return 0;
}
