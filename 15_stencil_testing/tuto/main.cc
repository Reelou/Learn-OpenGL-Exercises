#include "model/model.hh"

#include <SOIL.h>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hh"
#include <shader.hh>

void updateCamera(Camera &camera, float deltaTime, sf::Vector2f &formerPosition);
GLuint loadTexture(GLchar* path);

int main() {
  // Creating window
  sf::Window *window = new sf::Window(sf::VideoMode(800, 600), "OpenGL",
                                      sf::Style::Default, sf::ContextSettings(32));
  window->setVerticalSyncEnabled(true);
  glClearColor(0.07, 0.07, 0.07, 1);
  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;
  if (glewInit() == GLEW_OK)
    std::cout << "Glew initialized successfully" << std::endl;
  // Define the viewport dimensions
  glViewport(0, 0, 800, 600);

  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // Setup and compile our shaders
  Shader shader("tuto/shaders/advanced.vs.glsl", "tuto/shaders/advanced.frag.glsl");
  Shader shaderSingleColor("tuto/shaders/advanced.vs.glsl", "tuto/shaders/border.frag.glsl");

#pragma region "object_initialization"
  // Set the object data (buffers, vertex attributes)
  GLfloat cubeVertices[] = {
      // Positions          // Texture Coords
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  GLfloat planeVertices[] = {
      // Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
      5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

      5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
      5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
  };
  // Setup cube VAO
  GLuint cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);
  // Setup plane VAO
  GLuint planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);

  // Load textures
  GLuint cubeTexture = loadTexture((GLchar*)"../resources/textures/pattern4diffuseblack.jpg");
  GLuint floorTexture = loadTexture((GLchar*)"../resources/textures/metal.png");
#pragma endregion

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
  bool running = true;
  sf::Clock clock;
  sf::Vector2f mousePosition(sf::Mouse::getPosition());
  while (running) {
    sf::Event event;
    while (window->pollEvent(event))
      if (event.type == sf::Event::Closed)
        running = false;
    updateCamera(camera, clock.getElapsedTime().asSeconds(), mousePosition);
    clock.restart();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Set uniforms
    shaderSingleColor.Use();
    glm::mat4 model;
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(camera.getZoom(), (float)800/(float)600, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderSingleColor.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderSingleColor.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    // Draw floor as normal, we only care about the containers. The floor should NOT fill the stencil buffer so we set its mask to 0x00
    glStencilMask(0x00);
    // Floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    model = glm::mat4();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // == =============
    // 1st. Render pass, draw objects as normal, filling the stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    // Cubes
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // ===============
    // 2nd. Render pass, now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are now not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    shaderSingleColor.Use();
    GLfloat scale = 1.1f;
    // Cubes
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    glUniformMatrix4fv(glGetUniformLocation(shaderSingleColor.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    glUniformMatrix4fv(glGetUniformLocation(shaderSingleColor.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);

    window->display();
  }
  delete window;
}

GLuint loadTexture(GLchar* path)
{
  //Generate texture ID and load texture data
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width,height;
  unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
  // Assign texture to ID
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Parameters
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
  return textureID;
}

// Camera controls
void updateCamera(Camera &camera, float deltaTime, sf::Vector2f &formerPosition) {
  sf::Vector2f newPosition(sf::Mouse::getPosition());
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
  camera.processMouse(newPosition.x - formerPosition.x,
                      formerPosition.y - newPosition.y);
  formerPosition = newPosition;
}
