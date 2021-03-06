#include <gtest/gtest.h>

#include "Light.h"
#include "Color.h"
#include "Tuple.h"

TEST(Light_test, a_point_light_has_position_and_intensity) {
    Color intensity(1.0f, 1.0f, 1.0f);
    Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

    Light light(position, intensity);

    ASSERT_TRUE(light.intensity == intensity);
    ASSERT_TRUE(light.position == position);    
}