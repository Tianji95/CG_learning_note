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

### 十、物理系统的基础理论和算法

游戏中的物体分为几类：
1. 静态Actor：无法移动的例如墙，地板等
2. 动态actor：一些动态的物体，可以符合物理碰撞规律的，例如弹珠撞到了一个石头，石头会动。
3. trigger：当任何一个acter前面，其他的物体会作出相应的反应，例如自动门。
4. 反物理规律的：例如人在推箱子，但是推力没有设置好，箱子一下子就飞出去了。或者是有一些游戏机关，地板在不断的上下移动。让玩家上去。

Actor Shapes：
球（Spheres，用来表示球类物体，桌球等），胶囊（Capsules，用来表示人物）,盒子（Box，一些相对复杂的形体，例如城墙），凸包（Convex Meshes，一般用来表示石头等）、三角形（Triangle Mesh，一般用来表示房子，动态物体一般不适用这个，因为复杂度太高）、高度图（Height Field，一般用来表示地形）

一个物体一般都要设置质量或者密度。
还需要指定质心位置。
物理材质：一般会设置一个物体表面的摩擦力数值、以及弹性值

游戏世界中的力：重力、拉力、摩擦力、冲力（Impulse，例如爆炸产生的力）

物理引擎中，力可能是会频繁发生变化的，例如一个小球在做圆周运动，但是因为引擎是按时间更新的，且更新的时候不知道这个圆会做圆周运动，所以给的力经常会慢一拍，从而导致圆周运动不收敛，如下图所示：

[!euler method](./images/euler_method.png)
[!euler method](./images/forward_euler_method.png)

该问题的解决办法：隐式欧拉法：使用未来的力预测未来的位置，但是会导致能量衰减。在物理引擎里面能量衰减可解释（例如摩擦力）
[!euler method](./images/implicit_euler_method.png)

物理引擎更常用的方法是半隐式欧拉法
[!semi-implicit-euler-method](./images/semi-implicit-Euler-method.png)

刚体动力学：质量、速度、加速度、动量、
旋转R（使用旋转矩阵或者四元数表达）、
角速度（Angular Velocity）在物体的任意一不通过轴心的点，然后这个点有一个切向速度，把这个点和轴连在一起形成了一个方向，使用右手法则做一个叉积，就可以得到旋转轴的方向。这样可以同时表达旋转轴、切向速度和旋转方向，如下所示：
[!angular velocity](./images/angular_velocity.png)

角加速度（Angular Acceleration）角速度出于dt就是角加速度了。

转动惯量（Rotational Inertia）

角动量（Angular Momentum）角动量是守恒的，例如花样滑冰在旋转的时候，张开手转的慢。

力矩（Torque）：一个方向和叉乘一个力方向的轴，表示能够推动一个物体的力，通常在汽车中使用

碰撞检测：：

使用BVH快速检测碰撞
使用sort and sweep 来做碰撞检测，该方法优势在于只要把物体都提前排序以后，只移动少部分物体，效率会非常高。
[!broad phase sort and sweep](./images/broad%20phase%20sort%20and%20sweep.png)

物体求交：
圆、胶囊等求交相对简单
凸包的求交使用Minkowski 和和Minkowski difference来判断。Minkowski difference肯定会经过圆点，使用GJK算法找到圆点
另一种算法是通过判断是否存在一条边能把两个物体分开
[!separating axis](./images/separating%20axis%20theorem2d.png)

碰撞检测后的处理：添加一个新的力。

物理引擎会对世界中的物体做分组。从而不管那些不受影响的组，从而加速计算。

CCD（continue collision Detection）连续性碰撞检测，类似原神须弥的G点

物理引擎的deterministic：要求在不同的终端设备上，物理模拟出的效果是一致的，结果必须一致，否则会影响到玩家体验。包括浮点数误差等。

### 十一、物理系统高级应用

character controller：
主要是对角色做控制，一般把角色当成一个胶囊，或者两层胶囊（外层胶囊是防止角色距离墙壁太近导致看到墙壁后的东西。
再比如角色在走楼梯的时候，要提前将角色向上移动一个小距离
再比如角色在爬坡的时候，超过一定角度会先上去再掉下来。
再比如一个人在电梯上，当电梯下降的时候，人可能会发生抖动，这个时候的解决办法是把人和物体绑定到一起。

布娃娃系统（Ragdoll）：如果没有布娃娃系统 一个人在台阶上被打死，可能只播放骨骼动画，而没有掉下去的动画，上半身会悬空，闲得很假

布娃娃系统实现的方法就是把人体的主要骨骼标记出来，然后由TA对各个joint做约束。布娃娃系统是通过对各个joint做物理运算实现的。

在实际引擎中，一般是会把ragdoll和蒙皮动画放在一起做blending实现的。

衣料模拟：最开始的是通过骨骼来模拟和驱动衣料的动画。这样是比较快的。
另一种方法是通过ragdoll的方式，让几根骨骼驱动衣料，然后通过物理算骨骼的动画
效果最好的办法是基于mesh的衣料动画，physics mesh一般会比render mesh稀疏很多，然后render mesh 通过physics mesh 插值生成。而且physics mesh会有运动的约束权重，例如人身上的披风，靠近肩膀的地方能移动的范围会比较有限，而靠近下面的披风mesh约束会小很多。
衣料的物理模型实现是通过弹簧质点模拟实现的。他有两个力，一个是弹性产生的力，另外会有一个能量消耗力（damping）。
[!cloth solver](./images/clothsolver_mass-spring%20system.png)

verlet积分：是基于半隐式积分

目前布料的结算经常用的是Position Based Dynamic去结算，如下图所示：
[!position based dynamic](./images/cloth%20solver%20position%20based%20dynamics.png)

布料模拟的另一个问题是布料的自穿插。解决办法是使用最大速度的限制。或者把布料的物理模拟加厚


破坏系统：
在构建mesh 的时候就用hierarchy的方式把mesh分割成几个碎片，每一个碎片之间有一个connection，这个connection是有权重的，用来表示受到多大的力才会发生破裂。
voronoi算法：在一个地方随机撒一些种子，然后生成到每两个点相同距离的线。在生成碎片的时候，可以基于voronoi算法定义不同的策略。例如玻璃一般是从中心开始碎，而有的地方碎片会密一些。

载具系统：
一个车有四个轮子组成，每个轮子都是一个弹簧系统
车还有引擎作为动力，
轮子上有摩擦力
轮子上的悬挂有悬挂力

另外汽车上的重心对汽车的影响很大。重心靠前时就容易翻车。

坦克则需要模拟履带
飞机则需要空气动力学知识。

PBD和XPBD（把载具、衣料、碰撞等全都变成一种约束，用雅克比矩阵求解约束）
PBD的好处在于速度很快，而且效果比较好。


### 十二、游戏中的粒子和声音系统（特效）

粒子发射器：
会有很多发射器，每一个发射器发射粒子，所有的发射器打包叫做粒子系统。
每一个粒子都是会受到力的作用的，例如重力，风的阻力，粒子的运动轨迹受到粒子的速度，受力影响。
粒子还会有颜色的变化。
粒子也可以有碰撞检测，例如打铁花，掉落的铁屑落到地上会有弹落。

billboard 粒子：通过一系列有动画的billboard来模拟粒子，例如明日之后里面的火焰

mesh粒子：每一个粒子都是一个mesh，这样可以做出随机感很强的粒子。

Ribbon 粒子：粒子形成的是一条光带，例如光剑（魔法剑）挥剑的轨迹。这种会有一些粒子的时空滞留感。一般使用样条曲线插值来形成Ribbon粒子。

粒子渲染最大的问题：半透明问题。例如半透明物体的排序。例如按照粒子发射器做sort

渲染粒子的优化方法：将粒子渲染做降分辨率

GPU粒子：GPU很适合做粒子，因为粒子的数量庞大，同时计算简单。
[!GPU particles](./images/GPU_particles.png)
粒子有专门的一个数据机构，有一个alive Particles List还有一个dead list，每当一个粒子死亡，需要从alive list移到deadlist里面，而emiter每发射一个粒子，需要从deadlist把相应的粒子拿出来放到alive list里面，GPU每帧只需要对alive list里面的粒子做模拟即可。

粒子的计算跟视角无关，即使看不到的粒子也需要每帧计算，只是不需要绘制了而已。
GPU粒子可以把物理模拟，排序，视锥裁剪、depth buffer collision等都放到GPU上做。GPU上的粒子排序是使用的mergesort，这个是可以并发执行的。

大量的人也可以用粒子系统实现。使用Animated Particle Mesh，只不过在人物的粒子系统中，没有用物理模拟，而是用的动画状态机实现。还有鸟群，鱼群的实现。这里面使用SDF来辅助控制碰撞。

音效系统：
声音分为音调、音量和音色，音色本质是不同基波和不同幅度的音波叠加起来的效果，因此理论上人耳听不到的高频音也会对音色产生影响。
理论上声音的采样率只要大于两倍音频即可保证无损，但因为音色的影响，采样频率需要稍高一些，保证高频音也能采样到。

MP3只支持立体音，不支持5.1立体环绕音，而且Mp3有专利墙，所以大部分时候使用OGG格式。

声音的渲染系统：
listener的位置，速度还有朝向
声音的空间感：主要是利用两个耳朵接受到的声音大小，声音到达的时间差距，音色等
声音的power是音强的平方，

声音的衰弱：这个时候使用attenuation shape来模拟声音的衰弱，例如高音喇叭使用的是喇叭形的衰弱。小溪使用的是胶囊的衰弱场。

引擎还需要处理声波，在有障碍物时的声音

声音的混响：例如骑马通过一个桥洞。混响包含干音（就是直接的声音）、声音的回音，声音的拖尾。
多普勒效应：当声源在移动的时候，声音的效果是不一样的。
声音的采集：一般是在录音室中对整个空间的声场做采集
在3A大作里面，会设置很多个音源。来模拟环境音。

### 十三、引擎工具链基础

所见即所得的工具链
[!game engine runtime framework](./images/game_engine_runtime_framework.png)

引擎工具链的本质是要连接各个特色的人，把大家连接到一起协作，例如美术，策划，程序员连接到一起。

游戏引擎最开始的系统叫做GUI系统，GUI系统由immediate Mode，所见即所得的绘制。还有一种叫Retained Mode，将所有要绘制的东西放到类似commandbuffer里面，如果绘制内容不变的话则不改变commandbuffer。现在大部分都是Retained Mode GUI

Retained Mode GUI有很多设计方式，例如MVC，MVVM等，
其中MVC架构的思想是只能通过model来改变view，而view不能改变model，用户的操作只能通过controller来改变model。这种方式只有“单行线”，容易管理。
[!MVC](./images/MVC.png)

还有一个设计模式是MVP：就是把Controller编程persenter，作为中间者。
[!MVP](./images/MVP.png)

MVVM模型，把persenter换成viewmodel
只不过他中间用的是binding机制，view就只有XML，是艺术家来写的，viewmodel是把xml和model绑定到一起的作用,是一种数据转换。model拥有自己的数据。
[!MVVM](./images/mvvm.png)

在写工具的时候，最好不用自己写的GUI，而是使用现成的工具例如QT等。

数据的序列化：就是把当前的数据变成可以保存成文件的格式。或者可以变成可以网络传输的数据格式。例如存成txt或者json，都是序列化操作。
如果能存储成binary mode的话，文件的大小会小很多。

工具链还会遇到一个问题，就是数据会有很多重复，例如有十个一模一样的房子，但是会有一些微小的差别。这个时候，如果存十个binary就会有很多浪费。常用的技术是数据的继承，例如添加一个<ref>xxx.mat</ref>告诉引擎我们要从xxx.mat把所有的数据拷贝过来，然后再去新增别的数据即可。

如何做到资产版本的兼容性：例如新版本引擎可以导入老版本引擎的资产，或者老版本引擎可以导入新版本引擎的资产。
怎么处理：少的值新增一个默认值，多的值不处理。
或者给每一个资产定义一个单调递增的guid

工具链的鲁棒性：
1. 怎么做undo和redo
2. 工具crash了怎么办
使用命令设计模式，command需要原子化，然后把所有的command都保存到磁盘上。
每一个command都需要有revoke和invoke，还需要有一个uid
建议提前在工具链中集成command系统，这样可以使我们工具链非常干净。
command系统最主要的三个操作是add、delete和update。

如何处理工具链中各个资源格式不同的问题：
使用schema：将所有复杂的数据都拆分成一些“原子数据”，schema更像是一个分子式，是一个描述物体的格式，schema通常是一个xml，而且要有继承关系，例如军人的schema可以继承自人的schema。同时还需要能够相互reference数据。能够把数据关联在一起。

[!schema define](./images/schema_define.png)

工具还有一个能力就是要给不同的人给不同的View，例如不能给美术弧度，而是要给角度，要给调色板而不是RGB数值。

工具链还需要“所见即所得”具体的做法是直接复用引擎代码，例如下面的引擎架构
[!game tools framework](./images/game_tools_framework.png)

工具链还需要有“play in Editor”功能，就是直接在编辑器里面运行游戏。

工具链还需要有可扩展性，例如插件功能，可以让第三方开发者使用插件功能编写自己的插件。

工具链开发需要有很强的软件工程能力，以及非常熟悉游戏制作过程。

### 十四、引擎工具链高级概念和应用

World Editor
引擎需要支持多个viewport，因为可能会出现画中画。
同时世界编辑器还需要针对不同的美术去做选择性的编辑，例如把锁定某些东西不可编辑。让美术专注于自己的领域。
世界编辑器还需要Content Browser，需要可以浏览所有的资产，也可以检索资产。

鼠标选取功能：使用Ray Casting来实现物体选取。

Height Brush功能：能够通过笔刷来生成地形的高度。最好能够给美术提供一个能够自己导入笔刷的功能

instance Brush：能够通过刷子来快速的生成物体instance。

能够有环境系统，例如路上不能长树。这需要一套Rule System，然后使用这么一个Rule System使用PCG的方法生成出来。

World Editor还需要有插件系统，而且需要支持多个插件共存。还有plugin的版本系统。

游戏引擎的叙事系统：包含每个物体的时间线，然后多个物体的时间线放在一起会有动画或者叙事系统。

反射系统：就是能够让程序可以动态修改他的数据结构和方法。如下所示：
[!reflection](./images/reflection_demo.png)
C++如何实现反射：三步骤
1. 在代码里面确定需要反射的类型和成员变量等
2. 生成编译代码，提供accessors去获取函数和成员变量
3. 通过<string, accessor>的map去管理所有的accessors

现在我们的C++代码一定需要GPL去对编码做编译。例如Clang，获取他的AST（抽象语法树）。
[!reflection AST](./images/reflection_generate_schema_from_AST.png)
我们通过定义一些描述词，把希望做反射的数据结构描述一下，这个通过宏来实现。clang有一个__attribute__,它里面有一个叫annotate的关键字，他可以给我们的变量打标记，如下所示：
[!reflection attribute](./images/reflection_add_attribute.png)

visual script system：主要是解决编程语言的可扩展性。丢掉hard code method，可以动态增加方法。

code rendering：代码渲染：能够自动的把xml或者可视化图转换成代码，这种方式可以把代码和数据区分开。可以使用第三方插件Mustache

协同编辑：类似协同编辑文档。通过划分区域。或者asset分层等方式来实现。或者（one file per actor）但是这样会产生非常多的小文件。
协同编辑最终的状态是，每一个人可以实时看到别人的编辑结果，这其实是一个网络同步问题。这需要将对所有命令原子化。

引擎和DCC工具在相互抢工作，例如DCC工具想要做一些引擎runtime的事情，引擎想要做类似手K动画的功能。

### 十五、引擎的gameplay

gameplay是一个需要快速迭代的东西，引擎在做的时候，需要支持玩法快速迭代。
一般使用event来控制物体之间的交互，中间有很多细节，例如使用callback function来实现event系统，但是注册了callback以后，要保证owner不为空。
gameplay还需要消息分发机制。
event 在设计的时候会设计一个queue，然后在queue中对event做分类，然后通过batch保证event的并行化。（一个ring buffer）

event系统最好不要默认先后顺序，而是默认下一帧前保证所有的event都已经完成了。

游戏的hotfix：需要使用脚本来做热更新，C++较难使用热更新。并且脚本语言有利于开发效率，crash也不会影响到C++。例如lua、python等
hotfix的实现方式就是把脚本的函数指针换一下，这个时候要注意全局变量的处理。

可视化脚本（蓝图系统）：就是一个把float等类型编程不同颜色的点，然后连起来编程的一类系统。

3C系统（character、control、camera）
character系统就是人物和环境的交互，运动，动画等
control就是键盘、鼠标、各种外设的交互和控制。如何变得敏捷和丝滑。例如射击游戏瞄准时的吸附系统，还有手柄的反馈系统。
camera实际上是要和玩家绑定到一起，但是实际上相机并不是严格跟着玩家的，还是在不断移动和旋转，特别是2.5D游戏。有可能随着玩家的移动旋转的时候，相机旋转的浮动更大。

### 十六、引擎的基础AI

游戏中的AI导航系统：
1. 需要对世界有一个表达，例如map representation，
1.1 waypoint  network：把所有的岔路口放一些节点，这样会形成一个网络图。AI寻路的时候会首先寻找距离最近的节点或者路。这种方法的问题在于玩家总是会往路中间走。即使路很宽
1.2 grid：使用稠密的triangle、square、hexagon来表示地图，例如文明5就是用六边形表达。这种方法非常好实现，也容易debug，他的缺点是存储空间较为浪费。而且寻路的效率也是比较低的，而且无法寻路有层叠关系的路，例如桥上和桥下。
1.3 navigation mesh：把地图上所有可通行的区域用一个一个的polygon都连起来。相比较waypoint和grid，他对地图的覆盖是面覆盖代替点覆盖和线覆盖（可以是三角形也可以是别的凸多边形）。若果是凹多边形则有可能会经过一个不可行区域。但是这种方法不能做飞行系统的寻路。
1.4 sparse otree：空间寻路的实现需要使用sparse  voxel octree。但是他对存储空间要求比较高。

2. path finding：
寻路就是在representation形成的graph中寻找最短路径。dijkstra算法。但是dijkstra算法相对准确，实际寻路的时候不需要一个非常严格短的路径。这个时候就需要A\*算法。A\*算法是基于dijkstra算法衍生出来的寻路算法，他的思路是，在寻路的过程中不仅需要考虑已走的距离，还要考虑预测到目标点的距离，寻找一个两者相加和最小的点继续往前迭代。并且找到目标点后直接中断算法。从而达到加速的目的。

3. path smoothing：
只有1,2点会导致路径发生zigzag，会有些奇怪，要拐很多弯路，这里面可以使用烟囱算法来做smoothing（funnel algorithm），具体思路就是连着一个点，判断目标点是在该点烟囱的左边、右边还是中间，如果是中间的话就直接走直线，如果在左边的话，就照着最左边的路走。

4. steering：是为了解决一个问题，就是之前的寻路是立即生效的，但是遇到动作缓慢的物体，例如汽车拥有加速度和减速度，他在运行的时候无法走直线，实际走的是弧线。
常见的steering动作如下图所示：
[!steering behaviors](./images/steering_behaviors.png)

4.1 Seek Flee 追和逃，有一个目标点，追和逃 

4.2 VelocityMatch 在起步的时候要加速，快到目标点的时候要减速。这个时候需要根据距离目标点的距离反向算出加速度和速度。
4.3 align 朝向一致，例如一群鱼群，朝向要一致。

crowd系统，当有鱼群、人群、羊群的时候会有这么一个系统。微观方法上就是每个鱼都有斥力、引力和朝向，要保证鱼之间不会离得很远，也不会离得很近。同时鱼群的方向也要一致。在宏观上要有一个路径或者规则。
群体行为要避免碰撞（通过相互之间的斥力来解决，或者给每一个障碍物添加一个SDF，sdf越小斥力越大）

速度互斥算法：当发现对面来了物体，且速度会相撞，则会改变自己的速度。

behavior tree：在游戏中会有行为树来控制AI，行为树相比较状态机，拓扑结构更加清晰，行为树的例子如下：
[!behavior tree](./images/behavior%20tree.png)
[!behavior tree2](./images/behavior%20tree2.png)

Sequence:Sequence就是按顺序执行，例如先执行门是否关着，如果关着就开门，开门后开枪。

Selector：Selector就是依次选择执行，如下所示
[!behavior tree selector](./images/Behavior%20tree%20Selector.png)

Parallel:同时发生并执行的事情，例如一面开枪一面走路。

### 十七、引擎的高级AI

Hierarchical task network（HTN）
行为树的局限性是不直观。HTN是从任务目标出发，就像人类一样去做一个计划。是一种将大目标拆分成各个小目标，然后再拆分成更小的目标。
Sensor是为了从环境里面抓取状态
Property是每个玩家自己主观上对世界的感知
HTN Domain是一个任务系统
Planner是去做计划，但是这个计划很可能会随着World State变化而变化
Runner是执行计划
[!HTN_Framework](./images/HTN_Framework.png)

Task分为几种
Primitive  Task是包含Preconditions、Action和Effects三个部分的任务。
Compound Task 是在Primitive的基础上有一个任务执行的优先级，优先看A，如果A满足了就执行A，不满足然后再看B，举例如下所示

[!Compound Task](./images/compound_task.png)

相比较行为树，HTN就是把所有东西分成Task，planning就是去规划这些task，因为在执行task的时候可能会出现意料之外的情况，所以run的时候会动态改变task序列。叫做run时候的replan

HTN的执行效率比行为树高，因为不需要每次都从root节点tick，他是一个更High level的算法。HTN的缺点在于在做好plan 的时候，可能会因为策划的配置失误或者长时间的迭代，没有执行下一个任务。这个时候就需要程序员做一些静态检查工具来帮助完成


Goal-Oriented Action Planning（GOAP）基于目标的动作规划
[!GOAP](./images/GOAP.png)
只不过分为ACtionSet和Goal Set,
其中GoalSet中的Goal有Precondition和Priority，
和BT和HTN的区别在于，GOAP是把目标明确出来，而BT和HTN则是将目标隐含在自己的树状结构中
[!GOAP GoalSet](./images/GOAP_GoalSet.png)

GOAP的Action Set是有cost的，如下所示：
[!GOAP ActionSet](./images/GOAP_Actionset.png)

GOAP在执行的时候是以目标为主，倒叙思考，例如我要活着是个目标，然后我中毒了，我得喝解药这又是一个目标，然后为了达到喝解药这个目标要做哪些action。
然后Action Cost和state组成了一个图，distance是cost，Edge是Action，Node是Combination state
[!GOAP](./images/GOAP_State_Actino_Cost_Graph.png)

Monte Carlo Tree Search：
围棋就是使用了类似的方法，首先要把落子的可能性做数学建模，首先要把整个局面抽象成一个State，然后构建蒙特卡洛Tree，在下子以后重新构建一个蒙特卡洛Tree。
在模拟的过程中，会有一个Default Policy在里面，去判断本次落子的赢面大概有多大。然后选取一个赢面概率最大的节点，再去往下查找赢面。
在查找的时候，使用的方法是UCB（upper confidence Bounds），优先选择深度迭代，但是如果发现自己的探索广度小于父亲的一定比例，则需要优先广度搜索，如下所示：
[!upper_confidence_bounds](./images/upper_confidence_bounds.png)

Machine Learning ：
马尔科夫链基于概率的状态机来做。
强化学习中，经常先训练一个局部最优解，然后还要不断和别的训练结果左后互博，如下所示：
[!Reenforence](./images/train_renforce_learning_AI.PNG)

### 十八、网络游戏的基础架构

网络游戏遇到的挑战：延迟，丢包，掉线，外挂，同步，不同设备上的兼容性和一致性，多人并发度以及多人并发下的效率。
网络的七层结构：
[!OSI Model](./images/internet_OSI%20Model.PNG)

游戏中使用的网络协议一般是reliable UDP协议。因为TCP不是一个时间稳定的协议，而UDP虽然很快但是却不够可靠。
ARQ：实际上就是在UDP上使用滑动窗口协议，确认哪些包没有收到，如果没有收到就重新发一下包，保证不丢包。
[!xor](./images/online_game_xor.PNG)

reliable UDP其实就做了两件事，一个是使用滑动窗口协议保证不丢包，链接稳定，另一个是使用FEC保证丢包以后可以恢复。

RTT：时钟校准和时钟同步：
给服务器发一个包，然后让服务器再返回一个包，计算中间的时间差值。有一个NTP算法可以做这么一件事情。

在封装网络协议之上，使用RPC来给客户端程序员使用，更为方便。

游戏网络架构：

P2P架构：两个电脑联机到一起，或者一个网吧，可以两个人互为服务器，例如红警，w3等

Dedicated Server：有一个Host Server，会更稳定一些，并且需要大型商业服务器。

游戏同步技术

1. snapshot 同步：客户端只负责传输当前客户端的数据给服务器，服务器收集所有的数据再算，保证了大家数据的绝对一致性。因为很多快照，服务器会比较卡，所以一般服务器是10帧，客户端是30帧，剩余的帧都是插值出来的。同时因为有很多快照，数据量可能会比较大，所以一般会使用diff来存储快照。
snapshot的问题在于浪费了大量客户端的算力，而且对服务器的带宽需求较大。一般只有在局域网内使用。

2. 帧同步：锁步执行：所有的客户端都把数据汇总给服务器，然后由服务器将所有数据分发给各个客户端做计算，理论上各个客户端的计算结果应该是相同的，服务器只起到了转发数据的作用
2.1 initialization：在一开始进入游戏的时候，一定要确保各个玩家的数据都是相同的，例如王者荣耀进入游戏时候的init。
2.2 服务器分发数据给客户端，并且执行同步
[!deterministic lockstep](./images/Deterministic_lockstep.PNG)

但是这个图有个问题，就是一个人会卡着所有的人，（例如dota）一种优化方法就是每100ms就要收一次消息，像公交车一样，等不到也要出发，例如下面一张图：
[!bucket synchronization](./images/bucket_synchronization.PNG)
帧同步有一个关键点，就是游戏的确定性，就是物理引擎，vector等数学库，浮点精度一定要有一致性。

浮点数误差保证：使用分数
cos、sin等使用查表法。
game state也尽量弄成确定性的事情。
随机数的种子也要一致。

即使使用了帧同步，也会出现网络抖动和延迟的情况，这个时候一般客户端会cache一两帧服务器发来的数据。然后使用插值的方式做smooth。

快照策略：一般会把渲染帧和逻辑帧分开，然后本地会在拿到服务器数据后在本地做一个快照，然后在掉线10秒户重新从快照的地方将所有算力用来计算逻辑。部分游戏的服务器也会保存一些快照，保证玩家掉线后直接给玩家当前状态。游戏的观战和回放也会使用帧同步的快照技术。

帧同步的外挂问题：因为帧同步基本要保证每一个客户端都保存了所有玩家的所有状态，如果有战争迷雾的话，很容易写一些透视挂。

3. 状态同步：

大部分的MMORPG都用的状态同步。

不会把玩家所有的信息都传输给服务器，而是每个人只发送自己的一部分数据，而服务器是拥有所有玩家的状态的，在计算完以后会把一部分的数据（玩家的状态）发送个各个玩家。

Authorized：玩家自己的电脑，玩家只提出自己的动作
Server：会收集所有玩家的动作。然后计算结果后，将结果发送给各个玩家。
Replicated：在其他玩家的电脑模拟某个玩家。是一个玩家的复制。玩家的状态则是由服务端计算得到的。

状态同步通常会有一个延迟，因为需要把数据发送给server，然后server再发送到客户端，这个时候客户端会有一个overwatch，有一个预测。
[!state_sync_compare](./images/state_syc_compare.PNG)

### 十九、网络游戏的进阶架构

因为延迟导致的抖动：可以通过buffer和插值来修复，这种方法虽然可以让动作很平滑但是这种方法会导致延迟更高，而且这种延迟会导致碰撞游戏出现问题。

外插值可以解决碰撞游戏的问题。但是外插针需要预测未来的状态。
PVB算法：
[!Projective Velocity Blending](./images/Projective%20Velocity%20Blending.PNG)

[!Projective Velocity Blending2](./images/Projective%20Velocity%20Blending2.PNG)


一般符合物理学规律的游戏（例如战舰大战）使用外插针
一般比较敏捷，不符合物理规律的游戏使用内插针，也有把两个糅合在一起的用法。

射击游戏的射击判定一般放在客户端，因为可以避免网络延迟，这样会很容易写外挂。服务器端也做检测会检测外挂。

射击判断放在服务器端可能会有比较大的延迟，需要使用Lag Compensation（延迟补偿）解决：
把状态的时钟往回拨，根据服务器端的数据做猜测。这种方法会导致在竞技游戏中不公平。

动画前摇（可以给网络同步争取时间）

MMO的架构：
[!MMO_Architecture](./images/MMO_Architecture.PNG)

MMO链接服务器以前会先链接Login Server，然后会链接GateWay 要先保证账号是没问题的，其次保证不会被DDOS攻击。
Lobby：再经过GateWay后会进入游戏大厅，他相当于一个缓冲池。让所有用户都在这里等。
Character  Server：专门负责所有玩家的属性数据，例如发邮件，血量，装备等。
Trading System：安全性和原子性要求很高，
Social System：拉黑，拉小群聊天
游戏数据库会使用redis内存数据库
分布式服务器：主要是数据访问，网络的复杂性，数据的传输，数据的一致性。
负载均衡：要有哈希算法。

带宽优化：
浮点数优化，例如把xyz优化成x和y，double变成float，或者对游戏分区，然后再用定点数优化。
分区一般是把世界分成多个cell，或者使用十字链表。

反作弊：
1. 检查内存，抓用户的数据，把数据改了。单机游戏一般这么做，防止办法都是客户端加壳， 数据做混淆，
2. 修改本地存储文件，例如把一些材质修改成透明材质，这样就有透视挂了，一般对本地文件数值做哈希校验。
3. 网络包截获并修改。一般应对方法就是对网络包做加密，使用不对称加密算法一个公钥一个私钥，在最开始使用速度最慢但是最安全的ssh传输公钥。
4. AI作弊：比较难识别。


开放世界：
1. 把世界分成很多zoning，把世界分成几个小块，一般会用四叉树，一般每一个zoning会有一个自己的边界（border），数据的加载也会有一个延迟和阈值。只有超过一定阈值才会销毁另一个zone里面的数据。
2. instancing
3. replication：我将每一个character放到多个平行世界里面，分成多个层。例如天刀在押镖时候的不同服务器。

### 二十、面向数据编程和任务系统

worker架构：[!fork_join](./images/thread_fork_join.PNG)

Coroutine概念：相比较切换线程，Coroutine本质就是用了个调用栈，没有上下文切换，开销小得多，
[!coroutine](./images/coroutine.PNG)

Fiber-Based-Job-System
JobSystem：

OOP的问题：继承的深度通常会很深，而且很多操作可能既可以在父类里面做，也可以在子类中做。

[!performance](./images/performance_everything.PNG)



对Shadowmap做reproj，或者做culling
shadowmap本质是用视空间的精度对光空间的精度做采样。