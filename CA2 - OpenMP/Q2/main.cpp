#include <opencv2/opencv.hpp> 
#include <stdio.h> 
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


long long serial(int dimmension , double c_i, double c_j,float ratio){
    long long start, end;
    cv::Mat image(demension, demension, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();

    double z_i,z_c;
     for (int i = 0; i < dimmension; i++) {
        for (int j = 0; j < dimmension; j++) {
            double x = -ratio + (i * (2*ratio)) / double(demension);
            double y = -ratio + (j * (2*ratio)) / double(demension);
            bool converge = true;
            int itr = 0;
            z_i = x;
            z_c = y;
            for(itr =0; itr < 1000; itr++){
                double temp_i = (z_i*z_i) - (z_c*z_c) + c_i;
                double temp_c = (2 * z_i * z_c) + c_j;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }
            }
            int interval = (itr - 1) / 5;
            int r = (interval * 40 + 50) % 256;
            int g = (interval * 80 + 30) % 256;
            int b = (interval * 120 + 0) % 256;
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(r, g, b);
        }
    }
	end = micros();
    cv::imwrite("ser.png", image);
    return end - start;
}


long long parallel(int demension , double c_i,double c_j,float ratio){
    long long start, end;
    cv::Mat image(demension, demension, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();
    double x,y;
    double z_i,z_c;
    bool converge;
    double temp_i,temp_c;
    int itr;
    int interval;
    int r;
    int g;
    int b;

    #pragma omp parallel for collapse(2) shared(image,c_i,c_j) private(z_i,z_c,x,y,converge,temp_i,temp_c,itr,r,g,b,interval) schedule(static,10)
    for (int i = 0; i < demension; i++) {
        for (int j = 0; j < demension; j++) {
            x = -ratio + (i * (2*ratio)) / double(demension);
            y = -ratio + (j * (2*ratio)) / double(demension);
            converge = true;
            z_i = x;
            z_c = y;
            for(itr =0; itr < 1000; itr++){
                temp_i = (z_i*z_i) - (z_c*z_c) + c_i;
                temp_c = (2 * z_i * z_c) + c_j;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }
            }
            interval = (itr - 1) / 5;
            r = (interval * 40 + 50) % 256;
            g = (interval * 80 + 30) % 256;
            b = (interval * 120 + 0) % 256;
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(r, g, b);
        }
    }
	end = micros();
    cv::imwrite("par.png", image);
    return end - start;
}


int main(){
    long long ser_time = serial(800 , 0.285,0.01,2.0);
	long long par_time = parallel(800 , 0.285,0.01,2.0);
    // long long ser_time = serial(0.0,0.0);
	// long long par_time = parallel(0.0,0.0);

	std::cout << "Serial Time: " <<ser_time <<std::endl;
	std::cout << "parallel Time: " <<par_time <<std::endl;
	std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
    return 0;
}