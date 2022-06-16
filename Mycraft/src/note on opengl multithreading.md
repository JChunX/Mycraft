note on opengl multithreading

- opengl calls need to happen on one thread (context is thread dependent)
- Should move everything that requires gl into one thread ie main
- currently a scene thread handles chunk loading, but its mesh deletion is troublesome (VBO, VAO destruction happens without context), causing segfaults and memory leaks
- should move it to the main thread, while maintaining chunk loading on a separate thread