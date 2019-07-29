# Fr2D library    
The encapsulation of WindowsAPI and Direct2D, for getting an easier access to develop a windows game program.   

Frankenstein's 2D graphics library based on DirectX, for learning C++ and Computer Graphics programing.   
Includes part of 3D graphics libraries as well.   
It may be turned into a 2D game engine later???⁄(⁄⁄•⁄ω⁄•⁄⁄)⁄    

![miku.png](https://raw.githubusercontent.com/salty-Frankenstein/Fr2D/master/examples/miku.png)    
![好天气.png](https://raw.githubusercontent.com/salty-Frankenstein/Fr2D/master/examples/好天气.png)    

# Requirements
A Direct2D environment which includes the library "d2d1.lib" and "dwrite.lib".    
Library "windowscodecs.lib" is also needed.  
(The heads related have already been included.)  

# Already done:    
## fr_win.h    
The encapsulation of WindowsAPI, used for simply creating a window for rendering.    
**caution: a Display function and a WndProc definition is needed, these will be changed into callback functions later.**     
- class FrWnd: The encapsulation of WindowsAPI.     

## fr2d.h:    
All the classes need to be initailized with its own Create() function, and then be created by the factory class Fr2D.     
- class Fr2D: Serve as a factory class, encapsulation of the D2Dfactory and a render target.     
- class Fr2DBrush: As the ID2D1SolidColorBrush, create for drawing.     
- class Fr2DBitmap: As the WICBitmap and D2DBitmap, loading bmp pictures and drawing on the form.     
- struct FrPoint: the point structure with two float variables.    
- function stringToLPCWSTR: turn std::string into LPCWSTR.    
- class FrText: For text writing on the window, supports both std::string and LPCWSTR.    
