function handler(){
    callgrind_file=$(find . -maxdepth 1 -type f -iname "callgrind*")
    
    mv $callgrind_file debug/

    cd debug

    gprof2dot --format=callgrind --output=out.dot $callgrind_file
    dot -Tpng out.dot -o graph.png
    eog graph.png
}

trap handler INT
valgrind --tool=callgrind ./build/Mycraft




