#!/bin/bash

function get_flag() {
    FLAG_FMT="CTF" # Modify this accordingly
    [[ -z $CHALLENGE_ID || -z $TEAM_ID ]] && (echo "CHALLENGE_ID or TEAM_ID is empty. Make sure that the variables are set properly." && exit 1)
    exec 3<>/dev/tcp/172.17.0.1/9512
    echo -en "GET /flag?chal_id=$CHALLENGE_ID&team_id=$TEAM_ID HTTP/1.1\nHost: $FLAG_ENDPOINT_HOST\n\n\n" >&3
    while IFS= read -r -u 3 line; do tmp=$(echo "$line" | grep -ioE "$FLAG_FMT{.*}"); if [[ $? == 0 ]]; then flag=$(echo $tmp); fi; done
    exec 3<&-
    [[ "$flag" == "" ]] && return 1 || echo $flag
}
rm -f /flag* /app/flag*

export FLAG_FILE="/app/flag.txt"
rm -f /flag* /app/flag*
get_flag > $FLAG_FILE
rm -- "$0"
/docker-entrypoint.sh