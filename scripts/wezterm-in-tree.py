#!/usr/bin/python3

# Move first Wezterm ( github.com/wez/wezterm ) window to a dedicated workspace, leave subsequent windows where they are.

import i3ipc

if __name__ == "__main__":
    ipc = i3ipc.Connection()
    tree = ipc.get_tree()
    wezterm_workspace = 4
    if tree.find_marked('^wez$') == []:
        # Avoid wallpaper flickering when moving and switching by specifying both actions in the same Sway's command
        reply = ipc.command(f"move container workspace number {wezterm_workspace}, workspace number {wezterm_workspace}, focus, mark wez")
        assert(reply[0].success) # exit with non-zero status if the assertion fails

