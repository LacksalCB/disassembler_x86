BEGIN {
    current_block = ""
    field = "^[a-zA-Z_0-9]" 
    FS = "[:=]"

}

$1 == "COUNT" {
    printf "#define COUNT %d  \n",$2
    next
}

$1 == "OPCODE_LENGTH" {
    printf "#define OPCODE_LENGTH %d  \n",$2
    next
}

/^[a-zA-Z_0-9]+:?+[0-9]*+\s+*\{/ {
    print ""
    current_block = $1
    sub(/\s.*/, "", current_block)
    printf "// %s:\n", current_block
    if (current_block == "FLAGS") {
        n = 0
    } else {
        printf "char %s[16] = {", current_block 
    }
    next
}

/^\}/ {
    current_block = ""
    next
}

# Special case
current_block == "FLAGS" {
    flag = $1
    sub(/^\$/, "", flag)
    sub(/^[ \t]+|[ \t]+$/, "", flag)
    if (flag == "F_NONE") {
        printf "#define %s  0\n", flag
    } else {
        printf "#define %s  1<<%d\n", flag, n++
    }
    next
}

# Rest of the fields should be key value pairs of binary number to label
current_block ~ field {
    val = $1
    label = $2
}


# If I see an l-val with $, treat it as a pointer to a new array
# Basically just means it isn't a terminal value
label ~ /^\s*\$/ {    
    sub(/^\s*\$*/, "", val)
    printf "%s, ", label
}

END {

}

