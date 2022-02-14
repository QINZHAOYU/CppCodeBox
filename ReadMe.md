# CppCodeBox

cpp 代码片段盒子，收集工作中常用的工具模块和实现方法，学习测试新技术。

## 技术文档

+ [C/Cpp笔记](./docs/C-Cpp-Notes.md): c/c++ 使用笔记；
+ [Dijkstra图路径搜索算法](./docs/GraphSearchingAlgo.md): Dijkstra 图路径搜索算法；
+ [二进制文件格式](./docs/IOBinaryFormat.md): 一般二进制文件格式设计；


## 技术实现

+ [Any 类实现](./modules/Any.hpp): Any 类的实现（c++17已经提供）；
+ [常用数据平滑处理方法](./modules/DataSmoothingAlgo.hpp): 常用的一维数据序列平滑方法； 
+ [DllHelper 类实现](./modules/DllParser.hpp): 提供 dll 函数调用的封装接口，简化使用；
+ [函数特性萃取方法](./modules/FunctionTraits.hpp): 提供更进一步的函数特性萃取方法实现；
+ [万能函数封装方法](./modules/FuncWrapper.hpp): 提供万能函数封装调用方法； 
+ [Dijkstra 算法实现](./modules/GraphSearchingAlgo.hpp): 实现 Dijkstra 图搜索算法； 
+ [Lazy 类实现](./modules/lazy.hpp): Lazy 类的实现（c++17已经提供）；
+ [optional 类实现](./modules/optional.hpp): optional 类的实现（c++17已经提供）；
+ [range 类实现](./modules/range.hpp): 类似 python 的 range 类的实现； 
+ [资源守护方法](./modules/ScopeGuard.hpp): 提供资源（如文件句柄）守护方法，避免内存泄漏； 
+ [任务序列链式调用方法](./modules/TaskList.hpp): 提供任务序列的链式调用方法；
+ [TupleHelper 类实现](./modules/TupleHelper.hpp): 提供 tuple 类型帮助类；
+ [Variant 类实现](./modules/Variant.hpp): Variant 类的实现（c++17已经提供）；


## 框架上手

+ [Hdf5 Usages](./UsagesHdf5.cpp): 采用 Hdf5 实现数据存储；


## 测试用例

[TestUsages](./tests/ReadMe.md)
