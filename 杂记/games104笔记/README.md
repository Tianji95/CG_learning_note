### 一、 games104的几个主要点

1. basic elements，讲清楚游戏有几层，怎么入手，都有哪些模块
2. 渲染系统，怎么把那么多渲染相关的texture、material、model拼在一起，偏工程化
3. 动画系统
4. 物理系统
5. gameplay，事件系统、脚本系统、怎么做图形化
6. 特效系统，例如烟等、AI系统、相机系统
7. 工具链，例如反射、数据格式（数据格式兼容等）
8. 网络同步（状态同步、帧同步）
9. 相对前沿的概念（Motion Matching、PCG、DOP（面向数据的编程）、job system、luman、nanite等）

游戏引擎和工业软件例如CAD、建筑软件等有很多共通点，但是每个引擎都会根据自己的需求做定制
游戏引擎仍然是跑的最远的，因为在商业上被充分竞争。

### 二、游戏引擎架构
工具层、功能层（动画、绘制、物理、脚本等）、资源层、核心层（工具箱）、平台层（主要是做跨平台的，输入设备（鼠标手表等），操作系统上，硬件平台等）、第三方组件层（Physx、音乐库等）


资源层会把所有的格式都转换成引擎自己的格式，这种格式通常是GPU高效的，也会把那些无效的信息给扔掉。
现代游戏引擎中，最核心的功能是数据资源之间的关联（reference）
GUID:每一个资产的唯一识别号。
handle系统：是每一个资产的管理，看这个资源还是否存在，
GC系统：基本上掉帧的大部分原因都是GC
资源加载和卸载：在游戏过程中，资源是实时的加载和卸载的。
延迟加载：先加载一些模糊的贴图，粗糙的物体，等一阵子再加载细节较多的资源。
tick：每帧都需要更新的内容：一般先ticklogic，再去tickrender。
function层（功能层）：并行计算
core层：数学库，矩阵加减乘除、数据结构（数组、链表、二叉树等，这一部分要引擎自己写，如果不自己写会产生很多内存碎片）、内存管理
CPU的缓存经常是游戏的卡点（想起来苹果的CPU缓存就比较大）
平台层：不同平台的路径（反斜杠，回车符）等都不同、mac上用的是metal，windows用的是dx等，要用RHI给他全屏蔽掉。硬件架构不同，如果想要把各个平台的特性都用起来，也是一个很麻烦的
越底层的代码越稳定，稳定以后可能几年都不会改，例如platform和core层

### 三、如何构建游戏世界

物体分为动态物、静态物、还有环境
也可以分为属性和行为。

一开始游戏引擎是面向对象的游戏引擎，但是有些物体是较难区分福字关系的，例如军舰继承自船，坦克继承自车，但是水陆两栖登陆艇就很难继承了。因此就有了ECS，是Component Base的思想，把枪拆成多个组件，例如消音器、瞄准镜等，也可以把AI模块、模型模块、物理模块、旋转变换等都变成组件。
[!商业引擎的component](./images/components_in_unity_and_unreal.png)

游戏世界由GameObject组成，而每一个GO都是由Components组成

event机制就是观察者模式，当事件发生时，告知所有注册过的component

世界场景分为多个cell，划分了很多格子，当场景分布不均匀时，格子也是不均匀的。而且格子是一种二叉树式的hierarchy式的管理方式。

event系统需要保证消息发送是又先后顺序的，因此需要有一个“邮局系统”，所有的消息都会发送到这个邮局，然后再由邮局系统第二天发送给目标object

1. everything is an object
2. gameobject由component组成
3. gameobject的状态是通过tick loop来更新的
4. game object和其他对象交互的方式是通过event机制（观察者模式）
5. gameobject是通过hierarchy cell来管理的。

如果tick时间过长怎么办：一般会用deferred tick，把爆炸的计算分散到各个帧里面算，做延迟tick。
tick时渲染线程和逻辑线程怎么同步：tickrender主要做数据准备，ticklogic会比tickrender早一些。

component的缺点：如果只用组件的话效率会比较低，所以得把所有的component放在一起tick。另外component之间也需要消息通讯。

event怎么调试：不停地加log。。。。
动画和物理相互影响时怎么处理：一开始用动画差值的方式，后面使用物理计算。因为物理计算相对复杂。动画插值相对快速，这样能保证效率和效果都还不错。

### 四、游戏引擎的渲染实践

游戏渲染的几个挑战：
1. 游戏场景是十分复杂的。
2. 需要对现代的硬件架构十分熟悉，硬件架构
3. 越来越高的帧率要求和分辨率，同时要求较快的绘制速度。
4. CPU、带宽和内存的限制

渲染的outline：
[!渲染的outline](./images/render_outline.png)

texture sampling, 绘制一个像素点，需要采样两层mipmap，每个mipmap采样4个像素点。

SIMD和SIMT：SIMD是单指令多数据，例如计算vec4的加减法
SIMT是单指令多线程，例如计算100次vec4的加减乘除，重要的是100次

为什么每个顶点都要有自己的normal：因为形状边缘在插值的时候，可能会出现法线偏移的问题。

renderable里面有mesh、material、texture和shader，都是数据

而每个绘制物体（例如人）可能会用到很多材质，头的材质和身体的材质可能不同，因此引入了submesh的概念，每一个submesh都拥有自己的材质，存储的是index和vertex、material的offset。
为了节省空间，引擎使用的是pool，例如material pool，然后每个submesh存储的都是pool中的索引。

因为切换material很费时间，所以游戏引擎会sort 所有的材质，把相同材质放在一起绘制。

visibility culling是对每一个物体计算BBOX或者OBB，然后使用BVH加速结构，做层级剔除。因为BVH对动态物体比较友好，更新较快。

目前模型的表达正在从model变成cluster-based mesh pipeline

radiance：辐射度，辐射出去的能量
irradiance：入射的能量

shader管理：游戏引擎的shader数量会非常多，经常会有很多ubershader，然后通过uber生成更多的可执行shader。同时ubershader也会是跨平台的。

lightmap是给整个场景的光照pack到一张大的atlas中，这个lightmap包含整个场景的SH所以通常会比较大。
而lightprob则是在lightmap上做进一步的改进，每一个lightprob都存着周围的光照信息。

而IBL则是给整个环境一个光照图，中间有一些diffuse的光照，也有一些specular的光照结果。


PBR MR模型：相对更容易理解
PBR SG模型：相对更灵活 

### 六、大气、地形和云

地形渲染：使用heightfield（等高图），然后用网格（meshgred）叠加高度图计算生成地形。但是这种方法很难

地形的T Junction：当相邻的两个mesh，其中有一个mesh的LOD高一些，另一个LOD低一些，导致两个mesh相接处无法紧密结合时，就会出现T Junction，解决这个的办法只能是把另一块mesh也细分成和另一个相同的三角形、或者把比较细的那块mesh stishing到另一个mesh上，形成退化三角形。

使用quad tree来表示地形（而不是triangle的方法）


另一种表示地形的方法是使用纯三角形表达，因为quad tree的三角形数量太多了，而使用三角形则会使顶点数少一个数量级。

地形通常要和hull shader、domain shader等相关，现在升级成了mesh shader

地形通常会用很多个texture，用很多个material，然后通过一个系数做插值，常用的方法是根据不同texture的heightmap来做插值，控制插值的系数。
有时候通常使用视差贴图来保证凹凸感。
目前地形渲染已经使用virtual texture一统天下了，VT如果能喝DMA或者DirectStorage结合起来的话，texture就可以不经过CPU和内存，直接加载到GPU然后解压到显存

另外整个场景在绘制的时候会遇到浮点数精度误差，导致会出现两个相距很近的物体z值会相互打架。好的解决办法是，把物体用“相对于摄像机的距离”进行渲染。

树和草的渲染：使用billboard再加上decorator，让树和草显示出多样性。

一般decals也直接bake到VT上面了。

体积云：使用一张weather texture，表示云的厚度，再添加noise function，例如perlin noise或者worley noise，这两个加起来就能做体积云了。然后使用raymarching的方法，去判断是否已经git到云，直到hit到云，以及什么时候走出云，然后在做云里面的累计。

### 七、游戏中的渲染管线、后处理和其他的一切

AO的常用的算法：SSAO、HBAO、GTAO和RTAO

HeightFog和depth fog
现代游戏引擎用的是Voxel based Volumetric Fog。
fog 3d texture的分辨率是160x90，是因为屏幕分辨率是16x9

走样（Aliasing）分为好几种，一个是锯齿，另一个是高光的闪烁，还有就是texture采样精度不足导致的纹理扭曲。所有的Aliasing都是采样率不足导致的。应对方法是超采样。


bloom：解决的是光晕的问题
[!bloom](./images/bloom.png)
tone Mapping：是为了解决曝光过亮或者曝光过暗的问题。相当于把HDR映射到LDR区域。ACES则是一种调节过的tone mapping的曲线，效果更好，

color grading：有点像滤镜，通常使用lut实现，把颜色从一个颜色映射到另一种颜色。这个是提升游戏质量，效果最重要的后处理方式。
[!color grading](./images/color%20grading.png)


frame graph和unity的SRP一样，都是用来显示各个renderpass的前后依赖关系，从而让大家知道整个渲染流程是怎样的，有利于debug

[!ue5_render pipeline](./images/ue_render_pipeline.png)

### 八、游戏引擎的动画系统

视觉残留现象是游戏动画的基础。
2D动画：就是一些图片拼接起来，如果想要流畅，则可以使用2d录制3d动画，然后抠图抠出来。

动画的挑战：动画的自然感、动画的计算效率、动画需要和玩家及时交互。

live2D：通过把一个人物拆分成多个组件，例如头发，眼睛，衣服等，然后把所有的图元生成一些控制网格，通过编辑这些控制网格来编辑keyframe动画。

3D动画：顶点动画：把每一个顶点随着时间的变化存成一个动画，通常用于飘荡的旗帜等效果。这些动画数据一般存在一个texture上。叫做vertex animation texture。

morph target animation：一般用于人脸的动画，一般蒙皮动画用在人脸上会很不自然，morph target animation 有几个关键点权重，然后通过key poses来插值。

2D skinned animation：类似3d，把2D也通过骨骼来驱动

physics based animation：通过物理模拟动画中的衣料、IK等。

为什么不用T-Pose：因为Tpose会导致肩的地方精度不够，收到挤压

欧拉角的问题：必须要有xyz的旋转顺序

欧拉角导致的万向节：可以用来保证方向是稳定的。但是会导致坐标轴退化（x和y轴共轴），另外是很难插值，真实世界也不会绕着xyz轴旋转，会沿着任意轴旋转。

四元数只能在三维空间中使用，可以使用群伦证明

动画压缩：因为每一帧都需要存储每一个joint的scale、translation和rotation，如果游戏里有很多动画，则需要的存储空间是非常大的。这样是不可接受的，可行的办法就是，把scale都存成1，trans也都把不变的给删掉。
另一种办法就是，只存关键帧，计算关键帧插值出来的动画和原始动画做比较，如果小于一定阈值，则保存这个关键帧，这样保存出来的关键帧是非均匀的。

catmull-Rom 曲线：通过四个控制点，决定P1和P2之间的曲线，如下图所示：
[!catmull-Rom曲线](./images/catmull-Rom%E6%9B%B2%E7%BA%BF.png)
动画压缩也可以通过缩减浮点数，使用16bit整数来表示浮点数。

但是动画压缩有一个问题，就是会导致末端骨骼的精度不够，所以需要用一个error Compensation来评估动画对精度的敏感度。

### 九、高级动画技术

动画状态机：action state machine，动画之间的切换需要使用插值，插值的方式有很多种插值曲线。不同的插值曲线的效果是不同的。

layered ASM：角色的上半身有一套状态机，角色的中间用一套状态机，下半身也有一套状态机。中间是分开的。而现代游戏引擎使用的是动画树（Animation Blend Tree)
[!动画树](./images/action%20blend%20tree.png)
动画树就是layered ASM的超集，可以把所有的节点都按照树形结构结合在一起。UE的动画树是做的相对比较好的。

IK：其实就是一个稳定三角形，计算膝盖处的位置，如下图所示：
[!two bone IK](./images/two_bone_IK.png)
但是该方法求得的解空间是一个圆，所以需要美术给一个方向，最后的IK是在这个方向上得到最终的解。

Multi-Joint IK：相对比较难的，因为有无穷多解，需要依次做下面几个事情：
1. 先确定所有的骨骼长度能否达到目标点
2. 确定所有的骨骼折了以后是否过长而无法达到目标点，如下所示
[!multi-joint IK limit](./images/multi_joint_IK_liminate.png)
3. 根据人体每根骨骼的旋转约束，来保证IK的结算是自然的。
Heuristics Algorithm IK（例如CCD IK）：具体思路就是从最末端节点沿着目标点做旋转，然后不断迭代。
但是只纯IK会有问题，可能最末端的骨骼弯曲的很严重，但是前面的骨骼还没弯曲就已经达到最后的结果了。所以IK最开始的时候会有一个圆圈确定大致范围，先做一次迭代，然后逐渐缩小圆圈半径，如下所示
[!CCD_IK](./images/CCD_IK.png)

另一种CCD_IK的约束方法就是每一个骨骼都有一个最大旋转角度，保证旋转的角度不会过于畸形。

FABRIKF：是一种通过位移来最终达到效果的算法。该方法就是说，要先把末端骨骼直接通过平移和旋转挪到目标点上，然后再依次向上迭代。然后再把整个骨骼链反过来，先把头端骨骼连接到之前的骨骼圆点，再依次向末端骨骼移动迭代。反复迭代几次最终达到效果。
FABRIKF也会出现骨骼约束的问题。也是会通过旋转角或者平移范围来做约束。

IK存在多个末端节点：使用jacobian矩阵解决。

IK目前存在的问题：
1. 自碰撞和自冲突
2. IK无法提前预知，例如在遇到门或者小洞时，人需要提前蹲下，IK无法做到
3. IK无法做到人体自然的动作，

整个动画管线如下所示：
animation pipeline：
[!animation pipeline](./images/animation_pipeline.png)

Facial Action Unit：把人的表情归类到46类表情，因为很多表情是对称的，所以到最后归纳为28个常用的AU。然后通过blending来表示动画。真实的AU保存的是一些表情某个区域相对于neutral pose的区别。从而可以对面部做局部控制

面部表情一般是通过morth target animation + 蒙皮动画
蒙皮动画用来做眼睛移动等动作，morth target animation用来模拟肌肉，或者通过UV来表示动画。


