%%writefile  example.cu
#include <iostream>
#include <fstream>
#include <cmath>
#include <curand_kernel.h>

//#define M_PI 3.14159265358979323846f

struct Vec3 {
    float x, y, z;

    __host__ __device__ Vec3() : x(0), y(0), z(0) {}
    __host__ __device__ Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    __host__ __device__ Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    __host__ __device__ Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    __host__ __device__ Vec3 operator*(float t) const {
        return Vec3(x * t, y * t, z * t);
    }
    __host__ __device__ Vec3 operator/(float t) const {
        return Vec3(x / t, y / t, z / t);
    }
    __host__ __device__ float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    __host__ __device__ Vec3 cross(const Vec3& v) const {
        return Vec3(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
        );
    }
    __host__ __device__ float length() const {
        return sqrtf(x * x + y * y + z * z);
    }
    __host__ __device__ Vec3 normalize() const {
        float len = length();
        return *this / len;
    }
    __host__ __device__ Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    __host__ __device__ friend Vec3 operator*(float t, const Vec3& v) {
        return Vec3(t * v.x, t * v.y, t * v.z);
    }
};

struct Ray {
    Vec3 origin;
    Vec3 direction;

    __host__ __device__ Ray() {}
    __host__ __device__ Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}

    __host__ __device__ Vec3 at(float t) const {
        return origin + direction * t;
    }
};

#define SPHERE 0
#define PLANE 1

struct Hittable {
    int type;        // Object type: SPHERE or PLANE
    Vec3 center;     // For sphere and plane (point on the plane)
    Vec3 normal;     // For plane normal (for PLANE type)
    float radius;    // For sphere
    Vec3 color;      // Material color
};

__device__ bool hitSphere(const Hittable& sphere, const Ray& r, float t_min, float t_max, float& t, Vec3& normal) {
    Vec3 oc = r.origin - sphere.center;
    float a = r.direction.dot(r.direction);
    float half_b = oc.dot(r.direction);
    float c = oc.dot(oc) - sphere.radius * sphere.radius;
    float discriminant = half_b * half_b - a * c;
    if (discriminant > 0) {
        float sqrt_d = sqrtf(discriminant);
        float root = (-half_b - sqrt_d) / a;
        if (root < t_max && root > t_min) {
            t = root;
            Vec3 hitPoint = r.at(t);
            normal = (hitPoint - sphere.center).normalize();
            return true;
        }
        root = (-half_b + sqrt_d) / a;
        if (root < t_max && root > t_min) {
            t = root;
            Vec3 hitPoint = r.at(t);
            normal = (hitPoint - sphere.center).normalize();
            return true;
        }
    }
    return false;
}

__device__ bool hitPlane(const Hittable& plane, const Ray& r, float t_min, float t_max, float& t, Vec3& normal) {
    float denom = plane.normal.dot(r.direction);
    if (fabsf(denom) > 1e-6f) { // Not parallel
        t = (plane.center - r.origin).dot(plane.normal) / denom;
        if (t < t_max && t > t_min) {
            normal = plane.normal;
            return true;
        }
    }
    return false;
}


__device__ bool does_shadow_intersect(Vec3 point,Vec3 light_pos, Hittable* objects, int num_objects){
    float t_min = 0.001f;
    float t_max = 1e20f;
    Vec3 shadow_dir =  light_pos - point;
    shadow_dir = shadow_dir.normalize();
    Ray shadow(point, shadow_dir);
    float t;
    Vec3 temp_normal;
    for(int i = 0; i < num_objects;i++){
        if(objects[i].type == SPHERE){
            if(hitSphere(objects[i],shadow, t_min, t_max, t, temp_normal)){
                return true;
            }

        }
    }
    return false;
}

__device__ Vec3 rayColor(const Ray& r, Hittable* objects, int num_objects, Vec3 light_pos) {
    float t_min = 0.001f;
    float t_max = 1e20f;
    float closest_t = t_max;
    Vec3 color(0, 0, 0);
    Vec3 normal;
    int hit_index = -1;

    // Find closest hit
    for (int i = 0; i < num_objects; ++i) {
        float t;
        Vec3 temp_normal;
        bool hit = false;

        if (objects[i].type == SPHERE) {
            hit = hitSphere(objects[i], r, t_min, closest_t, t, temp_normal);
        } else if (objects[i].type == PLANE) {
            hit = hitPlane(objects[i], r, t_min, closest_t, t, temp_normal);
        }

        if (hit) {
            closest_t = t;
            normal = temp_normal;
            color = objects[i].color;
            hit_index = i;
        }
    }

    if (hit_index >= 0) {

        Vec3 shadow_color(0,0,0);
        Vec3 hit_point = r.at(closest_t);
        Vec3 diffuse;
        if(does_shadow_intersect(hit_point, light_pos, objects,num_objects)){
            diffuse = shadow_color;
        }
        else{
            Vec3 light_dir = (light_pos - r.at(closest_t)).normalize();
            float intensity = fmaxf(0.0f, normal.dot(light_dir));
            diffuse = intensity * color;
        }
        
       

        Vec3 ambient = 0.1f * color;

        Vec3 result_color = ambient + diffuse;

        return result_color;
    }

    Vec3 unit_direction = r.direction.normalize();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f); // Sky gradient
}

__global__ void renderKernel(Vec3* pixels, int width, int height, Hittable* objects, int num_objects, Vec3 light_pos,Vec3 lookfrom, Vec3 lookat, float vfov, Vec3 up) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height) return;

    int index = y * width + x;
    

    float aspect_ratio = float(width) / float(height);
    float viewport_height = 2.0f * tan(vfov / 2);
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = (lookfrom - lookat).length();
    Vec3 w = (lookfrom - lookat).normalize();
    Vec3 u = up.cross(w).normalize();
    Vec3 v = w.cross(u);
   
    Vec3 h = viewport_width * u;
    Vec3 vertical = viewport_height * v;

    Vec3 viewport_upper_left  = lookfrom - (w * focal_length) - (h / 2) - (vertical / 2);

    Vec3 d_u = h / float(width);
    Vec3 d_v = vertical / float(height);

    Vec3 pixel_location = viewport_upper_left  + (x + 0.5f) * d_u + (y + 0.5f) * d_v;

    
    Vec3 direction = (pixel_location - lookfrom).normalize();
    Ray r(lookfrom, direction);

    Vec3 color = rayColor(r, objects, num_objects, light_pos);

    color = Vec3(sqrtf(color.x), sqrtf(color.y), sqrtf(color.z));
    pixels[index] = color;
}

void saveToPPM(const Vec3* pixels, int width, int height, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);
    outFile << "P6\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j) { // Flip the image vertically
        for (int i = 0; i < width; ++i) {
            int index = j * width + i;
            unsigned char r = static_cast<unsigned char>(255.99f * fminf(fmaxf(pixels[index].x, 0.0f), 1.0f));
            unsigned char g = static_cast<unsigned char>(255.99f * fminf(fmaxf(pixels[index].y, 0.0f), 1.0f));
            unsigned char b = static_cast<unsigned char>(255.99f * fminf(fmaxf(pixels[index].z, 0.0f), 1.0f));
            outFile << r << g << b;
        }
    }
    outFile.close();
}

int main() {
    float vfov = 120 * (M_PI/180.0);
    Vec3 up(0,1,0);
    Vec3 look_from(0,0,1);
    Vec3 look_at(0,0,-1);


    const int width = 800;
    const int height = 600;
    size_t numPixels = width * height;
    Vec3* pixels;

    cudaMallocManaged(&pixels, numPixels * sizeof(Vec3));

    const int num_objects = 4;
    Hittable* objects;
    cudaMallocManaged(&objects, num_objects * sizeof(Hittable));

    objects[0].type = SPHERE;
    objects[0].center = Vec3(0.0f, 0.0f, -1.5f);
    objects[0].radius = 0.5f;
    objects[0].color = Vec3(0.8f, 0.1f, 0.1f); // Red

    objects[1].type = SPHERE;
    objects[1].center = Vec3(-1.0f, 0.0f, -2.0f);
    objects[1].radius = 0.5f;
    objects[1].color = Vec3(0.1f, 0.1f, 0.8f); // Blue

    objects[2].type = SPHERE;
    objects[2].center = Vec3(1.0f, 0.0f, -2.0f);
    objects[2].radius = 0.5f;
    objects[2].color = Vec3(0.1f, 0.8f, 0.1f); // Green

    objects[3].type = PLANE;
    objects[3].center = Vec3(0.0f, -0.5f, 0.0f); // Point on the plane
    objects[3].normal = Vec3(0.0f, 1.0f, 0.0f);  // Upward normal
    objects[3].color = Vec3(0.8f, 0.8f, 0.8f);   // Gray

    Vec3 light_pos = Vec3(5.0f, 5.0f, -5.0f);

    dim3 blockSize(16, 16);
    dim3 numBlocks((width + blockSize.x - 1) / blockSize.x,
                   (height + blockSize.y - 1) / blockSize.y);
    
    renderKernel<<<numBlocks, blockSize>>>(pixels, width, height, objects, num_objects, light_pos,look_from,look_at,vfov,up);
    cudaDeviceSynchronize();

    saveToPPM(pixels, width, height, "output.ppm");

    cudaFree(pixels);
    cudaFree(objects);

    return 0;
}
