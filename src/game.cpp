#include "game.h"

void Camera::onResize(int windowWidth, int windowHeight) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    update();
}

void Camera::center(float worldX, float worldY) {
    centerPos = glm::vec2(worldX, worldY);
    update();
}

void Camera::zoom(float worldTilesPerScreen) {
    zoomMagnitude = worldTilesPerScreen;
    update();
}

void Camera::update() {
    // windowWidth, windowHeight, zoomMagnitude, centerPos
    int mainSquarePix = std::min(windowWidth, windowHeight);
}

glm::mat4 Camera::getView() const {
    return view;
}
