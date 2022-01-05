#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <irrKlang.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Primitives.h"
//#include "ShaderManager.h"
#include "Inputs.h"
#include "Maths.h"
#include "Game.h"
#include "Texture.h"
#include "Text.h"
#include "RessourcesLoader.h"
//#include  "GameObject.h"
#include "Globals.h"
#include "Camera.h"
#include "Utilities.h"
#include "TileMap.h"

//Variable
double fps = 60;
int frameCount = 0;
double currentTime;
double lastTime;
double deltaTimeSum = 0;

Camera camera = Camera(-(float)SCREEN_WIDTH / 2.0f, (float)SCREEN_WIDTH / 2.0f, -(float)SCREEN_HEIGHT / 2.0f, (float)SCREEN_HEIGHT / 2.0f);

using namespace irrklang;

int main()
{ 
	
    //Init GLFW context 
	GLFWwindow* window = InitContext(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == nullptr) return -1;

	//Create ImGui context
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//init irrKlang
	ISoundEngine* SoundEngine = createIrrKlangDevice();


	//Load ressources
	LoadImages();
	LoadTextures();
	LoadShaders();
	LoadGameObjects();


	Font font = Font("Ressources/fonts/rockstar.otf");


	//Shader* textShader			= new Shader();
	//Text* text	= new Text();
	//int	init	= text->initfreetype("fonts/rockstar.otf");


	/*Collider collider_apple = Collider(lesbeanApple);
	Collider collider_apple2 = Collider(lesbeanApple2);*/
	Collider collider_water = Collider(grabPass);
	


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	ImColor bgColor = ImColor(82, 75, 108);

	irrklang::ISound* sd = SoundEngine->play2D("Ressources/Sounds/Mystery.mp3", true, false, true);
	if (sd) sd->setVolume(0.3);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //NANI!?
	TileMap tmt = TileMap("T1",Vector2<int>(32,32));
	tmt.tileObjects[0] = groundTile0;
	tmt.tileObjects[1] = groundTile1;
	tmt.tileObjects[2] = groundTile2;
	tmt.tileObjects[3] = wallTile0;

	//DEBUG ZONE----------------------
	int arr[4] = { 2,5,1,3 };
	iMat2 mat = iMat2(arr);
	//END ZONE
	//GameObjectClone instancingApple = GameObjectClone(lesbeanApple);
	std::map<int, std::pair<Drawable*, uint8_t>> drawOrder;

	GameObjectClone background0  = GameObjectClone(background);
	GameObjectClone background11 = GameObjectClone(background1);
	GameObjectClone background22 = GameObjectClone(background2) ;
	GameObjectClone background33 = GameObjectClone(background3) ;
	GameObjectClone background44 = GameObjectClone(background4) ;
	GameObjectClone bush11       = GameObjectClone(bush1) ;
	GameObjectClone bush22       = GameObjectClone(bush2) ;
	GameObjectClone tree11       = GameObjectClone(tree1) ;
	GameObjectClone tree22       = GameObjectClone(tree2) ;

	drawOrder[0] = std::make_pair<Drawable*, uint8_t>(&background0, 0) ;
	drawOrder[1] = std::make_pair<Drawable*, uint8_t>(&background11, 0) ;
	drawOrder[2] = std::make_pair<Drawable*, uint8_t>(&background22, 0) ;
	drawOrder[3] = std::make_pair<Drawable*, uint8_t>(&background33, 0) ;
	drawOrder[4] = std::make_pair<Drawable*, uint8_t>(&background44, 0)  ;
	drawOrder[5] = std::make_pair<Drawable*, uint8_t>(&bush11,0) ;
	drawOrder[6] = std::make_pair<Drawable*, uint8_t>(&bush22,0) ;
	drawOrder[7] = std::make_pair<Drawable*, uint8_t>(&tree11,0) ;
	drawOrder[8] = std::make_pair<Drawable*, uint8_t>(&tree22,0) ;

	
	double currentSprite = 0.0;
	bool isActive = true;
	const ImVec2 guiImageSize = ImVec2(50.0F, 50.0F);
	while (!glfwWindowShouldClose(window)){
		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
		ImGui::Text("fps = %f", fps);
		ImGui::ColorEdit3("Background Color", (float*)&bgColor);
		ImGui::SliderInt2("WarriorPos", &warrior->position.x, -400, 400);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Tilemap Editor", "Ctrl+W")) { TileMap::GuiActive = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

		}

		TileMap::Gui();

		isMouseOnGui = io.WantCaptureMouse;

		ImGui::End();
		//Debug--------------
		//std::cout << lesbeanApple->size.x << "  " << collider_apple2.size->x << std::endl;
		//if (isColliding(&collider_apple, &collider_apple2)) std::cout << "Hello Collision" << std::endl;
		
		//------------------
		uTime += deltaTime;
		glClearColor(bgColor.Value.x, bgColor.Value.y, bgColor.Value.z, 1.0); // 0.6f, .8f, .8f, 1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		//BindTextures
		tex->Bind(0);
		grabTex->Bind(1);
		//TODO::Shader managment when it comes to draw shapes
		// 
		//Update dynamic object position
		lesbeanApple->position.x += (-input_left + input_right)*300.0f*deltaTime;
		lesbeanApple->position.y += (-input_down + input_up)*300.0f*deltaTime;
		//while (IsColliding(&collider_apple, &collider_apple2)) {
		//	lesbeanApple->position.x -= (-input_left + input_right) * 300.0f * deltaTime;
		//	lesbeanApple->position.y -= (-input_down + input_up) * 300.0f * deltaTime;
		//}
		camera.position = lesbeanApple->position;

		camera.Update();
		tmt.Update();

		currentSprite += deltaTime *5.0;
		//Drawing shapes

		for (auto& it : drawOrder) {
			(it.second.first)->Draw(it.second.second);
		}

		for (auto it = tmt.tiles.begin(); it != tmt.tiles.end(); ++it) {
			(*it).Draw(0);
		}

		//ground->Draw(0);
		warriorTex->UpdateTexture(warriorTex->size.x, warriorTex->size.y, IMAGES_WARRIOR_IDLE_ARRAY[((int)currentSprite) % 6]->tex, 0, 1);
		warrior->Draw(0);
		//lesbeanApple->Draw(0);
		//instancingApple.Draw(0);
		//lesbeanApple2->Draw(0);
		glReadPixels(-camera.position.x, -camera.position.y, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		grabTex->UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		grabPass->Draw(1);
		postProcessing->position = camera.position;
		glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, behindPixels);
		grabTex->UpdateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels, 1);
		//lightSurface->Draw(1);
		//postProcessing->Draw(1);

		//Drawing debug things and tilemap grid
		//tmt.RenderGrid();
		font.DisplayText("Hello World", Vector2<int>(100, 100), shader_text, Vector3<float>(1.0f, 0.0f, 0.0f), 0);

		//Render Im::Gui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Update screen
		glfwSwapBuffers(window);
		glfwPollEvents();
		//Calculate fps
		if (DEBUG_MODE) {
			frameCount += 1;
			deltaTimeSum += deltaTime;
			if (frameCount == 60) {
				fps = 60.0 / deltaTimeSum;
				deltaTimeSum = 0;
				frameCount = 0;
			}
		}
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime; //Well it's negligeable operation
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
};