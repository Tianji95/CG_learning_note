# 光线追踪精粹 笔记

原书来源：https://www.realtimerendering.com/raytracinggems/

### 一、光线追踪基础

**重要性采样**：

```
指使用不均匀分布的PDF（概率密度函数）采样来减少误差。
```

**准蒙特卡洛采样**：

```
使用数论方法的样本低差异模式代替传统的伪随机数生成器来创建随机样本的方法。
```

**光线的表示方式**：

P(t) = O + t**d**,其中O是空间中的一个点（光线射线的原点），**d**是光线的方向。一般**d**是归一化后的，这样t就是距离了。

**问**：t是距离带来什么好处？

答：这样可以用tmin和tmax表示光线前进的最近最远的值，方便光线停止。

DXR中的光鲜数据结构：

```
struct RayDesc {
    float3 origin;
    float3 Direction;
    float tmin;
    float tmax
}
```

