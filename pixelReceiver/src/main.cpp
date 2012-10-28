#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include <assert.h>


#ifdef WIN32  
#include <windows.h>  
  
static bool ModifyWindowStyle(HWND hWnd, DWORD dwAdd, DWORD dwRemove, BOOL bEx)  
{  
   ::SetLastError(0);  
  
   DWORD dwStyle    = ::GetWindowLong(hWnd, (bEx ? GWL_EXSTYLE : GWL_STYLE));  
   DWORD dwNewStyle = (dwStyle & (~dwRemove)) | dwAdd;  
   ::SetWindowLong(hWnd, (bEx ? GWL_EXSTYLE : GWL_STYLE), dwNewStyle);  
  
   ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);  
  
   //NOTE: this is necessary for updating visuals!!!  
   ::UpdateWindow(hWnd);  
   ::ShowWindow(hWnd, SW_SHOW);    //SW_SHOWNORMAL, SW_SHOWNA  
   ::SetFocus(hWnd);  
  
   return (::GetLastError() == 0);  
}  
#endif  

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1600,900,  OF_WINDOW);			// <-------- setup the GL context
	window.setWindowTitle("factScreen");

#ifdef WIN32  
	HWND hwnd = ::FindWindowA(0, "factScreen"); //NOTE, the windowtitle is crucial in order to find the handle, so you have to set it before!!!!  
    if (hwnd != NULL)  
    {  
        bool winStyleChanged = FALSE;  
        winStyleChanged = ModifyWindowStyle(hwnd, 0, WS_BORDER | WS_THICKFRAME | WS_CAPTION, FALSE);   //undecorate  
        //winStyleChanged = ModifyWindowStyle(hwnd, WS_BORDER | WS_THICKFRAME | WS_CAPTION, 0, FALSE);    //decorate  
        assert(TRUE == winStyleChanged);  
        ::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);    //NOTE: repaint glutwindow to update style  
     }  
     else  
     {  
       printf("[WARNING] could NOT find window, title might be wrong\n");  
     }  
#endif  

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
