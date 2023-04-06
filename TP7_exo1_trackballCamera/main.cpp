#include <iostream>
#include <vector>
#include "glimac/TrackballCamera.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

static bool right_rotation = false;
static bool left_rotation  = false;
static bool up_rotation    = false;
static bool down_rotation  = false;
static bool zoom_in        = false;
static bool zoom_out       = false;

struct EarthProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.id(), "uTexture");
        uCloudTexture = glGetUniformLocation(m_Program.id(), "uTexture2");
    }
};

struct MoonProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
    }
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX2"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // on charge les shaders en multi shaders
    EarthProgram earthProgram{};
    MoonProgram  moonProgram{};

    // on charge les textures
    img::Image terre_map = p6::load_image_buffer("/home/pauline/Documents/S4/openGL/TP_OPENGL/assets/textures/EarthMap.jpg");

    img::Image lune_map = p6::load_image_buffer("/home/pauline/Documents/S4/openGL/TP_OPENGL/assets/textures/MoonMap.jpg");

    img::Image nuage_map = p6::load_image_buffer("/home/pauline/Documents/S4/openGL/TP_OPENGL/assets/textures/CloudMap.jpg");

    // On charge les shaders
    // const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl");

    // shader.use();

    // Variables uniformes
    // GLint uMVPMatrix_location    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    // GLint uMVMatrix_location     = glGetUniformLocation(shader.id(), "UMVMatrix");
    // GLint uNormalMatrix_location = glGetUniformLocation(shader.id(), "uNormalMatrix");
    // GLint uTexture_location      = glad_glGetUniformLocation(shader.id(), "uTexture");
    // GLint uTexture2_location     = glad_glGetUniformLocation(shader.id(), "uTexture2");

    glEnable(GL_DEPTH_TEST);

    /* texture de la terre */
    GLuint tex_terre = 0;
    glGenTextures(1, &tex_terre);

    glBindTexture(GL_TEXTURE_2D, tex_terre);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terre_map.width(), terre_map.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, terre_map.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    /* texture de la lune */
    GLuint tex_lune = 0;
    glGenTextures(1, &tex_lune);

    glBindTexture(GL_TEXTURE_2D, tex_lune);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lune_map.width(), lune_map.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, lune_map.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    /* texture des nuages */
    GLuint tex_nuage = 0;
    glGenTextures(1, &tex_nuage);

    glBindTexture(GL_TEXTURE_2D, tex_nuage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nuage_map.width(), nuage_map.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nuage_map.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Vertices de la sphere
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Creation du VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind du VBO sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // On envoie les données du vbo
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Debind du VBO (on evite des modifications involontaires)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creation du VAO;
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Bind le VAO
    glBindVertexArray(vao);

    // On dit à opengl qu'on bind le vao
    const GLuint VERTEX_ATTR_POSITION   = 0;
    const GLuint VERTEX_ATTR_NORMAL     = 1;
    const GLuint VERTEX_ATTR_TEXTCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORDS);

    // on indique à openGL où trouver les sommets
    // bind le vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // on indique le format de l'attribut de sommet position
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));

    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));

    glVertexAttribPointer(VERTEX_ATTR_TEXTCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    // debind du VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // debind du VAO
    glBindVertexArray(0);

    TrackBallCamera        camera;
    std::vector<glm::vec3> AxesRotation;
    std::vector<glm::vec3> AxesTranslation;

    for (int i = 0; i < 32; i++)
    {
        AxesRotation.push_back(glm::sphericalRand(1.f));
        AxesTranslation.push_back(glm::sphericalRand(2.f));
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        if (right_rotation)
        {
            camera.rotateLeft(-5.f);
        }
        if (left_rotation)
        {
            camera.rotateLeft(5.f);
        }
        if (up_rotation)
        {
            camera.rotateUp(5.f);
        }
        if (down_rotation)
        {
            camera.rotateUp(-5.f);
        }
        if (zoom_in)
        {
            camera.moveFront(0.1f);
        }
        if (zoom_out)
        {
            camera.moveFront(-0.1f);
        }

        glm::mat4 viewCamera = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        // planète

        earthProgram.m_Program.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_terre);
        glUniform1i(earthProgram.uEarthTexture, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_nuage);
        glUniform1i(earthProgram.uEarthTexture, 1);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        // glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        glm::mat4 MVMatrix     = glm::rotate(glm::mat4(1), 0.5f * ctx.time(), {0.f, 1.f, 0.f});
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * viewCamera * MVMatrix));
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1

        // lune

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_lune);
        glUniform1i(moonProgram.uTexture, 0);

        moonProgram.m_Program.use();

        for (int i = 0; i < 32; i++)
        {
            MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f});       // Translation
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), AxesRotation.at(i)); // Translation * Rotation
            MVMatrix = glm::translate(MVMatrix, AxesTranslation.at(i));       // Translation * Rotation * Translation
            MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});                 // Translation * Rotation * Translation * Scale

            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * viewCamera * MVMatrix));
            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);
    };

    ctx.key_pressed = [&](p6::Key key) {
        if (key.physical == GLFW_KEY_LEFT)
        {
            left_rotation = true;
        }
        if (key.physical == GLFW_KEY_RIGHT)
        {
            right_rotation = true;
        }
        if (key.physical == GLFW_KEY_W)
        {
            up_rotation = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            down_rotation = true;
        }
        if (key.physical == GLFW_KEY_UP)
        {
            zoom_in = true;
        }
        if (key.physical == GLFW_KEY_DOWN)
        {
            zoom_out = true;
        }
    };

    ctx.key_released = [&](p6::Key key) {
        if (key.physical == GLFW_KEY_LEFT)
        {
            left_rotation = false;
        }
        if (key.physical == GLFW_KEY_RIGHT)
        {
            right_rotation = false;
        }
        if (key.physical == GLFW_KEY_W)
        {
            up_rotation = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            down_rotation = false;
        }
        if (key.physical == GLFW_KEY_UP)
        {
            zoom_in = false;
        }
        if (key.physical == GLFW_KEY_DOWN)
        {
            zoom_out = false;
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteTextures(1, &tex_terre);
    glDeleteTextures(1, &tex_lune);
    glDeleteTextures(1, &tex_nuage);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
