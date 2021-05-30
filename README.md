这个repo主要记录自己看到的一些知识点，防止以后忘记，都是一些概述性质的东西，建议想要系统学习知识的话，还是看相关的论文或者官方文档

**MeshShader**：

```
其实就是一个专门用来替代GS的,  用法和CS一样，比传统的GPU Driven的对三角形的裁剪、剔除优势的地方在于：不需要写入memory，直接在管线里面做了。以后做GPU Driven就再也不用先用CS做剔除，把剔除后的结果存放到 storage buffer 然后输入到VS里面了。meshshader可以直接搞定光栅化之前的所有流程。
MeshShader的具体流程大概是：先把原始的mesh分成多个meshlets（这个是需要预计算的，或者load模型的时候计算）（像GPU Driven里面的clusters簇）每一个meshlet其实都有自己的VB和IB（index是从0开始的）每一个meshshader线程组都会处理一个meshlet。然后每一个meshlet输入到task shader里面（可以跳过），taskshader用于提前剔除整个meshlets（cluster culling），或者做一些LOD和曲面细分的操作。然后taskshader会调用很多mesh shader。mesh shader里面的操作就和CS一样了，主要做三角形级别的裁剪剔除（包括视锥体裁剪、亚像素级裁剪等）。

一句话总结：：Meshshader最主要的贡献就是替代了GPU Driven的CS，并且不用写内存，高效的裁剪剔除避免了quad overdraw等overdraw。
参考链接：
http://meshshading.vzout.com/mesh_shading.pdf
https://developer.nvidia.com/blog/introduction-turing-mesh-shaders/
https://zhuanlan.zhihu.com/p/110404763
```

![1](./images/1.PNG)

**UE5的Nanite**

```
写到mesh shader就想稍微说一下UE5的Nanite，因为在源码没放出来之前，大家都以为Nanite是用mesh shader做的东西。事实上Nanite和Mesh shader还是有区别的，Nanite用的是CS软实现了一套mesh shader的功能（估计是为了跨平台），他是用GPU Driven的思路，用CS实现了对三角形的culling和VS的一部分，然后送入到管线下游。很显然这样做需要大量的存储空间存储模型数据，所以Nanite使用了流式加载、层级cluster（类似于LOD，这里用到了Quadric Error的简模算法）、数据量化压缩（因为三角形很小，压缩对画质影响不大）等方法。渲染时候通过屏幕投影确定cluster的层级，mesh使用了Strip(三角形流与顶点重排)，并且使用了莫顿码重拍保证空间邻近性，如下图。还用到了kd-tree来高效的culling(层级cluster只是为了LOD)。做完上面这些操作后，Nanite还做了一个混合光栅化，比较小的三角形走软件光栅化（防止quad overdraw），比较大的三角形走硬件光栅化。

代码是对着知乎看的，还没看完，详细的实现方式以后再补（flag）

一句话总结：Nanite用的是CS软实现了一套mesh shader的功能（估计是为了跨平台），他是用GPU Driven的思路，用CS实现了对三角形的culling和VS的一部分以及光栅化的一部分。
参考资料：
https://zhuanlan.zhihu.com/p/376267968
```

![2](./images/2.PNG)

**Bindless**

```
Bindless是说直接把buffer/texture等资源的虚拟地址存储在bindless buffer的功能，在shader中索引bindless就可以获取资源了。主要是解决绑定资源到管线的开销（状态切换开销太大了），减少drawcall数量的目的（利于合批）。下图中左边是原始的管线流程，右边是bindless

实际上在实现的时候，就是一个可以是任意类型的资源数组，在descriptor中不需要制定数组大小，也不需要指定资源类型。
参考链接：https://zhuanlan.zhihu.com/p/136449475
https://zhuanlan.zhihu.com/p/94468215
```

![3](./images/3.jpg)

**PCF**

```
percentage closer Filtering，是一种滤波方式，主要是用来解决阴影的锯齿感，他是对shadow map采样的结果做滤波（不是对shadow map做滤波）。具体做法是，对某个要渲染的点p，查找shadowmap，同时对这个点周围的所有点查找shadow，最后用平均结果代表这个点的可见性.当然滤波核越大，最后得到的阴影越软（虽然它本意并不是做软阴影）
```

![4](./images/4.PNG)

**PCSS**

```
和PCF不一样，PCSS是一种软阴影的实现方式。他的实现思路是基于距离的，如下图所示。1. 要搜索到所有遮挡物，并且找到在一定范围内所有遮挡物的平均深度。2. 通过相似三角形估计软阴影的范围。3.通过软阴影的范围来用不同的PCF滤波核，使用PCF来做滤波。
PCSS的思路就是，遮挡物距离阴影越近，阴影越硬，反之越远。
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![5](./images/5.PNG)

