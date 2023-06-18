//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalStuff.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

#include "cShaderManager/cShaderManager.h"

#include "cVAOManager/cVAOManager.h"
#include "cLightManager.h"
// Now with textures!
#include "cBasicTextureManager/cBasicTextureManager.h"

#include "cMeshObject.h"

#include "cLightHelper.h"

#include "cFlyCamera.h"

//#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

extern unsigned int selectedMeshIndex;
extern unsigned int g_SelectedLightIndex;

struct sVertexXYZ_RGB
{
    float x, y, z;      // vec2 to vec3 
    float r, g, b;
};

// Google C++ dynamic array
// On the heap
//sVertexXYZ_RGB* pSexyVertex = new sVertexXYZ_RGB[3];
//
//// On the stack
//sVertexXYZ_RGB pVertexArray[3] =
//{
//    //   X      Y     Z     r     g     b
//    { -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },            // Spawn a vertex shader instance
//    {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },
//    {  0.0f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f }
// 
//};

// Will allocate this in a moment...
sVertexXYZ_RGB* pVertexArray = NULL;    

cMeshObject* g_pSkySphere = NULL;   // new cMeshObject();

extern glm::vec3 g_cameraEye = glm::vec3(9.8f, 0.0, 21.2f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, +1.0f, 0.0f);

// In IOHandlers.cpp
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// This is a list of the objects we want to draw in this scene
//std::vector< cMeshObject > g_vecMeshesToDraw;       // From this...
extern std::vector< cMeshObject* > g_vec_pMeshesToDraw;    // ...to this (vector of pointers)


// Returns true if there were no errors.
// If there are errors, vecErrors has error detail text.
bool LoadModels(cVAOManager* pModelManger, GLuint shaderProgram,
                std::vector< cMeshObject* >& vec_pMeshesToDraw,
                std::vector<std::string>& vecErrors,
                bool bLogProgressToConsole = true);

bool DrawObject(cMeshObject* pMeshToDraw,
                glm::mat4 matModel,
                cVAOManager* pModelManger,
                GLuint shaderProgram_ID);

void SetUpTexturesForMesh(cMeshObject* pCurrentMesh, 
                          GLuint shaderProgram_ID, 
                          cBasicTextureManager* pTheTextures);


//void ChangeLightSphereVisibility(bool bIsOn);
void DrawDebugLightSpheres(cVAOManager* pModelManager, GLuint shaderProgram_ID);

// Returns NULL (or 0) if not found
// Up to about 30-40 items, the linear search will be faster because of cache issues
cMeshObject* pFindObjectByFriendlyName(std::string theName);


cLightManager* g_pTheLights = NULL;
bool g_bShowDebugLightSpheres = true;

cFlyCamera* g_pFlyCamera = NULL;


int main(void)
{
    GLFWwindow* window = NULL;
    GLint vpos_location = 0;
    GLint vcol_location = 0;

    glfwSetErrorCallback(error_callback);

    if ( ! glfwInit() )
    {
//        exit(EXIT_FAILURE);
        return -1;
    }



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if ( ! window )
    {
        glfwTerminate();
//        exit(EXIT_FAILURE);
        return -1;
    }

   // ::g_pFlyCamera = new cFlyCamera();
   // g_pFlyCamera->setEye(-2.0, 2.0, 30.0f);
    // More for "typing" style stuff
    glfwSetKeyCallback(window, key_callback);

    // Mouse callbacks
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );
    glfwSwapInterval(1);




    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragementShader;

    pShaderManager->setBasePath("assets/shaders/");

    vertexShader.fileName = "vertexShader01.glsl";
    fragementShader.fileName = "fragmentShader01.glsl";

   if ( !pShaderManager->createProgramFromFile( "basicShader",
                                                 vertexShader, fragementShader ) )
   {
       std::cout << "Error: Can't create shader program:" << std::endl;
       std::cout << pShaderManager->getLastError() << std::endl;
   }

//   glUseProgram(program);
    GLuint shaderProgram_ID = pShaderManager->getIDFromFriendlyName("basicShader");
    glUseProgram(shaderProgram_ID);

    // Load the models into the VAO Manager

    cVAOManager* pModelManger = new cVAOManager();

    std::vector<std::string> vecErrors;
    if ( ! LoadModels(pModelManger, shaderProgram_ID, ::g_vec_pMeshesToDraw, vecErrors, true ) )
    {
        std::cout << "There were errors loading some models:" << std::endl;
        for ( std::string errText : vecErrors )
        {
            std::cout << errText << std::endl;
        }
    }//if ( ! LoadModels(...

    // Load the textures
    cBasicTextureManager* pTheTextures = new cBasicTextureManager();

    // Load some textures
    pTheTextures->SetBasePath("assets/textures");

   
    if ( pTheTextures->Create2DTextureFromBMPFile("txt_001_diff.bmp", true) )
    {
        std::cout << "Loaded txt_001_diff" << std::endl;
    }    
    if ( pTheTextures->Create2DTextureFromBMPFile("Texture_01_A.bmp", true) )
    {
        std::cout << "Loaded Texture_01_A" << std::endl;
    }    
    if ( pTheTextures->Create2DTextureFromBMPFile("BlueWhale.bmp", true) )
    {
        std::cout << "Loaded whale" << std::endl;
    }    

    if ( pTheTextures->Create2DTextureFromBMPFile("Texture_04_C.bmp", true) )
    {
        std::cout << "Loaded Texture_04_C" << std::endl;
    }  

    if (pTheTextures->Create2DTextureFromBMPFile("Texture_01_B.bmp", true))
    {
        std::cout << "Loaded Texture_01_B" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("YoungKiwiVines.bmp", true))
    {
        std::cout << "Loaded YoungKiwiVines" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("oceanWave.bmp", true))
    {
        std::cout << "Loaded oceanWave" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("TropicalFish01.bmp", true))
    {
        std::cout << "Loaded TropicalFish01" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("TropicalFish01x.bmp", true))
    {
        std::cout << "Loaded TropicalFish01x" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("TropicalFish02.bmp", true))
    {
        std::cout << "Loaded TropicalFish02" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("rock.bmp", true))
    {
        std::cout << "Loaded rock" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("sandTexture.bmp", true))
    {
        std::cout << "Loaded sandTexture" << std::endl;
    }


    if (pTheTextures->Create2DTextureFromBMPFile("shark.bmp", true))
    {
        std::cout << "Loaded shark" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("jellyFish.bmp", true))
    {
        std::cout << "Loaded jellyFish" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("underWater.bmp", true))
    {
        std::cout << "Loaded underWater" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("octopus.bmp", true))
    {
        std::cout << "Loaded octopus" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("skull.bmp", true))
    {
        std::cout << "Loaded skull" << std::endl;
    }


    if (pTheTextures->Create2DTextureFromBMPFile("plant.bmp", true))
    {
        std::cout << "Loaded plant" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("waterTexture.bmp", true))
    {
        std::cout << "Loaded waterTexture" << std::endl;
    }

    if (pTheTextures->Create2DTextureFromBMPFile("dark.bmp", true))
    {
        std::cout << "Loaded dark" << std::endl;
    }
    if (pTheTextures->Create2DTextureFromBMPFile("fish.bmp", true))
    {
        std::cout << "Loaded fish" << std::endl;
    }
    if (pTheTextures->Create2DTextureFromBMPFile("fish2.bmp", true))
    {
        std::cout << "Loaded fish2" << std::endl;
    }
    if (pTheTextures->Create2DTextureFromBMPFile("fish3.bmp", true))
    {
        std::cout << "Loaded fish3" << std::endl;
    }
   



    std::string errorString;
    pTheTextures->SetBasePath("assets/Textures/cubemap_textures");
    if ( pTheTextures->CreateCubeTextureFromBMPFiles("spaceSkybox",
                                                     "xpos.bmp", "xneg.bmp", 
                                                     "ypos.bmp" , "yneg.bmp",
                                                     "zpos.bmp", "zneg.bmp",
                                                     true, errorString ) )
    {
        std::cout << "Loaded the SpaceBox skybox OK" << std::endl;
    }
    else
    {
        std::cout << "Error loading the SpaceBox skybox: " << errorString << std::endl;
    }
                                                     
    pTheTextures->SetBasePath("assets/Textures/cubemap_textures");
    pTheTextures->CreateCubeTextureFromBMPFiles("tropicalDay",
                                                "bluecloud_lf.bmp","bluecloud_rt.bmp",
                                                "bluecloud_dn.bmp", "bluecloud_up.bmp", // Up and down revered in this cube map (is DirectX or Unreal format)
                                                "bluecloud_ft.bmp", "bluecloud_bk.bmp",
                                                true, errorString);

    // this is a height map (grey scale) 
   
  /*  if ( pTheTextures->Create2DTextureFromBMPFile("discardTexture.bmp", true) )
    {
        std::cout << "Loaded discardTexture.bmp" << std::endl;
    }    */

    
    ::g_pTheLights = new cLightManager();
    ::g_pTheLights->LoadUniformLocationsFromShader(shaderProgram_ID);

    float shininess = 1.0f;
    ::g_pTheLights->myLights[0].position = glm::vec4(84.9817f, -3.0f, 290.7997f,  0.0f);
    ::g_pTheLights->myLights[0].diffuse = glm::vec4(0.9f, 0.9f, 0.95f, 1.0f);    // Slightly warm white light
    ::g_pTheLights->myLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.01);    // Bright white light
    ::g_pTheLights->myLights[0].atten.x = 1.0f;     // Constant
    ::g_pTheLights->myLights[0].atten.y = 0.00816554f;     // Linear attenuation
    ::g_pTheLights->myLights[0].atten.z = 0.00001f;   // Quadratic attenuation 
    ::g_pTheLights->myLights[0].param1.x = 0.0f;    // 0 = Point light
    ::g_pTheLights->myLights[0].param2.x = 1.0f;    // 1 = on, 0 is off


    float heyHeyILoveMath = 0.0f;
   
    float nearPlane = 0.1f;          // Near plane
    float farPlane = 1000.0f;      // Far plane

    // Time right now
    double previousTime = glfwGetTime();
    float elapsedTime = 0.0f;
    bool isBlendingForward = true;

    // When this while exits, your program exits, too
    while (!glfwWindowShouldClose(window))
    {
        {   // how do you get the frame time? 
            // Get delta or frame time
            double currentTime = glfwGetTime();

            // currentTime is always more than previous time;
            double deltaTime = currentTime - previousTime;
            // Check to see if it's REALLY big. 
            if (deltaTime > 0.01) {
                deltaTime = 0.01;   // Some minimum
            }
            // You might also want to save the last X frames in a vector, then get the 
            // average, so any variation is evened out.

            // Save the last time
            previousTime = currentTime;
            elapsedTime += static_cast<float>(deltaTime);
            // Move something 10 units per second.
            //cTheMeshIMove.position.x += (10.0f * deltaTime);

 //           std::cout << deltaTime << " " << deltaTime * 1000.0f << " ms" << std::endl;
        }



        // Turn on "alpha transparency"
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

    //        mat4x4 m, p, mvp;
    //    glm::mat4 m, mvp;
        glm::mat4 mModel = glm::mat4(1.0f);
        glm::mat4 mProjection = glm::mat4(1.0f);
        glm::mat4 mView = glm::mat4(1.0f);

        mProjection = glm::perspective(0.6f,
                                         ratio,
                                         nearPlane, // 1.0f,          // Near plane
                                         farPlane); // 100000.0f);      // Far plane

        GLint matProjection_UL = glGetUniformLocation(shaderProgram_ID, "matProjection");
        glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(mProjection));


        // Look at the spider
//        cMeshObject* pSpider = pFindObjectByFriendlyName("Shipwereck");
////        pSpider->position.z -= 0.01f;
//
//
//        // Look at the spider with a spot light
//        // Direction is normalized vector from light to target
//        glm::vec3 rayToTarget = pSpider->position - glm::vec3(::g_pTheLights->myLights[1].position);
//        // Normlize makes the ray 1 unit long
//        rayToTarget = glm::normalize(rayToTarget);  // Also can divide by length of ray
//
//        // The direction is 4 floats because I used vec4 because the registers are all vec4s.
//        ::g_pTheLights->myLights[1].direction = glm::vec4(rayToTarget, 1.0f);


      

//        ::g_cameraTarget = pSpider->position;

        // Is there a fly camera?
        if (::g_pFlyCamera)
        {
            ::g_cameraEye = ::g_pFlyCamera->getEye();
            ::g_cameraTarget = ::g_pFlyCamera->getAt();
        }

        mView = glm::lookAt(::g_cameraEye,
                            ::g_cameraTarget,  // newTarget
                            ::g_upVector);

        GLint matView_UL = glGetUniformLocation(shaderProgram_ID, "matView");
        glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(mView));



        // Update the camera 'eye' location in the shader
        // uniform vec3 eyeLocation;
        GLint eyeLocation_UniformLocation = glGetUniformLocation(shaderProgram_ID, "eyeLocation");
        glUniform3f(eyeLocation_UniformLocation,
                    ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z);

        // Turn on "depth buffer" testing
        glEnable(GL_DEPTH_TEST);


        // Adding clearing the "depth" buffer 
       // glClearColor(0.0f, 0.66f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Change the screen clear colour
        // 100, 149, 237 (cornflower blue)
     //   glClearColor(100.0f/255.0f, 149.0f/255.0f, 237.0f/255.0f, 1.0f);

        // Update all the light stuff
        ::g_pTheLights->UpdateLightInfoToShader(shaderProgram_ID);



        if ( ::g_bShowDebugLightSpheres )
        {
            DrawDebugLightSpheres(pModelManger, shaderProgram_ID);
        }



        {// Draw skybox
            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

            // Proportional to the far plane
            // i.e. "big enough" that it's "far away" from the camera
            //      but not "too big" - parts of it are past the far plane.
            //      (if they are, then they won't be rendered)
            ::g_pSkySphere->scale = farPlane * 0.5f;

            // Place this sphere where the camera is
            ::g_pSkySphere->position = ::g_cameraEye;

            {// We'll use texture unit #20 for the first texture and #21 for the second texture
                GLint textureUnitNumber1 = 20;
                GLint textureUnitNumber2 = 21;

                // Pick texture unit #20 for the first texture
                glActiveTexture(GL_TEXTURE0 + textureUnitNumber1);
                GLuint skyboxTextureNumber1 = pTheTextures->getTextureIDFromName("tropicalDay");
                glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureNumber1);

                // Pick texture unit #21 for the second texture
                glActiveTexture(GL_TEXTURE0 + textureUnitNumber2);
                GLuint skyboxTextureNumber2 = pTheTextures->getTextureIDFromName("spaceSkybox");
                glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureNumber2);

                // uniform samplerCube skyBoxTexture;
                GLint skyBoxTexture1_UniformLocation = glGetUniformLocation(shaderProgram_ID, "skyBoxTexture1");
                glUniform1i(skyBoxTexture1_UniformLocation, textureUnitNumber1);

                GLint skyBoxTexture2_UniformLocation = glGetUniformLocation(shaderProgram_ID, "skyBoxTexture2");
                glUniform1i(skyBoxTexture2_UniformLocation, textureUnitNumber2);

                double currentTime = glfwGetTime();
                double deltaTime = currentTime - previousTime;
                if (deltaTime > 0.01) {
                    deltaTime = 0.01; // Some minimum
                }
                previousTime = currentTime;

                // Calculate the blend factor based on the elapsed time
                float transitionDuration = 10.0f; // Transition duration in seconds
                elapsedTime += static_cast<float>(deltaTime); // Update the elapsed time
                float blendFactor = elapsedTime / transitionDuration;
                if (!isBlendingForward) {
                    blendFactor = 1.0f - blendFactor; // Reverse blend factor if blending backward
                }
                blendFactor = glm::clamp(blendFactor, 0.0f, 1.0f); // Clamp the ble

                // uniform float blendFactor;
                GLint blendFactor_UniformLocation = glGetUniformLocation(shaderProgram_ID, "blendFactor");
                glUniform1f(blendFactor_UniformLocation, blendFactor); // Set the desired blending ratio (between 0 and 1)
                if (elapsedTime >= transitionDuration) {
                    // Reverse the blend direction
                    isBlendingForward = !isBlendingForward;
                    // Reset the elapsed time to continue the blending
                    elapsedTime = 0.0f;
                }
            }//Texture #0


            //uniform bool bIsSkyBoxObject;
            GLint bIsSkyBoxObject_UniformLocation = glGetUniformLocation(shaderProgram_ID, "bIsSkyBoxObject");
            glUniform1f(bIsSkyBoxObject_UniformLocation, (GLfloat)GL_TRUE);

            DrawObject(::g_pSkySphere, matModel, pModelManger, shaderProgram_ID);

            //uniform bool bIsSkyBoxObject;
            glUniform1f(bIsSkyBoxObject_UniformLocation, (GLfloat)GL_FALSE);
        }// Draw skybox




        for ( cMeshObject* pCurrentMesh : ::g_vec_pMeshesToDraw )
        {

            SetUpTexturesForMesh(pCurrentMesh, shaderProgram_ID, pTheTextures);


            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

            DrawObject(pCurrentMesh, matModel, pModelManger, shaderProgram_ID);

        }//for (std::vector< cMeshObject >


        {
            cMeshObject* ocean = pFindObjectByFriendlyName("oceanWave");

            SetUpTexturesForMesh(ocean, shaderProgram_ID, pTheTextures);

            // First texture
            glActiveTexture(GL_TEXTURE24);    // GL_TEXTURE0 = 33984
            GLuint heightMapTextureNumber = pTheTextures->getTextureIDFromName("oceanWave.bmp");
            glBindTexture(GL_TEXTURE_2D, heightMapTextureNumber);
            GLint heightMapUniformLocation = glGetUniformLocation(shaderProgram_ID, "heightMapTexture");
            glUniform1i(heightMapUniformLocation, 24);

            // Turn the mapping on
            GLint bIsHeightMapUniformLocation = glGetUniformLocation(shaderProgram_ID, "bIsHeightMap");
            GLint heightScaleUniformLocation = glGetUniformLocation(shaderProgram_ID, "heightScale");
            GLint heightMapOffsetUVUniformLocation = glGetUniformLocation(shaderProgram_ID, "heightMapOffsetUV");
            glUniform1f(bIsHeightMapUniformLocation, (GLfloat)GL_TRUE);
            glUniform1f(heightScaleUniformLocation, 0.05f);

            // First texture movement
            float amplitude = 0.2f;   // Adjust the wave amplitude
            float frequency = 0.1f;   // Adjust the wave frequency
            float speed = 0.35f;      // Adjust the wave speed
            float waveHeight = static_cast<float>(amplitude * sin((ocean->position.x + ocean->position.z) * frequency + glfwGetTime() * speed));
            glUniform2f(heightMapOffsetUVUniformLocation, waveHeight, 0.0f);

            // Second texture
            glActiveTexture(GL_TEXTURE25);    // GL_TEXTURE1
            GLuint secondTextureNumber = pTheTextures->getTextureIDFromName("waterTexture.bmp");
            glBindTexture(GL_TEXTURE_2D, secondTextureNumber);
            GLint secondTextureUniformLocation = glGetUniformLocation(shaderProgram_ID, "secondTexture");
            glUniform1i(secondTextureUniformLocation, 25);

            GLint bIsSecondHeightMapUniformLocation = glGetUniformLocation(shaderProgram_ID, "bIsSecondHeightMap");
            GLint secondHeightScaleUniformLocation = glGetUniformLocation(shaderProgram_ID, "secondHeightScale");
            GLint secondHeightMapOffsetUVUniformLocation = glGetUniformLocation(shaderProgram_ID, "secondHeightMapOffsetUV");
            glUniform1f(bIsSecondHeightMapUniformLocation, (GLfloat)GL_TRUE);
            glUniform1f(secondHeightScaleUniformLocation, 0.04f);

            // Second texture movement
            float amplitude2 = 0.3f;  // Adjust the wave amplitude
            float frequency2 = 0.1f;  // Adjust the wave frequency
            float speed2 = 0.15f;     // Adjust the wave speed
            float waveHeight2 = static_cast<float>(amplitude2 * sin((ocean->position.x - ocean->position.z) * frequency2 - glfwGetTime() * speed2));
            glUniform2f(secondHeightMapOffsetUVUniformLocation, waveHeight2, 0.0f);

          
            glEnable(GL_DEPTH_TEST);

        
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

    
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 
            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix
            ocean->bIsVisible = true;
            DrawObject(ocean, matModel, pModelManger, shaderProgram_ID);
            ocean->bIsVisible = false;

           
            //glDepthMask(GL_FALSE);
      

            glUniform1f(bIsHeightMapUniformLocation, (GLfloat)GL_FALSE);
            glUniform1f(bIsSecondHeightMapUniformLocation, (GLfloat)GL_FALSE);
        }

  

        glfwSwapBuffers(window);
        glfwPollEvents();       // Checks keyboard, mouse, joystick, etc.

        // Handle asyn keyboard inputs
        handleKeyboardInput(window);

        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z
            << " Light[" << g_SelectedLightIndex << "] "
            << "(xyz: " << ::g_pTheLights->myLights[g_SelectedLightIndex].position.x
            << ", " << ::g_pTheLights->myLights[g_SelectedLightIndex].position.y
            << ", " << ::g_pTheLights->myLights[g_SelectedLightIndex].position.z << ") "
            << " l_atten: " << ::g_pTheLights->myLights[g_SelectedLightIndex].atten.y
            << " q_atten: " << ::g_pTheLights->myLights[g_SelectedLightIndex].atten.z
            << " Selected Mesh " << ::selectedMeshIndex
            << " " << g_vec_pMeshesToDraw[selectedMeshIndex]->meshName;


        glfwSetWindowTitle( window, ssTitle.str().c_str() );

    }

    // STARTOF: Clean up...
    delete ::g_pFlyCamera;
    delete ::g_pTheLights;
    delete pModelManger;

    for ( cMeshObject* pCurrentMesh : ::g_vec_pMeshesToDraw )
    {
        delete pCurrentMesh;
    }
    ::g_vec_pMeshesToDraw.clear();
    // ENDOF: Clean up

    glfwDestroyWindow(window);

    glfwTerminate();
//    exit(EXIT_SUCCESS);
    return 0;
}

// Returns NULL (or 0) if not found
// Up to about 30-40 items, the linear search will be faster because of cache issues
cMeshObject* pFindObjectByFriendlyName(std::string theName)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++)
    {
        if (::g_vec_pMeshesToDraw[index]->friendlyName == theName)
        {
            // Found it!
            cMeshObject* pFoundMesh = ::g_vec_pMeshesToDraw[index];
            return pFoundMesh;
        }
    }
    // didn't find it
    return NULL;
}

void DrawDebugLightSpheres(cVAOManager* pModelManager, GLuint shaderProgram_ID)
{

    //cMeshObject* pDebugSphere = pFindObjectByFriendlyName("DebugSphere");
    //if ( ! pDebugSphere)
    //{
    //    // Didn't find the Debug Sphere model
    //    return;
    //}


    //// Save whatever state the debug sphere is currently in
    //cMeshObject::sMeshState oldState;
    //oldState.SaveState(pDebugSphere);

    //pDebugSphere->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);
    //pDebugSphere->bDontLight = true;
    //pDebugSphere->isWireframe = true;
    //pDebugSphere->bIsVisible = true;

    //const float LIGHT_SPHERES_TRANSPARENCY = 0.4f;
    //pDebugSphere->alphaTransparency = LIGHT_SPHERES_TRANSPARENCY; 


    //// Centre of light:
    //pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    //pDebugSphere->scale = 0.1f;
    //DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    //// To calculate the size of the debug spheres based on attenuation
    //cLightHelper myLightHelper;


    //// 90% of full brightness...
    //float distance90percent = myLightHelper.calcApproxDistFromAtten(0.9f,        // Target 90% brightness
    //                                                        0.001f,      // Within 0.001 of that value
    //                                                        100000.0f,    // Give up if I get this far away
    //                                                        ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
    //                                                        ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
    //                                                        ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    //pDebugSphere->diffuseColour = glm::vec3(0.0f, 1.0f, 1.0f);
    //pDebugSphere->scale = distance90percent;
    //DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);



    //// 50% of full brightness...
    //float distance50percent = myLightHelper.calcApproxDistFromAtten(0.5f,        // Target 90% brightness
    //                                                                0.001f,      // Within 0.001 of that value
    //                                                                100000.0f,    // Give up if I get this far away
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    //pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 0.0f);  // Yellow
    //pDebugSphere->scale = distance50percent;
    //DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    //// 25% of full brightness...
    //float distance25percent = myLightHelper.calcApproxDistFromAtten(0.25f,        // Target 90% brightness
    //                                                                0.001f,      // Within 0.001 of that value
    //                                                                100000.0f,    // Give up if I get this far away
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    //pDebugSphere->diffuseColour = glm::vec3(0.0f, 1.0f, 0.0f);  // Light blue or Cyan
    //pDebugSphere->scale = distance25percent;
    //DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    //// 5% of full brightness...
    //float distance05percent = myLightHelper.calcApproxDistFromAtten(0.05f,        // Target 90% brightness
    //                                                                0.001f,      // Within 0.001 of that value
    //                                                                100000.0f,    // Give up if I get this far away
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
    //                                                                ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    //pDebugSphere->diffuseColour = glm::vec3(0.0f, 0.0f, 2.0f);  // Blue
    //pDebugSphere->scale = distance05percent;
    //DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    //// Restore the old debug sphere state
    //oldState.RestoreState(pDebugSphere);




    return;
}// END OF: Place the sphere where the light #0 was90Percent


void SetUpTexturesForMesh(cMeshObject* pCurrentMesh, GLuint shaderProgram_ID, cBasicTextureManager* pTheTextures)
{
    // ***********************************************************
    // We love this, don't we? 


    {// Texture #0
        GLint textureNumber = 0;

        // Pick a texture unit (any one you want, up to 80)
        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture00 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[0]);

        glBindTexture(GL_TEXTURE_2D, meshTexture00);

        // uniform sampler2D texture01;
        GLint texture00_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture00");
        glUniform1i(texture00_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0


    {// Texture #1
        GLint textureNumber = 1;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture01 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[1]);
        glBindTexture(GL_TEXTURE_2D, meshTexture01);

        // uniform sampler2D texture01;
        GLint texture01_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture01");
        glUniform1i(texture01_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0

    {// Texture #2
        GLint textureNumber = 2;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture02 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[2]);
        glBindTexture(GL_TEXTURE_2D, meshTexture02);

        // uniform sampler2D texture01;
        GLint texture02_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture02");
        glUniform1i(texture02_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0

    {// Texture #3
        GLint textureNumber = 3;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture02 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[3]);
        glBindTexture(GL_TEXTURE_2D, meshTexture02);

        // uniform sampler2D texture01;
        GLint texture03_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture03");
        glUniform1i(texture03_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0

    {// Texture #3
        GLint textureNumber = 4;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture02 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[4]);
        glBindTexture(GL_TEXTURE_2D, meshTexture02);

        // uniform sampler2D texture01;
        GLint texture03_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture04");
        glUniform1i(texture03_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0


    // And set the mixing values as well
    //uniform vec4 textureMix_00_to_03;
    GLint textureMix_00_to_03_UniformLocation = glGetUniformLocation(shaderProgram_ID, "textureMix_00_to_03");
    // 
    glUniform4f(textureMix_00_to_03_UniformLocation,
                pCurrentMesh->textureMixingRatio[4],
                pCurrentMesh->textureMixingRatio[1],
                pCurrentMesh->textureMixingRatio[2],
                pCurrentMesh->textureMixingRatio[3]);


    return;
}
 
// ***********************************************************