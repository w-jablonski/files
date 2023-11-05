#!/usr/bin/python3

# Using Sway IPC move first window of an app to a dedicated workspace but leave subsequent windows where they are.

import i3ipc

if __name__ == "__main__":
    ipc = i3ipc.Connection()
    tree = ipc.get_tree()
    workspace = 4
    if tree.find_marked('^abc$') == []:
        # Avoid wallpaper flickering when moving and switching by specifying both actions in the same Sway's command
        reply = ipc.command(f"move container workspace number {workspace}, workspace number {workspace}, focus, mark abc")
        assert(reply[0].success) # exit with non-zero status if the assertion fails
