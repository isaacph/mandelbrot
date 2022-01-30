#include "physics.h"
#include <limits>

Shadow::Shadow() : min(0), max(0) {}
Shadow::Shadow(float min, float max) : min(min), max(max) {}
Shadow::Shadow(const std::vector<float>& points) : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::lowest()) {
    for (float pt : points) {
        min = std::min(min, pt);
        max = std::max(max, pt);
    }
}
bool intersect(const Shadow& a, const Shadow& b) {
    return a.min <= b.min && b.min <= a.max ||
            a.min <= b.max && b.max <= a.max ||
            b.min <= a.min && a.min <= b.max ||
            b.min <= a.max && a.max <= b.max;
}

float absMin(const std::vector<float>& args) {
    assert(!args.empty());
    float absMin = args[0];
    for(int i = 1; i < args.size(); ++i) {
        if(abs(args[i]) < abs(absMin)) {
            absMin = args[i];
        }
    }
    return absMin;
}

glm::vec2 absMin(const std::vector<glm::vec2>& args) {
    assert(!args.empty());
    glm::vec2 absMin = args[0];
    for(int i = 1; i < args.size(); ++i) {
        if(glm::length2(args[i]) < glm::length2(absMin)) {
            absMin = args[i];
        }
    }
    return absMin;
}

float resolveIntersect(Shadow pusher, Shadow mover) {
    if(!intersect(pusher, mover)) return 0;
    return absMin({pusher.max - mover.min, -(mover.max - pusher.min)});
}

std::vector<float> resolveOptions(const Shadow& pusher, Shadow& mover) {
    if(!intersect(pusher, mover)) return {};
    return {pusher.max - mover.min + 0.005f, -(mover.max - pusher.min + 0.005f)};
}

std::vector<glm::vec2> groupMul(const glm::vec2& v, const std::vector<float>& f) {
    std::vector<glm::vec2> array(f.size(), glm::vec2(0.0f));
    for(int i = 0; i < f.size(); ++i) {
        array[i] = glm::vec2(v.x * f[i], v.y * f[i]);
    }
    return array;
}

float min(const std::vector<float>& points) {
    assert(!points.empty());
    float min = points[0];
    for(int i = 1; i < points.size(); ++i) {
        if(points[i] < min) {
            min = points[i];
        }
    }
    return min;
}

float max(const std::vector<float>& points) {
    assert(!points.empty());
    float max = points[0];
    for(int i = 1; i < points.size(); ++i) {
        if(points[i] > max) {
            max = points[i];
        }
    }
    return max;
}

std::vector<float> project(const glm::vec2& v, const std::vector<glm::vec2>& points) {
    glm::vec2 u = glm::vec2(v.x / v.length(), v.y / v.length());
    std::vector<float> f(points.size(), 0.0f);
    for(int i = 0; i < points.size(); ++i) {
        f[i] = u.x * points[i].x + u.y * points[i].y;
    }
    return f;
}


std::vector<glm::vec2> Box::points() const {
    return {
        {position.x - (scale.x / 2.0f), position.y - (scale.y / 2.0f)},
        {position.x - (scale.x / 2.0f), position.y + (scale.y / 2.0f)},
        {position.x + (scale.x / 2.0f), position.y + (scale.y / 2.0f)},
        {position.x + (scale.x / 2.0f), position.y - (scale.y / 2.0f)},
    };
}

Shadow shadow(const Convex& convex, const glm::vec2& span) {
    if(span.x == 0 && span.y == 0) return {0, 0};
    return Shadow(project(span, convex.points()));
}

bool intersect(Box a, Box b) {
    return intersect(shadow(a, glm::vec2(1, 0)),
            shadow(b, glm::vec2(1, 0))) &&
           intersect(shadow(a, glm::vec2(0, 1)),
            shadow(b, glm::vec2(0, 1)));
}

std::vector<glm::vec2> resolveOptions(Box pusher, Box mover) {
    std::vector<glm::vec2> options;
    std::vector<glm::vec2> basis = {
        {1, 0},
        {0, 1}
    };
    for(glm::vec2& v : basis) {
        Shadow a = shadow(pusher, v);
        Shadow b = shadow(mover, v);
        if(!intersect(a, b)) {
            return {};
        }
        std::vector<glm::vec2> groupMulRes = groupMul(v, resolveOptions(a, b));
        options.insert(options.end(), groupMulRes.begin(), groupMulRes.end());
    }
    return options;
}

glm::vec2 resolve(Box pusher, Box mover) {
    return absMin(resolveOptions(pusher, mover));
}

glm::vec2 resolveX(Box pusher, Box mover) {
    if(!intersect(pusher, mover)) {
        return {0, 0};
    }
    return glm::vec2(resolveIntersect(
        shadow(pusher, glm::vec2{1, 0}), shadow(mover, glm::vec2{1, 0})
    ), 0);
}

glm::vec2 resolveY(Box pusher, Box mover) {
    if(!intersect(pusher, mover)) {
        return {0, 0};
    }
    return glm::vec2(0, resolveIntersect(
        shadow(pusher, glm::vec2{0, 1}), shadow(mover, glm::vec2{0, 1})
    ));
}
