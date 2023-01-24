做了很长时间的性能优化，想总结梳理一下移动端游戏性能剖析的一些方法，防止以后忘掉



做性能分析

工具是第一位的！！！！

工具是第一位的！！！！

工具是第一位的！！！！

重要的话说三遍，一个高效的，准确的工具胜过无数性能分析技巧。将多个工具组合使用通常比只一个工具然后死扣效率高得多。



**常用的工具有以下几种：**

android的systrace

renderdoc && mgd

锁频工具

[华为的Devecotesting](https://devecotesting.huawei.com/)

[华为的graphics profiler](https://developer.huawei.com/consumer/en/doc/development/Tools-Library/toolkit-download-0000001050819189)

高通的snapdragon profiler

simpleperf

苹果的XCode+



**常见的分析手法**

1. 初步定位：首先使用Devecotesting尝试分析手机的性能、功耗数据，获取应用运行的频点，功耗，性能。对应用有一个初步的了解。目前商用系统下，调度功能一般比较完好，频点+负载基本可以反应应用的计算负载。例如一个应用的GPU频点达到了600MHz，说明该应用的GPU负载较重。同理，如果CPU频点经常大于2.0GHz，也说明该应用的CPU负载较重。
2. CPU及性能定位：对于CPU负载较重，或者应用有帧率问题的，可以通过Android的systrace分析应用的CPU负载，查看各线程运行在哪一个核上，查看renderthread，swapbuffer的数据，查看掉帧原因。（同时可以通过锁频辅助查看是否是CPU导致的问题）
3. simpleperf可以进一步辅助确认CPU性能问题跟因，可以精确到函数级，如果是CPU问题导致渲染负载高，simpleperf可以协助解决问题。
4. GPU性能定位：对于GPU负载较重的，可以先通过renderdoc或者mgd分析应用的渲染指令流，对整个应用的渲染流程有一个大致的了解。一般drawcall数量大于500，CPU侧渲染负载就很重了，一般有15个以上renderpass，或者有体积渲染或者大量草地、树木、海水、粒子等大量半透明渲染的，或者有光线追踪的，GPU侧负载就很重了。GPU负载定位也可以使用锁频来辅助判断。
5. 使用graphics profiler或者高通的snapdragon profiler可以进一步辅助确认我们之前的判断，例如中间显示GPU cycles突然很高，或者overdraw很高，基本就能确定是GPU的瓶颈问题



**渲染管线瓶颈定位技巧**

| 瓶颈             | 定位方法                                                     | 解决方案                                                     |
| ---------------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| buffer带宽瓶颈   | 改变color和depth的位深度，32bits → 16 bits，或者修改GPU显存频率 | 优先渲染深度，减少alpha混合，尽可能关闭**深度写入**，从前到后渲染， |
| texture带宽瓶颈  | 把Mipmap的级别全改成0或者全改成最粗糙的级别                  | 减小texture尺寸、压缩纹理，使用更小的bits数量，使用mipmap    |
| PS、FS瓶颈       | 1. 先排除buffer带宽瓶颈，再改变分辨率，看看是否出现帧率变化。2.改变fragment shader（不能改变过多）防止原来不是瓶颈，改完以后变成瓶颈。3.改变GPU 频率 | early-z，backface-cull、避免使用半透明、使用显卡内部的原生纹理格式，纹理压缩，使用LUT代替计算，在VS里面 尽可能的算出来，避免normalize， |
| 顶点传输阶段瓶颈 | 调整顶点格式大小                                             | 压缩顶点格式，使用低精度数据，index使用16bit，连续存储数据，顶点排序 |
| VS阶段瓶颈定位   | 改变VS的复杂度（一般VS瓶颈说明三角形数量很多很小，并且cull掉了很多） | LOD，用CPU代替GPU运算，尽量提前break运算，                   |
| CPU瓶颈          | CPU频点调整                                                  | 合批操作，SIMD，尽量避免数据格式转换，避免除法、sin cos等三角函数，尽量内联，使用float代替double，避免虚函数，动态转换等 |
| CPU端的DDR瓶颈   | 先用color和depth的位深度排除buffer瓶颈，用Mipmap排除texture瓶颈，然后降低DDR频点， | 数据连续存储，ECS架构，避免使用指针等有跳转的数据结构，最好使用内存池，自己控制内存，尽量避免频繁释放申请内存，对数据使用不同的组织方式 |
|                  |                                                              |                                                              |







