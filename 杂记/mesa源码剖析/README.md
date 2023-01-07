

关于mesa架构方面，[这篇文章讲的比较清楚](https://blog.csdn.net/HaoBBNuanMM/article/details/109054106)，[还有这个](https://zhuanlan.zhihu.com/p/398226773)，[还有这个](http://joyxu.github.io/2021/05/13/gpu05/)

关于mesa的代码结构方面，[这篇文章讲的比较清楚](https://winddoing.github.io/post/39ae47e2.html)

mesa的官网下载地址：https://archive.mesa3d.org/

后续做源码剖析会根据opengl和vulkan不同API类别分别做剖析

opengl重点文件：.\src\mesa\main,    .\src\mesa\state_tracker,会涉及到一些,\src\gallium

# opengl-mesa实现

所有的入口函数都是_mesa_xxxx,例如_mesa_BindBuffer、_mesa_BindBufferRange等。

当前进度4/136

### buffer（共15个接口）

buffer入口 src\main\mesa\bufferobj 和 src\main\mesa\buffer

##### glGenBuffers和glCreateBuffers

注意这两个接口类似，但是他们的作用不太一样，他们都会调用create_buffers里面，这时会有一个dsa的标记，glCreateBuffers（dsa=true）会直接创建一个新的buffer，而glGenBuffers（dsa=false）则会创建一个虚假的buffer，把buffer创建延迟到glbindbuffer或者bufferdata（真正使用）的时候。然后调用new_gl_buffer_object申请一块新的buffer。

**流程：**添加锁——释放僵尸buffer——通过hash找到bufferid——调用new_gl_buffer_object——_mesa_bufferobj_alloc申请buffer——释放锁

##### glBindBuffer

入口函数_mesa_BindBuffer，调用到bind_buffer_object，

**流程：**查找当前绑定buffer是否重复绑定 —— 查找当前需要绑定的buffer —— 如果是虚假buffer则需要申请新buffer——调用_mesa_reference_buffer_object_做buffer指针赋值。切换到新buffer上

##### glBindBufferRange

入口函数_mesa_BindBufferRange——bind_buffer_range，该函数是把指定的buffer范围绑定到指定的GPU index上。

他会根据target的不同，通过switch到别的接口，如下所示：

![bindbufferrange](./images/bindbufferrange.PNG)

**流程：**_mesa_BindBufferRange——bind_buffer_range——\_mesa_reference_buffer_object(主要是把当前的buffer绑定到ctx上的uniformbuffer上) —— bind_uniform_buffer——bind_buffer（主要是把buffer绑定到ctx->UniformBufferBindings[index]上面，其实就是下面这段代码：

```
_mesa_reference_buffer_object(ctx, &binding->BufferObject, bufObj);
binding->Offset = offset;
binding->Size = size;
binding->AutomaticSize = autoSize;
```

##### glBindBufferBase

和glbindbufferrange类似

**流程：**

##### glBufferData 和glBufferSubData



##### glClearBufferSubData 和glClearBufferData



##### glMapBufferRange和glMapBuffer



##### glUnmapBuffer



##### glInvalidateBufferSubData和glInvalidateBufferData



##### glCopyBufferSubData



### shader（共22个接口）

##### glcreateshader

##### glshadersource

##### glcompileshader

##### glshaderbinary

##### glreleaseshadercompiler

##### glcreateprogram

##### glattachshader

##### gllinkprogram

##### gluseprogram

##### glcreateshaderprogramv

##### glprogramparameteri

##### glgenprogrampipelines

##### glbindprogrampipeline

##### gluseprogramstages

##### glactiveshaderprogram

##### glprogrambinary

##### glgetuniformlocation

##### glGetActiveUniform

##### glUniform{1234}v

##### uniformBlockBinding

##### GetSubroutineUniformLocation

##### MemoryBarrier

### query（共4个接口）

##### glGenQueries

##### glBeginQuery和glBeginQueryIndexed

##### glEndQuery和glEndQueryIndexed

##### glGetQueryiv

### texture（共25个接口）

##### glActiveTexture

##### glGenTextures

##### glBindTexture

##### glDeleteTextures

##### glGenSamplers

##### glBindSampler

##### glSamplerParameter

##### glGetSamplerParameter

##### glTexImage2D

##### glCopyTexImage2D

##### glTexSubImage2D

##### glCompressedTexImage2D

##### glTexImage2DMultisample

##### glTexBufferRange

##### glTexParameteri

##### glGetTexParameter

##### glGetTexImage

##### glEnable和glDisable

##### glGenerateMipmap

##### glTextureView

##### glTexStorage2D

##### glTexStorage2DMultisample

##### glInvalidateTexSubImage

##### glBindImageTexture

### framebuffer（共23个）

##### glGenFramebuffers

##### glBindFramebuffer

##### glFramebufferParameteri

##### glBindRenderbuffer

##### glGenRenderbuffers

##### glRenderbufferStorageMultisample

##### glRenderbufferStorage

##### glGetRenderbufferParameteriv

##### glGetShaderSource

##### glFramebufferRenderbuffer

##### glFramebufferTexture

##### glFramebufferTexture2D

##### glDrawBuffer和glDrawBuffers

##### glColorMask

##### glStencilMask

##### glClear

##### glClearColor

##### glInvalidateSubFramebuffer

##### glReadPixels

##### glReadBuffer

##### glBlitFramebuffer

##### glCopyImageSubData

##### glClampColor

### DrawVertexArray（共16个）

##### glVertexAttrib{1234}{s f d}

##### glVertexAttribFormat

##### glBindVertexBuffer

##### glVertexAttribBinding

##### glEnableVertexAttribArray和glDisableVertexAttribArray

##### glVertexBindingDivisor

##### glGenVertexArrays

##### glBindVertexArray

##### glDrawArrays

##### glDrawElementsInstancedBaseVertexBaseInstace

##### glDrawArraysIndirect

##### glMultiDrawArrays

##### glDrawRangeElementsBaseVertex

##### glMultiDrawElementsIndirect

##### glDispatchCompute

##### glDispatchComputeIndirect

### VertexAttribute（共5个）

##### glGetActiveAttrib

##### glGetAttribLocation

##### glBindAttribLocation

##### glValidProgram

##### glPatchParameterfv

### Transform Feedback（共9个）

##### glTransformFeedbackVaryings

##### glGetTransformFeedbackVarying

##### glGenTransformFeedbacks

##### glBindTransformFeedback

##### glBeginTransformFeedback

##### glEndTransformFeedback

##### glPauseTransformFeedback

##### glDrawTransformFeedback

##### glDrawTransformFeedbackStreamInstanced

### Rasterization(共17个)

##### glProvokingVertex

##### glGetMultisamplefv

##### glMinSampleShading

##### glPointSize

##### glPointParameter

##### glFrontFace

##### glCullFace

##### glPolygonMode

##### glPolygonOffset

##### glPixelStore

##### glScissorIndexed

##### glStencilFunc

##### glStencilOp

##### glDepthFunc

##### glBlendEquationSaparate

##### glBlendFuncSaparate

##### glBlendColor



# vulkan-mesa实现





