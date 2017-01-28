#include "catch.hpp"
extern "C" {
#include "../src/maths.h"
}

TEST_CASE("Scalar operations") {
  SECTION("Approximate equality") {
    float x = 0.5f;
    float y = 1.0f / 2.0f;
    float z = 2.0f / 3.0f;

    REQUIRE(aseq(x,y));
    REQUIRE(!aseq(x,z));
  }
}

TEST_CASE("Vector operations") {
  SECTION("Equality") {
    vec4f a = {0,1,2,3};
    vec4f b = {5,10,15,20};
    vec4f c = {0,1,2,3};

    REQUIRE(!veq(&a, &b));
    REQUIRE(veq(&a, &c));
  }

  SECTION("Approximate equality") {
    vec4f a = {0.5f, 0.25f, 0.125f, 0.0625f};
    vec4f b = {1.0f/2.0f, 1.0f/4.0f, 1.0f/8.0f, 1.0f/16.0f};

    REQUIRE(aveq(&a, &b));
  }

  SECTION("Vector addition") {
    vec4f a = {0,1,2,3};
    vec4f b = {5,10,15,20};

    vec4f res = vadd(&a, &b);

    REQUIRE(res.x == a.x + b.x);
    REQUIRE(res.y == a.y + b.y);
    REQUIRE(res.z == a.z + b.z);
    REQUIRE(res.w == a.w + b.w);
  }

  SECTION("Vector negation") {
    vec4f a = {0,1,2,3};

    vec4f res = vneg(&a);

    REQUIRE(res.x == -a.x);
    REQUIRE(res.y == -a.y);
    REQUIRE(res.z == -a.z);
    REQUIRE(res.w == -a.w);
  }

  SECTION("Vector reciprocal") {
    vec4f a = {1.0f,2.0f,3.0f,4.0f};
    vec4f b = {1.0f/1.0f,1.0f/2.0f,1.0f/3.0f,1.0f/4.0f};

    vec4f res = vrecp(&a);

    REQUIRE(aveq(&res, &b));
  }

  SECTION("Vector multiplication") {
    SECTION("Scalar") {
      vec4f a = {1.0f,2.0f,3.0f,4.0f};
      vec4f b = {2.0f,4.0f,6.0f,8.0f};

      vec4f res = vmuls(&a, 2.0f);

      REQUIRE(aveq(&b, &res));
    }

    SECTION("Vector") {
      vec4f a = {1.0f,2.0f,3.0f,4.0f};
      vec4f b = {2.0f,4.0f,6.0f,8.0f};
      vec4f c = {2.0f,8.0f,18.0f,32.0f};

      vec4f res = vmulv(&a, &b);

      REQUIRE(aveq(&c, &res));
    }
  }
}

TEST_CASE("Matrix operations") {
  SECTION("Equality") {
    mat4f a = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    mat4f b = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80};
    mat4f c = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    REQUIRE(!ameq(&a, &b));
    REQUIRE(ameq(&a, &c));
  }

  SECTION("Matrix vector multiplication") {
    mat4f a = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    vec4f b = {5,10,15,20};
    vec4f c = {400,450,500,550};

    vec4f res = mmulv(&a, &b);
    REQUIRE(aveq(&c, &res));
  }

  SECTION("Matrix matrix multiplication") {
    mat4f a = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    mat4f b = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80};
    mat4f c = {400,450,500,550,880,1010,1140,1270,1360,1570,1780,1990,1840,2130,2420,2710};

    mat4f res = mmulm(&a, &b);
    REQUIRE(ameq(&c, &res));
  }
}
