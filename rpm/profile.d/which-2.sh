# Initialization script for bash and sh
which() {
  alias | /usr/bin/which --tty-only --read-alias --show-dot --show-tilde $@
}
export -f which
