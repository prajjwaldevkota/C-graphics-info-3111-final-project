#include "globalStuff.h"

#include <vector>
#include <string>
#include "cVAOManager/cVAOManager.h"
#include "cMeshObject.h"
#include <iostream>
void AddObjects(const std::string& meshName, const glm::vec3& colour, const glm::vec3& position, const glm::vec3& orientation, float scale, const std::string& textureName, float textureMixingRatio, float alphaTransparency);
extern cMeshObject* g_pSkySphere;// = NULL;  
extern std::vector< cMeshObject* > g_vec_pMeshesToDraw;
// Returns true if there were no errors.
// If there are errors, vecErrors has error detail text.
bool LoadModels(cVAOManager* pModelManger, GLuint shaderProgram,
                std::vector< cMeshObject* > &vec_pMeshesToDraw,
                std::vector<std::string> &vecErrors, 
                bool bLogProgressToConsole /*=true*/)
{
    bool bAnyErrors = false;

    std::vector<std::string> vecModelsToLoad;


    // this is our "skybox" model
    vecModelsToLoad.push_back("assets/models/Isoshphere_smooth_inverted_normals_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Shipwreck_01_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Barrel_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Fish_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Seafloor2_ONLY_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Shark_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Whale_uv.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Bld_Dock_Bridge_01_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Shipwreck_01_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models/Smooth_UV_Sphere_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/archRock_uv.ply");
    vecModelsToLoad.push_back("assets/models9/skullRock_uv.ply");
    vecModelsToLoad.push_back("assets/models9/Jellyfish.ply");
    vecModelsToLoad.push_back("assets/models9/octopus.ply");
    vecModelsToLoad.push_back("assets/models9/fishcluster.ply");
    vecModelsToLoad.push_back("assets/models9/kiwi.ply");
    vecModelsToLoad.push_back("assets/models9/Grass.ply");
    vecModelsToLoad.push_back("assets/models9/oceanwaves_ONLY_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Grass_01_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Grass_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Grass_03.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Plants_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Plants_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Plants_03.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Flowers_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Flowers_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Seaweed_01.ply");
    vecModelsToLoad.push_back("assets/models9/Young Kiwi Vines_ONLY.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Rock_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Rock_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Env_Rock_03.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Item_Bottle_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Item_Bottle_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Item_Bottle_03.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Cannon_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Debris_02.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Crate_Cannonballs.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Fishing_Crate_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Barrel_Half_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Item_Skull_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Veh_Extra_Barrels_Group_01.ply");
    vecModelsToLoad.push_back("assets/models9/SM_Prop_Anchor_01.ply");
    vecModelsToLoad.push_back("assets/models9/TropicalFish01.ply");
    vecModelsToLoad.push_back("assets/models9/TropicalFish02.ply");
    vecModelsToLoad.push_back("assets/models9/TropicalFish03.ply");


    for ( std::string modelName : vecModelsToLoad )
    {
        sModelDrawInfo modelILoadedInfo;
        if ( pModelManger->LoadModelIntoVAO(modelName, modelILoadedInfo, shaderProgram) )
        {
            if (bLogProgressToConsole)
            {
                std::cout
                    << "Loaded <" << modelName << "> OK: "
                    << modelILoadedInfo.numberOfTriangles << " triangles, "
                    << modelILoadedInfo.numberOfVertices << " vertices." << std::endl;
            }
        }
        else
        {
            // Error
            bAnyErrors = true;

            vecErrors.push_back("Error: Didn't load >" + modelName + "<");

            if (bLogProgressToConsole)
            {
                std::cout << "Error: Didn't load >" << modelName << "<" << std::endl;
            }
        }//if ( pModelManger->LoadModelIntoVAO(...

    }//for ...vecModelsToLoad





    // Since we are using pointers, we can handle the debug objects differently.
    // Now we can re-use this same object and draw the sphere over and over, changing it slightly.
    cMeshObject* pDebugSphere = new cMeshObject();
//    pDebugSphere->meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    pDebugSphere->meshName = "assets/models/Smooth_UV_Sphere_xyz_n_uv.ply";
    pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    pDebugSphere->isWireframe = true;
    pDebugSphere->friendlyName = "DebugSphere";
    pDebugSphere->bDontLight = true;
    pDebugSphere->bIsVisible = false;       // **** NOTE THAT IT'S INVISIBLE ****
   // vec_pMeshesToDraw.push_back(pDebugSphere);


    // SPace interiors
    ::g_pSkySphere = new cMeshObject();
    ::g_pSkySphere->meshName = "assets/models/Isoshphere_smooth_inverted_normals_xyz_n_uv.ply";
    ::g_pSkySphere->friendlyName = "Sky Sphere";
    ::g_pSkySphere->textureName[0] = "24taylor-notebook3-superJumbo.bmp";
    ::g_pSkySphere->textureMixingRatio[0] = 1.0f;
    ::g_pSkySphere->scale = 10.0f;
  //  ::g_pSkySphere->bIsVisible = false;
//    pSkySphere->bIsVisible = false;
//    vec_pMeshesToDraw.push_back(pSkySphere);


    //Index position 1 seems to be messed up
    cMeshObject* ship = new cMeshObject();
    ship->meshName = "assets/models9/SM_Prop_Shipwreck_01_xyz_n_uv.ply";
    ship->diffuseColour = glm::vec3(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f); // Brown color
    ship->orientation = glm::vec3(26.0001f, 299.109f, 32.6001f);
    ship->friendlyName = "Shipwereck";
    ship->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    ship->specularPower_or_shininess = 80.0f;
    ship->position = glm::vec3(05.31003f, -1.68f, -1.89f);
    ship->textureName[2] = "Texture_01_A.bmp";         
    ship->textureMixingRatio[2] = 1.0f;
    //ship->alphaTransparency = 0.4f;
    ship->scale = 0.24f;
    vec_pMeshesToDraw.push_back(ship);


  

    cMeshObject* barrel = new cMeshObject();
    barrel->meshName = "assets/models9/Barrel_uv.ply";
    barrel->diffuseColour = glm::vec3(1.0f, 0.0f, 0.5f);
    barrel->friendlyName = "barrel";
    barrel->orientation = glm::vec3(57.5396f, -1.59f, 10.2f);
    barrel->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    barrel->specularPower_or_shininess = 50.0f;
    barrel->position = glm::vec3(1.67021f, -2.21996f, 6.35065f);
    barrel->scale = 2.05986f;
    barrel->textureName[2] = "Texture_01_B.bmp";
    barrel->textureMixingRatio[2]=1.0f;
    vec_pMeshesToDraw.push_back(barrel);

    cMeshObject* fish = new cMeshObject();
    fish->meshName = "assets/models9/Fish_uv.ply";
    fish->diffuseColour = glm::vec3(0.0f, 2.0f, 0.5f);
    fish->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    fish->specularPower_or_shininess = 100.0f;
    fish->position = glm::vec3(-2.35986f, -0.430002f, 4.6905f);
    fish->orientation = glm::vec3(91.2992f, 7.2f, 14.7f);
    fish->scale = 1.38998f;
    fish->textureName[1] = "TropicalFish01.bmp";
    fish->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(fish);

    cMeshObject* individualFish1 =  new cMeshObject();
    individualFish1->meshName = "assets/models9/Fish_uv.ply";
    individualFish1->diffuseColour = glm::vec3(0.7f, 0.5f, 1.0f);
    individualFish1->position = glm::vec3(0.030041f, -0.730005f, -1.66962f);
    individualFish1->orientation = glm::vec3(27.9001f, 2.0f, 10.8f);
    individualFish1->scale = 2.0f;
    individualFish1->isWireframe = false;
    individualFish1->textureName[1] = "fish.bmp";
    individualFish1->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(individualFish1);

    cMeshObject* individualFish2 = new cMeshObject();
    individualFish2->meshName = "assets/models9/Fish_uv.ply";
    individualFish2->diffuseColour = glm::vec3(0.5f, 1.0f, 0.5f);
    individualFish2->position = glm::vec3(-9.01994f, 0.519997f, -12.1798f);
    individualFish2->orientation = glm::vec3(92.7791f, 11.7f, 9.7f);
    individualFish2->scale = 1.5f;
    individualFish2->isWireframe = false;
    individualFish2->textureName[1] = "fish2.bmp";
    individualFish2->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(individualFish2);

    cMeshObject* individualFish3 = new cMeshObject();
    individualFish3->meshName = "assets/models9/Fish_uv.ply";
    individualFish3->diffuseColour = glm::vec3(1.0f, 0.7f, 0.7f);
    individualFish3->position = glm::vec3(-20.55f, -0.250003f, -10.5197f);
    individualFish3->orientation = glm::vec3(5.20996f, -0.109998f, 9.9f);
    individualFish3->scale = 3.0f;
    individualFish3->isWireframe = false;
    individualFish3->textureName[1] = "fish3.bmp";
    individualFish3->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(individualFish3);

    cMeshObject* shark = new cMeshObject();
    shark->meshName = "assets/models9/Shark_uv.ply";
    shark->diffuseColour = glm::vec3(1.0f, 0.0f, 0.5f);
    shark->friendlyName = "shark";
    shark->orientation = glm::vec3(298.91f, 7.0f, 157.801f);
    shark->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    shark->specularPower_or_shininess = 9.0f;
    shark->position = glm::vec3(-4.27982f, 1.47999f, -1.3f);
    shark->scale = 0.799933f;
    shark->alphaTransparency = 1.0f;
    shark->textureName[1] = "shark.bmp";
    shark->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(shark);

    cMeshObject* dock = new cMeshObject();
    dock->meshName = "assets/models9/SM_Bld_Dock_Bridge_01_xyz_n_uv.ply";
    dock->diffuseColour = glm::vec3(0.4f, 1.0f, 0.5f);
    dock->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    dock->orientation = glm::vec3(35.4f, 0.0f, 20.1f);
    dock->specularPower_or_shininess = 100.0f;
    dock->position = glm::vec3(1.49f, -1.34988f, -2.41f);
    dock->scale = 0.650002f;
    dock->textureName[3] = "Texture_04_C.bmp";
    dock->textureMixingRatio[3]=1.0f;
    vec_pMeshesToDraw.push_back(dock);

    cMeshObject* Whale = new cMeshObject();
    Whale->meshName = "assets/models9/Whale_uv.ply";
    Whale->diffuseColour = glm::vec3(0.4f, 1.0f, 0.5f);
    Whale->position = glm::vec3(-9.4398f, 1.28f, 2.26008f);
    Whale->orientation = glm::vec3(287.968f, 24.27f, 8.3f);
    Whale->scale = 15.8102f;
    Whale->textureName[3] = "BlueWhale.bmp";
    Whale->textureMixingRatio[3] = 1.0f;
    vec_pMeshesToDraw.push_back(Whale);

    cMeshObject* seaFloor = new cMeshObject();
    seaFloor->meshName = "assets/models9/Seafloor2_ONLY_xyz_n_uv.ply";
    seaFloor->diffuseColour = glm::vec3(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f);
    seaFloor->position = glm::vec3(-2.59f, -2.56f, 0.31f);
    //seaFloor->orientation = glm::vec3(98.5999f, 0.0f, 0.0f);
    seaFloor->scale = 0.09f;
    seaFloor->textureName[1] = "sandTexture.bmp";
    seaFloor->textureMixingRatio[1] = 1.0f;
    seaFloor->specularPower_or_shininess = 56.0f;
    vec_pMeshesToDraw.push_back(seaFloor);

    cMeshObject* archRock = new cMeshObject();
    archRock->meshName = "assets/models9/archRock_uv.ply";
    archRock->diffuseColour = glm::vec3(90.0f / 255.0f, 77.0f / 255.0f, 65.0f / 255.0f);
    archRock->position = glm::vec3(5.24017f, -0.139852f, -1.61924f);
    archRock->orientation = glm::vec3(181.602f, 263.707f, 344.612f);
    archRock->scale = 0.320001f;
    archRock->specularPower_or_shininess = 100.0f;
    archRock->textureName[1] = "Texture_01_A.bmp";
    archRock->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(archRock);

    cMeshObject* SkullRock = new cMeshObject();
    SkullRock->meshName = "assets/models9/skullRock_uv.ply";
    SkullRock->diffuseColour = glm::vec3(101.0f / 255.0f, 83.0f / 255.0f, 83.0f / 255.0f);
    SkullRock->position = glm::vec3(13.6103f, -1.03978f, 1.62109f);
    SkullRock->orientation = glm::vec3(429.909f, 269.528f, 48.1412f);
    SkullRock->scale = 0.179854f;
    SkullRock->specularPower_or_shininess = 100.00f;
    SkullRock->textureName[1] = "skull.bmp";
    SkullRock->textureMixingRatio[1] = 0.89f;
    vec_pMeshesToDraw.push_back(SkullRock);

    cMeshObject* octopus = new cMeshObject();
    octopus->meshName = "assets/models9/octopus.ply";
    octopus->diffuseColour = glm::vec3(0.6f, 0.4f, 0.8f);
    octopus->position = glm::vec3(2.26027f, -1.47001f, 12.211f);
    octopus->orientation = glm::vec3(28.2001f, 16.7f, 12.6f);
    octopus->scale = 0.290004f;
    octopus->isWireframe = false;
    octopus->textureName[1] = "octopus.bmp";
    octopus->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(octopus);

    cMeshObject* jellyFish = new cMeshObject();
    jellyFish->meshName = "assets/models9/Jellyfish.ply";
    jellyFish->diffuseColour = glm::vec3(1.0f, 0.87f, 0.12f); // Brown color
    jellyFish->position = glm::vec3(10.42f, -1.46f, 1.01f);
    jellyFish->orientation = glm::vec3(251.406f, 0.0f, 0.9f);
    jellyFish->scale = 0.5f;
    jellyFish->isWireframe = false;
    jellyFish->textureName[1] = "jellyFish.bmp";
    jellyFish->textureMixingRatio[1] = 1.0f;
    jellyFish->alphaTransparency = 0.35f;
    vec_pMeshesToDraw.push_back(jellyFish);

    cMeshObject* jellyFish1 = new cMeshObject();
    jellyFish1->meshName = "assets/models9/Jellyfish.ply";
    jellyFish1->diffuseColour = glm::vec3(149.0f / 255.0f, 202.0f / 255.0f, 208.0f / 255.0f); // Brown color
    jellyFish1->position = glm::vec3(7.02001f, -1.6f, 12.2703f);
    jellyFish1->orientation = glm::vec3(280.008f, 2.4f, 12.4f);
    jellyFish1->scale = 0.5f;
    jellyFish1->isWireframe = false;
    jellyFish1->textureName[1] = "jellyFish.bmp";
    jellyFish1->alphaTransparency = 0.33f;
    jellyFish1->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(jellyFish1);

    cMeshObject* jellyFish2 = new cMeshObject();
    jellyFish2->meshName = "assets/models9/Jellyfish.ply";
    jellyFish2->diffuseColour = glm::vec3(0.70f, 1.0f, 0.12f); // Brown color
    jellyFish2->position = glm::vec3(-4.98999f, 0.52f, 1.53f);
    jellyFish2->orientation = glm::vec3(267.507f, 0.0f, 0.0f);
    jellyFish2->scale = 0.5f;
    jellyFish2->isWireframe = false;
    jellyFish2->textureName[1] = "jellyFish.bmp";
    jellyFish2->alphaTransparency = 0.33f;

    jellyFish2->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(jellyFish2);

    cMeshObject* oceanWave = new cMeshObject();
    oceanWave->meshName = "assets/models9/oceanwaves_ONLY_xyz_n_uv.ply";
    oceanWave->diffuseColour = glm::vec3(0.0f, 0.0f, 2.0f);
    oceanWave->position = glm::vec3(0.42f, 7.00013f, 1.65f);
    oceanWave->orientation = glm::vec3(108.588f, 272.159f, 105.099f);
    oceanWave->scale = 11.9002f;
    oceanWave->friendlyName = "oceanWave";
    oceanWave->textureName[1] = "oceanWave.bmp";
    oceanWave->textureMixingRatio[1] = 1.0f;
    oceanWave->textureName[2] = "waterTexture.bmp";
    oceanWave->textureMixingRatio[2] = 1.0f;
    oceanWave->alphaTransparency = 0.45f;
    vec_pMeshesToDraw.push_back(oceanWave);

    cMeshObject* jellyFish3 = new cMeshObject();
    jellyFish3->meshName = "assets/models9/Jellyfish.ply";
    jellyFish3->diffuseColour = glm::vec3(0.70f, 1.0f, 0.12f); // Brown color
    jellyFish3->position = glm::vec3(-4.98999f, 4.01f, -0.58999f);
    jellyFish3->orientation = glm::vec3(393.564f, 4.19999f, 9.78999f);
    jellyFish3->scale = 0.65f;
    jellyFish3->isWireframe = false;
    jellyFish3->textureName[1] = "jellyFish.bmp";
    jellyFish3->alphaTransparency = 0.8f;
    jellyFish3->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(jellyFish3);

    cMeshObject* fishCluster = new cMeshObject();
    fishCluster->meshName = "assets/models9/fishcluster.ply";
    fishCluster->diffuseColour = glm::vec3(1.0f, 0.0f, 0.0f); // Brown color
    fishCluster->position = glm::vec3(-13.27f, 3.51f, 4.25f);
    fishCluster->orientation = glm::vec3(51.3399f, 60.9694f, 51.8998f);
    fishCluster->scale = 3.21986f;
    fishCluster->textureName[1] = "fish.bmp";
    fishCluster->textureMixingRatio[1] = 1.0f;
    fishCluster->isWireframe = false;
    vec_pMeshesToDraw.push_back(fishCluster);

    cMeshObject* grass = new cMeshObject();
    grass->meshName = "assets/models9/Grass.ply";
    grass->diffuseColour = glm::vec3(0.4f, 0.6f, 0.4f); // Brown color
    grass->position = glm::vec3(6.29999f, -1.41f, 7.46008f);
    grass->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
    grass->scale = 0.640002f;
    grass->specularPower_or_shininess = 100.0f;
    grass->textureName[1] = "YoungKiwiVines.bmp";
    grass->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(grass);

    cMeshObject* kiwi = new cMeshObject();
    kiwi->meshName = "assets/models9/kiwi.ply";
    kiwi->diffuseColour = glm::vec3(0.4f, 0.8f, 0.7f); // Brown color
    kiwi->position = glm::vec3(2.74003f, -2.5f, 15.3504f);
    kiwi->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
    kiwi->scale = 3.21986f;
    kiwi->textureName[1] = "YoungKiwiVines.bmp";
    kiwi->textureMixingRatio[1] = 1.0f;
    kiwi->specularPower_or_shininess = 100.0f;
    vec_pMeshesToDraw.push_back(kiwi);

    cMeshObject* fishCluster1 = new cMeshObject();
    fishCluster1->meshName = "assets/models9/fishcluster.ply";
    fishCluster1->diffuseColour = glm::vec3(1.0f, 0.5f, 1.0f); // Brown color
    fishCluster1->position = glm::vec3(4.75022f, -0.589999f, 12.7802f);
    fishCluster1->orientation = glm::vec3(155.74f, 60.9694f, 51.8998f);
    fishCluster1->scale = 3.21986f;
    fishCluster1->textureName[1] = "TropicalFish02.bmp";
    fishCluster1->textureMixingRatio[1] = 1.0f;
    vec_pMeshesToDraw.push_back(fishCluster1);

    cMeshObject* fishCluster2 = new cMeshObject();
    fishCluster2->meshName = "assets/models9/fishcluster.ply";
    fishCluster2->diffuseColour = glm::vec3(1.0f, 1.0f, 0.0f); // Brown color
    fishCluster2->position = glm::vec3(5.76033f, -0.23f, 2.66002f);
    fishCluster2->orientation = glm::vec3(84.3394f, 74.7692f, 77.6994f);
    fishCluster2->scale = 3.21986f;
    fishCluster2->textureName[1] = "TropicalFish01x.bmp";
    fishCluster2->textureMixingRatio[1] = 1.0f;
    fishCluster2->specularPower_or_shininess = 90.0f;
    vec_pMeshesToDraw.push_back(fishCluster2);


    AddObjects("assets/models9/SM_Env_Grass_01_xyz_n_uv.ply", glm::vec3(0.3f, 0.7f, 0.5f), glm::vec3(-4.48961f, -2.47995f, -1.95962f), glm::vec3(337.912f, 263.707f, 344.612f), 3.0f, "underWater.bmp",1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Flowers_02.ply", glm::vec3(0.2f, 0.7f, 0.4f), glm::vec3(-7.9f, -2.96f, 5.63003f), glm::vec3(341.412f, 263.707f, 344.612f), 2.53f, "underWater.bmp",1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Flowers_02.ply", glm::vec3(0.6f, 0.7f, 0.5f), glm::vec3(7.81036f, -2.91989f, -2.44994f), glm::vec3(388.715f, 311.41f, 359.313f), 1.74998f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/Young Kiwi Vines_ONLY.ply", glm::vec3(0.3f, 0.6f, 0.5f), glm::vec3(-5.10997f, -2.46997f, 10.3503f), glm::vec3(199.903f, 356.513f, 534.517f), 3.0f, "plant.bmp",1.0f,1.0f);
    AddObjects("assets/models9/SM_Env_Grass_01_xyz_n_uv.ply", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(2.62003f, -2.29997f, 8.03009f), glm::vec3(337.912f, 263.707f, 344.612f), 1.21f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/kiwi.ply", glm::vec3(0.4f, 0.8f, 0.7f), glm::vec3(2.74003f, -2.5f, 15.3504f), glm::vec3(358.213f, 264.607f, 350.512f), 3.0f, "YoungKiwiVines.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/kiwi.ply", glm::vec3(0.5f, 0.5f, 0.3f), glm::vec3(-10.73f, -2.55f, -5.20991f), glm::vec3(15.1f, 0.0f, 0.0f), 3.0f, "YoungKiwiVines.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/kiwi.ply", glm::vec3(0.0f, 1.0f, 0.6f), glm::vec3(5.73007f, -2.18f, -1.66973f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, "YoungKiwiVines.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Seaweed_01.ply", glm::vec3(0.4f, 0.5f, 0.6f), glm::vec3(10.0f, -3.04f, 13.7302f), glm::vec3(306.41f, 263.707f, 344.612f), 1.05f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Plants_02.ply", glm::vec3(0.5f, 0.4f, 0.7f), glm::vec3(-1.71999f, -1.38f, 7.45008f), glm::vec3(353.512f, 268.207f, 344.612f), 0.94f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Plants_02.ply", glm::vec3(0.6f, 0.3f, 0.8f), glm::vec3(5.58003f, -2.2f, 13.7103f), glm::vec3(358.213f, 264.607f, 350.512f), 1.0f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Grass_02.ply", glm::vec3(0.7f, 0.2f, 0.9f), glm::vec3(6.29999f, -1.97f, -4.4f), glm::vec3(337.912f, 263.707f, 344.612f), 1.0f, "underWater.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Grass_03.ply", glm::vec3(0.4f, 0.6f, 0.4f), glm::vec3(6.29999f, -1.41f, 7.46008f), glm::vec3(337.912f, 263.707f, 344.612f), 0.640002f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Flowers_01.ply", glm::vec3(0.4f, 0.6f, 0.2f), glm::vec3(-5.02996f, -1.03f, 4.63027f), glm::vec3(207.703f, 356.513f, 558.711f), 1.45f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Plants_03.ply", glm::vec3(0.7f, 0.3f, 0.1f), glm::vec3(-7.21996f, -3.14989f, -11.6897f), glm::vec3(367.114f, 275.208f, 350.512f), 1.02f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Plants_01.ply", glm::vec3(0.8f, 0.2f, 0.3f), glm::vec3(2.74003f, -2.5f, 15.3504f), glm::vec3(358.213f, 264.607f, 350.512f), 2.24f, "underWater.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Plants_01.ply", glm::vec3(0.5f, 0.7f, 0.6f), glm::vec3(10.7602f, -2.36999f, 5.10031f), glm::vec3(358.213f, 264.607f, 350.512f), 2.42f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Grass_01.ply", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(2.62003f, -2.29997f, 8.03009f), glm::vec3(337.912f, 263.707f, 344.612f), 1.21f, "underWater.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Grass_02.ply", glm::vec3(0.8f, 0.4f, 0.4f), glm::vec3(-1.09987f, -2.27f, -4.4f), glm::vec3(337.912f, 263.707f, 344.612f), 1.13f, "Texture_01_A.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Seaweed_01.ply", glm::vec3(0.2f, 1.0f, 0.3f), glm::vec3(7.89009f, -4.03999f, 4.4102f), glm::vec3(326.611f, 274.208f, 344.612f), 1.05f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Seaweed_01.ply", glm::vec3(0.01f, 1.0f, 0.2f), glm::vec3(-13.24f, -3.56f, -1.58981f), glm::vec3(319.411f, 263.707f, 344.612f), 1.05f, "underWater.bmp", 1.0f, 1.0f);


    //rocks
    AddObjects("assets/models9/SM_Env_Rock_01.ply", glm::vec3(0.571118f, 0.387037f, 0.7757f), glm::vec3(11.7702f, -2.24f, -3.35974f), glm::vec3(13.2f, 17.7f, 7.3f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_01.ply", glm::vec3(0.664336f, 0.369105f, 0.806405f), glm::vec3(7.13025f, -1.88f, 1.56026f), glm::vec3(98.2991f, 17.7f, 7.3f), 0.349866f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_01.ply", glm::vec3(0.521169f, 0.390363f, 0.760138f), glm::vec3(-9.9698f, -2.0f, -14.9598f), glm::vec3(43.2999f, 17.7f, 7.3f), 0.329981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_01.ply", glm::vec3(0.565032f, 0.38301f, 0.781775f), glm::vec3(-1.44979f, -1.88f, 11.2304f), glm::vec3(156.9f, 30.3f, 7.3f), 0.339981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_01.ply", glm::vec3(0.607144f, 0.383625f, 0.795115f), glm::vec3(3.62022f, -2.57f, 6.91042f), glm::vec3(32.8001f, 17.7f, 27.1001f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_02.ply", glm::vec3(0.587764f, 0.38504f, 0.78531f), glm::vec3(10.9104f, -2.57f, 6.91042f), glm::vec3(32.8001f, 17.7f, 27.1001f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_02.ply", glm::vec3(0.609518f, 0.361682f, 0.811567f), glm::vec3(-0.279777f, -2.33f, -5.18944f), glm::vec3(34.5001f, 17.7f, 27.1001f), 0.469981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_02.ply", glm::vec3(0.59428f, 0.381058f, 0.77482f), glm::vec3(-5.39964f, -1.89997f, -3.60955f), glm::vec3(62.2996f, 19.4f, 37.1f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_02.ply", glm::vec3(0.563849f, 0.402789f, 0.764032f), glm::vec3(-15.1497f, -2.06997f, 1.20045f), glm::vec3(54.0998f, 17.7f, 27.1001f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_02.ply", glm::vec3(0.560275f, 0.385716f, 0.779614f), glm::vec3(3.62022f, -2.57f, 6.91042f), glm::vec3(32.8001f, 17.7f, 27.1001f), 0.489981f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.570721f, 0.388423f, 0.783282f), glm::vec3(9.27989f, -2.19f, 12.8506f), glm::vec3(96.8991f, 35.2f, 33.8001f), 0.62f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.597719f, 0.375629f, 0.789038f), glm::vec3(-5.61009f, -2.39988f, -11.2495f), glm::vec3(45.0999f, 45.3998f, 91.7992f), 1.0f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.52735f, 0.402496f, 0.773791f), glm::vec3(-12.0399f, -2.04f, 4.25116f), glm::vec3(38.3f, 28.9f, 44.0999f), 1.08f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.574092f, 0.370303f, 0.801436f), glm::vec3(-15.6401f, -1.83991f, 3.02085f), glm::vec3(54.6998f, 17.7f, 97.3991), 1.0f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.584124f, 0.384613f, 0.777008f), glm::vec3(6.22982f, -2.00994f, -3.52943f), glm::vec3(61.5997f, 17.7f, 83.8993f), 0.230001f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.550317f, 0.402096f, 0.777625f), glm::vec3(5.84986f, -2.46f, -0.279564f), glm::vec3(32.8001f, 18.6f, 27.1001f), 0.65f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.586678f, 0.380343f, 0.797119f), glm::vec3(17.2502f, -1.82997f, -2.24927f), glm::vec3(90.5992f, 17.7f, 27.1001), 0.72f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.541597f, 0.398263f, 0.780641f), glm::vec3(-27.2301f, -1.60995f, -11.9995f), glm::vec3(142.6f, 17.7f, 27.1001f), 1.0f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.575092f, 0.389129f, 0.795875f), glm::vec3(-25.8702f, -1.84f, -7.4094f), glm::vec3(46.2999f, 17.7f, 27.1001), 1.0f, "rock.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Env_Rock_03.ply", glm::vec3(0.556518f, 0.391462f, 0.772838f), glm::vec3(11.4102f, -1.86982f, -7.50933f), glm::vec3(38.8f, 126.299f, 27.1001f), 1.56f, "rock.bmp", 1.0f, 1.0f);



    AddObjects("assets/models9/SM_Prop_Crate_Cannonballs.ply", glm::vec3(1.0f, 0.8f, 0.4f), glm::vec3(4.16001f, -1.39f, 3.16004f), glm::vec3(98.7479f, 0.0f, 0.0f), 1.68987f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Veh_Extra_Barrels_Group_01.ply", glm::vec3(0.2f, 0.8f, 0.6f), glm::vec3(-3.8f, -1.8f, 4.80001f), glm::vec3(124.399f, 0.4f, 0.4f), 0.84f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Prop_Anchor_01.ply", glm::vec3(0.3f, 0.3f, 0.2f), glm::vec3(5.88012f, -2.21996f, 13.0606f), glm::vec3(311.024f, 145.621f, 46.1999f), 0.849866f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Prop_Barrel_Half_01.ply", glm::vec3(1.0f, 0.9f, 0.4f), glm::vec3(8.38021f, -2.21996f, 7.95065f), glm::vec3(51.7397f, -1.59f, 10.2f), 2.05986f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Item_Skull_01.ply", glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(8.38021f, -2.21996f, 7.95065f), glm::vec3(51.7397f, -1.59f, 10.2f), 2.01986f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Prop_Debris_02.ply", glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(5.48059f, -1.70996f, 6.18091f), glm::vec3(184.602f, 3.7f, 0.0f), 1.25f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Prop_Cannon_01.ply", glm::vec3(0.4f, 0.2f, 0.1f), glm::vec3(0.830212f, -1.99996f, 7.95065f), glm::vec3(47.4998f, 0.0f, 0.0f), 1.52998f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Prop_Fishing_Crate_01.ply", glm::vec3(0.4f, 0.3f, 0.1f), glm::vec3(-18.7898f, -2.04992f, -12.1092f), glm::vec3(31.3001f, 0.0f, 4.1f), 4.60999f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Item_Bottle_01.ply", glm::vec3(0.5f, 0.0f, 0.1f), glm::vec3(5.23023f, -2.00996f, 6.73071f), glm::vec3(31.4001f, 8.09999f, 116.799f), 3.83986f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Item_Bottle_03.ply", glm::vec3(0.5f, 0.0f, 0.1f), glm::vec3(0.820228f, -2.56991f, 0.650656f), glm::vec3(28.9001f, 12.1f, 52.6998f), 6.76993f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Item_Bottle_02.ply", glm::vec3(0.5f, 0.0f, 0.1f), glm::vec3(1.14022f, -2.21996f, 1.61067f), glm::vec3(68.5995f, 0.0f, 0.0f), 4.44987f, "Texture_01_B.bmp", 1.0f, 1.0f);
    AddObjects("assets/models9/SM_Item_Bottle_02.ply", glm::vec3(0.5f, 0.0f, 0.1f), glm::vec3(10.5903f, -2.21996f, 9.54069f), glm::vec3(56.2396f, -1.59f, 10.2f), 2.34986f, "Texture_01_B.bmp", 1.0f, 1.0f);

    return ! bAnyErrors;
}


void AddObjects(const std::string& meshName, const glm::vec3& colour, const glm::vec3& position, const glm::vec3& orientation, float scale, const std::string& textureName, float textureMixingRatio, float alphaTransparency) {
    cMeshObject* object = new cMeshObject();
    object->meshName = meshName;
    object->diffuseColour = colour;
    object->position = position;
    object->orientation = orientation;
    object->scale = scale;
    object->textureName[1] = textureName;
    object->textureMixingRatio[1] = textureMixingRatio;
    object->alphaTransparency = alphaTransparency;

    g_vec_pMeshesToDraw.push_back(object);
}





