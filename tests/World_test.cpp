#include <gtest/gtest.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "World.h"
#include "Light.h"
#include "Sphere.h"
#include "Transformations.h"
#include "Ray.h"
#include "Intersection.h"
#include "Computation.h"
#include "Plane.h"

TEST(World_test, constructor_creates_world_with_no_objects_or_lights) {
    World world;

    ASSERT_EQ(world.objects.size(), 0);
}

TEST(World_test, default_world_creates_world_with_specified_objects_and_lights) {
    Tuple lightPosition = Tuple::Point(-10.0f, 10.0f, -10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);

    Color expectedColor(0.8f, 1.0f, 0.6f);

    World world = World::DefaultWorld();

    ASSERT_TRUE(world.objects[0]->material.color == expectedColor);
    ASSERT_EQ(world.objects[0]->material.diffuse, 0.7f);
    ASSERT_EQ(world.objects[0]->material.specular, 0.2f);

    ASSERT_TRUE(world.objects[1]->transform == scaling(0.5f, 0.5f, 0.5f));

    ASSERT_TRUE(world.light.position == lightPosition);
    ASSERT_TRUE(world.light.intensity == lightColor);
}

TEST(World_test, intersects_a_world_with_a_ray) {
    World world = World::DefaultWorld();

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection);

    std::vector<Intersection> intersections = intersectsWorld(ray, world);

    ASSERT_EQ(intersections.size(), 4);
    ASSERT_EQ(intersections[0].t, 4.0f);
    ASSERT_EQ(intersections[1].t, 4.5f);
    ASSERT_EQ(intersections[2].t, 5.5f);
    ASSERT_EQ(intersections[3].t, 6.0f);
}

TEST(World_test, shades_an_intersection) {
    World world = World::DefaultWorld();

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection);

    Object* sphere = world.objects[0];

    Intersection intersection(*sphere, 4.0f);
    std::vector<Intersection> intersections = {intersection};
    Computation comp = prepareComputation(intersection, ray, intersections);

    Color result = shadeHit(world, comp, 0);
    Color expected(0.38066f, 0.47583f, 0.2855f);

    ASSERT_TRUE(result == expected);
}

TEST(World_test, shades_an_intersection_from_the_inside) {
    World world = World::DefaultWorld();

    //light
    Tuple lightPoint = Tuple::Point(0.0f, 0.25f, 0.0f);
    Color lightColor = Color(1.0f, 1.0f, 1.0f);
    Light light(lightPoint, lightColor);
    world.light = light;

    //ray
    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, 0.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection);

    Object* sphere = world.objects[1];

    Intersection intersection(*sphere, 0.5f);
    std::vector<Intersection> intersections = {intersection};
    Computation comp = prepareComputation(intersection, ray, intersections);

    Color result = shadeHit(world, comp, 0);
    Color expected(0.90498f, 0.90498f, 0.90498f);

    ASSERT_TRUE(result == expected);
}

TEST(World_test, shadeHit_is_given_an_intersection_in_shadow) {
    World world; 
    world.light = Light(Tuple::Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Sphere sphere1;
    world.objects.push_back(&sphere1);

    Sphere sphere2;
    sphere2.setTransformation(translation(0.0f, 0.0f, 10.0f));
    world.objects.push_back(&sphere2);

    Ray ray(Tuple::Point(0.0f, 0.0f, 5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));
    Intersection i(sphere2, 4.0f);
    std::vector<Intersection> intersections = {i};
    Computation comp = prepareComputation(i, ray, intersections);
    
    Color result = shadeHit(world, comp, 0);
    Color expected(0.1f, 0.1f, 0.1f);

    ASSERT_TRUE(result == expected);
}

TEST(World_test, color_when_a_ray_misses) {
    World world = World::DefaultWorld();

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 1.0f, 0.0f);
    Ray ray(rayOrigin, rayDirection);

    Color result = colorAt(world, ray, 0);
    Color expected(0.0f, 0.0f, 0.0f);

    ASSERT_TRUE(result == expected);
}

TEST(World_test, color_when_a_ray_hit) {
    World world = World::DefaultWorld();

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection);

    Color result = colorAt(world, ray, 0);
    Color expected(0.38066f, 0.47583f, 0.2855f);

    ASSERT_TRUE(result == expected);
}

TEST(World_test, color_with_intersection_behind_the_ray) {
    World world = World::DefaultWorld();

    Object* outer = world.objects[0];
    outer->material.ambient = 1.0f;

    Object* inner = world.objects[1];
    inner->material.ambient = 1.0f;

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, 0.75f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Ray ray(rayOrigin, rayDirection);

    Color result = colorAt(world, ray, 0);
    ASSERT_TRUE(result == inner->material.color);
}

TEST(World_test, there_is_no_shadow_when_nothing_is_collinear_with_point_and_light) {
    World world = World::DefaultWorld();
    Tuple point = Tuple::Point(0.0f, 10.0f, 0.0f);

    ASSERT_FALSE(world.isShadow(point));
}

TEST(World_test, the_shadow_when_object_is_between_the_point_and_the_light) {
    World world = World::DefaultWorld();
    Tuple point = Tuple::Point(10.0f, -10.0f, 10.0f);

    ASSERT_TRUE(world.isShadow(point));
}

TEST(World_test, there_is_no_shadown_when_object_is_behind_the_light) {
    World world = World::DefaultWorld();
    Tuple point = Tuple::Point(-200.0f, 200.0f, -20.0f);

    ASSERT_FALSE(world.isShadow(point));
}

TEST(World_test, there_is_no_shadown_when_object_is_behind_the_point) {
    World world = World::DefaultWorld();
    Tuple point = Tuple::Point(-200.0f, 200.0f, -20.0f);

    ASSERT_FALSE(world.isShadow(point));
}

TEST(World_test, reflected_color_for_non_reflective_material) {
    World world = World::DefaultWorld();

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, 0.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection); 

    Object* sphere = world.objects[1];
    sphere->material.ambient = 1.0f;
    Intersection i(*sphere, 1.0f);
    std::vector<Intersection> intersections = {i};
    Computation comp = prepareComputation(i, ray, intersections);

    Color color = reflectedColor(world, comp, 0);

    ASSERT_TRUE(color == Color(0.0f, 0.0f, 0.0f));
}

TEST(World_test, reflected_color_for_reflective_material) {
    World world = World::DefaultWorld();

    Plane plane;
    plane.material.reflective = 0.5f;
    plane.setTransformation(translation(0.0f, -1.0f, 0.0f));

    world.objects.push_back(&plane);
    
    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -3.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i(plane, sqrt(2.0f));
    std::vector<Intersection> intersections = {i};
    Computation comp = prepareComputation(i, ray, intersections);
    Color color = reflectedColor(world, comp, 3);

    ASSERT_TRUE(color == Color(0.19032f, 0.2379f, 0.14274f));
}

TEST(World_test, shade_hit_with_a_reflective_material) {
     World world = World::DefaultWorld();

    Plane plane;
    plane.material.reflective = 0.5f;
    plane.setTransformation(translation(0.0f, -1.0f, 0.0f));

    world.objects.push_back(&plane);
    
    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -3.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i(plane, sqrt(2.0f));
    std::vector<Intersection> intersections = {i};
    Computation comp = prepareComputation(i, ray, intersections);
    Color color = shadeHit(world, comp, 3);
    
    ASSERT_TRUE(color == Color(0.87677f, 0.92436f, 0.82918f));
}

TEST(World_test, colorAt_with_mutually_reflective_surfaces) {
    World world = World::DefaultWorld();

    Plane plane;
    plane.material.reflective = 0.5f;
    plane.setTransformation(translation(0.0f, -1.0f, 0.0f));

    world.objects.push_back(&plane);
    
    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -3.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i(plane, sqrt(2.0f));
    std::vector<Intersection> intersections = {i};
    Computation comp = prepareComputation(i, ray, intersections);

    Color color = reflectedColor(world, comp, 0);

    ASSERT_TRUE(color == Color(0.0f, 0.0f, 0.0f));
}

TEST(World_test, refracted_color_with_opaque_surface) {
    World world = World::DefaultWorld();
    Object* sphere = world.objects[0];

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i1(*sphere, 4.0f);
    Intersection i2(*sphere, 6.0f);
    std::vector<Intersection> intersections = {i1, i2};
    Computation comp = prepareComputation(intersections[0], ray, intersections); 

    Color color = refractedColor(world, comp, 5);

    ASSERT_TRUE(color == Color(0.0f, 0.0f, 0.0f));
}

TEST(World_test, refracted_colorat_max_recursive_death) {
    World world = World::DefaultWorld();
    Object* sphere = world.objects[0];
    sphere->material.transparency = 1.0f;
    sphere->material.refractive_index = 1.5f;

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, -5.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 0.0f, 1.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i1(*sphere, 4.0f);
    Intersection i2(*sphere, 6.0f);
    std::vector<Intersection> intersections = {i1, i2};
    Computation comp = prepareComputation(intersections[0], ray, intersections); 

    Color color = refractedColor(world, comp, 0);

    ASSERT_TRUE(color == Color(0.0f, 0.0f, 0.0f));
}

TEST(World_test, refracted_color_under_total_internal_reflection) {
    World world = World::DefaultWorld();
    Object* sphere = world.objects[0];
    sphere->material.transparency = 1.0f;
    sphere->material.refractive_index = 1.5f;

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, sqrt(2.0f)/2.0f);
    Tuple rayDirection = Tuple::Vector(0.0f, 1.0f,0.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i1(*sphere, -sqrt(2.0f)/2.0f);
    Intersection i2(*sphere, sqrt(2.0f)/2.0f);
    std::vector<Intersection> intersections = {i1, i2};

    Computation comp = prepareComputation(intersections[1], ray, intersections); 

    Color color = refractedColor(world, comp, 5);

    ASSERT_TRUE(color == Color(0.0f, 0.0f, 0.0f));
}

TEST(World_test, refracted_color_with_refracted_ray) {
    World world = World::DefaultWorld();
    Object* sphere1 = world.objects[0];
    sphere1->material.ambient = 1.0f;
    sphere1->material.pattern = new TestPattern();

    Object* sphere2 = world.objects[1];
    sphere2->material.transparency = 1.0f;
    sphere2->material.refractive_index = 1.5f;

    Tuple rayOrigin = Tuple::Point(0.0f, 0.0f, 0.1f);
    Tuple rayDirection = Tuple::Vector(0.0f, 1.0f, 0.0f);
    Ray ray(rayOrigin, rayDirection); 

    Intersection i1(*sphere1, -0.9899f);
    Intersection i2(*sphere2, -0.4899f);
    Intersection i3(*sphere2, 0.4899f);
    Intersection i4(*sphere1, 0.9899f);

    std::vector<Intersection> intersections = {i1, i2, i3, i4};

    Computation comp = prepareComputation(intersections[2], ray, intersections); 

    Color color = refractedColor(world, comp, 5);

    ASSERT_TRUE(color == Color(0.0f, 0.99888f, 0.04725f));
}

TEST(World_test, shade_hit_with_transparent_material) {
    World world = World::DefaultWorld();

    Plane floor;
    floor.setTransformation(translation(0.0f, -1.0f, 0.0f));
    floor.material.transparency = 0.5f;
    floor.material.refractive_index = 1.5f;
    world.objects.push_back(&floor);

    Sphere sphere;
    sphere.setTransformation(translation(0.0f, -3.5f, -0.5f));
    sphere.material.color = Color(1.0f, 0.0f, 0.0f);
    sphere.material.ambient = 0.5f;
    world.objects.push_back(&sphere);

    Ray ray(Tuple::Point(0.0f, 0.0f, -3.0f), Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f));

    std::vector<Intersection> intersections = {Intersection(floor, sqrt(2.0f))};

    Computation comp = prepareComputation(intersections[0], ray, intersections);

    Color color = shadeHit(world, comp, 5);

    ASSERT_TRUE(color == Color(0.93642f, 0.68642f, 0.68642f));
}

TEST(World_test, shade_hit_with_reflective_and_transparent_material) {
    World world = World::DefaultWorld();

    Plane floor;
    floor.setTransformation(translation(0.0f, -1.0f, 0.0f));
    floor.material.reflective = 0.5f;
    floor.material.transparency = 0.5f;
    floor.material.refractive_index = 1.5f;
    world.objects.push_back(&floor);

    Sphere sphere;
    sphere.setTransformation(translation(0.0f, -3.5f, -0.5f));
    sphere.material.color = Color(1.0f, 0.0f, 0.0f);
    sphere.material.ambient = 0.5f;
    world.objects.push_back(&sphere);

    Ray ray(Tuple::Point(0.0f, 0.0f, -3.0f), Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f));

    std::vector<Intersection> intersections = {Intersection(floor, sqrt(2.0f))};

    Computation comp = prepareComputation(intersections[0], ray, intersections);

    Color color = shadeHit(world, comp, 5);

    ASSERT_TRUE(color == Color(0.93391, 0.69643, 0.69243));
}

