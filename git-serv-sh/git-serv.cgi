#!/bin/sh
#
# Bart Trojanowski <bart@jukie.net>
#
# This here is a poor-man's cgi script that serves contents of a
# git repo.  It's written using posix sh so it's should be 
# easy to deploy.
#
# Assuming the following system layout...
#
#    /home/git
#    ├── examples.git         <-- project(s) to serve
#    └── git-serv
#        └── git-serv.cgi
#
# lighttpd config:
#
#    # make sure the directories of interest is handled by git-serv.cgi
#    url.rewrite-once = (
#        "^(/examples(/.*)?)$" => "/git-serv/git-serv.cgi?q=$1",
#    )
#
#    $HTTP["url"] =~ "^/git-serv/" {
#        server.document-root     =   "/home/git/"
#        index-file.names         = ( "/home/git/git-serv/git-serv.cgi" )
#        cgi.assign               = ( "/home/git/git-serv/git-serv.cgi" => "" )
#    }       
#

TOP=/home/git/

# request looks like /examples/path/file
req="${REQUEST_URI##/}"

proj="${req%%/*}"   # first word is project name
path="${req#*/}"    # rest is path in project
path="${path##/}"   # no leading slashes
path="${path%%/}"   # to trailing slashes

# verify we have a project by this name
if ! cd "/home/git/$proj.git" ; then
        if ! cd "/home/git/$proj/.git" ; then
                if ! cd "/home/git/$proj" ; then
                        echo "404 - No Such Project '$proj'"
                        echo 1
                fi
        fi
fi

# this function sets the global 'type' and 'hash' variables
# that represent the given path
get_type_and_hash_of () {
        if [ -n "$1" ] ; then
                info=$(git --bare ls-tree HEAD "./$1")
                rest="${info#* }"   # skip first word
                type="${rest% *}"   # type = next word
                rest="${rest#* }"   # skip next word
                hash="${rest%	*}" # hash = next word
        else
                type=tree
                hash=
        fi
}

# get node type (blob or tree) and its hash
get_type_and_hash_of "$path"

case "$type" in
        tree)
                echo "<head><title>$REQUEST_URI</title></head>"
                echo "<html><body>"
                echo "In /$proj/<b>$path</b> ...<br><br>"
                if [ -n "$path" ] ; then
                        here="/$path"
                        upone="${here%/*}"
                        echo "<a href=/$proj/$upone>&lt;&lt;Parent directory</a><br>"
                fi
                echo "<table>"
                git --bare ls-tree --long HEAD "./$path/" \
                | while read mode type ref size path ; do
                        name="${path##*/}"
                        case "$type" in
                        tree)
                                echo "<tr><td><a href=/examples/$path/>$name/</a><br></td></tr>"
                                ;;
                        blob)
                                echo "<tr><td><a href=/examples/$path>$name</a></td><td align=right>$size<br></td></tr>"
                                ;;
                        esac
                done
                echo "</table>"

                # maybe there is a README?
                get_type_and_hash_of "$path/README"
                if [ -n "$hash" -a "$type" = "blob" ] ; then
                        echo "<br><hr>"
                        echo "<pre>"
                        git --bare cat-file blob "$hash"
                        echo "</pre>"
                fi

                echo "</body></html>"
                ;;
        blob)
                exec git --bare cat-file blob "$hash"
                ;;
        *)
                echo "404 - Not Found"
                ;;
esac
