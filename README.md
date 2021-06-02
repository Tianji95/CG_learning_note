



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

![21](./images/21.PNG)

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
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![7](./images/7.PNG)

![8](./images/8.PNG)

**Split Sum、环境光照**

```
环境光照实际上存了两个信息，一个是光照信息，另一个是BRDF信息，在渲染方程里面是光照信息和BRDF乘起来和visibility一起积分的。环境光照做了一个近似，把光照信息和BRDF拆开分别积分，并且存成两张图来计算光照。那么问题就简化成如何生成这两张图：
1. 光照信息只要一个入射光颜色，一个入射角就可以存成一个buffer。这个buffer可以是一个球因为做了近似，所以glossy BRDF的lobe是有一定方向的，我们就需要对这个buffer做prefilter，这样本质上就是在一个点上采样一个范围的颜色。如下图所示
2. BRDF有五维信息需要积分（菲涅尔项（反射率RGB，入射角度）、roughness），需要做的就是把里面的项拆出来，然后存成一个table。这里面应用了schlick的近似把反射率RGB三个通道拆成了一个R0和一个入射角度，这样把R0这个常数拆出来，整个BRDF就变成了入射角度和roughness的函数，把入射角度和roughness的积分预计算存成一个二维的texture，就把整个BRDF解决掉了。
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![10](./images/10.PNG)

![9](./images/9.PNG)

**SH、球谐光照、球谐函数**

```
球谐函数可以理解为一个高维的傅里叶变换。他有一组基函数，然后把我们计算出来的光照BRDF展开（投影）到基函数上，存储起来。SH有一些有意思的特征，包括旋转不变性。光照是可以旋转的
```

**PRT**

```
PRT在diffuse下：
diffuse BRDF项是比较低频的，而SH本身可以表示从低频到高频的信息，因此可以把BRDF投影到3阶SH上面（后面几阶描述的是高频信息，对于diffuse BRDF没有影响），而既然材质都是diffuse了，光照信息也可以不要高频信息。可以直接用SH来表示光照。
对于下图的渲染方程来说，环境光照的做法其实是把lighting项、visibility项和BRDF项都看成一个球面函数，然后对于每一个shading point把这三项乘起来，然后对四面八方的光线的贡献加权平均。这样做很耗时.PRT的做法是，把BRDF项（常数）拆出来，把光照用球谐基函数表示出来，带入到渲染方程当中，把基函数、visibility项和角度一起积分，就相当于是把后者又投影到基函数上，这样对于每一个shading point就可以存两个球谐函数系数的图，渲染的时候把这两个texture的值乘起来就好了。如下图所示

PRT假设光照是可以变化的，但是物体都是不能变化的。
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![12](./images/12.PNG)

![13](./images/13.PNG)

```
PRT在glossy下：
glossy下和diffuse下唯一不同的一点是BRDF不再是一个常数了，而是一个关于方向的函数，如下图所示，那么其实把之前Precompute的东西弄成一个矩阵就行了（原来是关于系数的一个向量）,那么原来的两个texture，可能一个texture存9个系数就行（3阶SH），变成了一个要存25个系数（5阶SH，glossy有高频信息，需要阶数更高），另一个要存625个系数（25*25，是一个矩阵），在实际渲染的时候，每一个shading point只需要把矩阵和向量乘一下就行了，如下图所示
因此PRT不是很适合做glossy材质，可以用小波变换来代替SH来表示glossy，但是小波变换不支持快速旋转
```

![14](./images/14.PNG)

![11](./images/11.PNG)

**RSM（reflection shadow map）**

```
把每一个shadow map上的像素都当成次级光源，并且把所有的反射物（次级光源当成diffuse的），然后用这些次级光源照亮shading point.当然shadowmap上需要存一些每个点的世界坐标，用来判断和shading point的距离。
这样其实就很简单了，查找所有shadowmap上距离shading point比较近的点，然后乘一个常数（diffuse）再做积分。把Visibility忽略掉，就得到了下图中白色的式子。每一个shading point直接就可以算出来颜色。
RSM和VPL的思想很相近。
缺点：漏光、没有可见性检测，认为每一个次级光源都是diffuse的，
优点：实现简单
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![15](./images/15.PNG)

**LPV、light propagation volumes**

```
1. 整体思路是把整个场景划分成多个格子，然后找到所有直接光照找到的表面，作为次级光源（可以用RSM实现）
2. 查找每一个格子都包含哪些次级光源，计算每一个格子向各个方向的radiance和，并且用SH表示
3. 根据第二步每一个格子的SH，向各个相邻格子传播，或者说对于每一个格子，查找他相邻格子传播过来的光照。同时用SH表示，一般迭代四五次就稳定了
4. 对于每一个shading point，查找他在哪个格子里面，用格子里面的radiance渲染这个shading point的间接光照
5. 格子可以使用层次结构加速

缺点：不考虑每个格子里面的遮挡，传播时候格子之间的遮挡等,如下图所示
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![16](./images/16.PNG)

**VXGI、SVOGI、voxel global illumination**

```
把场景分割成体素，组成层次结构，次级光源是体素而不是像素。
1. 决定了哪些voxel里面有次级光源，每一个voxel里面记录一个输入光照的分布，一个法线分布，每个hierarchy层都要更新
2. 在渲染的时候，我们从camera发射多条光线，对于任意一个pixel，我们知道camera ray的方向，还知道这个pixel的材质和法线，那么就能得到出射光线的cone，我们根据hierarchy的体素信息，把圆锥往前步进，收集所有次级光源对这个方向上的影响。最后渲染到shading point上。
缺点：开销太大，动态物体每帧都要体素花，开销太大
优点：质量好
参考文献：https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html
```

![17](./images/17.PNG)

![18](./images/18.PNG)

**SSDO/screenspace directional occlusion**

```
SSDO和SSAO的思路完全相反,SSAO认为光源是从很远的地方来，周围的地方会挡道光。SSDO则是考虑了来自近处的光照，远处的光照只考虑直接光照。其他的做法完全一样。
SSDO有一个假设，就是采样点能不能被shading point看到，取决于采样点能不能被摄像机看到。如果被挡住了，并没有简单的扔掉，而是计算这些被挡住的点对shading point的贡献
缺点：只在屏幕空间做，不看visibility
```

![19](./images/19.PNG)

**SSR、Screen Space Reflection、 Screen Space RayTracing**

```
SSR的思路很简单，就是利用GBuffer里面的normalbuffer和depthbuffer，从屏幕空间发出光线，射向各个glossy的表面，并且在知道法线分布和摄像机入射方向时，知道出射方向，利用depth的mipmap使用指数增强算法（1,2,4,8）向前trace（因为这个时候没有SDF）直到trace到屏幕上另一个点，用另一个点的渲染作为颜色。因为采样开销很大，所以会使用时间和空间上复用采样。如下图所示
```

![20](./images/20.PNG)

**GGX**

```
其实就是一个法线分布函数，如下图所示。他的特点在于他在边缘衰减比较慢，会有一个光晕的效果。在diffuse下会更亮一点
```

![22](./images/22.PNG)

**GTR**

```
GTR是一种更通用化的GGT，有一个参数可以调整，在很高的时候接近backmann分布，在比较低时为GGX发现分布模型,如下图所示
```

![23](./images/23.PNG)

**kulla-Conty近似**

```
在微表面模型中，微表面之间会相互遮挡，因此visibility项的存在会产生光线在微表面之间相互bounce，如果我们只bounce，就会导致能量损失，最后渲染出来的图会比较暗，kulla-conty就是对多次bounce的结果做近似
他的思想也很简单，先计算一次bounce的结果,把一次bounce的能量嘉和记为E(u0)，然后用1-E(u0)就表示损失的能量，使用一个系数来补充这些能量。这个系数的计算比较麻烦，需要存成一张texture，这些texture保存的就是这些系数（一个是u0，一个是roughness）。
对于有颜色的BRDF，会认为颜色本身就是对能量的吸收，最后会乘上一个颜色系数。
```

**LTC、Linearly Transformed Cosines**

```
线性变换的余弦，是为了解决在多边形光源下，如何做GGX法线分布的渲染。他的思想很简单，就是预计算出了一个transform，把所有点的BRDF转换成二维cos lobe，然后面光源会跟着这个变换做变换。这样就把所有shading point的BRDF都变化成一致的（原本不同的shading point的BRDF是各不相同的，这样给定任意一个面光源，不同的shading point都要做不同的积分，很麻烦），而现在BRDF就变成了一个固定的cos值，而面光源不变，这样就可以对面光源做积分。可以实现对于任意的BRDF，任意的面光源，得到最终的渲染值。具体的方法和解析解如下图
```

![24](./images/24.PNG)

![25](./images/25.PNG)

**Lumen简析**



**体积雾渲染**



