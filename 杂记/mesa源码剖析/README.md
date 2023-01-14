

关于mesa架构方面，[这篇文章讲的比较清楚](https://blog.csdn.net/HaoBBNuanMM/article/details/109054106)，[还有这个](https://zhuanlan.zhihu.com/p/398226773)，[还有这个](http://joyxu.github.io/2021/05/13/gpu05/)

关于mesa的代码结构方面，[这篇文章讲的比较清楚](https://winddoing.github.io/post/39ae47e2.html)

mesa的官网下载地址：https://archive.mesa3d.org/

后续做源码剖析会根据opengl和vulkan不同API类别分别做剖析

opengl重点文件：.\src\mesa\main,    .\src\mesa\state_tracker,会涉及到一些,\src\gallium

![mesa_arch3](./images/mesa_arch3.png)

**经典流程：**

```
mesa/main下面文件:  _mesa_BufferData —— _mesa_buffer_data —— _buffer_data —— pipe->buffer_subdata

gallium/drivers下面文件：
r600_buffer_subdata（以Amd r600为例）/ i915_buffer_subdata（以intel i915为例）—— memcpy
```

# opengl-mesa实现

所有的入口函数都是_mesa_xxxx,例如_mesa_BindBuffer、_mesa_BindBufferRange等。

当前进度61/136

### buffer（共15个接口）

buffer入口 src\main\mesa\bufferobj

gallium的r600(AMD驱动接口)对应gallium/drivers/r600/r600_blit.c和gallium/drivers/r600/r600_buffer_common.c

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

和glbindbufferrange类似，只不过bindbufferbase的接口少了offset和buffersize。里面的实现一样。但有一点不同，就是他传入offset和size的时候会传入一个AutomaticSize的flag，表示后面再获取size。在真正使用的时候，会默认使用当时的buffersize，只有AutomaticSize为false时，才会使用bindbufferrange的size。如下所示：

![bindbufferbase](./images/bindbufferbase.PNG)

**流程：**_mesa_BindBufferBase——bind_buffer_base_uniform_buffer——\_mesa_reference_buffer_object(主要是把当前的buffer绑定到ctx上的uniformbuffer上) —— bind_uniform_buffer——bind_buffer

##### glBindBuffersBase和glBindBuffersRange

预留接口，在ARB_multi_bind扩展中使用，实现的时候就是比glBindBufferBase/glBindBufferRange多一个for循环

##### glBufferData 和glBufferSubData

**流程：**_mesa_BufferData —— get_buffer（获取当前buffer）—— _mesa_buffer_data——\_ buffer_data——\_mesa_buffer_unmap_all_mappings(把所有已经map的buffer都unmap掉，因为现在要写入数据了)——\_mesa_bufferobj_data——bufferobj_data——pipe->buffer_subdata（gallium）——r600_buffer_subdata（以Amd r600为例）/ i915_buffer_subdata（以intel i915为例）—— memcpy

##### glClearBufferSubData 和glClearBufferData

如果没有硬件驱动实现，则会退化成软件实现，软件实现就是使用memcpy把clearvalue复制到dest上面。虽然硬件感觉也差不多。。

**流程：** _mesa_ClearBufferSubData —— clear_buffer_sub_data —— clear_buffer_subdata_sw / pipe->r600_clear_buffer —— 硬件dma支持，或者走memcpy流程。

##### glMapBufferRange和glMapBuffer

首先要getbuffer，然后把pointer、length、offset和access都存进去。

这里会有一些防呆设计，比如对调用者没有调用对时的修改。。map是会对每一个index存一套offset、length和access的。map出来的pointer也是通过pipe驱动获得。

glMapBuffer也是走的glMapBufferRange的流程。

**流程：**_mesa_MapBufferRange —— map_buffer_range —— _mesa_bufferobj_map_range——r600_buffer_map_sync_with_rings —— radeon_winsys里面的buffer_map（这个是闭源的，可以参考纯软的kms_dri_sw_winsys.c）

##### glUnmapBuffer

unmap就是把gl_buffer_object里面的内容全部置为空，然后调用驱动的buffer_unmap

##### glInvalidateBufferSubData和glInvalidateBufferData

也是首先使用lookup_bufferobj查找到buffer，然后调用bufferobj_invalidate。注意如果这个buffer已经被map了，是无法做invalidate操作的。

这两个会调用到r600_invalidate_resource，AMD的驱动会reallocate buffer并且更新所有资源的binding。

glInvalidateBufferData走的也是glInvalidateBufferSubData的路子

##### glCopyBufferSubData

首先getbuffer，然后调用bufferobj_copy_subdata，当前的buffer是不能被map的，如果被map会发生崩溃。

然后调用驱动的resource_copy_region，将一块pixel从一个res拷贝到另一个，这两个res必须是同样的格式。

然后会调用到r600_resource_copy_region里面，也会检查能否使用dma，如果不行的话就会fallback到r600_resource_copy_region里面，使用memcpy和buffer_map/texture_map来拷贝buffer。

### shader（共22个接口）

shader入口 src\mesa\main\shaderapi.c和src\mesa\main\pipelineobj.c

##### glcreateshader

createshader之前需要添加锁，然后需要确认当前shader的stage，opengl的shader stage如下所示：

![glshaderstage](./images/glshaderstage.PNG)

然后会创建一个新的shader，关键代码只有几行，很简单：

```c++
struct gl_shader *shader;
shader = rzalloc(NULL, struct gl_shader);
if (shader) {
    shader->Stage = stage;
    shader->Name = name;
    shader->RefCount = 1;
    shader->info.Geom.VerticesOut = -1;
    shader->info.Geom.InputType = SHADER_PRIM_TRIANGLES;
    shader->info.Geom.OutputType = SHADER_PRIM_TRIANGLE_STRIP;
}  
```

##### glshadersource和glshadersourceARB

这个接口的入口函数是\_mesa_ShaderSource

首先会去找需要shadersource的shaderobj，中间会申请一个新的source的空间，把shader拷贝进这个新的变量里面，然后计算sha1，将sha1保存到最开始寻找到的shaderobj里面，然后将shader内容保存到shaderobj里面。

shadersource只做了保存变量和计算sha1的操作，没有编译也没有做优化。

##### glcompileshader

编译源代码：src/compiler/glsl/glcpp/

入口函数\_mesa_CompileShader和_mesa_compile_shader

首先仍然是寻找shaderobj，如果shadersource是空的话，会返回一个COMPILE_FAILURE，而不会崩溃或者抛出GL_ERROR。之后会初始化build_in function，例如step、textureSize2D等，只需初始化一次。

然后就去编译shader，shader的编译过程有些复杂，[这篇文章](http://joyxu.github.io/2021/06/15/gpu-mesa-compile/)可以帮忙梳理下流程，里面有张图可以作为学习参考。

![mesa_compile_shader](./images/mesa_compile_shader.png)

在glcompileshader里面，走的应该是glslshader到GLSL IR的步骤。包括检查shader里面是否有#include（有的话就检查剩余的shadercache，有的话就跳过），还有preprocess（删除空行（\n,\r,\r\n,\n\r）等，这里用的是glcpp处理，细节追不到），这个时候应该就已经解析成语法树了。然后通过_mesa_ast_to_hir转换成hir，经过precision等的优化，还有编译验证，sha1计算等操作，compile这一步就算结束了。

##### glshaderbinary

仍然需要先寻找shaderobj，验证通过后直接将spirv保存下来即可，按照上面那张图来看。spirv和glsl ir是同级的。spirv的shader有一个gl_spirv_module属性，他用来保证同样的spirv被不同shaderobj引用时的一个计数。

##### glreleaseshadercompiler

入口函数_mesa_ReleaseShaderCompiler，只是把单例builtin_builder的builin_users数量减少并且release掉。

##### glcreateprogram

入口函数_mesa_CreateProgram和create_shader_program。和glcreateshader类似，整体来说也是创建了一个gl_shader_program，代码简单来看就是下面这样的。

```C++
struct gl_shader_program *shProg;
shProg = rzalloc(NULL, struct gl_shader_program);
if (shProg) {
    shProg->Name = name;
    shProg->data->RefCount = 1;
    shProg->data->InfoLog = ralloc_strdup(data, "");
    if (!shProg->data) {
        ralloc_free(shProg);
        return NULL;
    }
	shProg->Type = GL_SHADER_PROGRAM_MESA;
    shProg->RefCount = 1;
    shProg->AttributeBindings = string_to_uint_map_ctor();
    shProg->FragDataBindings = string_to_uint_map_ctor();
    shProg->FragDataIndexBindings = string_to_uint_map_ctor();
    shProg->Geom.UsesEndPrimitive = false;
    shProg->Geom.ActiveStreamMask = 0;
    shProg->TransformFeedback.BufferMode = GL_INTERLEAVED_ATTRIBS;
    exec_list_make_empty(&shProg->EmptyUniformLocations);
}
return shProg;
```

##### glattachshader

入口函数_mesa_AttachShader和attach_shader。

顾名思义，就是把shader和对应的program赋值一下，然后把老的shader里面的reference去掉。就没别的操作了。

##### gllinkprogram

走的是src/compiler/glsl/linker.cpp的代码

入口函数_mesa_LinkProgram和link_program，link之前首先要保证transform_feedback没有在使用当前program。然后依然要保证shader的builtin函数是已经完成了的。然后执行\_mesa_glsl_link_shader，这时可以使用spriv，也可以用glsl  ir来做link，然后根据shader stage，一个一个的去把uniform、ssbo、block、bindless等解析出来，和现有的CPU端obj信息 link起来。经过这一个阶段，我们的shader就变成可执行代码了。

在link阶段会做很多优化，例如对varing 的优化，对ssbo的优化，检查是否存在循环link，还有很多优化工作，最后还会验证整个shader是否link对了，做一些validation

##### gluseprogram

入口函数_mesa_use_shader_program。

有两个步骤，一个是_mesa_use_program，一个是\_mesa_active_program.

_mesa_use_program就是切换到这个program，同时切换的还有vertexprogra、fragmentprogram和geo progra，并且unreference老的program，允许out-of-order draw（其实就是drawcall 合并）。同时还会更新render_state的一系列状态量，验证uniform和sampler是否是valid的，验证fragment/xfb是否可用，geometry shader是否支持等。

整体看下来gluseprogarm耗时还是挺长的，并不是简单的仅仅切换一个program

##### glcreateshaderprogramv

入口函数_mesa_CreateShaderProgramv

把几件事情放在一起做了，首先create_shader,然后做shadersource、在做glcompileshader，再做createprogram，再做attachshader和linkprogram，最后再delete shader。基本上算是个缝合怪。一个接口完成以后就可以直接useprogram了。

##### glprogramparameteri

只是更改了一下某个program下面某个参数的值，pname只支持两个枚举，分别是GL_PROGRAM_BINARY_RETRIEVABLE_HINT和GL_PROGRAM_SEPARABLE。感觉这个接口很少用

##### glgenprogrampipelines

这个函数的入口在pipelineobj.c的_mesa_GenProgramPipelines函数中，接口的作用是把从vertex 阶段到fragment shader阶段的所有配置都存起来。和其他genxxx一样，这个接口创建了n个新的pipeline object，伪代码如下：

```
struct gl_pipeline_object *obj = rzalloc(NULL, struct gl_pipeline_object);
if (obj) {
    obj->Name = name;
    obj->RefCount = 1;
    obj->Flags = _mesa_get_shader_flags();
    obj->InfoLog = NULL;
}
```

##### glbindprogrampipeline

入口函数也在pipeline's里面，bind_program_pipeline。

首先需要找到有没有对应的pipelineobj，然后将pipelineobj绑定到对应的binding point上。然后把program里面所有的stage里面的uniform等绑定点设置好。之后设置vertex process的状态，设置drawcall乱序绘制的优化，还有绘制raster的一些设置选项，例如draw的mask（font face/back face/triangles/triangle_strip等）

##### gluseprogramstages

这个是opengl的特性，主要是把某个program绑定到pipeline的多个stage上面。从而复用某个管线的阶段性代码。

在实现过程中，也会首先寻找是否存在这个pipeline和这个program，以及当前的pipeline是否存在geometry shader/compute/tessellation 的stage。然后依次将stage的maskbit和各个stage bit做比较，如果有某个stage则把某个program绑定到对应的pipeline object上。然后会根据不同的stage最终调用到gluseprogram。

##### glactiveshaderprogram

也是opengl的特性，opengl的pipelineobject保证同一个vertex shader可以和多个fragment shader组合，减少重复，而glactiveshaderprogram这个接口就是类似glactivetexture和gluseprogram结合的意思，就是激活某个stage，然后在这个stage上调用gluseprogram。

在mesa的实现里面，是先寻找pipeline object，再去寻找对应的program，然后把当前pipeline的activeprogram替换成当前寻找的program。

##### glprogrambinary

入口函数_mesa_ProgramBinary，

简单暴力，把当前program的shader数据清除掉，然后替换成传入的shaderbinary数据，同时计算sha1和调用对应的gluseprogram。中间会验证binary是否是正确的，也会验证binary的crc值，

##### glgetuniformlocation

入口在mesa\main\uniforms.c的_mesa_GetUniformLocation_impl(GLuint programObj, const GLcharARB *name, bool glthread)里面，作用是找到某个program对应某个特定uniform名称的location。

具体做法就是先找到对应name 的resource，mesa维护了一套name - resource的map，然后再去根据resource找到对应的array index

##### glGetActiveUniform

下面是这个接口的详细定义，其中length以后都是return的值，意思就是说只要有index和program，就能拿到这个uniform的所有信息

![glGetActiveUniform](./images/glgetactiveuniform.PNG)

在mesa里面，入口函数在uniform_query.cpp里面的_mesa_GetActiveUniform_impl中，

仍然是先要找到对应的program资源，然后根据资源获取这个uniform的名字，长度，对应的数据类型，以及arraysize。

##### glUniform{1234}v

实际上所有的gluniform*到最后都会调用到mesa\main\uniform_query.cpp中的_mesa_uniform这个接口.gluniform\*的那些接口在mesa\main\uniforms.c中都可以找到，看起来还挺整齐壮观的。

因为把gluniform*都归一化到同一个入口里面了，所以mesa里面的做法其实挺复杂的，gluniform也并没有想象中的那么快，首先他要验证uniform的值是否valid，对于64bit的值，还需要做拆分，然后去将uniform值做packing，然后将packing的值通过memcpy保存到一个driver_storage里面。然后使用_mesa_flush_vertices_for_uniforms，将数据通过driver刷到GPU上。中间还有一些数据转换，例如float2half。

##### uniformBlockBinding

这个函数就是把某个uniform绑定到特定program的绑定点上，入口函数也在uniforms.c里面，

实现起来也含简单，mesa存了一个uniformblocks的map，直接对对应uniformindex新增一个绑定点即可，代码如下

```C++
static void uniform_block_binding(struct gl_context *ctx, struct gl_shader_program *shProg, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
   if (shProg->data->UniformBlocks[uniformBlockIndex].Binding !=
       uniformBlockBinding) {

      FLUSH_VERTICES(ctx, 0, 0);
      ctx->NewDriverState |= ST_NEW_UNIFORM_BUFFER;

      shProg->data->UniformBlocks[uniformBlockIndex].Binding =
         uniformBlockBinding;
   }
}
```

##### GetSubroutineUniformLocation

入口函数在mesa\main\shaderapi.c的_mesa_GetSubroutineUniformLocation。这个函数的作用就是在glsl中，支持subroutine关键字，他相当于是shader的子程序，类似函数指针。GetSubroutineUniformLocation就是获取子程序uniform的位置，结合glGetSubroutineIndex可以获取对应函数指针的位置，就可以在CPU端控制GPU调用的函数指针。这里有个例子：

https://blog.csdn.net/coldkaweh/article/details/49966731

在mesa的实现中，首先要判断shader、program是否valid，然后找到对应的shaderstage。然后根据shaderstage获取具体是哪种resource类型。然后走的是glGetUniformLocation的路线，先找到对应name 的resource，再去根据resource找到对应的array index

##### MemoryBarrier

入口函数在mesa\main\barrier.c的memory_barrier_by_region里面。这里面实现很简单，判断barrier的类型，添加flag，然后直接调用驱动pipe里面的memory_barrier接口。以AMD为例，会调用到gallium\drivers\r600\\r600_state_common.c里面的r600_memory_barrier。这个函数只是设置了一些flag，估计到执行的时候才会有实际操作。

### query（共4个接口）

##### glGenQueries

入口函数mesa\main\queryobj.c里面的create_queries，和glGenBuffers\glCreateBuffers一样，glGenQueries和glCreateQueries也类似

具体的操作也是创建了一个query_object，代码如下：

```
struct gl_query_object *q = CALLOC_STRUCT(gl_query_object);
if (q) {
    q->Id = id;
    q->Ready = GL_TRUE;
    q->pq = NULL;
    q->type = PIPE_QUERY_TYPES; /* an invalid value */
    return q;
}
```

##### glBeginQuery和glBeginQueryIndexed

两个都会调用到mesa\main\queryobj.c里面的_mesa_BeginQueryIndexed，不过glBeginQuery会把index设置成0。

首先，mesa查找当前target query的binding point，然后回去查找是否存在当前的query object。然后将GL query type转换成gallium的query type，例如GL_ANY_SAMPLES_PASSED转换成PIPE_QUERY_OCCLUSION_PREDICATE、然后调用pipe的create_query，例如r600_create_query，之后又分为软件query和硬件query。硬件query会调用操作系统的virtual memory

##### glEndQuery和glEndQueryIndexed

和glbeginquery上面相同，甚至连流程都一样

##### glGetQueryiv

实际上接口是_mesa_GetQueryIndexediv，接口是

| `void **glGetQueryIndexediv**(` | GLenum target,     |
| ------------------------------- | ------------------ |
|                                 | GLuint index,      |
|                                 | GLenum pname,      |
|                                 | GLint * params`)`; |

params会根据传入的pname，返回对应的值，而值全都存在context里面。基本就是一个switch搞定一切

### texture（共25个接口）

实现文件集中在mesa\main\texobj.c、mesa\main\texstate.c、mesa\main\samplerobj.c

##### glActiveTexture

入口函数在mesa\main\texstate.c里面的active_texture，只做了一件事：

```
   ctx->Texture.CurrentUnit = texUnit;
   if (ctx->Transform.MatrixMode == GL_TEXTURE) {
      /* update current stack pointer */
      ctx->CurrentStack = &ctx->TextureMatrixStack[texUnit];
   }
```

##### glGenTextures

glCreateTextures 和glGenTextures一模一样，不像buffer是延迟申请的。

入口函数在mesa\main\texobj.c里面的create_textures上，做了两件事情，一个是创建一个texture obj，这个比较简单，然后初始化这个textureobj，这一步很复杂，因为textureobj的属性太多了，包括sample的规则texture的Attrib、mipmap、layer、是否是bindless等等。但他没有调用驱动，说明是上层控制的。

##### glBindTexture

bindtexture入口函数mesa\main\texobj.c里面的_mesa_bindTexture，整体的流程和glbindbuffer类似

首先调用_mesa_lookup_or_create_texture，寻找是否存在需要bind的texture，如果存在的话，直接返回要bind的texture，否则则新创建一个texture，并且也会初始化这个新建的texture，类似glGenTextures。

##### glDeleteTextures

入口函数mesa\main\texobj.c里面的delete_textures，因为可以delete多个textures，所以里面首先就是一个for循环，然后依次寻找texture——lock texture —— unbind texture from  fbo —— unbind texture from texunits  —— unbind texture  from  image unit ——把texture 查找表、sampler等都删掉，把当前texture object reference 成null，因为是mesa自己实现的智能指针，所以会自动删掉。（这里总感觉会有风险）

##### glGenSamplers

入口函数mesa\main\samplerobj.c里面的_mesa_GenSamplers，然后调用create_samplers，和其他的glGenxxx一样，glGenSamplers其实就是申请了一个新的sampler_boject，然后对samplerobj的所有属性做初始化，sampler的属性也比较多，包括compareMode，wrap的方式等等。

因为这个接口是复数形式，所以里面也有一个for循环。在申请资源前后也需要加锁。所有申请资源的操作都需要加锁。

##### glBindSampler

入口函数在mesa\main\samplerobj.c的bind_samplers，

函数的实现很简单，先加锁，然后查找是否有对应的sampler obj，将当前的sampler切换成要绑定的sampler，然后解锁。

##### glSamplerParameter

入口函数在mesa\main\samplerobj.c的_mesa_SamplerParameteri

这个函数的作用是设置采样器参数，例如glSamplerParameter(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT),

实现方式就是先通过一个switch，判断中间形参pname的值，然后判断param和pname的值是否valid，然后将参数复制进去就行了。对于有mipmap的sampler，还要把对应mipmap的参数设置好。

##### glGetSamplerParameter

入口函数在mesa\main\samplerobj.c的_mesa_GetSamplerParameteriv，检查参数合理性，然后返回sampler对应的参数即可，实现很简单。

##### glTexImage2D

入口函数mesa\main\teximage.c里面的_mesa_TextureImage2DEXT, \_mesa_TexImage2D等，但是包括glTexImage1D/2D/3D、glTextureImage1D/2D/3DEXT、MultiTexSubImage1D/2D/3D、glCompressedTexImage1D/2D/3D等在内的所有接口，到最后都会调用到teximage里面，

这个接口在实现的时候会首先判断target是不是有效的（包括pc上的gl，gles对target格式的支持都不同），然后根据texture target从context里面获取对应的texture object。如果是compressed image，则需要把compressed的数据解压转换成decompressing 的texture，这里是用CPU转换的。

如果是proxytexture，则需要get对应的proxy texture，并且更新 proxy texture的属性。如果是non-proxy的texture，则需要首先拿到texture_image的object（主要是有mipmap的关系），然后把当前textureImage的数据清空掉。然后设置初始化textureImage的属性和。调用st_TexSubImage，转换成gallium的坐标系，还有转换图片的格式和数据地址，最后调用gallium的texture_subdata。（这个是最快的方法）

在这个过程中，有许多检查，例如格式不匹配，格式不支持，或者获取不到格式，或者不支持NPOT的格式，他们最终都会fallback到mesa的CPU实现。

在中间还会检查是否存在unpack的PBO，如果存在的话，则没有办法使用gallium的texture_subdata，则需要通过map—— memcpy——unmap的方式上传纹理数据。

最最后还需要check和gen一下mipmap。

整体看下来，glTexImage2D等接口实现起来非常复杂。估计效率也是十分堪忧的，和预期一致。

##### glCopyTexImage2D

入口函数mesa\main\teximage.c里面的_mesa_CopyTextureImage1D/2DEXT，\_mesa_CopyTexImage1D/2D、\_mesa_CopyMultiTexImage1D/2DEXT等。

以上这些接口最终都会调用到copyteximage函数里面。

首先要做的仍然是格式转换，获取texture obj，然后加锁，判断是否可以避免rellocation（这个可以快20倍，主要是要保证格式一致，border一致，长宽一致），如果可以避免rellocation，则直接使用st_CopyTexSubImage，这个时候有两种实现方法，一种是硬件使用pipe的blit方法，另外一种是fallback到软件实现。最后需要check_gen_mipmap，并且unlock texture。

如果需要rellocation，则首先仍然是判断格式是否符合，以及TexImage是否有proxy，如果有的话，先用proxy做一些条件上的剔除（主要是image大小的检查）。然后get texture image的obj，并且把原有数据free掉。做alloc新的TextureImageBuffer，然后再走之前的copytexsubimage。同时要检查对应的FBO之间的引用关系。

##### glTexSubImage2D

和glteximage相比，gltexubimage2D直接调用st_TexSubImage，要直观很多。这里面最主要的原因是glTexSubImage2D不需要考虑是否压缩，格式也没有那么复杂。仅从mesa实现的角度来看，这个接口比glteximage要快一些。

##### glCompressedTexImage2D

和glteximage一模一样，只不过里面调用了不少类似compressed_texture_error_check的函数，很多判断是单独进行的。

##### glTexImage2DMultisample

入口函数mesa\main\teximage.c的_mesa_TexImage2DMultisample,

首先需要检查当前硬件是否支持MSAA，不支持或者sample数量小于1的话直接就return。然后检查texture format是否支持，然后判断sample count和opengl版本，硬件支持的关系是否符合要求，然后再去获取texImage，free textureImage里面的内存，inittexImage，然后在state_tracker里面创建纹理，

但是和glteximage2D不同的是，他并没有传入数据，而sample count只是teximageobj里面的一个属性，设置完以后就结束了。

##### glTexBufferRange

这个接口是把一个buffer和一个texture连在一起。当然texture object一定是一个buffer texture。buffertexture是没有mipmap的，也没有filter，但是他可以允许shader使用texelFetch去访问一大块内存。

入口函数也在mesa\main\teximage.c里面，是texture_buffer_range，这个入口会把glTexBuffer、glTexBufferRange、glTextureBuffer和glTextureBufferRange归纳到一起。

这个接口首先也是check format，检查是否支持texture buffer，然后lock texture，然后把bufferobject和texobj attach到一起。unlock texture，对于texture object，解锁所有的sampler（因为buffertexture是没有sampler的）。结束

##### glTexParameteri

入口函数mesa\main\texparam.c里面的_mesa_texture_parameteri.

主要是根据pname，以及对应的target，找到对应的texobj，检查赋值是否valid，然后赋值。对于GL_TEXTURE_LOD_BIAS，则需要把所有的texture parameter都失效掉（例如把所有的sampler view失效）

##### glGetTexParameter

入口函数在mesa\main\texparam.c里面的_mesa_GetTexParameteriv

这个接口实现起来也很简单，直接从textureobj里面的状态获取就好了，只是获取的时候需要关注不同opengl的版本，以及opengl和gles的区别

##### glGetTexImage

这个接口主要是获取texture里面pixel的数据的，

入口函数_mesa_GetTexImage

首先仍然是先拿到当前的texobj，然后获取texture image的大小（width、height和depth）然后根据mipmaplevel获取对应的teximage object。然后会调用state_tracker的st_GetTexSubImage，调用pipe的blit接口.blit接口是相对高效的方法，**如果有些条件不符合，例如是GL_DEPTH_STENCIL（因为有些driver对DEPTH stencil支持不好，所以干脆就走了软件blit（这里是用compute shader实现的），会慢很多）。如果再有某些条件不满足，则使用CPU做copy，效率就更低了。**

##### glEnable和glDisable

入口函数在mesa\main\enable.c的_mesa_Enable里面，两者到最后都会调用到\_mesa_set_enable.

和想象中的一样，这个接口实现起来基本就是对context做一个赋值操作，这个函数的代码有1000行。但是有的glenum会有额外操作，以GL_BLEND为例，如果新设置的enable状态和老的状态不一样，则需要更新vertices的状态，还需要draw out of order的状态，还有render_state的状态。也就是说，某些glEnum会影响到别的操作的地方，都需要进行更新。

##### glGenerateMipmap

入口函数在mesa\main\genmipmap.c。

实现起来仍然是先lock_texture，然后选择需要生成的textureimage，确保需要生成的mipmap format是valid的。并且对于compressed texture，是不允许直接生成mipmap的。最后会根据是否是cubemap，来调用state_tracker里面的st_generate_mipmap，判断硬件是否有generate_mipmap的接口，这个仍然是最快的生成方法，如果没有这个接口，则使用mesa的软件实现。这个时候就能生成compressed texture的mipmap了



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





