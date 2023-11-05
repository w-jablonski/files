## .zshrc

umask 027

source /usr/share/zsh/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
source /usr/share/zsh/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh
source /usr/share/zsh/plugins/zsh-history-substring-search/zsh-history-substring-search.zsh
source /usr/share/fzf/completion.zsh
source /usr/share/fzf/key-bindings.zsh

HISTFILE=~/.histfile
HISTSIZE=10000
SAVEHIST=10000

setopt autocd extendedglob nomatch notify
unsetopt beep
bindkey -e
zstyle :compinstall filename '/home/V/.zshrc'
zstyle ':completion:*' rehash true
autoload -Uz compinit
compinit

bindkey '^[[A'     history-substring-search-up
bindkey '^[[B'     history-substring-search-down
bindkey "^[[H"     beginning-of-line
bindkey "^[[F"     end-of-line
bindkey "^H"       backward-kill-word
bindkey "^[[3;5~"  kill-word
bindkey '^[[3~'    delete-char
bindkey '^[[1;5D'  backward-word
bindkey '^[[1;5C'  forward-word

export HISTORY_SUBSTRING_SEARCH_FUZZY=1
setopt HIST_IGNORE_ALL_DUPS
setopt HIST_FIND_NO_DUPS
setopt share_history
setopt HIST_IGNORE_SPACE
setopt HIST_REDUCE_BLANKS

PS1='%(?..%K{red}%F{#ffffff} %? )%K{#ffffff}%F{black}    %5~ %k%F{#ffffff}%k%f '

export PATH="$HOME/.local/bin:$PATH"
export npm_config_prefix="$HOME/.local"
export MOZ_ENABLE_WAYLAND=1
export PAGER="most"
export EDITOR="micro"
export TERMINAL="/usr/bin/foot"
## export WAYLAND_DISPLAY=sway
## export DISPLAY=sway
export QT_QPA_PLATFORMTHEME=qt5ct
export FZF_DEFAULT_OPTS="--layout=default --height=100%"

alias see="bash -c set"
alias supa="sudo pacman"
alias supas="sudo pacman -Syu"
alias ls='ls -A --group-directories-first --color=auto'
alias e="mc -e"
alias mi="micro"
alias Q="exit"
alias clipboard="cliphist list | fzf | cliphist decode | wl-copy"
alias launcher="sway-launcher-desktop"
alias screenshot="sleep 5 && grim -c - | swappy -f -"

gsettings set org.gnome.desktop.interface gtk-theme 'Arc-Dark-solid'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'
gsettings set org.gnome.desktop.interface toolbar-icons-size 'small'
gsettings set org.gnome.desktop.interface overlay-scrolling false
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'

