
#include <Hittable_list.hpp>
#include <Sphere.hpp>

#include <Application.hpp>

int main() {

    // world

    Hittable_List world;
    world.add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));
    world.add(std::make_shared<Sphere>(point3(0, 0, -1), 0.5));

    Application app;

    app.aspect_ratio = 16.0 / 9.0;
    app.image_w = 1080;

    app.Initalization();

    app.Update(world);

    return 0;
}