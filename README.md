

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



**VSM、VSSM**

```
Variance Soft Shadow Mapping，是一种对PCSS的近似，速度更快。他的思想也很简单，是为了简化遮挡物距离的计算开销，并且简化PCSS中第三步PCF的计算开销。VSM存了两个buffer，一个是depth buffer，另一个是depth*depth的buffer，这两个buffer可以存成SAT（summed area table）。
1. 计算一定范围内遮挡物平均距离：假设在一个filter的blocker里，所有大于t的值都等于t，计算所有大于shading point（t）的平均距离，再用1-P(x>t)得到小于t的遮挡物的平均距离，计算开销O(1),如下图所示
2. 和PCSS一样，根据遮挡物平均距离计算出filter范围的大小
3. 因为我们有任意矩形的depth*depth和depth的SAT，所以可以通过这两个buffer算出任意矩形中的depth均值μ和方差σ，又知道采样点t的深度，所以可以直接用切比雪夫不等式P(x>t)≤(σ*σ)/(σ*σ+(t-μ)*(t-μ))计算大于采样点深度t的概率，用这个概率作为采样点的值。计算复杂度O(1)

因为VSM做了很多假设，所以VSM有一定的局限性，在遇到深度分布不均匀的时候（不够正态分布），就会出现错误。
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![6](./images/6.PNG)



**Moment Shadow Map、MSM**

```
MSM是VSM的一种效果上的改进，他采用了depth/depth2/depth3/depth4四个buffer，来做方差和均值计算，事实上VSM是只用了depth和depth2的特殊的MSM。但是MSM的存储消耗更大
```

**SDF、Distance Field、有向距离场、Distance Functions**

```
SDF表示的是距离这个点最近的物体边缘的距离，
1. 可以用来做两个物体的blending
2. 可以做ray matching（步进的时候知道所在点的距离场值，就说明在这个值内没有物体，可以放心往前走）
3. 可以计算某个方向的遮挡角度，如下图所示,从而衍生出了基于SDF的软阴影。基于SDF的软阴影采用了系数k对arcsin的近似，k值越大，阴影越硬（θ角小，sin值小）
```

![7](./images/7.PNG)

![8](./images/8.PNG)

**Split Sum、环境光照**

```
环境光照实际上存了两个信息，一个是光照信息，另一个是BRDF信息，在渲染方程里面是光照信息和BRDF乘起来和visibility一起积分的。环境光照做了一个近似，把光照信息和BRDF拆开分别积分，并且存成两张图来计算光照。那么问题就简化成如何生成这两张图：
1. 光照信息只要一个入射光颜色，一个入射角就可以存成一个buffer。这个buffer可以是一个球因为做了近似，所以glossy BRDF的lobe是有一定方向的，我们就需要对这个buffer做prefilter，这样本质上就是在一个点上采样一个范围的颜色。如下图所示
2. BRDF有五维信息需要积分（菲涅尔项（反射率RGB，入射角度）、roughness），需要做的就是把里面的项拆出来，然后存成一个table。这里面应用了schlick的近似把反射率RGB三个通道拆成了一个R0和一个入射角度，这样把R0这个常数拆出来，整个BRDF就变成了入射角度和roughness的函数，把入射角度和roughness的积分预计算存成一个二维的texture，就把整个BRDF解决掉了。
```





![10](./images/10.PNG)

![9](./images/9.PNG)
