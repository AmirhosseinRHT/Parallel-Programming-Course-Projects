#include <opencv2/opencv.hpp> 
#include <stdio.h> 
#include <intrin.h>
#include <iostream>
#include <chrono>
#include <vector>




int main(){

    std::vector<std::pair<double, double>> mandelbort_points;
    cv::Mat image(800, 800, CV_8UC3, cv::Scalar(0, 0, 0));
    for(double x = -2; x <= 2; x+=0.005){
        for(double y = -2;y <= 2; y+=0.005){
            bool converge = true;
            double z_i = 0;
            double z_c = 0;
            for(int itr =0; itr < 1000; itr++){
                double temp_i = pow(z_i,2) - pow(z_c,2) + x;
                double temp_c = (2 * z_i * z_c) + y;
                z_i = temp_i;
                z_c = temp_c;
                if ((z_i * z_i + z_c * z_c) > 4){
                    converge = false;
                    break;
                }
            }
            if (converge){
                mandelbort_points.emplace_back(x,y);
            }
        }
    }
    for(int i =0; i < mandelbort_points.size();i++){
        double x = mandelbort_points[i].first;
        double y = mandelbort_points[i].second;
        int px = static_cast<int>((x - (-2)) / (2 - (-2)) * 800);
        int py = static_cast<int>((y - (-2)) / (2 - (-2)) * 800);
        image.at<cv::Vec3b>(py, px) = cv::Vec3b(90, 0, 44);
    }
    // cv::imshow("Mandelbrot Set", image);
    // cv::waitKey(0);
    cv::imwrite("C:/Users/farbo/Desktop/university-s7/PP/Parallel-Programming/CA2 - OpenMP/Q1/Q1mandelbrot_set.png", image);
    return 0;
}