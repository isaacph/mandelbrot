#include "game.h"
#include <iostream>

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
    float tileSize = mainSquarePix / zoomMagnitude;
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(windowWidth / 2.0f, windowHeight / 2.0f, 0.0f));
    view = glm::scale(view, glm::vec3(tileSize, tileSize, 0));
    view = glm::translate(view, glm::vec3(-centerPos.x, -centerPos.y, 0));
    invView = glm::inverse(view);
}

glm::vec2 Camera::getCenter() const {
    return centerPos;
}

glm::mat4 Camera::getView() const {
    return view;
}

glm::vec2 Camera::toWorldCoordinate(glm::vec2 screenCoordinate) const {
    int mainSquarePix = std::min(windowWidth, windowHeight);
    float tileSize = mainSquarePix / zoomMagnitude;
    screenCoordinate -= glm::vec2{windowWidth / 2.0f, windowHeight / 2.0f};
    screenCoordinate /= tileSize;
    screenCoordinate += centerPos;
    return screenCoordinate;
}

glm::vec2 Camera::toScreenCoordinate(glm::vec2 screenCoordinate) const {
    int mainSquarePix = std::min(windowWidth, windowHeight);
    float tileSize = mainSquarePix / zoomMagnitude;
    screenCoordinate -= centerPos;
    screenCoordinate *= tileSize;
    screenCoordinate += glm::vec2{windowWidth / 2.0f, windowHeight / 2.0f};
    return screenCoordinate;
}

