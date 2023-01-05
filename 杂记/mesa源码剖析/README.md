

关于mesa架构方面，[这篇文章讲的比较清楚](https://blog.csdn.net/HaoBBNuanMM/article/details/109054106)，[还有这个](https://zhuanlan.zhihu.com/p/398226773)，[还有这个](http://joyxu.github.io/2021/05/13/gpu05/)

关于mesa的代码结构方面，[这篇文章讲的比较清楚](https://winddoing.github.io/post/39ae47e2.html)

mesa的官网下载地址：https://archive.mesa3d.org/

后续做源码剖析会根据opengl和vulkan不同API类别分别做剖析

opengl重点文件：.\src\mesa\main,    .\src\mesa\state_tracker,会涉及到一些,\src\gallium

# opengl-mesa实现

所有的入口函数都是_mesa_xxxx,例如_mesa_BindBuffer、_mesa_BindBufferRange等。

### buffer

buffer入口 src\main\mesa\bufferobj 和 src\main\mesa\buffer

##### glGenBuffers和glCreateBuffers

注意这两个接口类似，但是他们的作用不太一样，他们都会调用create_buffers里面，这时会有一个dsa的标记，glCreateBuffers（dsa=true）会直接创建一个新的buffer，而glGenBuffers（dsa=false）则会创建一个虚假的buffer，把buffer创建延迟到glbindbuffer或者bufferdata（真正使用）的时候。然后调用new_gl_buffer_object申请一块新的buffer。

**流程：**添加锁——释放僵尸buffer——通过hash找到bufferid——调用new_gl_buffer_object——_mesa_bufferobj_alloc申请buffer——释放锁

##### glBindBuffer

入口函数_mesa_BindBuffer，调用到bind_buffer_object，

**流程：**查找当前绑定buffer是否重复绑定 —— 查找当前需要绑定的buffer —— 如果是虚假buffer则需要申请新buffer——调用_mesa_reference_buffer_object_做buffer指针赋值。切换到新buffer上

##### glBindBufferRange

入口函数_mesa_BindBufferRange——bind_buffer_range，该函数是把指定的buffer范围绑定到指定的GPU index上。



##### glBindBufferBase



##### glBufferData 和glBufferSubData



##### glClearBufferSubData 和glClearBufferData



##### glMapBufferRange和glMapBuffer



##### glUnmapBuffer



##### glInvalidateBufferSubData和glInvalidateBufferData



##### glCopyBufferSubData







### shader





### query





### texture





### framebuffer





### VertexArray





### VertexAttribute



### Transform Feedback





### Rasterization



# vulkan-mesa实现





