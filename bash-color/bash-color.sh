# this can be included into scripts
# then use Tcol, Tfg, Tbg, Treset to generate shell colour escape sequences

HAVE_TERMINAL=false
[ -t 0 ] && HAVE_TERMINAL=true

if ${HAVE_TERMINAL} ; then
        Tcolconv() {
                local base=0
                case $1 in
                        dark)    base=0;shift;;
                        bright)  base=8;shift;;
                esac
                local col=0
                case $1 in
                        black)   col=0;;
                        red)     col=1;;
                        green)   col=2;;
                        yellow)  col=3;;
                        blue)    col=4;;
                        maganta) col=5;;
                        cyan)    col=6;;
                        white)   col=7;;
                        *)       col="${1:-0}";;
                esac
                [ $col -lt 8 ] && col=$(( $col + $base ))
                echo $col
        }
        Tfg() { local col=$( Tcolconv $@ ) ; printf '\e[38;5;%dm' $col ; }
        Tbg() { local col=$( Tcolconv $@ ) ; printf '\e[48;5;%dm' $col ; }
        Tcol() { Tfg "$1" ; Tbg "$2" ; }

        Treset="$(printf '\e[m')"
        Tcol0="\r"
else
        Tfg() { :; }
        Tbg() { :; }
        Tcol() { :; }
        Treset=""
        Tcol0="\r"
fi

