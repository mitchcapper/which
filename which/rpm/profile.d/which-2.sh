# Initialization script for bash and sh
function which() {
  (alias && declare -f) | /usr/bin/which --tty-only --read-alias --read-functions --show-dot --show-tilde $@
}
export -f which
