#include "Engine.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Color.h"
#include "Shader/Shader.h"
#include "Window/Window.h"
#include "Window/Input/Input.h"

void Engine::setup()
{
}

void Engine::update()
{
}

void Engine::run()
{
    setup();
    float FirstFrame = 0.0f;

    glm::vec3 color(Color::Blue);
    while(!glfwWindowShouldClose(Window))
    {
        const auto CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - FirstFrame;
        FirstFrame = CurrentFrame;
        glClearColor(color.x, color.y, color.z, 1.f);
        glClear(GL_COLOR_BUFFER_BIT    | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Shader::Program);
		
        update();
		
        glUniformMatrix4fv(MainCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getProjection(Window::Width, Window::Height)));
        glUniformMatrix4fv(MainCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getView()));
        

        KeyBoardInput::processInput(Window, &Player);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwTerminate();
}
