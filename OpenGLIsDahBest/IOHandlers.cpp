#include "globalStuff.h"

#include "cLightManager.h"
#include "cFlyCamera.h"

#include "cMeshObject.h"
#include <string>
#include <iostream>
#include <fstream>

extern cLightManager* g_pTheLights;// = NULL;

extern bool g_bShowDebugLightSpheres;// = true;

unsigned int g_SelectedLightIndex = 0;



std::vector<cMeshObject* > g_vec_pMeshesToDraw;
unsigned int selectedMeshIndex = 1;

// Defined in theMain.cpp
cMeshObject* pFindObjectByFriendlyName(std::string theName);
void  SavingMeshObjects(std::vector< cMeshObject* > g_vec_pMeshesToDraw);

extern cFlyCamera* g_pFlyCamera;

bool g_MouseLeftButtonDown = true;
float g_MouseLastWheelPosition = 0.0f;
bool g_InitialMousePositionSampled = false;
glm::vec2 g_MouseLastPosition = glm::vec2(0.0f, 0.0f);

// Called when the mouse moves over the window
// Can also get this with:
//        double xpos, ypos;
//        glfwGetCursorPos(window, &xpos, &ypos);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // The mouse positions aren't zero at start up, 
    //  so wait until the 1st callback to set the initial values
    //if ( ! ::g_InitialMousePositionSampled )
    //{
    //    ::g_MouseLastPosition.x = (float)xpos;
    //    ::g_MouseLastPosition.y = (float)ypos;
    //    ::g_InitialMousePositionSampled = true;
    //}

    //if ( ::g_MouseLeftButtonDown )
    //{
    //    float deltaX = (float)xpos - ::g_MouseLastPosition.x;
    //    float deltaY = (float)ypos - ::g_MouseLastPosition.y;

    //    if (deltaX > 0.0f)
    //    {
    //        ::g_pFlyCamera->RotateOrYawLeft(deltaX);
    //    }
    //    if (deltaX < 0.0f)
    //    {
    //        ::g_pFlyCamera->RotateOrYawRight(-deltaX);
    //    }

    //    if (deltaY > 0.0f)
    //    {
    //        ::g_pFlyCamera->PitchUp(deltaY);
    //    }
    //    if (deltaY < 0.0f)
    //    {
    //        ::g_pFlyCamera->PitchDown(-deltaY);
    //    }
    //}//if ( ::g_MouseLeftButtonDown )

    //::g_MouseLastPosition.x = static_cast<float>(xpos);
    //::g_MouseLastPosition.y = static_cast<float>(ypos);

    //return;
}

// Called when the mouse buttons are pressed (or relased) over the window
// Can also get this with:
//        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
//        if (state == GLFW_PRESS)
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        ::g_MouseLeftButtonDown = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        ::g_MouseLeftButtonDown = false;
    }

    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float movementSpeed = ::g_pFlyCamera->getMovementSpeed();

    const float MOUSE_WHEEL_CHANGE_SENSITIVITY = 0.1f;

    movementSpeed += ((float)yoffset * MOUSE_WHEEL_CHANGE_SENSITIVITY);

    if ( movementSpeed < 0.0f )
    {
        movementSpeed = 0.0f;
    }

    ::g_pFlyCamera->setMovementSpeed(movementSpeed);

    return;
}

//    void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Turns off and on the "debug spheres"
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        ::g_bShowDebugLightSpheres = !::g_bShowDebugLightSpheres;
    }

    // Is the shift and ONLY the shift down?
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        if ((key == GLFW_KEY_7) && (action == GLFW_PRESS))
        {
            ::g_SelectedLightIndex++;
            // Too large? 
            if (::g_SelectedLightIndex == cLightManager::NUMBEROFLIGHTS )
            {
                ::g_SelectedLightIndex = 0;
            }
        }        

        if ((key == GLFW_KEY_6) && (action == GLFW_PRESS))
        {
            if ( ::g_SelectedLightIndex == 0 )
            {
                ::g_SelectedLightIndex = cLightManager::NUMBEROFLIGHTS - 1;
            }
            else
            {
                ::g_SelectedLightIndex--;
            }
        }       

    }// if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)



    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other keys as well
        if ((key == GLFW_KEY_1) && action == GLFW_PRESS) {

            ::selectedMeshIndex++;
            std::cout << ::g_vec_pMeshesToDraw.size() << std::endl;
            if (::selectedMeshIndex == ::g_vec_pMeshesToDraw.size()) {
                ::selectedMeshIndex = 0;
            }
        }

        if ((key == GLFW_KEY_2) && action == GLFW_PRESS) {

            if (::selectedMeshIndex == 0) {
                ::selectedMeshIndex = static_cast<unsigned int>(::g_vec_pMeshesToDraw.size() - 1);
            }
            else {
                ::selectedMeshIndex--;
            }

        }

    }


   const float CAMERA_MOVE_SPEED = 0.1f;

    if (mods == GLFW_MOD_SHIFT)
    {
        // Shift ONLY is down
    }

    // Note this is a bit mask 
    // 0001 0010 0100 
    // 0111 &
    // 0110
    // ----
    // 0000
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other keys as well
    }


    //if ( key == GLFW_KEY_A )    
    //{
    //    // Move "left"
    //    ::g_cameraEye.x += CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_D )    
    //{
    //    // Move "right"
    //    ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
    //}

    //if ( key == GLFW_KEY_W )    
    //{
    //    // Move "forward"
    //    ::g_cameraEye.z += CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_S )    
    //{
    //    // Move "backwards"
    //    ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
    //}

    //if ( key == GLFW_KEY_Q )    
    //{
    //    // Move "down"
    //    ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_E )    
    //{
    //    // Move "up"
    //    ::g_cameraEye.y += CAMERA_MOVE_SPEED;
    //}
}

bool isShiftDown(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

bool isCtrlDown(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        return true;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        return true;
    }
    return false;

}

void handleKeyboardInput(GLFWwindow* window)
{
    const float CAMERA_MOVE_SPEED = 0.1f;
//    const float CAMERA_MOVE_SPEED = 10.0f;
    const float LIGHT_MOVE_SPEED = 0.05f;
    //const float CAMERA_MOVE_SPEED = 10.0f;
    //const float LIGHT_MOVE_SPEED = 5.0f;

    if (isShiftDown(window))
    {
        // I'll be messing with the lights...
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            // Move "left"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            // Move "right"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.x -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            // Move "forward"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.z += CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            // Move "backwards"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.z -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            // Move "down"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.y -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
            // Move "up"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.y += CAMERA_MOVE_SPEED;
        }

        const float LINEAR_ATTEN_CHANGE_PERCENT = 0.01f;

        if (glfwGetKey(window, GLFW_KEY_1))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.y *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_2))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.y *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }

        if (glfwGetKey(window, GLFW_KEY_3))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.z *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_4))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.z *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }

        //glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
        const float SPOT_LIGHT_ANGLE_CHANGE_PERCENT = 0.001f;

        if (glfwGetKey(window, GLFW_KEY_UP))        // Increase inner angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.y *= (1.0f + SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN))      // Decrease inner angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.y *= (1.0f - SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT))    // Increase outer angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.z *= (1.0f + SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT))      // Decrease outer angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.z *= (1.0f - SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }

        if (glfwGetKey(window, GLFW_KEY_5))
        {
//            ::g_pTheLights->myLights[g_SelectedLightIndex].param2.x = 1.0f;
            ::g_pTheLights->myLights[g_SelectedLightIndex].TurnOn();
        }
        if (glfwGetKey(window, GLFW_KEY_6))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].TurnOff();
//            ::g_pTheLights->myLights[g_SelectedLightIndex].param2.x = 0.0f;
        }    

        // Oh no!!!
//        if (glfwGetKey(window, GLFW_KEY_7))
//        {
//            ::g_SelectedLightIndex++;
//        }



    }
    else if (isCtrlDown(window))
    {
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            // Move "left"
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.x += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.y += 0.1f;

        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.z += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_Y))
        {
            // Move "left"
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.x -= 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_U))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.y -= 0.01f;

        }
        if (glfwGetKey(window, GLFW_KEY_I))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->orientation.z -= 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            // Move "left"
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.x += 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_W))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.y += 0.01f;

        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.z += 0.01f;

        }
        if (glfwGetKey(window, GLFW_KEY_Z))
        {
            // Move "left"
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.x -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_X))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.y -= 0.1f;

        }
        if (glfwGetKey(window, GLFW_KEY_C))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->position.z -= 0.1f;

        }
        if (glfwGetKey(window, GLFW_KEY_R))
        {
            g_vec_pMeshesToDraw[selectedMeshIndex]->scale -= 0.01f;

        }
        if (glfwGetKey(window, GLFW_KEY_T)) {
            g_vec_pMeshesToDraw[selectedMeshIndex]->scale += 0.01f;

        }
    }//if (isCTRlDown(window)
    else   // Do camera stuff
    {
        // Use fly camera?
        if (::g_pFlyCamera)
        {
            // Do camera stuff
            if (glfwGetKey(window, GLFW_KEY_A))
            {
                // Strafe left
                ::g_pFlyCamera->StrafeLeft();
                std::cout << "Left" << std::endl;
            }

            if (glfwGetKey(window, GLFW_KEY_D))
            {
                // Strafe right
                ::g_pFlyCamera->StrafeRight();
            }

            if (glfwGetKey(window, GLFW_KEY_W))
            {
                // Move "forward"
                ::g_pFlyCamera->MoveBackwards();
            }
            if (glfwGetKey(window, GLFW_KEY_S))
            {
                // Move "backwards"
                ::g_pFlyCamera->MoveForwards();
            }

            if (glfwGetKey(window, GLFW_KEY_Q))
            {
                // Move "down"
                ::g_pFlyCamera->MoveDown();
            }
            if (glfwGetKey(window, GLFW_KEY_E))
            {
                // Move "up"
                ::g_pFlyCamera->MoveUp();
            }
        }
        else
        {
            // Do camera stuff
            if (glfwGetKey(window, GLFW_KEY_A))
            {
                // Move "left"
                ::g_cameraEye.x += CAMERA_MOVE_SPEED;
            }

            if (glfwGetKey(window, GLFW_KEY_D))
            {
                // Move "right"
                ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
            }

            if (glfwGetKey(window, GLFW_KEY_W))
            {
                // Move "forward"
                ::g_cameraEye.z += CAMERA_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_S))
            {
                // Move "backwards"
                ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
            }

            if (glfwGetKey(window, GLFW_KEY_Q))
            {
                // Move "down"
                ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_E))
            {
                // Move "up"
                ::g_cameraEye.y += CAMERA_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_1)) {
                ::g_cameraEye = glm::vec3(11.6f, -1.2f, 5.99995f);
            }
            if (glfwGetKey(window, GLFW_KEY_2)) {
                ::g_cameraEye = glm::vec3(-19.7f, 30.001f, 0.599957f);
            }
            if (glfwGetKey(window, GLFW_KEY_3)) {
                ::g_cameraEye = glm::vec3(9.8f, 0.0, 21.2f);
            }
        }// if (::g_pFlyCamera)
        if (glfwGetKey(window, GLFW_KEY_V)) {
            SavingMeshObjects(g_vec_pMeshesToDraw);
        }

    }//if (isShiftDown(window)

    return;
}


void  SavingMeshObjects(std::vector< cMeshObject* > g_vec_pMeshesToDraw) {
    std::ofstream MyFile("MeshOjects.txt");
    if (MyFile.is_open()) {

        for (unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++)
        {
            cMeshObject* currentMesh = ::g_vec_pMeshesToDraw[index];

            MyFile << currentMesh->meshName << "\n";
            MyFile << index << "\n";
            MyFile << currentMesh->position.x << "f, " << currentMesh->position.y << "f, " << currentMesh->position.z <<"f " << "\n";
            MyFile << currentMesh->orientation.x << "f, " << currentMesh->orientation.y << "f, " << currentMesh->orientation.z << "f "<< "\n";
            MyFile << currentMesh->diffuseColour.r << "f, " << currentMesh->diffuseColour.g << "f, " << currentMesh->diffuseColour.b << "f " << "\n";
            MyFile << currentMesh->scale << "\n";
            MyFile << "\n";

        }
    }
    else {
        std::cout << "couldn't write a file" << std::endl;
    }
}