#include <gtest/gtest.h>
#include <cmath>

#include "Material.h"
#include "Sphere.h"
#include "Color.h"
#include "Light.h"
#include "Stripe.h"

TEST(Material_test, creates_default_material) {
    Material material;

    Color expected(1.0f, 1.0f, 1.0f);

    ASSERT_TRUE(material.color == expected);
    ASSERT_EQ(material.ambient, 0.1f);
    ASSERT_EQ(material.diffuse, 0.9f);
    ASSERT_EQ(material.specular, 0.9f);
    ASSERT_EQ(material.shininess, 200.0f);
    ASSERT_EQ(material.reflective, 0.0f);
}

TEST(Material_test, material_may_have_color_reassigned) {
    Material material;

    Color newColor(0.5f, 0.2f, 1.0f);

    material.color = newColor;

    ASSERT_TRUE(material.color == newColor);
    ASSERT_EQ(material.ambient, 0.1f);
    ASSERT_EQ(material.diffuse, 0.9f);
    ASSERT_EQ(material.specular, 0.9f);
    ASSERT_EQ(material.shininess, 200.0f);
}

TEST(Material_test, two_materials_are_equal_if_they_have_the_same_properties) {
    Material material1;
    Material material2;
    Material material3;

    material3.diffuse = 0.3f;

    ASSERT_TRUE(material1 == material2);
    ASSERT_FALSE(material1 == material3);
    
    ASSERT_TRUE(material1 != material3);
    ASSERT_FALSE(material1 != material2);
}

TEST(Material_test, default_material_have_transparency_and_refractive_index) {
    Material material; 

    ASSERT_EQ(material.transparency, 0.0f);
    ASSERT_EQ(material.refractive_index, 1.0f);
}

TEST(Lighting_test, lighting_with_the_eye_between_the_light_and_the_object) {
    Sphere sphere; 
    Material material;
    sphere.material = material;

    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);

    Tuple lightPosition = Tuple::Point(0.0f, 0.0f, -10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);
    Light light(lightPosition, lightColor);
    bool inShadow = false; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);

    Color expected(1.9f, 1.9f, 1.9f);
    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_the_eye_between_the_light_and_the_object_with_eye_offset_45_degree) {
    Sphere sphere; 
    Material material;
    sphere.material = material;


    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);

    Tuple lightPosition = Tuple::Point(0.0f, 0.0f, -10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);
    Light light(lightPosition, lightColor);
    bool inShadow = false; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);
    
    Color expected(1.0f, 1.0f, 1.0f);
    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_the_eye_between_the_light_and_the_object_with_light_offset_45_degree) {
    Sphere sphere; 
    Material material;
    sphere.material = material;

    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);

    Tuple lightPosition = Tuple::Point(0.0f, 10.0f, -10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);
    Light light(lightPosition, lightColor);
    bool inShadow = false; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);
    Color expected(0.7364f, 0.7364f, 0.7364f);
    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_the_eye_in_the_path_of_the_reflection_vector) {
    Sphere sphere; 
    Material material;
    sphere.material = material;

    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, -sqrt(2.0f)/2.0f, -sqrt(2.0f)/2.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);

    Tuple lightPosition = Tuple::Point(0.0f, 10.0f, -10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);
    Light light(lightPosition, lightColor);
    bool inShadow = false; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);

    Color expected(1.6364f, 1.6364f, 1.6364f);
    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_the_light_behind_the_object) {
    Sphere sphere; 
    Material material;
    sphere.material = material;

    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);

    Tuple lightPosition = Tuple::Point(0.0f, 0.0f, 10.0f);
    Color lightColor(1.0f, 1.0f, 1.0f);
    Light light(lightPosition, lightColor);
    bool inShadow = false; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);

    Color expected(0.1f, 0.1f, 0.1f);
    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_surface_in_shadow){
    Sphere sphere; 
    Material material;
    sphere.material = material;

    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Tuple eyeDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Light light(Tuple::Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = true; 

    Color result = lighting(&sphere, light, position, eyeDirection, normal, inShadow);
    Color expected(0.1f, 0.1f, 0.1f);

    ASSERT_TRUE(result == expected);
}

TEST(Lighting_test, lighting_with_a_pattern_applied) {
    Sphere sphere; 

    Material material;
    Stripe p = Stripe(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f));
    material.setPattern(p);
    material.ambient = 1.0f;
    material.diffuse = 0.0f;
    material.specular = 0.0f;

    sphere.material = material;

    Tuple eyeDirection = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Tuple normal = Tuple::Vector(0.0f, 0.0f, -1.0f);
    Light light(Tuple::Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Color c1 = lighting(&sphere, light, Tuple::Point(0.9f, 0.0f, 0.0f), eyeDirection, normal, false);
    Color c2 = lighting(&sphere, light, Tuple::Point(1.1f, 0.0f, 0.0f), eyeDirection, normal, false);

    ASSERT_TRUE(c1 == Color(1.0f, 1.0f, 1.0f));
    ASSERT_TRUE(c2 == Color(0.0f, 0.0f, 0.0f));
}
