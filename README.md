# opengl-learning

## 配置GLEW

```
$ sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev
$ git clone https://github.com/nigels-com/glew.git glew
$ cd audo && make
$ cd ../glew
$ cmake ./cmake
$ make && sudo make install
```


## 配置glfw3


```
$ sudo apt-get install libglfw3-dev
$ git clone https://github.com/glfw/glfw
$ cd build
$ cmake ../
$ make && sudo make install
```

## 配置glut

```
$ sudo apt-get install freeglut3-dev
#include <GL/glut.h>
````

