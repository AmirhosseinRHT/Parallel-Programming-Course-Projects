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

long long serial(Mat image1,Mat paddedImage){
	long long start, end;
	Mat outputImage;
	outputImage.create(image1.size(), image1.type());

	uchar* out = outputImage.ptr<uchar>();
	uchar* front = image1.ptr<uchar>();
	uchar* logo = paddedImage.ptr<uchar>();

	int width = image1.cols;
    int height = image1.rows;

	start = micros();
	for (int i = 0; i < height; i++) { 
		for (int j = 0; j < width; j++) { 
			for(int c=0; c < 3; c++){
			int index = (i * width + j)*3 + c; 
			short logo_value =(uchar)((((short)*(logo + index))* 5) >> 3);
			out[index] = saturate_cast<uchar>(*(front + index) +logo_value) ;
			}
		} 
	}
	end = micros();
	imwrite("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/codes/Q1/ser_out.png", outputImage);
	return end-start;
}



long long parallel(Mat image1,Mat paddedImage){
	long long start, end;
	Mat outputImage;
	outputImage.create(image1.size(), image1.type());

	uchar* out = outputImage.ptr<uchar>();
	uchar* front = image1.ptr<uchar>();
	uchar* logo = paddedImage.ptr<uchar>();

	int width = image1.cols;
    int height = image1.rows;
	__m128i m1,m2;
	start = micros();


	for (int i = 0; i < height; i++) { 
		for (int j = 0; j < width*3; j+=8) { 
			int index = (i *width*3) + j;
			// std::cout << index <<std::endl;
			m1 = _mm_loadu_si128((__m128i*) (front + index));  
			m2 = _mm_loadu_si128((__m128i*)(logo+index));
			m2 = _mm_mullo_epi16(_mm_cvtepu8_epi16(m2), _mm_set1_epi16(5));
			m2 = _mm_srli_epi16(m2, 3);
			__m128i m1_16 = _mm_cvtepu8_epi16(m1);
			__m128i result_16 = _mm_adds_epu16(m1_16, m2);
			__m128i result = _mm_packus_epi16(result_16, result_16);
			_mm_storeu_si128((__m128i*)(out + index), result); 
		} 
	}
	end = micros();

	
	imwrite("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/codes/Q1/par_out.png", outputImage);
	return end - start;
}

int main(int argc, char** argv) 
{ 

	Mat image1,image2; 

	image1 = imread("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/assets/Q1/front.png"); 
	image2 = imread("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA1 - SIMD/assets/Q1/logo.png");
	cv::Mat paddedImage; 
	cv::copyMakeBorder(image2, paddedImage, 0, image1.rows-image2.rows, 0, image1.cols-image2.cols, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
	long long ser_time = serial(image1,paddedImage);
	long long par_time = parallel(image1,paddedImage);
	std::cout << "Serial Time: " <<ser_time <<std::endl;
	std::cout << "parallel Time: " <<par_time <<std::endl;
	std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
 	return 0;
}
