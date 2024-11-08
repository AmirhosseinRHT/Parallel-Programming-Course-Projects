#include <opencv2/opencv.hpp> 
#include <stdio.h> 
#include <intrin.h>
#include <iostream>
#include <vector>
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


	int frame_width = int(capture.get(3));
	int frame_height = int(capture.get(4));
	double fps =  capture.get(CAP_PROP_FPS);  
	std::cout << "fps: " << fps<< std::endl;
	VideoWriter out_video("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/codes/Q4/Q4ser.avi",VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width,frame_height),false);
	
	std::vector<Mat> frames;
	Mat frame;
	for(; ; ){
		capture >> frame;
		if(frame.empty()){
            break;
		}
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		frames.push_back(frame);
	}

    std::vector<Mat> out_frames(frames.size() - 1);  
    for (size_t i = 0; i < out_frames.size(); i++) {  
        out_frames[i] = Mat::zeros(frames[0].size(), frames[0].type());  
    } 

	start = micros();
    for(int i =1;i < frames.size();i++)
    {
        frame1 = frames[i-1];
        frame2 = frames[i];

		uchar* f1 = frame1.ptr<uchar>();
		uchar* f2 = frame2.ptr<uchar>();
		uchar* out = out_frames[i-1].ptr<uchar>();
		int width = frame1.cols;
    	int height = frame1.rows;
		for (int i = 0; i < height; i++) { 
			for (int j = 0; j < width; j++) { 
				int index = i * width + j;
				short val1 = static_cast<short>(*(f2+index));
				short val2 = -static_cast<short>(*(f1 + index));
				short value =static_cast<short>(val1 + val2);
				uchar finalVal =  (value ^ (value >> 15)) - (value >> 15);
				out[index] =  finalVal;
			} 
		}
	}
	end = micros();
	for(int i=0; i < out_frames.size(); i++){
		out_video.write(out_frames[i]);
	}
	capture.release();
	out_video.release();
	return end-start;
}



long long parallel(std::string filename){
	long long start, end;
    VideoCapture capture(filename);
    Mat frame1;
	Mat frame2;
    if( !capture.isOpened())
        throw "Error when reading steam_avi";


	int frame_width = int(capture.get(3));
	int frame_height = int(capture.get(4));
	double fps =  capture.get(CAP_PROP_FPS);  
	std::cout << "fps: " << fps<< std::endl;
	VideoWriter out_video("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/codes/Q4/Q4par.avi",VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width,frame_height),false);
	
	std::vector<Mat> frames;
	Mat frame;
	for(; ; ){
		capture >> frame;
		if(frame.empty()){
            break;
		}
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		frames.push_back(frame);
	}

	Mat out_frame;
	
    std::vector<Mat> out_frames(frames.size() - 1);  
    for (size_t i = 0; i < out_frames.size(); i++) {  
        out_frames[i] = Mat::zeros(frames[0].size(), frames[0].type());  
    }

	start = micros();
    for(int i =1;i < frames.size();i++)
    {
        frame1 = frames[i-1];
        frame2 = frames[i];

		uchar* f1 = frame1.ptr<uchar>();
		uchar* f2 = frame2.ptr<uchar>();
		uchar* out = out_frames[i-1].ptr<uchar>();
		int width = frame1.cols;
    	int height = frame1.rows;
		for (int i = 0; i < height; i++) { 
			for (int j = 0; j < width; j+=8) { 
				int index = i * width + j;
				__m128i m1 = _mm_loadu_si128((__m128i*) (f1 + index));
				__m128i m2 = _mm_loadu_si128((__m128i*) (f2 + index));
				m1 = _mm_sub_epi16(_mm_setzero_si128(),_mm_cvtepu8_epi16(m1));
				m1 = _mm_adds_epi16(m1, _mm_cvtepu8_epi16(m2));
				m1 = _mm_abs_epi16(m1);
				m1 = _mm_packus_epi16(m1,m1);
				_mm_storeu_si128((__m128i*) (out + index), m1);
			}

		}
	}
	end = micros();
	for(int i=0; i < out_frames.size(); i++){
		out_video.write(out_frames[i]);
	}
	capture.release();
	out_video.release();
	return end-start;
}

int main(int argc, char** argv) 
{ 
	long long ser_time = serial("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/assets/Q4/Q4.mp4");
	long long par_time = parallel("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/assets/Q4/Q4.mp4");
	std::cout << "Serial Time: " <<ser_time <<std::endl;
	std::cout << "parallel Time: " <<par_time <<std::endl;
	std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
 	return 0;
}
