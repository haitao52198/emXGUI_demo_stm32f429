
#include <stdio.h>
#include "emXGUI.h"
#include "x_libc.h"
#include "GUI_AppDef.h"
#include "emxgui_png.h"
#include "gui_font_port.h"


/**********************分界线*********************/

BOOL Load_state = FALSE;
HWND GUI_Boot_hwnd;
/* 各类控件ID */
enum eID
{
  ID_LOGO,
  ID_TEXT1,
  ID_TEXT2,
  ID_PROGBAR,
};
/* 外部图片数据 */
extern char bootlogo[];
/* 外部图片数据大小 */
extern unsigned int bootlogo_size(void);
HWND Boot_progbar = NULL;

/**
  * @brief  加载资源线程
  */
static void App_Load_Res(void )
{
  static int thread=0;
  static rt_thread_t h_load;

  if(thread==0)
  {  
    h_load=rt_thread_create("Load Res",(void(*)(void*))App_Load_Res,NULL,5*1024,1,5);
    thread =1;
    rt_thread_startup(h_load);//启动线程
    return;
  }
  while(thread) //线程已创建了
  {     
    HFONT hFont;
 
    /* 加载字体到外部SDRAM，返回defaultFont */    
    hFont = GUI_Extern_FontInit();
    if(hFont==NULL)
    {
      GUI_ERROR("GUI Extern Default Font Init Failed.");
      return;
    }
    else
    {
      /* 重设默认字体 */
      GUI_SetDefFont(hFont);  
    }    
    
    //发消息给启动窗口，关闭
    SendMessage(GUI_Boot_hwnd,WM_CLOSE,0,0);
    Load_state = TRUE;
    thread = 0;       

    rt_thread_delete(h_load);

  }
  return;
}

/**
  * @brief  启动界面回调函数
  */
static	LRESULT	win_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static  BITMAP png_bm;
  static  PNG_DEC *png_dec; 
  PROGRESSBAR_CFG cfg;
  switch (msg)
  {  
    case WM_CREATE:
    {
      RECT rc;

      GetClientRect(hwnd,&rc); //获得窗口的客户区矩形

      /* 根据图片数据创建PNG_DEC句柄 */
      png_dec = PNG_Open((u8 *)bootlogo, bootlogo_size());
      /* 把图片转换成bitmap */
      PNG_GetBitmap(png_dec, &png_bm);
      CreateWindow(TEXTBOX, L"emXGUI booting", WS_VISIBLE, 
                    0,260,800,40,
                    hwnd, ID_TEXT1, NULL, NULL);
      SendMessage(GetDlgItem(hwnd, ID_TEXT1),TBM_SET_TEXTFLAG,0,
                    DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_BKGND); 

      CreateWindow(TEXTBOX, L"Copying FontLIB form SPIFALSH to SDRAM", WS_VISIBLE, 
                    0,300,800,40,
                    hwnd, ID_TEXT2, NULL, NULL);
      SendMessage(GetDlgItem(hwnd, ID_TEXT2),TBM_SET_TEXTFLAG,0,
                    DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_BKGND); 


      //PROGRESSBAR_CFG结构体的大小
      cfg.cbSize	 = sizeof(PROGRESSBAR_CFG);
      //开启所有的功能
      cfg.fMask    = PB_CFG_ALL;
      //文字格式水平，垂直居中
      cfg.TextFlag = DT_VCENTER|DT_CENTER;  

      Boot_progbar = CreateWindow(PROGRESSBAR,L"Loading",
                                     PBS_TEXT|PBS_ALIGN_LEFT|WS_VISIBLE,
                                    50, 380, 700, 40 ,hwnd,ID_PROGBAR,NULL,NULL);

      SendMessage(Boot_progbar,PBM_GET_CFG,TRUE,(LPARAM)&cfg);
      SendMessage(Boot_progbar,PBM_SET_CFG,TRUE,(LPARAM)&cfg);
      SendMessage(Boot_progbar,PBM_SET_RANGLE,TRUE, FONT_NUM);
      SendMessage(Boot_progbar,PBM_SET_VALUE,TRUE,0); 
      SetTimer(hwnd, 1, 20, TMR_SINGLE|TMR_START, NULL);
      
      break;
    }
    
    case WM_TIMER:
    {
      /* 启动界面创建后timer时间后才开始加载 */
      App_Load_Res();
      break;         
    }
    case WM_ERASEBKGND:
    {
      HDC hdc =(HDC)wParam;
      RECT rc =*(RECT*)lParam;

      SetBrushColor(hdc, MapRGB(hdc, 0, 0, 0));
      FillRect(hdc, &rc);           
      /* 显示图片 */
      DrawBitmap(hdc, 250, 80, &png_bm, NULL);  
      return TRUE;

    }


    case	WM_CTLCOLOR:
    {
      u16 id;
      CTLCOLOR *cr;
      id =LOWORD(wParam);				
      cr =(CTLCOLOR*)lParam;
      if(id == ID_TEXT1 || id == ID_TEXT2)
      {
        cr->TextColor =RGB888(255,255,255);//文字颜色（RGB888颜色格式)
        cr->BackColor =RGB888(0,0,0);//背景颜色（RGB888颜色格式)
        //cr->BorderColor =RGB888(255,10,10);//边框颜色（RGB888颜色格式)
        return TRUE;
      }

      break;
    }  
    case WM_CLOSE: //窗口销毁时，会自动产生该消息，在这里做一些资源释放的操作.
    {
      /* 关闭PNG_DEC句柄 */
      PNG_Close(png_dec);
      DestroyWindow(hwnd); 
      Boot_progbar = NULL;
      return TRUE; //调用PostQuitMessage，使用主窗口结束并退出消息循环.
    }      
    default:
      return	DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return	WM_NULL;                                     
}

extern void 	GUI_Board_App_Desktop(void);
extern void	GUI_RES_WRITER_DIALOG(void);
extern void	GUI_DEMO_SlideWindow(void);

void	GUI_Boot_Interface_DIALOG(void)
{

  WNDCLASS	wcex;
  MSG msg;
  
  /* 设默认字体为ASCII 内部flash字库，防止出错 */
  GUI_SetDefFont(defaultFontEn);  

  wcex.Tag 		    = WNDCLASS_TAG;

  wcex.Style			= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= win_proc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= NULL;//hInst;
  wcex.hIcon			= NULL;
  wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);

  //创建启动提示
  GUI_Boot_hwnd = CreateWindowEx(	WS_EX_LOCKPOS|WS_EX_FRAMEBUFFER,
                              &wcex,
                              L"Booting",
                              WS_VISIBLE|WS_CLIPCHILDREN|WS_OVERLAPPED,
                              0,0,GUI_XSIZE,GUI_YSIZE,
                              NULL,0,NULL,NULL);


  //显示窗口.
  ShowWindow(GUI_Boot_hwnd,SW_SHOW);

  while(GetMessage(&msg,GUI_Boot_hwnd))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  /* 启动界面在加载完资源后会关闭，执行以下代码，创建应用线程 */
  if(Load_state == TRUE)
  {
     rt_thread_t h;             
     
     if(res_not_found_flag)
     {
        /* 若找不到资源，进入资源烧录应用 */
        h=rt_thread_create("GUI_FLASH_WRITER",GUI_RES_WRITER_DIALOG,NULL,8*1024,5,5);
        rt_thread_startup(h);			

     }
     else
     {	
        /* 找到资源，正常跑应用*/ 
     
        h=rt_thread_create("GUI_APP",GUI_Board_App_Desktop,NULL,8*1024,5,5);
        rt_thread_startup(h);			
        h=rt_thread_create("GUI_SLIDE_WIN",GUI_DEMO_SlideWindow,NULL,4096,5,5);
        rt_thread_startup(h);
     }   
  } 


}
