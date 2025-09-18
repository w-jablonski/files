#!/usr/bin/python3

# Using Sway IPC switch to or move focused window to another workspace

# A suggested use is to call this script and the four flags by respective four
# mouse buttons (left, wheel tilt left, wheel tilt right, right) or fingers
# on the keyboard. Each flag switches to its assigned workspace, unless it is
# already active in which case a "second choice" workspace gets picked. This
# way, a single or double click can take you to 8 workspaces. The 'move' flag
# moves currently focused window to another workspace using similar logic.
# In case that neither of the two workspaces associated with the sent flag is
# currently active, and only one of them exists (has windows), the switch action
# chooses the workspace that exists, even if it's the "second choice" one.
# The move action, however, always goes with its "first choice" workspace.

# www.reddit.com/r/swaywm/comments/1mbjgrm/best_workspaces_switching_with_mouse_and_keyboard/

import i3ipc
from argparse import ArgumentParser

if __name__ == "__main__":

    arguments_parser = ArgumentParser()
    arguments_parser.add_argument('-1', '--one', action='store_true')
    arguments_parser.add_argument('-2', '--two', action='store_true')
    arguments_parser.add_argument('-3', '--three', action='store_true')
    arguments_parser.add_argument('-4', '--four', action='store_true')
    arguments_parser.add_argument('-m', '--move', action='store_true')
    arguments = arguments_parser.parse_args()
    # a flag must be present
    assert(arguments.one or arguments.two or arguments.three or arguments.four)

    ipc = i3ipc.Connection()
    tree = ipc.get_tree()
    current_ws = tree.find_focused().workspace().num
    workspaces = tree.workspaces()
    ws_numbers = [workspace.num for workspace in workspaces]

    # assign two workspaces to each flag:
    if arguments.one:
        first_ws   = 1
        then_ws    = 2
    elif arguments.two:
        first_ws   = 3
        then_ws    = 4
    elif arguments.three:
        first_ws   = 5
        then_ws    = 6
    elif arguments.four:
        first_ws   = 7
        then_ws    = 8

    if current_ws != first_ws and current_ws != then_ws:
        move_ws = first_ws
        # if the "second choice" workspace exists but the "first choice" does not:
        if then_ws in ws_numbers and first_ws not in ws_numbers:
            go_ws = then_ws
        else:
            go_ws = first_ws
    elif current_ws == first_ws:
        move_ws = then_ws
        go_ws   = then_ws
    else:
        move_ws = first_ws
        go_ws   = first_ws

    if arguments.move:
        # move active window to chosen workspace and go there
        reply = ipc.command("move window workspace number {}, workspace number {}"
                            .format(move_ws, move_ws))
    else:
        # go to chosen workspace
        reply = ipc.command("workspace number {}".format(go_ws))
        
    # exit with non-zero status if assertion fails
    assert(reply[0].success)

