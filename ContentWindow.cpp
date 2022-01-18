#include "ContentWindow.h"

using Microsoft::WRL::ComPtr;
using DirectX::XMFLOAT3;

ContentWindow::ContentWindow(int width, int height, const char* name) :
	WindowBase(width, height, name),
	m_stateBlock(nullptr),
	m_layout(nullptr),
	m_mouseState(nullptr)
	//
	//
	//m_cursor(LoadCursor(NULL, ))
{
	// Create the device resources
	m_deviceResources = std::make_shared<DeviceResources>(m_hWnd);
	m_deviceResources->OnResize(); // Calling OnResize will create the render target, etc.

	// Create the state block 
	ThrowIfFailed(
		m_deviceResources->D2DFactory()->CreateDrawingStateBlock(m_stateBlock.GetAddressOf())
	);

	// Create the layout to fill the entire screen with a single grid square
	// Because the layout will be rendered using DIPS, we must pass the DIPS values for height/width
	m_layout = std::make_shared<Layout>(m_deviceResources, 0.0f, 0.0f, m_deviceResources->PixelsToDIPS(Height()), m_deviceResources->PixelsToDIPS(Width()));

	// Create the mouse state object
	m_mouseState = std::make_shared<MouseState>();
}

ContentWindow::~ContentWindow()
{
	// Each control bound to the layout has a pointer back to the layout, so it cannot simply
	// be set to nullptr because it won't be destructed.
	m_layout->ReleaseLayout();
	m_layout = nullptr;
}

void ContentWindow::Update()
{

	std::ostringstream oss;
	oss << "Time: " << m_timer.GetTotalSeconds();
	//SetWindowText(m_hWnd, oss.str().c_str());

	m_timer.Tick([&]() 
		{
			// Pass the Update call along to the layout, which will pass it along to each child control
			m_layout->Update(m_timer);

			// There should only be a single simulation which is accessible via SimulationManager
			// Because multiple controls may need to read from the simulation data, the update to the simulation
			// must come from the main window, so that multiple controls don't try updating the simulation
			SimulationManager::Update(m_timer);		
		}
	);	
}

bool ContentWindow::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return false;

	// Pass the Render call along to the layout, which will pass it along to each child control
	// return m_layout->Render();

	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	m_deviceResources->ResetViewport();

	FLOAT background[4] = { 45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f };
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), background);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	

	// Draw all 3D simulation controls first
	m_layout->Render3DControls();

	// Draw all 2D / Menu controls next
	m_deviceResources->ResetViewport();

	ID2D1DeviceContext* context2 = m_deviceResources->D2DDeviceContext();
	context2->SaveDrawingState(m_stateBlock.Get());
	context2->BeginDraw();
	context2->SetTransform(m_deviceResources->OrientationTransform2D());

	m_layout->Render2DControls();

	// re-render the captured control to make sure it is on top of the UI
	m_layout->Render2DCapturedControl();


	HRESULT hr = context2->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicsResources();
	}

	context2->RestoreDrawingState(m_stateBlock.Get());

	return true;
}

void ContentWindow::Present()
{
	// Present the render target to the screen
	m_deviceResources->Present();
}



LRESULT ContentWindow::OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	return 0;
}

LRESULT ContentWindow::OnDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	DiscardGraphicsResources();
	return 0;
}

LRESULT ContentWindow::OnLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call SetCapture to continue to get WM_MOUSEMOVE messages even if the mouse goes off the client area
	SetCapture(hWnd);

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonDown(m_mouseState);


	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call ReleaseCapture so the application does not get WM_MOUSEMOVE messages if the mouse if off screen
	ReleaseCapture();

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(false);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonUp(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonUp(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonDoubleClick(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonDoubleClick(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->MButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->MButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->RButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->RButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT ContentWindow::OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Resize the device resources
	m_deviceResources->OnResize();

	// Update the Layout
	m_layout->OnResize(0.0f, 0.0f, m_deviceResources->PixelsToDIPS(Height()), m_deviceResources->PixelsToDIPS(Width()));

	return 0;
}

LRESULT ContentWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pts = MAKEPOINTS(lParam);
	std::ostringstream oss;
	oss << "Pixels: (" << pts.x << ", " << pts.y << ") - DIPS: (" << m_deviceResources->PixelsToDIPS(pts.x) << ", " << m_deviceResources->PixelsToDIPS(pts.y) << ") - Main";
	//SetWindowText(hWnd, oss.str().c_str());

	// ====================================

	// Change cursor to arrow


	// Make sure mouse tracking is enabled
	m_mouseState->EnableMouseTracking(m_hWnd);

	// Store the mouse location in DIPS
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));

	// Pass the coordinates of the mouse to the main layout
	m_layout->OnMouseMove(m_mouseState);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Mouse is no longer over client area, so disable mouse tracking
	m_mouseState->DisableMouseTracking();

	// Pass the OnMouseLeave event to the layout
	m_layout->OnMouseLeave();
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseLeave();
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pts = MAKEPOINTS(lParam);
	int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	// if the wheel Delta is 1/-1, it is from the mouse pad, so just elevate it to 120
	if (wheelDelta == 1 || wheelDelta == -1)
		wheelDelta *= 120;

	// Pass the wheel delta to main layout / controls
	m_layout->OnMouseWheel(wheelDelta);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseWheel(wheelDelta);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ContentWindow::DiscardGraphicsResources()
{
	m_deviceResources = nullptr;
}

float ContentWindow::Height()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return static_cast<float>(rect.bottom);
}
float ContentWindow::Width()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return static_cast<float>(rect.right);
}

LRESULT ContentWindow::OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Minimums should be set in DIPS to be uniform across devices
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMinTrackSize.x = static_cast<LONG>(m_deviceResources->DIPSToPixels(500));
	lpMMI->ptMinTrackSize.y = static_cast<LONG>(m_deviceResources->DIPSToPixels(300));

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ContentWindow::OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//static std::string title;
	//title.push_back((char)wParam);
	//SetWindowText(hWnd, title.c_str());

	// Pass the char to main layout / controls
	m_layout->OnChar((char)wParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_layout->OnKeyUp(static_cast<unsigned char>(wParam));

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_layout->OnKeyDown(static_cast<unsigned char>(wParam));

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/*
// Button click methods
void ContentWindow::NewSimulationButtonClick()
{
	// Get the layout for the pane on the right
	std::shared_ptr<Layout> layout = m_layout->GetSubLayout(2, 1);
	if (layout == nullptr)
		layout = m_layout->CreateSubLayout(2, 1);
	else
		layout->Clear();


	RowColDefinitions rowDefs;
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);		// Simulation Name sublayout
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 50.0f);		// Add Atom combo box
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 300.0f);	// Atom Position / Velocity
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f);		// Add atom button
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Atom ListView
	layout->SetRowDefinitions(rowDefs);

	// ============================================================================================================
	// Sub Layout for simulation name controls
	std::shared_ptr<Layout> simulationNameSubLayout = layout->CreateSubLayout(0, 0);

	RowColDefinitions simulationNameColumns;
	simulationNameColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);
	simulationNameColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	simulationNameSubLayout->SetColumnDefinitions(simulationNameColumns);

	// Text for Simulation Name
	std::shared_ptr<Text> simulationNameText = simulationNameSubLayout->CreateControl<Text>(0, 0);
	simulationNameText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	simulationNameText->SetText(L"Simulation Name:");
	simulationNameText->Margin(5.0f, 0.0f);

	// Text Input for Simulation Name
	std::shared_ptr<TextInput> simulationNameTextInput = simulationNameSubLayout->CreateControl<TextInput>(0, 1);
	simulationNameTextInput->Margin(0.0f, 0.0f, 5.0f, 0.0f);
	// ============================================================================================================
	// ============================================================================================================
	// Sub Layout for add atom controls
	std::shared_ptr<Layout> addAtomSubLayout = layout->CreateSubLayout(1, 0);

	RowColDefinitions addAtomColumns;
	addAtomColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);
	addAtomColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	addAtomSubLayout->SetColumnDefinitions(addAtomColumns);

	// Text for Add atom
	std::shared_ptr<Text> addAtomText = addAtomSubLayout->CreateControl<Text>(0, 0);
	addAtomText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	addAtomText->SetText(L"Atom Type:");
	addAtomText->Margin(5.0f, 0.0f);

	std::shared_ptr<ComboBox> atomComboBox = addAtomSubLayout->CreateControl<ComboBox>(0, 1);
	atomComboBox->AddComboBoxItem(L"Hydrogen");
	atomComboBox->AddComboBoxItem(L"Helium");
	atomComboBox->AddComboBoxItem(L"Lithium");
	atomComboBox->AddComboBoxItem(L"Beryllium");
	atomComboBox->AddComboBoxItem(L"Boron");
	atomComboBox->AddComboBoxItem(L"Carbon");
	atomComboBox->AddComboBoxItem(L"Nitrogen");
	atomComboBox->AddComboBoxItem(L"Oxygen");
	atomComboBox->AddComboBoxItem(L"Flourine");
	atomComboBox->AddComboBoxItem(L"Neon");
	atomComboBox->Margin(5.0f, 10.0f);
	// atomComboBox->SelectionChanged() - defined below so it can capture the listview

	

	// ============================================================================================================
	// ============================================================================================================
	// Sub Layout for editing position and velocity
	std::shared_ptr<Layout> atomPositionVelocitySubLayout = layout->CreateSubLayout(2, 0);

	RowColDefinitions positionVelocityColumns;
	positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 60.0f);
	positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	atomPositionVelocitySubLayout->SetColumnDefinitions(positionVelocityColumns);

	RowColDefinitions positionVelocityRows;
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions:
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions Slider
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position:
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position X
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Y
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Z
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity:
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity X
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Y
	positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Z
	atomPositionVelocitySubLayout->SetRowDefinitions(positionVelocityRows);

	// Text for "Box Dimensions"
	std::shared_ptr<Text> boxDimensionsText = atomPositionVelocitySubLayout->CreateControl<Text>(0, 0, 1, 2);
	boxDimensionsText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	boxDimensionsText->SetText(L"Box Dimensions");
	boxDimensionsText->Margin(5.0f, 0.0f);

	// Box Dimension Slider
	std::shared_ptr<Slider> boxDimensionSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(1, 0, 1, 2);
	boxDimensionSlider->SetMin(2.0f);
	boxDimensionSlider->SetMax(100.0f);
	boxDimensionSlider->Margin(10.0f, 2.0f);
	boxDimensionSlider->ValueChanged([](float value)
	{
		SimulationManager::BoxDimensions(value);
	});




	// Text for "Position"
	std::shared_ptr<Text> positionText = atomPositionVelocitySubLayout->CreateControl<Text>(2, 0);
	positionText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	positionText->SetText(L"Position");
	positionText->Margin(5.0f, 0.0f);

	// Text for X Position
	std::shared_ptr<Text> positionXText = atomPositionVelocitySubLayout->CreateControl<Text>(3, 0);
	positionXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	positionXText->SetText(L"X:");
	positionXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for X Position
	std::shared_ptr<Slider> positionXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(3, 1);
	positionXSlider->SetMin(-1.0f);
	positionXSlider->SetMax(1.0f);
	positionXSlider->Margin(0.0f, 2.0f);





	// Text for Y Position
	std::shared_ptr<Text> positionYText = atomPositionVelocitySubLayout->CreateControl<Text>(4, 0);
	positionYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	positionYText->SetText(L"Y:");
	positionYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for Y Position
	std::shared_ptr<Slider> positionYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(4, 1);
	positionYSlider->SetMin(-1.0f);
	positionYSlider->SetMax(1.0f);
	positionYSlider->Margin(0.0f, 2.0f);




	// Text for Z Position
	std::shared_ptr<Text> positionZText = atomPositionVelocitySubLayout->CreateControl<Text>(5, 0);
	positionZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	positionZText->SetText(L"Z:");
	positionZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for Z Position
	std::shared_ptr<Slider> positionZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(5, 1);
	positionZSlider->SetMin(-1.0f);
	positionZSlider->SetMax(1.0f);
	positionZSlider->Margin(0.0f, 2.0f);



	// ===========================================================
	// Now that all the position sliders have been created, we can 
	// reference them in the lambda for the box dimension slider
	// - Capture values by value, capturing shared_ptr's by reference
	//   will cause some kind of read access error
	boxDimensionSlider->ValueChanged(
		[weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
		weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
		weakPZSlider = std::weak_ptr<Slider>(positionZSlider)](float value)
		{
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();

			SimulationManager::BoxDimensions(value);
			float half = value / 2.0f;
			pXSlider->SetMin(-half);
			pXSlider->SetMax(half);
			pYSlider->SetMin(-half);
			pYSlider->SetMax(half);
			pZSlider->SetMin(-half);
			pZSlider->SetMax(half);
		}
	);
	// ===========================================================



	// Text for "Velocity"
	std::shared_ptr<Text> velocityText = atomPositionVelocitySubLayout->CreateControl<Text>(6, 0);
	velocityText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	velocityText->SetText(L"Velocity");
	velocityText->Margin(5.0f, 0.0f);

	// Text for X Velocity
	std::shared_ptr<Text> velocityXText = atomPositionVelocitySubLayout->CreateControl<Text>(7, 0);
	velocityXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	velocityXText->SetText(L"X:");
	velocityXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for X Velocity
	std::shared_ptr<Slider> velocityXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(7, 1);
	velocityXSlider->SetMin(-100.0f);
	velocityXSlider->SetMax(100.0f);
	velocityXSlider->Margin(0.0f, 2.0f);



	// Text for Y Velocity
	std::shared_ptr<Text> velocityYText = atomPositionVelocitySubLayout->CreateControl<Text>(8, 0);
	velocityYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	velocityYText->SetText(L"Y:");
	velocityYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for Y Velocity
	std::shared_ptr<Slider> velocityYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(8, 1);
	velocityYSlider->SetMin(-100.0f);
	velocityYSlider->SetMax(100.0f);
	velocityYSlider->Margin(0.0f, 2.0f);



	// Text for Z Velocity
	std::shared_ptr<Text> velocityZText = atomPositionVelocitySubLayout->CreateControl<Text>(9, 0);
	velocityZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
	velocityZText->SetText(L"Z:");
	velocityZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

	// Slider for Z Velocity
	std::shared_ptr<Slider> velocityZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(9, 1);
	velocityZSlider->SetMin(-100.0f);
	velocityZSlider->SetMax(100.0f);
	velocityZSlider->Margin(0.0f, 2.0f);


	// ============================================================================================================
	// ============================================================================================================
	// Add atom button
	std::shared_ptr<Button> addAtomButton = layout->CreateControl<Button>(3, 0);
	addAtomButton->SetColorTheme(THEME_NEW_SIMULATION_ADD_BUTTON_COLOR);
	addAtomButton->SetBorderTheme(THEME_NEW_SIMULATION_ADD_BUTTON_BORDER);
	addAtomButton->Margin(20.0f, 5.0f);	

	std::shared_ptr<Layout> addAtomButtonLayout = addAtomButton->GetLayout();

	std::shared_ptr<Text> addAtomButtonText = addAtomButtonLayout->CreateControl<Text>();
	addAtomButtonText->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
	addAtomButtonText->SetText(L"Add New Atom");


	// ============================================================================================================
	// ============================================================================================================
	// Atom List View
	std::shared_ptr<ListView<Atom>> atomListView = layout->CreateControl<ListView<Atom>>(4, 0);
	atomListView->SetItemHeight(40.0f);
	atomListView->SetFormatFunction(
		[weakDeviceResources = std::weak_ptr<DeviceResources>(m_deviceResources),
		weakListView = std::weak_ptr<ListView<Atom>>(atomListView),
		weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
		weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
		weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
		weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
		weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
		weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
		weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)](std::shared_ptr<Atom> atom, bool highlighted)
		{
			auto deviceResources = weakDeviceResources.lock();
			auto listView = weakListView.lock();

			// Create a new layout object with the same height and a reasonable width (it will get resized)
			std::shared_ptr<Layout> newLayout = std::make_shared<Layout>(deviceResources, 0.0f, 0.0f, 40.0f, 1000.0f);

			std::shared_ptr<Button> newButton = newLayout->CreateControl<Button>();
			if (highlighted)
				newButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
			else
				newButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
			newButton->SetBorderTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_BORDER);
			newButton->Margin(5.0f, 0.0f);
			std::shared_ptr<Layout> newButtonLayout = newButton->GetLayout();

			RowColDefinitions columnDefs;
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 75.0f);	// Name of the atom type
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 47.0f);	// Position: / Velocity:
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// Position / Velocity values
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);	// Remove Button
			newButtonLayout->SetColumnDefinitions(columnDefs);

			RowColDefinitions rowDefs;
			rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
			rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
			newButtonLayout->SetRowDefinitions(rowDefs);

			// Text of the atom type
			std::shared_ptr<Text> atomNameText = newButtonLayout->CreateControl<Text>(0, 0, 2, 1);
			atomNameText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_TEXT);
			atomNameText->SetText(atom->Name());
			atomNameText->Margin(5.0f, 0.0f, 0.0f, 0.0f);


			std::shared_ptr<Text> positionText = newButtonLayout->CreateControl<Text>(0, 1);
			positionText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);
			positionText->SetText(L"Position:");

			std::shared_ptr<Text> positionValueText = newButtonLayout->CreateControl<Text>(0, 2);
			positionValueText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);

			std::ostringstream positionOSS;
			positionOSS.precision(3);
			positionOSS << std::fixed << "(" << atom->Position().x << ", " << atom->Position().y << ", " << atom->Position().z << ")";
			positionValueText->SetText(positionOSS.str());


			
			std::shared_ptr<Text> velocityText = newButtonLayout->CreateControl<Text>(1, 1);
			velocityText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);
			velocityText->SetText(L"Velocity:");

			std::shared_ptr<Text> velocityValueText = newButtonLayout->CreateControl<Text>(1, 2);
			velocityValueText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);

			std::ostringstream velocityOSS;
			velocityOSS.precision(3);
			velocityOSS << std::fixed << "(" << atom->Velocity().x << ", " << atom->Velocity().y << ", " << atom->Velocity().z << ")";
			velocityValueText->SetText(velocityOSS.str());


			// Remove button with "X" in it to remove this listview item
			std::shared_ptr<Button> removeButton = newButtonLayout->CreateControl<Button>(0, 3, 2, 1);
			removeButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_COLOR);
			removeButton->SetBorderTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_BORDER);
			removeButton->Margin(5.0f, 10.0f);
			
			removeButton->Click(
				[weakListView = std::weak_ptr<ListView<Atom>>(listView), 
				weakAtom = std::weak_ptr<Atom>(atom),
				weakPXSlider,
				weakPYSlider,
				weakPZSlider,
				weakVXSlider,
				weakVYSlider,
				weakVZSlider,
				weakComboBox]()
				{	
					auto listView = weakListView.lock();
					auto atom = weakAtom.lock();
					auto pXSlider = weakPXSlider.lock();
					auto pYSlider = weakPYSlider.lock();
					auto pZSlider = weakPZSlider.lock();
					auto vXSlider = weakVXSlider.lock();
					auto vYSlider = weakVYSlider.lock();
					auto vZSlider = weakVZSlider.lock();
					auto comboBox = weakComboBox.lock();

					// Do not allow deleting of the last item
					if (listView->ItemCount() == 1)
						return;

					// Get the index of the current atom and whether or not it was highlighted
					int index = listView->ItemIndex(atom);
					bool isHighlighted = listView->IsItemHighlighted(atom);

					listView->RemoveItem(atom);
					SimulationManager::RemoveAtom(atom);

					// If the atom to be deleted was the highlighted one, we must try to re-select another atom
					if (isHighlighted)
					{
						// If the index is now beyond the range of the total atoms, decrement the index
						if (index == listView->ItemCount())
							--index;

						listView->HighlightItem(index);
						SimulationManager::SelectAtom(index);

						std::shared_ptr<Atom> selectedAtom = SimulationManager::GetSelectedAtom();

						// Update the position/velocity sliders						
						pXSlider->SetValue(selectedAtom->Position().x);
						pYSlider->SetValue(selectedAtom->Position().y);
						pZSlider->SetValue(selectedAtom->Position().z);
						vXSlider->SetValue(selectedAtom->Velocity().x);
						vYSlider->SetValue(selectedAtom->Velocity().y);
						vZSlider->SetValue(selectedAtom->Velocity().z);

						// Update the atom type comboBox
						switch (selectedAtom->ElementType())
						{
						case Element::HYDROGEN:		comboBox->SelectItem(L"Hydrogen"); break;
						case Element::HELIUM:		comboBox->SelectItem(L"Helium"); break;
						case Element::LITHIUM:		comboBox->SelectItem(L"Lithium"); break;
						case Element::BERYLLIUM:	comboBox->SelectItem(L"Beryllium"); break;
						case Element::BORON:		comboBox->SelectItem(L"Boron"); break;
						case Element::CARBON:		comboBox->SelectItem(L"Carbon"); break;
						case Element::NITROGEN:		comboBox->SelectItem(L"Nitrogen"); break;
						case Element::OXYGEN:		comboBox->SelectItem(L"Oxygen"); break;
						case Element::FLOURINE:		comboBox->SelectItem(L"Flourine"); break;
						case Element::NEON:			comboBox->SelectItem(L"Neon"); break;
						}
					}
				}
			);
			
			std::shared_ptr<Layout> removeButtonLayout = removeButton->GetLayout();
			std::shared_ptr<Text> removeGlyph = removeButtonLayout->CreateControl<Text>();
			removeGlyph->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_GLYPH);
			removeGlyph->SetText(L"\xE711");

			return newLayout;
		}
	);
	atomListView->SetHighlightItemLayoutMethod([](std::shared_ptr<Layout> layout) 
		{
			std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
		}
	);
	atomListView->SetUnhighlightItemLayoutMethod([](std::shared_ptr<Layout> layout)
		{
			std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
		}
	);
	atomListView->SetItemClickMethod(
		[weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
		weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
		weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
		weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
		weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
		weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
		weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)](std::shared_ptr<Atom> atom) 
		{
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();
			auto vXSlider = weakVXSlider.lock();
			auto vYSlider = weakVYSlider.lock();
			auto vZSlider = weakVZSlider.lock();
			auto comboBox = weakComboBox.lock();

			SimulationManager::SelectAtom(atom);

			// Update the position/velocity sliders
			pXSlider->SetValue(atom->Position().x);
			pYSlider->SetValue(atom->Position().y);
			pZSlider->SetValue(atom->Position().z);
			vXSlider->SetValue(atom->Velocity().x);
			vYSlider->SetValue(atom->Velocity().y);
			vZSlider->SetValue(atom->Velocity().z);

			// Update the atom type comboBox
			switch (atom->ElementType())
			{
			case Element::HYDROGEN:		comboBox->SelectItem(L"Hydrogen"); break;
			case Element::HELIUM:		comboBox->SelectItem(L"Helium"); break;
			case Element::LITHIUM:		comboBox->SelectItem(L"Lithium"); break;
			case Element::BERYLLIUM:	comboBox->SelectItem(L"Beryllium"); break;
			case Element::BORON:		comboBox->SelectItem(L"Boron"); break;
			case Element::CARBON:		comboBox->SelectItem(L"Carbon"); break;
			case Element::NITROGEN:		comboBox->SelectItem(L"Nitrogen"); break;
			case Element::OXYGEN:		comboBox->SelectItem(L"Oxygen"); break;
			case Element::FLOURINE:		comboBox->SelectItem(L"Flourine"); break;
			case Element::NEON:			comboBox->SelectItem(L"Neon"); break;
			}
		}
	);
	atomListView->SetValueChangedUpdateLayoutMethod(
		[](std::shared_ptr<Atom> atom, std::shared_ptr<Layout> layout)
		{
			std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			std::shared_ptr<Layout> buttonLayout = button->GetLayout();

			// Text of the atom type
			std::shared_ptr<Text> atomNameText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(0));
			atomNameText->SetText(atom->Name());

			// Atom position values
			std::shared_ptr<Text> positionValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(2));
			std::ostringstream positionOSS;
			positionOSS.precision(3);
			positionOSS << std::fixed << "(" << atom->Position().x << ", " << atom->Position().y << ", " << atom->Position().z << ")";
			positionValueText->SetText(positionOSS.str());

			// Atom velocity values
			std::shared_ptr<Text> velocityValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(4));
			std::ostringstream velocityOSS;
			velocityOSS.precision(3);
			velocityOSS << std::fixed << "(" << atom->Velocity().x << ", " << atom->Velocity().y << ", " << atom->Velocity().z << ")";
			velocityValueText->SetText(velocityOSS.str());
		}
	);

	

	addAtomButton->Click(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView), 
		weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
		weakPYSlider = std::weak_ptr<Slider>(positionYSlider), 
		weakPZSlider = std::weak_ptr<Slider>(positionZSlider), 
		weakVXSlider = std::weak_ptr<Slider>(velocityXSlider), 
		weakVYSlider = std::weak_ptr<Slider>(velocityYSlider), 
		weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
		weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)]()
		{
			auto listView = weakListView.lock();
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();
			auto vXSlider = weakVXSlider.lock();
			auto vYSlider = weakVYSlider.lock();
			auto vZSlider = weakVZSlider.lock();
			auto comboBox = weakComboBox.lock();			

			// Add a new hydrogen atom to the center of the simulation and let
			// that be the newly selected atom
			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			SimulationManager::SelectAtom(SimulationManager::AddNewAtom<Hydrogen>(position, velocity));

			// Update the position/velocity sliders
			pXSlider->SetValue(0.0f);
			pYSlider->SetValue(0.0f);
			pZSlider->SetValue(0.0f);
			vXSlider->SetValue(0.0f);
			vYSlider->SetValue(0.0f);
			vZSlider->SetValue(0.0f);

			// Add the new atom to the list view
			// NOTE: Must add it to the list view BEFORE updating the combobox selected item because
			//       it will trigger an update to the listview item, which will cause an error because
			//       it hasn't been added yet
			listView->AddItem(SimulationManager::GetSelectedAtom());
			listView->HighlightItem(SimulationManager::GetSelectedAtom());

			// Update the Atom type combo box to display "Hydrogen"
			comboBox->SelectItem(L"Hydrogen");			
		}
	);

	positionXSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionX(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);
	positionYSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionY(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);
	positionZSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionZ(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);
	velocityXSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value) 
		{
			SimulationManager::SelectedAtomVelocityX(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);
	velocityYSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomVelocityY(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);
	velocityZSlider->ValueChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomVelocityZ(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
	);

	atomComboBox->SelectionChanged(
		[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](std::wstring value)
		{
			auto listView = weakListView.lock();
			int index = listView->ItemIndex(SimulationManager::GetSelectedAtom());

			if (value == L"Hydrogen")
				SimulationManager::ChangeSelectedAtomType<Hydrogen>();
			else if (value == L"Helium")
				SimulationManager::ChangeSelectedAtomType<Helium>();
			else if (value == L"Lithium")
				SimulationManager::ChangeSelectedAtomType<Lithium>();
			else if (value == L"Beryllium")
				SimulationManager::ChangeSelectedAtomType<Beryllium>();
			else if (value == L"Boron")
				SimulationManager::ChangeSelectedAtomType<Boron>();
			else if (value == L"Carbon")
				SimulationManager::ChangeSelectedAtomType<Carbon>();
			else if (value == L"Nitrogen")
				SimulationManager::ChangeSelectedAtomType<Nitrogen>();
			else if (value == L"Oxygen")
				SimulationManager::ChangeSelectedAtomType<Oxygen>();
			else if (value == L"Flourine")
				SimulationManager::ChangeSelectedAtomType<Flourine>();
			else if (value == L"Neon")
				SimulationManager::ChangeSelectedAtomType<Neon>();

			// Must completely replace the item instead of updating it because the 
			// SimulationManager::ChangeSelectedAtomType method will delete the existing
			// selected atom and replace it with a new one, which the listview does not 
			// know about
			listView->ReplaceItemAt(SimulationManager::GetSelectedAtom(), index);	

			// Must make to re-select the new atom
			listView->HighlightItem(SimulationManager::GetSelectedAtom());
		}
	);


	// ============================================================================================================
	// ============================================================================================================
	// Add a Hydrogen Atom to the simulation
	SimulationManager::Pause();
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	std::shared_ptr<Hydrogen> firstAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);
	SimulationManager::SelectAtom(firstAtom);

	// Add the atom to the list view
	atomListView->AddItem(firstAtom);

	// Select the first item so the button is highlighted
	atomListView->HighlightItem(firstAtom);
}
*/