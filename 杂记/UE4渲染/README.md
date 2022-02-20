学习系列文章：https://www.cnblogs.com/timlly/p/13512787.html#%E5%86%85%E5%AE%B9%E7%BA%B2%E7%9B%AE

内存模型

C++的多线程，条件变量，promise 和future，mutex等

future的使用方式：访问未来的值（另一个线程可能还没有保存某个值）

Vulkan同步机制：semaphore（信号）用于同步Queue；Fence（栅栏）用于同步GPU和CPU；Event（事件）和Barrier（屏障）用于同步Command Buffer。

unity的几种渲染模式：单线程渲染，多线程渲染jobified rendering，graphics jobs

寒霜引擎的framegraph，是为了隔离上层逻辑（renderer）和下层资源的（shader、renderContext、图形API等）从而开启多线程渲染。framegraph的每一个frame都有setup、compile和execute三个阶段，setup是创建各个renderpass的资源，包括输入纹理输出纹理等，compile就是剔除掉没有用的资源，做优化，例如在第一次使用的时候申请，最后一次使用时释放，

顽皮狗的引擎架构则引入了纤程（fiber）这是一种轻量级用户级线程，上下文切换十分快，一个work system会有6个worker threads，会有160个Fibers组成的fiber stack，还有会3个Job queues，除了IO线程以外的所有东西都是一个work



UE4里面的FRunnable和FRunnableThread是相辅相成的，缺一而不可，一个是运行的载体，一个是运行的内容。FRunnable是可以多线程运行的object，而FRunnableThread则是对应的线程。UE4会有一些平台相关的类继承自FRunnableThread，例如**FRunnableThreadWin** 和**FRunnableThreadPThread**：（POSIX Thread） 

UE4的QueueWork是执行队列化任务的接口，中间的任务会被FQueuedThreadPool执行。TAsyncQueuedWork不能独立的执行任务，需要依赖FQueuedThreadPool执行



TaskGraph（基于DAG的有依赖关系的并行任务系统），TaskGraph最终还是会交由RennableThread来执行具体任务。



UE4的渲染线程里面的command list是平台无关的抽象图形API，RHI线程才会把command list转换成指定的图形API调用，

RHI线程、渲染线程和逻辑线程中间的数据是各自拥有独一份的，然后通过消息发送更新另一个线程里面的数据。



UE4.22的渲染绘制流程有一层meshDrawCommand，用来做mesh的排序、缓存和drawcall合并等工作。



UE4的绘制流程：

可见性剔除→ primitiveSceneProxy →（把相同材质的物体通过材质和顶点指针hash放在一起，做LOD子模型放到collector里面，再由这个collector分配给batch，每一个meshbatch还会记录会给哪一个meshpass使用）FMeshBatch → （这个阶段会遍历每一个pass，然后利用passmeshprocessor对mesh做排序，合并等，生成绘制指令，并且合并drawcall，这里排序用的也是指针的哈希，然后把相近的绘制放在一起（材质、距离等），从而提升缓存命中率。这里面会收集所有绘制需要的资源绑定，例如VBIB，uniform，材质等drawcall相关的参数）MeshDrawCommand→（这里主要是并行做RHI，把任务都放到TaskGraph里面，缓存命令等）RHICommandList command →DrawIndexedPrimitive

