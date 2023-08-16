////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 1 of SUTD Course 50.017 (May-Aug Term, 2022)
//
//    Mesh Viewer
//
//  2022-May-26
//
//
////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderSource.h"
#include "shader.h"


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
glm::vec3 camera_position = glm::vec3 (0.0f, 0.0f, 15.0f);
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

 // Mesh color table
glm::vec3 colorTable[4] = 
 {
    glm::vec3(0.6, 1.0, 0.6),
    glm::vec3(1.0, 0.6, 0.6),
    glm::vec3(0.6, 0.6, 1.0),
    glm::vec3(1.0, 1.0, 0.6) 
};

// Mesh rendering color
int colorID = 0;
glm::vec3  meshColor;


// declaration
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



/******************************************************************************/
/***************   Functions to be filled in for Assignment 1    **************/
/***************    IMPORTANT: you ONLY need to work on these    **************/
/***************                functions in this section        **************/
/******************************************************************************/


// TODO: insert your code in this function for Mesh Loading
//       1) store vertices and normals in verList with order (v.x, v.y, v.z, n.x, n.y, n.z)
//       2) store vertex indices of each triangle in triList 
int LoadInput(vector<float> &verList, vector<unsigned> &triList)
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

            //DEBUGGING
            //for (int i=0; i<vu.size(); i++)
            //{
            //   cout << " " << vu.at(i);
            //}
            //cout << "done" << endl;

        }
    }
    myfile.close();

    //checking and transfer
    cout<<"number of vertexes " <<vecv.size()<<endl;
    cout<<"number of normals " <<vecn.size()<<endl;
    cout<<"number of triangle faces " <<vecf.size()<<endl;


    ////////////////////////////////////////////////////
    // 3. Conver the stored data into data for rendering

    verList.clear();
    triList.clear();

    vector<glm::vec3> vecn_reorder; // same order as the vecv
    vecn_reorder.resize(vecv.size());

    for (int i = 0; i < vecf.size(); ++i) 
    {
        // Case 1: input file contains vertex positions, normals, and texture coordinates
        if (vecf[i].size() == 9 )
        {
            triList.push_back(vecf[i][0]-1);
            triList.push_back(vecf[i][3]-1);
            triList.push_back(vecf[i][6]-1);

            vecn_reorder[vecf[i][0]-1] = vecn[vecf[i][2]-1];
            vecn_reorder[vecf[i][3]-1] = vecn[vecf[i][5]-1];
            vecn_reorder[vecf[i][6]-1] = vecn[vecf[i][8]-1];
        }
        // Case 2: input file contains vertex positions and normals
        else if (vecf[i].size() == 6 )
        {
            triList.push_back(vecf[i][0]-1);
            triList.push_back(vecf[i][2]-1);
            triList.push_back(vecf[i][4]-1);

            vecn_reorder[vecf[i][0]-1] = vecn[vecf[i][1]-1];
            vecn_reorder[vecf[i][2]-1] = vecn[vecf[i][3]-1];
            vecn_reorder[vecf[i][4]-1] = vecn[vecf[i][5]-1];
        }
        // Case 3: input file contains vertex positions only
        else if (vecf[i].size() == 3 )
        {
            triList.push_back(vecf[i][0]-1);
            triList.push_back(vecf[i][1]-1);
            triList.push_back(vecf[i][2]-1);
        } 
        else
        {
            cout<<"obj format error"<<endl;
        }
    }

    // transfer to render data (vertex)
    for (int i = 0; i < vecv.size(); ++i) 
    {
        // save vertices
        verList.push_back(vecv[i][0]);
        verList.push_back(vecv[i][1]);
        verList.push_back(vecv[i][2]);

        // save face normals
        verList.push_back(vecn_reorder[i][0]);
        verList.push_back(vecn_reorder[i][1]);
        verList.push_back(vecn_reorder[i][2]);
    }

    return 0;
}

// TODO: insert your code in this function for Mesh Coloring
void SetMeshColor(int &colorID)
{
    colorID++;
    if( colorID == 4 )
        colorID = 0;

    meshColor = colorTable[colorID];
}

// TODO: insert your code in this function for Mesh Transformation (Rotation)
void RotateModel(float angle, glm::vec3 axis)
{
    glm::vec3 rotateCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::translate(rotateMatrix, rotateCenter);
    rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
    rotateMatrix = glm::translate(rotateMatrix, -rotateCenter);

   	modelMatrix = rotateMatrix * modelMatrix;
}

// TODO: insert your code in this function for Mesh Transformation (Translation)
void TranslateModel(glm::vec3 transVec)
{
    glm::mat4 translateMatrix = glm::mat4(1.0f);
    translateMatrix = glm::translate(translateMatrix, transVec);

    modelMatrix = translateMatrix * modelMatrix;
}

// TODO: insert your code in this function for Mesh Transformation (Scaling)
void ScaleModel(float scale)
{
    glm::vec3 scaleCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::translate(scaleMatrix, scaleCenter);
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale));
    scaleMatrix = glm::translate(scaleMatrix, -scaleCenter);

   	modelMatrix = scaleMatrix * modelMatrix;
}




/******************************************************************************/
/***************                  Callback Function              **************/
/******************************************************************************/

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
        SetMeshColor( colorID );
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




/******************************************************************************/
/***************                    Main Function                **************/
/******************************************************************************/

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
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Assignment 1 - Mesh Viewer", NULL, NULL);
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

    // tell GLFW to capture our mouse
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

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    shader myShader;
    myShader.setUpShader(vertexShaderSource,fragmentShaderSource);

    // Load input mesh data
    vector<float> verList;          // This is the list of vertices and normals for rendering
    vector<unsigned> triList;       // This is the list of faces for rendering
    LoadInput(verList, triList);

    // create buffers/arrays
    unsigned int VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verList.size() * sizeof(float), &verList[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triList.size() * sizeof(unsigned int), &triList[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3* sizeof(float))));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    myShader.use();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera_fovy), (float)winWidth / (float)winHeight, _Z_NEAR, _Z_FAR);
        glm::mat4 view = glm::lookAt(camera_position, camera_target, camera_up);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        // render the loaded model
        //glm::vec3 aColor = glm::vec3 (0.6f, 1.0f, 0.6f);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "meshColor"), 1, &colorTable[colorID][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera_position[0]);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, triList.size(), GL_UNSIGNED_INT, 0);
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

