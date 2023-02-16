#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;
    moveSpeed = 100;
    up_move = true;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;
    grow = false;
    growSpeed = 3;

    // Initialize angularStep
    angularStep = 0;
    rotateSpeed = 5;

    // Bonus
    moveX = 0;
    moveY = 0;
    jumpForce = 8;
    accY = jumpForce;
    rotateJump = 0;
    G = 50;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(1, 0, 1));
    AddMeshToList(square4);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!

    if (up_move == true) {
        translateX += deltaTimeSeconds * moveSpeed;
        translateY += deltaTimeSeconds * moveSpeed;
        if (translateY > 600)
            up_move = false;
    }
    else {
        translateX -= deltaTimeSeconds * moveSpeed;
        translateY -= deltaTimeSeconds * moveSpeed;
        if (translateY < 250)
            up_move = true;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);

    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    angularStep += -rotateSpeed * deltaTimeSeconds;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400 + cy, 250 + cy) * transform2D::Rotate(angularStep) * transform2D::Translate(-cx, -cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    if (grow == true) {
        scaleX += growSpeed * deltaTimeSeconds;
        scaleY += growSpeed * deltaTimeSeconds;
        if (scaleX > 5)
            grow = false;
    }
    else {
        scaleX -= growSpeed * deltaTimeSeconds;
        scaleY -= growSpeed * deltaTimeSeconds;
        if (scaleX < 1)
            grow = true;
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650 + cx, 250 + cy) * transform2D::Scale(scaleX, scaleY) * transform2D::Translate(-cx, -cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);


    // Bonus
    moveY += accY * G * deltaTimeSeconds;
    accY -= 0.1;

    if (moveY < 0) {
        moveY = 0;
        accY = jumpForce;
        return;
    }

    if (right == true) {
        moveX += moveSpeed * 2 * deltaTimeSeconds;
        if (moveX > 1200)
            right = false;
    }
    else {
        moveX -= moveSpeed * 2 * deltaTimeSeconds;
        if (moveX < 0)
            right = true;
    }

    rotateJump = 180 * (jumpForce + accY) / (2 * jumpForce * G);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(moveX, moveY);
    modelMatrix *= transform2D::Translate(cx, cy) * transform2D::Rotate(rotateJump) * transform2D::Translate(-cx, -cy);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
