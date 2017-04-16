#!/bin/bash
# headers.sh
# aoneill - 03/19/17

function get_border() {
  file=$1
  gcc -nostdinc -dNI -E "$file" 2>/dev/null \
    | sed -e "/^$/d" \
    | grep -v "# 1 " \
    | grep -A 1 "$file" \
    | grep define \
    | head -n 1 \
    | sed -e "s/^#define //"
}

function end_lnum() {
  border=$1

  if cat -n $file | grep -A 1 "#ifndef[ \t]\+$border$" \
       | grep "#define[ \t]\+$border$" &>/dev/null; then
    count=0
    cat -n $file \
      | grep "#ifn\?def\|#endif" \
      | while IFS= read -r line; do
        echo "$line" | grep "#if" &>/dev/null && count=$((count + 1))
        echo "$line" | grep "#endif" &>/dev/null && count=$((count - 1))
        [[ "$count" -le "0" ]] && echo "$line" && break
    done \
      | sed -e "s|^[ \t]\+\([0-9]\+\).*|\1|g"
  fi
}

function main() {
  file=$1
  prefix=$2

  border="$(get_border "$file")"
  if [[ "$border" ]]; then
    endif="$(end_lnum "$border")"
    if [[ "$endif" ]]; then
      tmp=$(mktemp)

      final="$(echo "$file" \
                 | sed -e "s|^${prefix}/||" \
                       -e "s|[/\.]|_|g" \
                 | tr '[:lower:]' '[:upper:]')"
      final="__${final}__"

      # Head parts
      head -n $((endif - 1)) $file \
        | sed -e "s|${border}|${final}|g" >> $tmp

      # Matching endif
      head -n $endif $file \
        | tail -n 1 \
        | sed -e "s|^\([ \t]*\)#endif.*|\1#endif /* ${final} */|g" >> $tmp

      tail -n +$((endif + 1)) $file \
        | sed -e "s|${border}|${final}|g" >> $tmp

      mv "$tmp" "$file"
    fi
  fi
}

main $@
