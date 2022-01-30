#ifndef SRC_PHYSICS_H_INCLUDED
#define SRC_PHYSICS_H_INCLUDED
#include <cassert>
#include <math.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

class Convex {
public:
    virtual std::vector<glm::vec2> points() const = 0;
};

class Box : public Convex {
public:
    glm::vec2 position;
    glm::vec2 scale;
    virtual std::vector<glm::vec2> points() const;
};

class Shadow {
public:
    float min, max;
    Shadow();
    Shadow(float min, float max);
    Shadow(const std::vector<float>& points);
};

float absMin(const std::vector<float>& args);
glm::vec2 absMin(const std::vector<glm::vec2>& args);
float resolveIntersect(Shadow pusher, Shadow mover);
std::vector<float> resolveOptions(const Shadow& pusher, Shadow& mover);
std::vector<glm::vec2> groupMul(const glm::vec2& v, const std::vector<float>& f);
float min(const std::vector<float>& points);
float max(const std::vector<float>& points);
std::vector<float> project(const glm::vec2& v, const std::vector<glm::vec2>& points);
bool intersect(const Shadow& a, const Shadow& b); 
Shadow shadow(const Convex& convex, const glm::vec2& span);
bool intersect(Box a, Box b);
std::vector<glm::vec2> resolveOptions(Box pusher, Box mover);
glm::vec2 resolve(Box pusher, Box mover);
glm::vec2 resolveX(Box pusher, Box mover);
glm::vec2 resolveY(Box pusher, Box mover);

#endif
