#include <SDL3/SDL.h>
#include <Util.hpp>

#include <Hittable.hpp>
#include <Hittable_list.hpp>
#include <Sphere.hpp>

color lerp(color starting_value, double a, color ending_value)
{
    return (1.0 - a) * starting_value + a * ending_value;
}

color RaySceneColor(const Ray& r, const Hittable& world)
{
    //objects
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
    {

        //Returns a normalized shader between 0-1 when it was original -1 -1
        return 0.5 * (rec.normal + color(1, 1, 1));
    }


    //background
    Vec3 unit_dir = unit_vector(r.GetDirection());
    //ratio value
    auto a = 0.5 * (unit_dir.y() + 1.0);
    return lerp(color(1.0, 1.0, 1.0), a, color(0.5, 0.7, 1.0));
}

int main() {

    //View port aspect ratio
    auto aspect_ratio = 16.0 / 9.0;

    //Creating the image based off the ideal aspect ratio
    int image_w = 1080;
    int image_h = int(image_w / aspect_ratio);
    image_h = (image_h < 1) ? 1 : image_h;

    //Ensuring the viewport is the same proportions as the image
    auto viewport_h = 2.0;
    auto viewport_w = viewport_h * (double(image_w) / image_h);

    // world

    Hittable_List world;
    world.add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));
    world.add(std::make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(point3(1, 0, -1), 0.25));
    world.add(std::make_shared<Sphere>(point3(-1, 0, -1), 0.25));




    //Camera

    //The length from the camera to the viewport
    double focal_length = 1.0;
    point3 camera_center = point3(0, 0, 0);

    //Adjusting the Y vector to compensate for the right hand coordinate system. Ensuring the Y value moves down the screen during rendering
    Vec3 viewport_detla_x = Vec3(viewport_w, 0, 0);
    Vec3 viewport_detla_y = Vec3(0, -viewport_h, 0);
    
    //Calculate the vector for each pixel in the x and y axis
    Vec3 pixel_delta_x = viewport_detla_x / image_w;
    Vec3 pixel_delta_y = viewport_detla_y / image_h;

    //Applies the focal length
    auto cam_focal = camera_center - Vec3(0, 0, focal_length);

    //Half the size of the viewport to get closer to the top left hand
    auto viewport_halfed = viewport_detla_x / 2 + viewport_detla_y / 2;
    auto upper_left_viewport_pos = cam_focal - viewport_halfed;

    //The vector axis for x and y pixels
    auto pixel_detla = (pixel_delta_x + pixel_delta_y);
    
    auto upper_left_pixel_loc = upper_left_viewport_pos + 0.5 * pixel_detla;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracer", image_w, image_h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,//4 bytes pre pixel, 8 bits pre_channel
        SDL_TEXTUREACCESS_STREAMING, //The image will change often
        image_w, image_h
    );

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
        }

        //pointer to the pixel info of the image
        void* pixels;

        //pitch is the size of the image in bytes
        int pitch;

        //lock the screen for rendering
        SDL_LockTexture(texture, NULL, &pixels, &pitch);

        for (int y = 0; y < image_h; y++) {
            Uint32* row = (Uint32*)((Uint8*)pixels + y * pitch);
            for (int x = 0; x < image_w; x++) {
                
                //The screen gets rendered from top left to bottom right using this
                auto pixel_center = upper_left_pixel_loc + (x * pixel_delta_x) + (y * pixel_delta_y);

                auto ray_dir = pixel_center - camera_center;
                
                Ray ray(camera_center, ray_dir);
                
                auto pixel_color = RaySceneColor(ray, world);

                row[x] = WriteColor(pixel_color);
            }
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}