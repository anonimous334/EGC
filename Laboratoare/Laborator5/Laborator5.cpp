#include "Laborator5.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator5::Laborator5()
{
	left = -10.0;
	right = 10.0;
	bottom = -8.0;
	top = 8.0;
	zNear = 0.0;
	zFar = 50.0;

	fov = 45.0f;
	aspect = (GLfloat)1280 / (GLfloat)720;

	glm::mat4 CarMatrix = glm::mat4(1);
	CarMatrix[3][1] = 4.0;
}

Laborator5::~Laborator5()
{
}

void Laborator5::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			Color_Plat[i][k] = rand() % 4;
			if (Color_Plat[i][k] == 1) {
				Color_Plat[i][k] = rand() % 4;
			}
		}
	}
	Color_Plat[1][0] = 0;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator5::Update(float deltaTimeSeconds)
{
	static int in_fcn = 0;
	if (CarMatrix[3][2] < (huita - dim - (2* padding) - (dim / 2.0f))) {
		huita = huita - dim - (2 * padding) - dim;
		padding = ((float )(rand() % 100) / 100.0) + 1.0;
	
		Color_Plat[0][0] = Color_Plat[0][2];
		Color_Plat[1][0] = Color_Plat[1][2];
		Color_Plat[2][0] = Color_Plat[2][2];

		//toate noi inafare de cele din stanga
		for (int i = 0; i < 3; i++) {
			for (int k = 1; k < 3; k++) {
				Color_Plat[i][k] = rand() % 4;
				if (Color_Plat[i][k] == 1) {
					Color_Plat[i][k] = rand() % 4;
				}
			}
		}
		in_fcn += 1;
		if (in_fcn == 6) {
			plus_speed = 1.0;
			in_fcn = 0;
			flag_speed = 0;
		}
	}

	glm::vec3 Color_proprie;
	glm::vec3 trebuie = glm::vec3(0,0,0);

	float val_x = 0;
	int parcurgere = 0;
	int flag_k = 0;
	for (int k = 0; k < 3; k++) {
		if (k == 0) {
			val_x = -2;
		} else if(k == 1) {
			val_x = 0;
		} else if (k == 2) {
			val_x = 2;
		}
		for (int i = 0; i < 3; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			if (i == 0) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(val_x, 0, huita));
			}
			else if (i == 1) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(val_x, 0, huita - dim - (padding)));
			}
			else if (i == 2) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(val_x, 0, huita - dim - (2 * padding) - dim));
			}
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 0.001f, 5.0f));
			
			
			if (Color_Plat[k][i] == 0) { //color verde
				Color_proprie = glm::vec3(0,1,0);
			} else if (Color_Plat[k][i] == 1) { //color ros
				Color_proprie = glm::vec3(1, 0, 0);
			} else if (Color_Plat[k][i] == 2) { //color galben
				Color_proprie = glm::vec3(1, 1, 0);
			} else if (Color_Plat[k][i] == 3) { //color portocaliu
				Color_proprie = glm::vec3(1, 0.5, 0);
			}
			if (Coliziune(CarMatrix, modelMatrix) == 1) {
				cade -= 1;
				if (Color_Plat[k][i] == 1) {
					exit(0);
				}
				if (Color_Plat[k][i] == 3) {
					plus_speed = norm_speed;
					flag_speed = 1;
				}
				trebuie = glm::vec3(0.5, 0.2, 0.8); //Purple
			} else {
				//printf("%d %d\n", k, i);
				trebuie = Color_proprie;
			}
			RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, trebuie);
		}
		parcurgere += 1;
	}

	if (cade == 0) { //a facut coliziune cu 1 o lasam
		cout << "E ok\n";
		cade = 1;
		//exit(0);
	} else {
		if (inAir == 0 && Not_1 > 0) {
			cout << "Cade iopta\n";
			cade_precis = 1;
			terrain = -5.0;
		}
	}
	Not_1 += 1;

	//Personaj
	if (cade_precis == 0) {
		UpWardsSpeed += gravity * deltaTimeSeconds;
		CarMatrix = glm::translate(CarMatrix, glm::vec3(0, (UpWardsSpeed * deltaTimeSeconds), 0));
		if (CarMatrix[3][1] < terrain) {
			UpWardsSpeed = 0;
			inAir = false;
			CarMatrix[3][1] = terrain;
		}
		if (flag_afis == 1) {
			RenderMesh(meshes["sphere"], shaders["ShaderLab6"], CarMatrix, glm::vec3(1, 0, 0));
		}
	}
	float static level = 0;
	if (cade_precis == 1) {
		level += terrain * deltaTimeSeconds * 0.1;
		CarMatrix = glm::translate(CarMatrix, glm::vec3(0, level, 0));
		camera->TranslateUpword(level);
		if (CarMatrix[3][1] < terrain) {
			UpWardsSpeed = 0;
			inAir = false;
			CarMatrix[3][1] = terrain;
		}
		if (flag_afis == 1) {
			RenderMesh(meshes["sphere"], shaders["ShaderLab6"], CarMatrix, glm::vec3(1, 0, 0));
		}
		if (CarMatrix[3][1] <= -5.0) {
			exit(0);
		}
	}
}

void Laborator5::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Laborator5::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	GLint obj_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(obj_color, 1, glm::value_ptr(color));
	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 5.0f;
		if (cade_precis == 0) {
			if (window->KeyHold(GLFW_KEY_W)) {
				camera->TranslateForward(cameraSpeed * deltaTime * plus_speed);
				CarMatrix = glm::translate(CarMatrix, glm::vec3(0, 0, -(cameraSpeed * deltaTime * plus_speed)));
			}
			if (window->KeyHold(GLFW_KEY_A)) {
				camera->TranslateRight(-(cameraSpeed * deltaTime));
				CarMatrix = glm::translate(CarMatrix, glm::vec3(-(cameraSpeed * deltaTime), 0, 0));
			}
			if (window->KeyHold(GLFW_KEY_S)) {
			}
			if (window->KeyHold(GLFW_KEY_D)) {
				camera->TranslateRight(cameraSpeed * deltaTime);
				CarMatrix = glm::translate(CarMatrix, glm::vec3((cameraSpeed * deltaTime), 0, 0));
			}
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(-(cameraSpeed * deltaTime));
		}
		if (window->KeyHold(GLFW_KEY_SPACE)) {
			if (!inAir) {
				UpWardsSpeed = jump_power;
				inAir = true;
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_1)) {
		flag_afis = 0;
	}
	if (window->KeyHold(GLFW_KEY_3)) {
		flag_afis = 1;
	}
}

void Laborator5::OnKeyPress(int key, int mods)
{
}

void Laborator5::OnKeyRelease(int key, int mods)
{
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			//camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			//camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			//camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			//dcamera->RotateThirdPerson_OY(-sensivityOY * deltaX);
			//camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
		}

	}
}

void Laborator5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Laborator5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Laborator5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator5::OnWindowResize(int width, int height)
{
}

int Laborator5::Coliziune(glm::mat4 Car, glm::mat4 Platforma) {
	return ( ((Platforma[3][0] - 0.5) <= Car[3][0]) && (Car[3][0] <= ((Platforma[3][0]) + 0.5)) &&
			(Car[3][1] == 0.4f) &&
			((Platforma[3][2] + 2.5) >= Car[3][2]) && ((Platforma[3][2] - 2.5) <= Car[3][2])
		);
}