# Fr2D library    
The encapsulation of WindowsAPI and Direct2D, for getting an easier access to develop a windows game program.    

# Already done:    
## fr_win.h    
The encapsulation of WindowsAPI, used for simply creating a window for rendering.    
**caution: a Display function and a WndProc definition is needed, these will be changed into callback functions later.**     
class FrWnd: The encapsulation of WindowsAPI     

## fr2d.h:    
All the classes need to be initailized with its own Create() function, and then be created by the factory class Fr2D.     
class Fr2D: Serve as a factory class, encapsulation of the D2Dfactory and a render target.     
class Fr2DBrush: As the ID2D1SolidColorBrush, create for drawing.     
class Fr2DBitmap: As the WICBipmap and D2DBitmap, loading bmp pictures and drawing on the form.     
struct FrPoint: the point structure with two float variables.    
