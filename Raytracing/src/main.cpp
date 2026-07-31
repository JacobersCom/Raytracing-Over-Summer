#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

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
    if (world.hit(r, Interval(0, infinity), rec))
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

    SDL_Window* window = SDL_CreateWindow("Raytracer", 1500, 800, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	//Set up ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,//4 bytes pre pixel, 8 bits pre_channel
        SDL_TEXTUREACCESS_STREAMING, //The image will change often
        image_w, image_h
    );

    //pointer to the pixel info of the image
    void* pixels;

    //pitch is the size of the image in bytes
    int pitch;

	bool render_requested = false;
    bool quit = false;
    SDL_Event e;
	double sphere_x_position = 0.0;
	double sphere_radius = 0.5;

	ImGui_ImplSDL3_SetMouseCaptureMode(ImGui_ImplSDL3_MouseCaptureMode_Enabled);
    while (!quit) {

        while (SDL_PollEvent(&e)) {
		    ImGui_ImplSDL3_ProcessEvent(&e);
            
            if (e.type == SDL_EVENT_QUIT)
                quit = true;

           
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        if (ImGui::Button("Render"))
        {
            render_requested = true;
        }
        if (ImGui::Button("Clear Viewport"))
        {
			SDL_RenderClear(renderer);
            render_requested = false;
        }
        ImGui::InputDouble("Sphere x position", &sphere_x_position);
        ImGui::InputDouble("Sphere radius", &sphere_radius);

        if (ImGui::Button("Add Sphere"))
		{
			world.add(std::make_shared<Sphere>(point3(sphere_x_position, 0, -1), sphere_radius));
		}
        ImGui::End();

		ImGui::Begin("Scene");
		ImGui::Image((void*)texture, ImVec2(image_w, image_h));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        
        if (render_requested)
        {

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
        }

        SDL_UnlockTexture(texture);
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}