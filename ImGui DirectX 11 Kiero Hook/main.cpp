#include "includes.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;


bool init = false;




void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	ImGui::SetNextWindowSize(ImVec2(800, 600));
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0;

	style.WindowRounding = 3;
	style.GrabRounding = 1;
	style.GrabMinSize = 20;
	style.FrameRounding = 3;


	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	;
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);

	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool isMenuOpen = true;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (GetAsyncKeyState(VK_INSERT) & 1) {
		isMenuOpen = !isMenuOpen;
	}

	bool bEHotkey = false;
	bool bEHotkey2 = false;
	bool bEHotkey3 = false;
	static bool myCheckbox = false; // variabel boolean untuk checkbox
	static bool bMyCheckbox = false;
	static bool bOHKCheckbox = false;
	static bool bRSECheckbox = false;
	static bool bultCheckbox = false;
	static bool bECheckbox = false;
	static bool bGMCheckbox = false;
	static bool binbattlecheckbox = false;

	if (isMenuOpen)
	{
		ImGui::Begin("HSR KNTL");
	
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

		if (ImGui::BeginTabBar("MenuTabBar"))
		{
			if (ImGui::BeginTabItem("Player Menu"))
			{
				ImGui::Text("Player Cheat");
				
				ImGui::Checkbox("Bypass ( if u enable this u will see console window, don't close it )", &bMyCheckbox);
				if (bMyCheckbox) {
					static bool bypassEnabled = false;
					if (!bypassEnabled) {
						enablebypass_new::init();
						bypassEnabled = true;
						LOG_INFO("ANTICHEAT BYPASSED");
						LOG_DEBUG("DON'T BLAME ME IF U GET BAN");
						LOG_WARNING("ANY CHEAT IS NOT SAFE 100% PLAY NORMAL IF U WON'T GET BAN");
						LOG_INFO("I am not an expert in creating cheat just inserting offsets from [Discuss] Honkai: Star Rail and special thanks to Z4ee I learned a lot from you");
					}
					ImGui::Checkbox("Speed Hack", &myCheckbox);
					ImGui::Checkbox("Noclip", &myCheckbox);
					ImGui::Checkbox("Normal World Skill Point", &myCheckbox);
					ImGui::Checkbox("Peeking", &bRSECheckbox);
					
				} else{
					myCheckbox = false; 
					bRSECheckbox = false;
				}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Battle Menu"))
				{
					ImGui::Text("Battle Cheat (Enable In Batlle )");

					ImGui::Checkbox("Oh NO i Can't detect you on battle or no, if you in battle enable this <3", &binbattlecheckbox);
					if (binbattlecheckbox) {
						static bool inbattle = false;
						if (!inbattle) {
							static bool inbattle = true;
						}
							ImGui::Checkbox("One Hit KIll ( Better Use hotkey for this )", &bOHKCheckbox);
							
							ImGui::Checkbox("Unloack Auto Battle", &bOHKCheckbox);

							ImGui::Checkbox("Unlimited Ult ( don't use )", &bultCheckbox);



							ImGui::Checkbox("Skill E (dont use outside of battle)", &bECheckbox);



							ImGui::Checkbox("Good Mode (Work but you will get verification failure, usless )", &bGMCheckbox);

						
						
						
					}
						else {

						bOHKCheckbox = false;
						bultCheckbox = false;
						bultCheckbox = false;
						bGMCheckbox = false;
						}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Hotkey Menu"))
				{

					ImGui::Checkbox("Press Home Button To Refill", &bEHotkey);

					ImGui::Checkbox("Press End when battle end", &bEHotkey2);



					ImGui::Checkbox("One Hit Kill", &bEHotkey3);

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Settings Menu"))
				{

					ImGui::EndTabItem();
				}
			
				ImGui::EndTabBar();
			}


			ImGui::End();
		}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
	
	if (myCheckbox)
	{
		sh::menu();
	}
	if (bEHotkey && GetAsyncKeyState(VK_HOME) & 1) {
		e::skille();
	}
	if (bEHotkey2 && GetAsyncKeyState(VK_END) & 1) {
		e::skillend();
	}
	if (bEHotkey3 && GetAsyncKeyState(VK_DELETE) & 1) {
		onehit::ohk();
	}
	if (bOHKCheckbox) {
		onehit::ohk();
	}
	if (bRSECheckbox) {
		intip::intip();
	}
	if (bultCheckbox) {
		ult::menu();
	}
	if (bECheckbox) {
		e::skillend();
	}
	if (bGMCheckbox) {
		gm::gm();
	}



	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}

	DWORD WINAPI MainThread(LPVOID lpReserved)
	{
		bool init_hook = false;
		do
		{
			if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
			{
				kiero::bind(8, (void**)&oPresent, hkPresent);
				init_hook = true;
			}
		} while (!init_hook);
		return TRUE;
	}



	BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
	{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hMod);
			CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
			break;
		case DLL_PROCESS_DETACH:
			kiero::shutdown();
			break;
		}
		return TRUE;
	}
