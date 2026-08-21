#include <Application.hpp>
#include <Color.hpp>

namespace 
{
    void* pixels; //Window pixel data
    int pitch; // current position at pixel + y * pitch

    bool render_requested = false;
    bool quit = false;

    SDL_Event e;
    
    double sphere_x_position = 0.0; //inital position of sphere
    double sphere_radius = 0.5; //inital radius of sphere

}
void Application::Initalization()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return;
    }

    window = SDL_CreateWindow("Raytracer", 1500, 800, SDL_WINDOW_RESIZABLE);
    render = SDL_CreateRenderer(window, NULL);


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, render);
    ImGui_ImplSDLRenderer3_Init(render);

    InitData();

    render_image = SDL_CreateTexture(
        render,
        SDL_PIXELFORMAT_RGBA8888,//4 bytes pre pixel, 8 bits pre_channel
        SDL_TEXTUREACCESS_STREAMING, //The image will change often
        image_w, image_h
    );
}

void Application::Update(Hittable_List& world)
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

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
            SDL_RenderClear(render);
            render_requested = false;
        }

		ImGui::Text("Sphere X Position");
        ImGui::InputDouble(" ", &sphere_x_position);
        ImGui::Text("Sphere Radius");
        ImGui::InputDouble("##xx", &sphere_radius);

        if (ImGui::Button("Add Sphere"))
        {
            world.add(std::make_shared<Sphere>(point3(sphere_x_position, 0, -1), sphere_radius));
        }
        if (ImGui::Button("Remove Sphere"))
        {
            if (!world.objects.empty())
            {
                world.objects.pop_back();
            }
        } 

        static int selected_entity = -1;
        ImGui::Separator();
		ImGui::Text("Entities:");
        ImGui::BeginChild("Entity-List", ImVec2(0, 160), true);
        for (int i = 0; i < (int)world.objects.size(); ++i)
        {
            char label[64];
            std::snprintf(label, sizeof(label), "Entity %d", i);
            if (ImGui::Selectable(label, selected_entity == i))
                selected_entity = i;
        }
        if (selected_entity >= 0 && selected_entity < (int)world.objects.size())
        {
            if (ImGui::Button("Remove Selected"))
            {
                world.objects.erase(world.objects.begin() + selected_entity);
                selected_entity = -1;
            }
        }
        ImGui::EndChild();
        ImGui::End();

        ImGui::Begin("Scene");
        ImGui::Image((void*)render_image, ImVec2(image_w, image_h));
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::DockSpaceOverViewport();
        ImGui::End();



        if (render_requested)
        {
            //lock the screen for rendering
            SDL_LockTexture(render_image, NULL, &pixels, &pitch);

            for (int y = 0; y < image_h; y++) {
                Uint32* row = (Uint32*)((Uint8*)pixels + y * pitch);
                for (int x = 0; x < image_w; x++) {

                    color pixel_color(0, 0, 0);
                    //The screen gets rendered from top left to bottom right using this
                    for (int sample = 0; sample < sample_per_pixel; sample++)
                    {
                        Ray r = GetRayAt(x, y);
                        pixel_color += RaySceneColor(r, max_depth,world);
                    }


                    row[x] = WriteColor(pixel_color_scale * pixel_color);
                }
            }
            render_requested = false;
        }

        SDL_UnlockTexture(render_image);
        ImGui::Render();
        SDL_SetRenderScale(render, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_RenderClear(render);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), render);
        SDL_RenderPresent(render);

        
    }

    CleanUp();
}

void Application::InitData()
{

    pixel_color_scale = 1.0 / sample_per_pixel;

    //View port aspect ratio
    auto aspect_ratio = 16.0 / 9.0;

    //Creating the image based off the ideal aspect ratio
    int image_w = 1080;
    image_h = int(image_w / aspect_ratio);
    image_h = (image_h < 1) ? 1 : image_h;

    //Ensuring the viewport is the same proportions as the image
    auto viewport_h = 2.0;
    auto viewport_w = viewport_h * (double(image_w) / image_h);

    //The length from the camera to the viewport
    double focal_length = 1.0;
    camera_center = point3(0, 0, 0);

    //Adjusting the Y vector to compensate for the right hand coordinate system. Ensuring the Y value moves down the screen during rendering
    auto viewport_detla_x = Vec3(viewport_w, 0, 0);
    auto viewport_detla_y = Vec3(0, -viewport_h, 0);

    //Calculate the vector for each pixel in the x and y axis
    pixel_delta_x = viewport_detla_x / image_w;
    pixel_delta_y = viewport_detla_y / image_h;

    //Applies the focal length
    auto cam_focal = camera_center - Vec3(0, 0, focal_length);

    //Half the size of the viewport to get closer to the top left hand
    auto viewport_halfed = viewport_detla_x / 2 + viewport_detla_y / 2;
    auto upper_left_viewport_pos = cam_focal - viewport_halfed;

    //The vector axis for x and y pixels
    auto pixel_detla = (pixel_delta_x + pixel_delta_y);

    pixel00_loc = upper_left_viewport_pos + 0.5 * pixel_detla;
}

void Application::CleanUp()
{
    SDL_DestroyTexture(render_image);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

color Application::RaySceneColor(const Ray& r, int depth, const Hittable& world) const
{
    if (depth <= 0)
        return color(0, 0, 0);

    //objects
    hit_record rec;
    if (world.hit(r, Interval(0.001, infinity), rec))
    {
		Vec3 direction = random_vec_on_sphere(rec.normal);
        //Create a new ray pointing in the direction of the surface normals
        return 0.5 * RaySceneColor(Ray(rec.hit_point, direction),depth - 1, world);
    }

    //background
    Vec3 unit_dir = unit_vector(r.GetDirection());
    //ratio value
    auto a = 0.5 * (unit_dir.y() + 1.0);
    //Just a lerp
    return (1-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

Ray Application::GetRayAt(int x, int y)
{
    auto offset = sample_pixel_square();
    auto pixel_sample = pixel00_loc + ((x + offset.x()) * pixel_delta_x) + ((y + offset.y()) * pixel_delta_y);

    auto ray_origin = camera_center;
    auto ray_dir = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_dir);
}
