case "$1" in

    "")
    echo "ax: Usage:   ax -c|-d|-l|-p [files]"
    ;;

    -c|--copy)
    shift
    echo > ~/.ax
    for arg in "$@"
    do
        echo "$(pwd)/$arg" >> ~/.ax
    done
    ;;
    
    -d|--delete)
    cat ~/.ax | xargs rm -rf
    shift
    ;;
    
    -l|--list)
    cat ~/.ax
    shift
    ;;
    
    -p|--paste)
    cat ~/.ax | xargs cp -r -t . 
    shift
    ;;
    
    
    *)
    
    ;;
esac
