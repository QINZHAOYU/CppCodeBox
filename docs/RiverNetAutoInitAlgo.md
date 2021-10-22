# 河网状态自动初始化算法

依据一维水力模型的 **水位** 边界，结合实测点水位，对河网状态（水位和流量）进行自动初始化赋值。 
以此，得到尽可能平滑的水面线，提高模型计算稳定性和合理性。

![RiverNet](./imgs/rivernet.png)  
![RiverNet](./imgs/rivernet2.png)

如上图所示，存在一个包含环的河网，其中存在 6 个边界和 4 个实测点；河网中包含多个环状河道。

1. 存取每两个边界间最短路径（$C_{6}^{2}$），考虑河道宽度在路径生成中的作用（保持主河道完整）；
2. 依据两个边界水位，依次对上述路径上断面水位作平滑处理；
3. 对于不同路径中重复部分的断面，其水位为各次平滑处理结果的均值；
4. 根据边界间路径上断面平滑处理后的结果，再对剩余河道上断面作平滑处理；
5. 如果河网中存在实测监测点，根据其实测水位校正断面水位；
6. 根据断面水位，采用均匀流假设，计算各断面流量值，完成河网状态初始化。


**注意**：

1. 只支持水位边界，因为存在以下情况：
河网边界设置在水库坝上，如果通过水库泄流计算边界水位则明显不符合实际。

2. 假设汊点处水位相等。

3. 考虑河宽的效果尚不能确定；考虑路径是否需要配置权重（主河与支流）。

4. 监测点的水位校正方法未定。