#include "Direct3D9.h"
#include "Visuals.h"

#ifndef D3DFONT_MAX
#define D3DFONT_MAX	10
#endif

namespace Direct3D9
{
	struct D3DFont_t
	{
		std::string	szName;
		ID3DXFont*	pFont;

	}D3DFont[ D3DFONT_MAX ];
	
	Direct3D9Render*		pRender				= new Direct3D9Render();
	D3DPRESENT_PARAMETERS	pParameter			= { 0 };
	HWND					hWndOverlay			= NULL;
	HWND					hWndTarget			= NULL;
	IDirect3D9Ex*			pD3D				= NULL;
	IDirect3DDevice9Ex*		pDevice				= NULL;
	ID3DXLine*				pLine				= NULL;
	int						iLastAddedFont		= 1;
	int						iResolution[ 2 ]	= { 0 };
	int						iCenter[ 2 ]		= { 0 };
	MARGINS					mgDWMMargins		= { -1, -1, -1, -1 };
	MSG						uMSG				= { 0 };
	std::string				szGameTitle			= "";

	Direct3D9Render::Direct3D9Render( void )
	{

	}
	Direct3D9Render::~Direct3D9Render( void )
	{

	}
	void Direct3D9Render::String( int x, int y, Color color, ID3DXFont* pFont, const char *fmt, ... )
	{
		char Buffer[ 4048 ];
		va_list vaList;
		va_start( vaList, fmt );
		vsprintf_s( Buffer, fmt, vaList );
		RECT rect_pos = { x, y, x + 500, y + 100 };
		pFont->DrawTextA( NULL, ( LPCSTR )Buffer, -1, &rect_pos, DT_TOP, D3DCOLOR_ARGB( color.a(), color.r(), color.g(), color.b() ) );
		va_end( vaList );
	}
	void Direct3D9Render::StringOutlined( int x, int y, Color color, ID3DXFont* pFont, const char *fmt, ... )
	{
		char Buffer[ 4048 ];
		va_list vaList;
		va_start( vaList, fmt );
		vsprintf_s( Buffer, fmt, vaList );
		this->String( x - 1, y, Color( 0, 0, 0 ), pFont, Buffer );
		this->String( x, y - 1, Color( 0, 0, 0 ), pFont, Buffer );	
		this->String( x + 1, y, Color( 0, 0, 0 ), pFont, Buffer );
		this->String( x, y + 1, Color( 0, 0, 0 ), pFont, Buffer );
		this->String( x, y, color, pFont, Buffer );
		va_end( vaList );
	}
	void Direct3D9Render::Rect( int x, int y, int l, int h, Color color )
	{
		D3DRECT rect = { x, y, x + l, y + h };
		pDevice->Clear( 1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB( color.a(), color.r(), color.g(), color.b() ), 0, 0 );
	}
	void Direct3D9Render::BorderBox( int x, int y, int l, int h, int thickness, Color color )
	{
		this->Rect( x, y, l, thickness, color );
		this->Rect( x, y, thickness, h, color );
		this->Rect( x + l, y, thickness, h, color );
		this->Rect( x, y + h, l+thickness, thickness, color );
	}
	void Direct3D9Render::BorderBoxOutlined( int x, int y, int l, int h, int thickness, Color color, Color outlined )
	{
		this->BorderBox( x, y, l, h, thickness, color );
		this->BorderBox( x - 1, y - 1, l + 2, h + 2, 1, outlined );
		this->BorderBox( x + 1, y + 1, l - 2, h - 2, 1, outlined );
	}
	void Direct3D9Render::Texture(int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE Sprite )
	{
		Sprite->Draw( dTexture,NULL,NULL,&D3DXVECTOR3( ( float )x, ( float )y, 0.0f ), 0xFFFFFFFF );
	}
	void Direct3D9Render::RectOutlined( int x, int y, int l, int h, Color rectcolor, Color outlinedcolor, int thickness )
	{
		Rect( x, y, l, h, rectcolor );
		BorderBox( x - thickness, y - thickness, l + thickness, h + thickness, thickness, outlinedcolor );
	}
	void Direct3D9Render::Line( int x, int y, int x2, int y2, Color color, float thickness )
	{
		pLine->SetWidth( thickness );
		D3DXVECTOR2 points[ 2 ];
		points[ 0 ].x = ( float )x;
		points[ 1 ].x = ( float )x2;
		points[ 0 ].y = ( float )y;
		points[ 1 ].y = ( float )y2;
		pLine->Draw( points,  2, D3DCOLOR_ARGB( color.a(), color.r(), color.g(), color.b() ) );
	}
	void Direct3D9Render::GardientRect( int x, int y, int w, int h, int thickness, bool outlined, Color from, Color to, Color Outlined )
	{
		float a = ( ( float )to.a() - ( float )from.a() ) / h;
		float r = ( ( float )to.r() - ( float )from.r() ) / h;
		float g = ( ( float )to.g() - ( float )from.g() ) / h;
		float b = ( ( float )to.b() - ( float )from.b() ) / h;

		for( int i = 0; i < h; i++ )
		{
			int A = from.a() + ( int )a * i;
			int R = from.r() + ( int )r * i;
			int G = from.g() + ( int )g * i;
			int B = from.b() + ( int )b * i;
			this->Rect( x, y + i, w, 1, Color( A, R, G, B ) );
		}
		if( outlined )
		{
			this->BorderBox( x - thickness, y - thickness, w + thickness, h + thickness, thickness, Outlined );
		}
	}
	int GetLastAddedFont( void )
	{
		return iLastAddedFont;
	}
	void SetLastAddedFont( int Value )
	{
		iLastAddedFont = Value;
	}
	bool AddFont( std::string szName, std::string szFontName, int Height, int Width, int Weight, bool Italic )
	{
		int i = GetLastAddedFont();

		if( FAILED( D3DXCreateFontA( pDevice, ( INT )Height, ( UINT )Width, ( INT )Weight, 1, ( BOOL )Italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, ( char* )szFontName.c_str(), &D3DFont[ i ].pFont ) ) )
			return false;

		D3DFont[ i ].szName = szName;

		SetLastAddedFont( i + 1 );
		
		return true;
	}
	HRESULT Startup( HWND hWindow )
	{
		BOOL	bComposition	= false;
		DWORD	msqAAQuality	= 0x0;
		HRESULT	hResult			= NULL;

		DwmIsCompositionEnabled( &bComposition );
		if( !bComposition )
			return E_FAIL;

		hResult = Direct3DCreate9Ex( D3D_SDK_VERSION, &pD3D );
		if( FAILED( hResult ) )
			return E_FAIL;

		ZeroMemory( &pParameter, sizeof( pParameter ) );
		pParameter.Windowed					= TRUE;
		pParameter.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		pParameter.BackBufferFormat			= D3DFMT_A8R8G8B8;
		pParameter.EnableAutoDepthStencil	= TRUE;
		pParameter.AutoDepthStencilFormat	= D3DFMT_D16;
		pParameter.MultiSampleType			= D3DMULTISAMPLE_NONE;
		pParameter.PresentationInterval		= 0x80000000L;

		if( SUCCEEDED( pD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8,  true, D3DMULTISAMPLE_NONMASKABLE, &msqAAQuality ) ) )
		{
			pParameter.MultiSampleType		= D3DMULTISAMPLE_NONMASKABLE;
			pParameter.MultiSampleQuality	= msqAAQuality - 1;
		}
		else
		{	
			pParameter.MultiSampleType		= D3DMULTISAMPLE_NONE;
		}
			
		hResult = pD3D->CreateDeviceEx( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParameter, NULL, &pDevice );
		if( FAILED( hResult ) )
			return E_FAIL;

		if( FAILED( D3DXCreateFontA( pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial", &D3DFont[ 0 ].pFont ) ) )
			return E_FAIL;

		//==============================[ Add your custom Font here ]=================================//

		if( !AddFont( "Arial", "Verdana", 14, 0, FW_NORMAL, false ) )
			return E_FAIL;

		if( !AddFont( "Arial Bold", "Arial", 14, 0, FW_BOLD, false ) )
			return E_FAIL;

		//============================================================================================//

		if( FAILED( D3DXCreateLine( pDevice, &pLine ) ) )
			return E_FAIL;

		pLine->SetWidth( 1.0f );
		pLine->SetPattern( 0xFFFFFFFF );
		pLine->SetAntialias( false );

		return S_OK;
		
	}
	LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( uMsg )
		{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
		case WM_KEYDOWN:
			return 0;
		case WM_ERASEBKGND:
			SendMessage( hWnd, WM_PAINT, NULL, NULL );
			return TRUE;
		case WM_PAINT:
			return 0;
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	void Shutdown( void )
	{
		if( pDevice != NULL )
			pDevice->Release();

		if( pD3D != NULL )
			pD3D->Release();

		if( pLine != NULL )
			pLine->Release();

		for( int i = 0; i < GetLastAddedFont(); i++ )
		{
			D3DFont[ i ].pFont->Release();
			D3DFont[ i ].szName = "";
		}

		SetLastAddedFont( 1 );
	}
	bool Render( void )
	{
		static bool bInit = false;

		if( !bInit )
		{
			
			bInit = true;
		}
		if( PeekMessage( &uMSG, NULL, 0, 0, PM_REMOVE ) )
		{
			if( uMSG.message == WM_QUIT )
				return false;
			TranslateMessage( &uMSG );
			DispatchMessage( &uMSG );
		}
		
		if( !FindWindowA( NULL, ( LPCSTR )szGameTitle.c_str() ) )
			return false;

		pDevice->Clear( NULL, NULL, D3DCLEAR_TARGET, 0x00000000, 1.f, NULL );
		
		if( SUCCEEDED( pDevice->BeginScene() ) )
		{
			if( hWndTarget == GetForegroundWindow() )
			{
				pVisuals->Main();				
			}
			pDevice->EndScene();
		}
		
		pDevice->PresentEx( NULL, NULL, NULL, NULL, NULL );

		return true;
	}
	ID3DXFont* GetFont( std::string szName )
	{
		for( int i = 1; i < GetLastAddedFont(); i++ )
		{
			if( !strcmp( ( char* )szName.c_str(), ( char* )D3DFont[ i ].szName.c_str() ) )
				return D3DFont[ i ].pFont;
		}
		return D3DFont[ 0 ].pFont;
	}
	int getWidth( void )
	{
		return iResolution[ 0 ];
	}
	int getHeight( void )
	{
		return iResolution[ 1 ];
	}
	HRESULT CreateOverlay( std::string szGameWindowTitle, std::string szOverlayTitle )
	{
		szGameTitle = szGameWindowTitle;

		if( !( char* )szGameTitle.c_str() )
			return E_FAIL;

		hWndTarget = FindWindowA( NULL, ( char* )szGameWindowTitle.c_str() );
		if( !hWndTarget )
			return E_FAIL;

		RECT rect_client, rect_app;

		GetClientRect( hWndTarget, &rect_client );
		iResolution[ 0 ] = rect_client.right;
		iResolution[ 1 ] = rect_client.bottom;
		iCenter[ 0 ] = iResolution[ 0 ] / 2;
		iCenter[ 1 ] = iResolution[ 1 ] / 2;

		WNDCLASSEX wc	= {	sizeof( WNDCLASSEX ), 0, WndProc, 0, 0, NULL, LoadIcon( NULL, IDI_APPLICATION ),
							LoadCursor( 0, IDC_ARROW ), 0, 0, ( LPCSTR )szOverlayTitle.c_str(), LoadIcon( 0, IDI_APPLICATION ) };

		RegisterClassEx( &wc );

		hWndOverlay		= CreateWindowEx( WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_TRANSPARENT | WS_EX_LAYERED, ( LPCSTR )szOverlayTitle.c_str(), ( LPCSTR )szOverlayTitle.c_str(),
						  WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, iResolution[ 0 ], iResolution[ 1 ], 0, 0, NULL, 0 );

		if( !hWndOverlay )
			return E_FAIL;

		DwmExtendFrameIntoClientArea( hWndOverlay, &mgDWMMargins );
		ShowWindow( hWndOverlay, SW_SHOWDEFAULT );
		UpdateWindow( hWndOverlay );
		GetWindowRect( hWndTarget, &rect_app );

		int border_x = GetSystemMetrics( SM_CXBORDER );
		int border_y = GetSystemMetrics( SM_CYCAPTION );

		MoveWindow( hWndOverlay, rect_app.left, rect_app.top, iResolution[ 0 ], iResolution[ 1 ], false );

		if( SUCCEEDED( Startup( hWndOverlay ) ) )
		{
			Shutdown();
			return( SUCCEEDED( Startup( hWndOverlay ) ) );
		}
		else
			return E_FAIL;
	}
}