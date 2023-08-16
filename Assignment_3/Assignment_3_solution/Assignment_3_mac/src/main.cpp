////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 3 of SUTD Course 50.017 (May-Aug Term, 2022)
//
//    Texture Mapping
//
//  2022-June-23
//
//
////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include <fstream>
#include <chrono>
#include <thread>
#include "math.h"
#include "shaderSource.h"
#include "shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



using namespace std;


#define MAX_BUFFER_SIZE            1024

#define _ROTATE_FACTOR              0.005f
#define _SCALE_FACTOR               0.01f
#define _TRANS_FACTOR               0.02f

#define _Z_NEAR                     0.001f
#define _Z_FAR                      100.0f


/***********************************************************************/
/**************************   global variables   ***********************/
/***********************************************************************/

// Window size
unsigned int winWidth  = 800;
unsigned int winHeight = 600;

// Camera
glm::vec3 camera_position = glm::vec3 (0.0f, 0.0f, 3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float camera_fovy = 45.0f;
glm::mat4 projection;

// Mouse interaction
bool leftMouseButtonHold = false;
bool isFirstMouse = true;
float prevMouseX;
float prevMouseY;
glm::mat4 modelMatrix = glm::mat4(1.0f);

// Vectors to save mesh data
vector<float> render_ver_nor_tex;       // List of points and normals for rendering
vector<unsigned> render_f;              // List of faces for rendering

//main data for rendering
Object myObject;

// render
unsigned int VBO, VAO,EBO;


// declaration
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);




///=========================================================================================///
///                    Functions to be filled in for Assignment 3    
///          IMPORTANT: you ONLY need to work on these functions in this section      
///=========================================================================================///


//// TODO: fill this function to realize plane mapping
void calcPlaneMapping(void)
{
    /* calculate plane mapping                                 */
    /* loop over all vertices and update myObject.vertices[i].t */

    float x, y;

    for( int i  = 0 ; i < myObject.vertices.size(); i++)
    {
        x = myObject.vertices[i].v[0];
        y = myObject.vertices[i].v[1];

        myObject.vertices[i].t[0] = (x - myObject.bBox.minP[0]) / (myObject.bBox.maxP[0]-myObject.bBox.minP[0]);
        myObject.vertices[i].t[1] = (y - myObject.bBox.minP[1]) / (myObject.bBox.maxP[1]-myObject.bBox.minP[1]);
    }
}
//// TODO: fill this function to realize cylindrical mapping
void calcCylindricalMapping(void)
{
    /* calculate cylindrical mapping                           */
    /* loop over all vertices and update myObject.vertices[i].t */

    float x, y, z;
    float theta;

    for( int i  = 0 ; i < myObject.vertices.size(); i++)
    {
        x = myObject.vertices[i].v[0];
        y = myObject.vertices[i].v[1];
        z = myObject.vertices[i].v[2];
        theta = atan2(y, x) + PI;

        myObject.vertices[i].t[0] = theta/(2*PI);
        myObject.vertices[i].t[1] = ( z - myObject.bBox.minP[2] )/( myObject.bBox.maxP[2] - myObject.bBox.minP[2] );
    }
}
//// TODO: fill this function to realize sphere mapping
void calcSphereMapping(void)
{
    /* calculate spherical mapping                             */
    /* loop over all vertices and update myObject.vertices[i].t */

    float x, y, z, r;
    float theta, phi;

    for( int i  = 0 ; i < myObject.vertices.size(); i++)
    {
        x = myObject.vertices[i].v[0];
        y = myObject.vertices[i].v[1];
        z = myObject.vertices[i].v[2];
        r = sqrt(x*x+y*y);
        theta = atan2(y,x) + PI;
        phi = atan2(r,z);

        myObject.vertices[i].t[0] = theta/(2*PI);
        myObject.vertices[i].t[1] = phi/PI;
    }
}




///=========================================================================================///
///                            Functions to Load and Render a 3D Model 
///=========================================================================================///

//// scale the model to the same size
void scaleToUnitBox(void)
{
    Point maxP;
    Point minP;

    maxP[0] = maxP[1] = maxP[2] = -1e35;
    minP[0] = minP[1] = minP[2] =  1e35;

    Vector vBoxSize;
    Point  bboxCenterP;

    for (unsigned int i = 0; i < myObject.vertices.size(); i++)
    {
        Vertex& v = myObject.vertices[i];

        if (v.v[0] < minP[0])     minP[0] = v.v[0];
        if (v.v[1] < minP[1])     minP[1] = v.v[1];

        if (v.v[2] < minP[2])     minP[2] = v.v[2];
        if (v.v[0] > maxP[0])     maxP[0] = v.v[0];

        if (v.v[1] > maxP[1])     maxP[1] = v.v[1];
        if (v.v[2] > maxP[2])     maxP[2] = v.v[2];
    }

    subPnt(vBoxSize, maxP, minP);

    addScaled(bboxCenterP, minP, vBoxSize, 0.5);

    float modelScale = 1.f / maxComponent3(vBoxSize);

    for (unsigned int i = 0; i < myObject.vertices.size(); i++)
    {
        Vertex& v = myObject.vertices[i];

        v.v[0] = (v.v[0] - bboxCenterP[0]) * modelScale;
        v.v[1] = (v.v[1] - bboxCenterP[1]) * modelScale;
        v.v[2] = (v.v[2] - bboxCenterP[2]) * modelScale;
    }

    myObject.bBox.minP[0] = (minP[0] - bboxCenterP[0]) * modelScale;
    myObject.bBox.minP[1] = (minP[1] - bboxCenterP[1]) * modelScale;
    myObject.bBox.minP[2] = (minP[2] - bboxCenterP[2]) * modelScale;

    myObject.bBox.maxP[0] = (maxP[0] - bboxCenterP[0]) * modelScale;
    myObject.bBox.maxP[1] = (maxP[1] - bboxCenterP[1]) * modelScale;
    myObject.bBox.maxP[2] = (maxP[2] - bboxCenterP[2]) * modelScale;

    myObject.bBox.vSize[0] = vBoxSize[0] * modelScale;
    myObject.bBox.vSize[1] = vBoxSize[1] * modelScale;
    myObject.bBox.vSize[2] = vBoxSize[2] * modelScale;
}

int LoadInput()
{
    /////////////////////////////////////////////////////
    // 1. Open the input file

    // Input file name
    string inputString;

    //taking from standard input
    cout << "Please enter filename.obj: ";
    cin >> inputString;
    cout << "Displaying: " << inputString <<endl;

    ifstream myfile(inputString);

    if (myfile.is_open() == false)
    {
        cout << "Error: cannot open the file.";
        return 1;
    }


    /////////////////////////////////////////////////////
    // 2. Read data from the input file

    vector<glm::vec3> vecv;         // This is the list of points (3D vectors)
    vector<glm::vec3> vecn;         // This is the list of normals (also 3D vectors)
    vector<glm::vec3> vect;         // This is the list of texture coordinates (optional)
    vector<vector<unsigned>> vecf;  // This is the list of faces (indices into vecv and vecn)

    glm::vec3 v;
    string s;
    char buffer[MAX_BUFFER_SIZE];

    // load the OBJ file here
    //writing to vecv, vecn, and vecf

    cout << "obj file is open"<<endl;
    while ( myfile.getline(buffer, MAX_BUFFER_SIZE) )
    {
        stringstream ss(buffer);

        ss >> s;

        //printf("%s\n", s.c_str());

        if (s == "v"){
            ss >> v[0] >> v[1] >> v[2];

            vecv.push_back(glm::vec3 (v[0],v[1],v[2]));
        }
        else if (s == "vn"){
            ss >> v[0] >> v[1] >> v[2];

            vecn.push_back(glm::vec3 (v[0],v[1],v[2]));
        }
        else if (s == "vt"){
            ss >> v[0] >> v[1] >> v[2];
            vect.push_back(glm::vec3 (v[0],v[1],v[2]));

        }
        else if (s == "f"){
            string abc;
            string def;
            string ghi;

            vector<unsigned> vu;
            int counter = 0;

            while (counter < 3){
                ss >> abc;

                char charArray1[1024];
                strncpy(charArray1, abc.c_str(), sizeof(charArray1));
                charArray1[sizeof(charArray1) - 1] = 0;

                char *result = strtok(charArray1, "/");
                while (result != NULL){
                    int number = atoi(result);
                    vu.push_back(number);
                    result = strtok (NULL, "/");
                }
                counter ++;
            }

            vecf.push_back(vu);

        }
    }
    myfile.close();

    //checking and transfer
    cout<<"number of vertexes " <<vecv.size()<<endl;
    cout<<"number of normals " <<vecn.size()<<endl;
    cout<<"number of triangle faces " <<vecf.size()<<endl;

    vector<glm::vec3> vecn_reorder; // same order as the vecv
    vector<glm::vec3> vecf_reorder;
    vecn_reorder.resize(vecv.size());
    vecf_reorder.resize(vecf.size());

    // Re-order the normal vector
    for (int i = 0; i < vecf.size(); ++i)
    {
        // Case 1: input file contains vertex positions, normals, and texture coordinates
        if (vecf[i].size() == 9 )
        {
            vecf_reorder[i][0] = vecf[i][0]-1;
            vecf_reorder[i][1] = vecf[i][3]-1;
            vecf_reorder[i][2] = vecf[i][6]-1;

            vecn_reorder[vecf[i][0]-1] = vecn[vecf[i][2]-1];
            vecn_reorder[vecf[i][3]-1] = vecn[vecf[i][5]-1];
            vecn_reorder[vecf[i][6]-1] = vecn[vecf[i][8]-1];
        }
            // Case 2: input file contains vertex positions and normals
        else if (vecf[i].size() == 6 )
        {
            vecf_reorder[i][0] = vecf[i][0]-1;
            vecf_reorder[i][1] = vecf[i][2]-1;
            vecf_reorder[i][2] = vecf[i][4]-1;

            vecn_reorder[vecf[i][0]-1] = vecn[vecf[i][1]-1];
            vecn_reorder[vecf[i][2]-1] = vecn[vecf[i][3]-1];
            vecn_reorder[vecf[i][4]-1] = vecn[vecf[i][5]-1];
        }
            // Case 3: input file contains vertex positions only
        else if (vecf[i].size() == 3 )
        {
            vecf_reorder[i][0] = vecf[i][0]-1;
            vecf_reorder[i][1] = vecf[i][1]-1;
            vecf_reorder[i][2] = vecf[i][2]-1;
        }
        else
        {
            cout<<"obj format error"<<endl;
        }
    }

    myObject.vertices.clear();

    for (int i = 0; i < vecv.size(); i++)
    {
        Vertex ver;

        ver.v[0] = vecv[i][0];
        ver.v[1] = vecv[i][1];
        ver.v[2] = vecv[i][2];

        ver.n[0] = vecn_reorder[i][0];
        ver.n[1] = vecn_reorder[i][1];
        ver.n[2] = vecn_reorder[i][2];

        ver.t[0] = 0.0f;
        ver.t[1] = 0.0f;

        myObject.vertices.push_back( ver );
    }

    Face face;
    myObject.faces.clear();
    for (int i = 0; i < vecf_reorder.size(); i++)
    {
        face.v1 = vecf_reorder[i][0];
        face.v2 = vecf_reorder[i][1];
        face.v3 = vecf_reorder[i][2];

        myObject.faces.push_back(face);
    }

    scaleToUnitBox();

    return 0;
}

bool CreateRenderData()
{
    if (0 == myObject.vertices.size())
    {
        return false;
    }
    else
    {

        render_ver_nor_tex.clear();
        render_f.clear();

        for (int i = 0; i < myObject.vertices.size(); ++i) 
        {

            render_ver_nor_tex.push_back(myObject.vertices[i].v[0]);
            render_ver_nor_tex.push_back(myObject.vertices[i].v[1]);
            render_ver_nor_tex.push_back(myObject.vertices[i].v[2]);

            render_ver_nor_tex.push_back(myObject.vertices[i].n[0]);
            render_ver_nor_tex.push_back(myObject.vertices[i].n[1]);
            render_ver_nor_tex.push_back(myObject.vertices[i].n[2]);

            render_ver_nor_tex.push_back(myObject.vertices[i].t[0]);
            render_ver_nor_tex.push_back(myObject.vertices[i].t[1]);
        }

        for (int j = 0; j < myObject.faces.size(); ++j) 
        {
            render_f.push_back(myObject.faces[j].v1 );
            render_f.push_back(myObject.faces[j].v2 );
            render_f.push_back(myObject.faces[j].v3 );
        }

        return true;
    }
}




///=========================================================================================///
///                            Functions for Manipulating 3D Model  
///=========================================================================================///

void RotateModel(float angle, glm::vec3 axis)
{
    glm::vec3 rotateCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::translate(rotateMatrix, rotateCenter);
    rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
    rotateMatrix = glm::translate(rotateMatrix, -rotateCenter);

    modelMatrix = rotateMatrix * modelMatrix;
}

void TranslateModel(glm::vec3 transVec)
{
    glm::mat4 translateMatrix = glm::mat4(1.0f);
    translateMatrix = glm::translate(translateMatrix, transVec);

    modelMatrix = translateMatrix * modelMatrix;
}

void ScaleModel(float scale)
{
    glm::vec3 scaleCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::translate(scaleMatrix, scaleCenter);
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale));
    scaleMatrix = glm::translate(scaleMatrix, -scaleCenter);

    modelMatrix = scaleMatrix * modelMatrix;
}




///=========================================================================================///
///                                    Callback Functions
///=========================================================================================///

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);

    winWidth  = width;
    winHeight = height;
}


// glfw: whenever a key is pressed, this callback is called
// ----------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        calcCylindricalMapping();
        CreateRenderData();

        // load data into vertex buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, render_ver_nor_tex.size() * sizeof(float), &render_ver_nor_tex[0], GL_STATIC_DRAW);
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        calcSphereMapping();
        CreateRenderData();

        // load data into vertex buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, render_ver_nor_tex.size() * sizeof(float), &render_ver_nor_tex[0], GL_STATIC_DRAW);

    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        calcPlaneMapping();
        CreateRenderData();

        // load data into vertex buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, render_ver_nor_tex.size() * sizeof(float), &render_ver_nor_tex[0], GL_STATIC_DRAW);
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftMouseButtonHold = true;
    }
    else
    {
        leftMouseButtonHold = false;
    }
}


// glfw: whenever the cursor moves, this callback is called
// -------------------------------------------------------
void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY)
{
    float  dx, dy;
    float  nx, ny, scale, angle;


    if ( leftMouseButtonHold )
    {
        if (isFirstMouse)
        {
            prevMouseX = mouseX;
            prevMouseY = mouseY;
            isFirstMouse = false;
        }

        else
        {
            if( glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
            {
                float dx =         _TRANS_FACTOR * (mouseX - prevMouseX);
                float dy = -1.0f * _TRANS_FACTOR * (mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

                TranslateModel( glm::vec3(dx, dy, 0) );
            }

            else
            {
                float dx =   mouseX - prevMouseX;
                float dy = -(mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

                // Rotation
                nx    = -dy;
                ny    =  dx;
                scale = sqrt(nx*nx + ny*ny);

                // We use "ArcBall Rotation" to compute the rotation axis and angle based on the mouse motion
                nx    = nx / scale;
                ny    = ny / scale;
                angle = scale * _ROTATE_FACTOR;

                RotateModel( angle, glm::vec3(nx, ny, 0.0f) );
            }
        }
    }

    else
    {
        isFirstMouse = true;
    }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    float scale = 1.0f + _SCALE_FACTOR * yOffset;

    ScaleModel( scale );
}





///=========================================================================================///
///                                      Main Function
///=========================================================================================///

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Assignment 3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // construct the shaders
    //shader
    shader myShader;
    myShader.setUpShader(vertexShaderSource,fragmentShaderSource);

    LoadInput();

    CreateRenderData();

    // create buffers/arrays for surface
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, render_ver_nor_tex.size() * sizeof(float), &render_ver_nor_tex[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_f.size() * sizeof(unsigned int), &render_f[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), ((void*)(3* sizeof(float))));
    glEnableVertexAttribArray(1);

    // vertex texture coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // load and create a texture
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../data/texture.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // render mesh
    myShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // ------
        glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera_fovy), (float)winWidth / (float)winHeight, _Z_NEAR, _Z_FAR);
        glm::mat4 view = glm::lookAt(camera_position, camera_target, camera_up);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        // render the loaded model
        glm::vec3 aColor = glm::vec3 (0.9f, 0.9f, 0.9f);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "aColor"), 1, &aColor[0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera_position[0]);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, render_f.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(myShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

