import pyglet
from pyglet.window import Window
from pyglet import image

def newwindow(width,height,title):
    Window.clear()
    window = Window(width,height,title)
    return pyglet.window.Window(width=width, height=height, caption=title)

def changeicon(window_icon: str, window):
        newwindowicon = image.load(window_icon)
        window.set_icon(newwindowicon)
        

def closewindow(window):
    window.close()

def fullscreen(is_fullscreen, window):
    try:
        window.set_fullscreen(is_fullscreen)
    except RuntimeWarning:
         return
    
def runwindow():
    pyglet.app.run()