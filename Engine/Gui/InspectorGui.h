#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "HierarchyGui.h"
#include "ScriptManager.h"
#include "Utilities.h"
class InspectorGui {
public:
	static bool isActive;
	static void Show() {

		if (!isActive) return;

		ImGui::Begin("Inspector", &isActive);

		GameObject* go = nullptr;
		Sprite* sprite = nullptr;
		Transform* transform = nullptr;
		Collider2* collider = nullptr;
		Script* script = nullptr;
		Camera* cam = nullptr;

		if (HierarchyGui::selected > -1 && HierarchyGui::selected < Scene::currentScene->sceneObjs.size()) {
			go = &Scene::currentScene->sceneObjs[HierarchyGui::selected];
			//TODO::Check performance of getting all components each frame
			sprite = go->GetComponent<Sprite>();
			transform = go->GetComponent<Transform>();
			collider = go->GetComponent<Collider2>();
			script = go->GetComponent<Script>();
			cam = go->GetComponent<Camera>();

			if (transform != nullptr) {
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragInt2("Position", &transform->position.x);
					ImGui::DragFloat2("Scale", &transform->scale.x, 0.01f);
					if (ImGui::Button("Delete##1")) go->DeleteComponent<Transform>();
				}
			}

			if (sprite != nullptr) {
				if (ImGui::CollapsingHeader("Sprite")) {
					ImGui::DragScalar("Layering Order", ImGuiDataType_S16, &sprite->sortingLayer);
					ImGui::NewLine();
					ImGui::NewLine();
					ImGui::Text("Texture");
					ImGui::SameLine();
					if (ImGui::Button(sprite->texture->name.c_str(), ImVec2(150,0))) {
						std::string path = GetFile("Image Files\0*.png;*.jpeg;*.jpg\0*.*\0");
						if (path != "") sprite->SetTexture(path);
					}

					ImGui::NewLine();
					ImGui::NewLine();

				    if (ImGui::Button(sprite->shader->name.c_str())) {
						std::string path = GetFile("Shader Files\0*.shader\0*.*\0");
						if (path != "") sprite->SetShader(path);
					}
					ImGui::NewLine();
					if (ImGui::Button("Delete##2")) go->DeleteComponent<Sprite>();
				}
			}

			if (script != nullptr) {

				if (ImGui::CollapsingHeader("Script")) {
					std::string text = "None";
					if (script->script != nullptr) text = script->script->name();
					ImGui::Text("Script: ");
					ImGui::SameLine();

					if (ImGui::Button(text.c_str(), ImVec2(150, 0))) {
						std::string path = GetFile("Text Files\0*.h\0*.*\0");
						if (path != "") script->script = CreateScript(GetFileName(path), go); //TODO::Get if file is valid
					}
					if (ImGui::Button("Delete##3")) go->DeleteComponent<Script>();
				}
			}

			if (collider != nullptr) {
				if (ImGui::CollapsingHeader("Collider")) {
					if (ImGui::Button("Delete##4")) go->DeleteComponent<Collider2>();
				}
			}

			if (cam != nullptr) {
				if (ImGui::CollapsingHeader("Camera")) {
					if (ImGui::Button("Delete##5")) go->DeleteComponent<Camera>(); //TODO::ACTIVE CAMERA ERROR!!
				}
			}

		
			ImGui::Separator();
			ImGui::NewLine();
			ImGui::NewLine();

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("popup0");
			}

			if (ImGui::BeginPopup("popup0")) {
				if (ImGui::Selectable("Transform") && transform == nullptr)		go->AddComponent<Transform>();
				if (ImGui::Selectable("Sprite") && sprite == nullptr)			go->AddComponent<Sprite>();
				if (ImGui::Selectable("Script") && script == nullptr)			go->AddComponent<Script>();
				if (ImGui::Selectable("Collider") && collider == nullptr)		go->AddComponent<Collider2>();
				if (ImGui::Selectable("Camera") && cam == nullptr)				go->AddComponent<Camera>();

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
};