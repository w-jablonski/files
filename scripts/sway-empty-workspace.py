#!/usr/bin/python3

# Using Sway IPC switch workspaces with mouse or keyboard binds

import i3ipc
from argparse import ArgumentParser

if __name__ == "__main__":

    arguments_parser = ArgumentParser() # called with mouse or keyboard binds
    arguments_parser.add_argument('-1', '--one', action='store_true')
    arguments_parser.add_argument('-2', '--two', action='store_true')
    arguments_parser.add_argument('-3', '--three', action='store_true')
    arguments_parser.add_argument('-4', '--four', action='store_true')
    arguments = arguments_parser.parse_args()
    assert(arguments.one or arguments.two or arguments.three or arguments.four) # a flag must be present

    ipc = i3ipc.Connection()
    tree = ipc.get_tree()
    current_workspace = tree.find_focused().workspace().num
    workspaces = tree.workspaces()
    workspace_numbers = [workspace.num for workspace in workspaces]

    if arguments.one:
        goto_workspace = 1
        then_workspace = 2
    elif arguments.two:
        goto_workspace = 3
        then_workspace = 4
    elif arguments.three:
        goto_workspace = 5
        then_workspace = 6
    elif arguments.four:
        goto_workspace = 7
        then_workspace = 8

    # Go to specified workspace. If already there, go to "second choice" workspace if it has windows
    if current_workspace != goto_workspace:
        reply = ipc.command("workspace number {}".format(goto_workspace))
    elif then_workspace in workspace_numbers:
        reply = ipc.command("workspace number {}".format(then_workspace))
    assert(reply[0].success) # exit with non-zero status if assertion fails
