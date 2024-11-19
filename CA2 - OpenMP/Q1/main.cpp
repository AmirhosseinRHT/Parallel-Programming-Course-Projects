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

long long serial(int height,int width,float ratio){
    long long start, end;
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();
     for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double x = -ratio + (i * (2*ratio)) / height;
            double y = -ratio + (j * (2*ratio)) / width;
            bool converge = true;
            int itr = 0;
            double z_i = 0;
            double z_c = 0;
            for(itr =0; itr < 1000; itr++){
                double temp_i = (z_i*z_i) - (z_c*z_c) + x;
                double temp_c = (2 * z_i * z_c) + y;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }

            }
            int interval = (itr - 1) / 10;
            int r = (interval * 5) % 256;
            int g = (interval * 10) % 256;
            int b = (interval * 15) % 256;
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(r , g, b);
            
        }
    }
	end = micros();
    // cv::imshow("Mandelbrot Set", image);
    // cv::waitKey(0);
    cv::imwrite("ser.png", image);
    return end - start;
}


long long parallel(int height,int width,float ratio){
    long long start, end;
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
	start = micros();
    double x,y;
    double z_i,z_c;
    bool converge;
    double temp_i,temp_c;
    int itr;

    #pragma omp parallel for shared(image) private(z_i,z_c,x,y,converge,temp_i,temp_c,itr) schedule(static, 10)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            x = -ratio + (i * (2*ratio)) / height;
            y = -ratio + (j * (2*ratio)) / width;
            converge = true;
            z_i = 0;
            z_c = 0;
            itr = 0;
            for( itr =0; itr < 1000; itr++){
                temp_i = (z_i*z_i) - (z_c*z_c) + x;
                temp_c = (2 * z_i * z_c) + y;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }

            }
            int interval = (itr - 1) / 10;
            int r = (interval * 5) % 256;
            int g = (interval * 10) % 256;
            int b = (interval * 15) % 256;
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(r , g, b); 
        }
    }
	end = micros();
    // cv::imshow("Mandelbrot Set", image);
    // cv::waitKey(0);
    cv::imwrite("par.png", image);
    return end - start;
}


int main(){
    int height = 800;
    int width = 800;
    long long ser_time = serial(height,width,2);
	long long par_time = parallel(height,width,2);
	std::cout << "Serial Time: " <<ser_time <<std::endl;
	std::cout << "parallel Time: " <<par_time <<std::endl;
	std::cout << "Speed Up: " << (double) ser_time / (double) par_time << std::endl;
    return 0;
}