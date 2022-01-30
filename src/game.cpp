#include "game.h"
#include <iostream>

glm::mat4 toMatrix(Box box) {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(box.position.x, box.position.y, 0));
    matrix = glm::scale(matrix, glm::vec3(box.scale.x, box.scale.y, 0));
    return matrix;
}

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

std::unique_ptr<GameObject> makePlayer(b2World* world, glm::vec2 position) {
    std::unique_ptr<GameObject> obj = std::unique_ptr<GameObject>(new GameObject(world));
    obj->types = {GameObject::PLAYER};
    obj->name = "Player";
    
    BoxBodyType* boxBody = new BoxBodyType();
    boxBody->scale = glm::vec2(1.0f, 2.0f);
    obj->bodyType = std::unique_ptr<BodyType>(boxBody);

    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(position.x, position.y);
    playerBodyDef.fixedRotation = true;
    b2Body* playerBody = world->CreateBody(&playerBodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(boxBody->scale.x / 2.0f, boxBody->scale.y / 2.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    b2Fixture* playerFixture = playerBody->CreateFixture(&fixtureDef);
    playerFixture->SetFriction(5.0f);

    obj->rigidBody = playerBody;
    obj->fixture = playerFixture;

    playerBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(obj.get());

    return obj;
}

std::unique_ptr<GameObject> makeGroundType(b2World* world, Box bodyDef) {
    std::unique_ptr<GameObject> obj = std::unique_ptr<GameObject>(new GameObject(world));
    obj->types = {GameObject::GROUND};
    obj->name = "Ground";
    
    BoxBodyType* boxBody = new BoxBodyType();
    boxBody->scale = bodyDef.scale;
    obj->bodyType = std::unique_ptr<BodyType>(boxBody);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(bodyDef.position.x, bodyDef.position.y);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape b2GroundBox;
    b2GroundBox.SetAsBox(bodyDef.scale.x / 2, bodyDef.scale.y / 2);
    b2Fixture* groundFixture = groundBody->CreateFixture(&b2GroundBox, 0.0f);
    groundFixture->SetFriction(1.0f);

    obj->rigidBody = groundBody;
    obj->fixture = groundFixture;

    groundBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(obj.get());

    return obj;
}

GameObject::GameObject(b2World* world) : world(world) {}
GameObject::~GameObject() {
    rigidBody->DestroyFixture(fixture);
    world->DestroyBody(rigidBody);
}

void Game::BeginContact(b2Contact* contact) {
}
 
void Game::EndContact(b2Contact* contact) {
}
 
void Game::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}
 
void Game::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}
