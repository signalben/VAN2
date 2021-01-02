import keyboard
def printPressedKey(e):
    print("key pressed  : {}".format(e.name))

keyboard.hook(printPressedKey)
keyboard.wait('esc')