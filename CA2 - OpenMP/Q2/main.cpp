#include <opencv2/opencv.hpp> 
#include <stdio.h> 
#include <intrin.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <omp.h>

uint64_t micros()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}


long long serial(double c_i, double c_j){
    long long start, end;
    cv::Mat image(800, 800, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();

    double z_i,z_c;
     for (int i = 0; i < 800; i++) {
        for (int j = 0; j < 800; j++) {
            double x = -2.0 + (i * (4.0)) / 800.0;
            double y = -2.0 + (j * (4.0)) / 800.0;
            bool converge = true;
            z_i = x;
            z_c = y;
            for(int itr =0; itr < 1000; itr++){
                double temp_i = pow(z_i,2) - pow(z_c,2) + c_i;
                double temp_c = (2 * z_i * z_c) + c_j;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }

            }
            if (converge){
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(90, 0, 44);
            }
        }
    }
	end = micros();
    // cv::imshow("Mandelbrot Set", image);
    // cv::waitKey(0);
    cv::imwrite("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA2 - OpenMP/Q2/ser.png", image);
    return end - start;
}


long long parallel(double c_i,double c_j){
    long long start, end;
    cv::Mat image(800, 800, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();
    double x,y;
    double z_i,z_c;
    bool converge;
    double temp_i,temp_c;
    #pragma omp parallel for collapse(2) shared(image,c_i,c_j) private(z_i,z_c,x,y,converge,temp_i,temp_c)
    for (int i = 0; i < 800; i++) {
        for (int j = 0; j < 800; j++) {
            x = -2.0 + (i * (4.0)) / 800.0;
            y = -2.0 + (j * (4.0)) / 800.0;
            converge = true;
            z_i = x;
            z_c = y;
            for(int itr =0; itr < 50; itr++){
                temp_i = pow(z_i,2) - pow(z_c,2) + c_i;
                temp_c = (2 * z_i * z_c) + c_j;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }

            }
            if (converge){
                image.at<cv::Vec3b>(j,i) = cv::Vec3b(90, 0, 44);
            }
        }
    }
	end = micros();
    // cv::imshow("Mandelbrot Set", image);
    // cv::waitKey(0);
    cv::imwrite("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA2 - OpenMP/Q2/par.png", image);
    return end - start;
}


int main(){
    long long ser_time = serial(0.285,0.01);
	long long par_time = parallel(0.285,0.01);
    // long long ser_time = serial(0.0,0.0);
	// long long par_time = parallel(0.0,0.0);

	std::cout << "Serial Time: " <<ser_time <<std::endl;
	std::cout << "parallel Time: " <<par_time <<std::endl;
	std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
    return 0;
}