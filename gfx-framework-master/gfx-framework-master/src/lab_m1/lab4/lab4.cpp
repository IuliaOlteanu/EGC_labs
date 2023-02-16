#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    sunSpeed = 0.5;
    sunMoveX = 0;
    moveSunRight = true;

    planetAngularSpeed = M_PI / 9;
    planetRot = 0;
    planetDistanceSun = 2;

    sateliteAngularSpeed = M_PI_2;
    sateliteRot = 0;
    sateliteDistancePlanet = 2;


    angleJump = 0.f;

    startPos = glm::vec3(1, 1, 1);
    endPos = glm::vec3(1 + 4, 1, 1);

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();
    Sun(deltaTimeSeconds);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    RenderScene();
    DrawCoordinateSystem();
    Sun(deltaTimeSeconds);
}
void Lab4::Sun(float deltaTimeSeconds) {
    // mutarea soarelui stanga-dreapta

    if (moveSunRight) {
        sunMoveX += sunSpeed * deltaTimeSeconds;
        if (sunMoveX > 10)
            moveSunRight = false;
    }
    else {
        sunMoveX -= sunSpeed * deltaTimeSeconds;
        if (sunMoveX < -10)
            moveSunRight = true;
    }
    
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0 + sunMoveX, 1, -5);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // mutarea planetei in jurul soarelui

    planetRot += planetAngularSpeed * deltaTimeSeconds;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(sunMoveX + planetDistanceSun, 0.5, -5) * // pozitie care sa urmeze miscarea soarelui
        transform3D::RotateOY(planetRot) * // rotatia in jurul soarelui
        transform3D::Translate(planetDistanceSun, 0.5, -5) * // rotatia in jurul soarelui
        transform3D::Scale(0.5, 0.5, 0.5) *
        transform3D::RotateOY(planetRot); // rotire in jurul axei pamantului
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // nu mai resetez modelul
    sateliteRot += sateliteAngularSpeed * deltaTimeSeconds;
    modelMatrix *= transform3D::RotateOY(sateliteRot) * // rotatia in jurul satelitului
        transform3D::Translate(sateliteDistancePlanet, 0.2, 0) * // pozitionez pt a o roti
        transform3D::Scale(0.2, 0.2, 0.2) *
        transform3D::RotateOY(sateliteRot); // rotire in jurul axei sale
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}
void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)
    // rotatie
    // move up
    if (window->KeyHold(GLFW_KEY_W)) {
        translateY += deltaTime * 2;
    }
    // move left
    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= deltaTime / 2;
    }
    // move down
    if (window->KeyHold(GLFW_KEY_S)) {
        translateY -= deltaTime * 2;
    }
    // move right
    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += deltaTime / 2;
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        translateZ -= deltaTime * 2;
    }
    // move right
    if (window->KeyHold(GLFW_KEY_F)) {
        translateZ += deltaTime / 2;
    }

    // scalare
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime / 2;
        scaleY += deltaTime / 2;
        scaleZ += deltaTime / 2;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime / 2;
        scaleY -= deltaTime / 2;
        scaleZ -= deltaTime / 2;
    }

    // rotatie
    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += deltaTime / 2;

    }
    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= deltaTime / 2;

    }
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += deltaTime / 2;

    }
    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= deltaTime / 2;

    }
    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += deltaTime / 2;

    }
    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= deltaTime / 2;
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    // TODO(student): Add viewport movement and scaling logic
    if (key == GLFW_KEY_I) {
        miniViewportArea.x += 10;
    }
    if (key == GLFW_KEY_J) {
        miniViewportArea.x -= 10;
    }
    if (key == GLFW_KEY_K) {
        miniViewportArea.y += 10;
    }
    if (key == GLFW_KEY_L) {
        miniViewportArea.y -= 10;
    }
    if (key == GLFW_KEY_U) {
        miniViewportArea.width *= 2;
        miniViewportArea.height *= 2;
    }
    if (key == GLFW_KEY_O) {
        miniViewportArea.width /= 2;
        miniViewportArea.height /= 2;
    }

}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
