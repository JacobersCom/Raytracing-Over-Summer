#include <Window.hpp>

color Camera::RaySceneColor(const Ray& r, const Hittable& world) const
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
    //Just a lerp
    return (1-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
