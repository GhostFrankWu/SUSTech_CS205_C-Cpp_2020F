# 大矩阵乘法接口及其加速  
#### *文档创建于11月15日。11月16日0点起的所有编辑都会特别声明*  
## 🌟基本功能  
- 定义了一个矩阵类  
对矩阵的初始化是可自定义且鲁棒的(e.g可以初始化一个114\*514的矩阵，但是不可以初始化-19\*19的矩阵)  
其中数据声明为private，对行列的访问方法返回值为常量不可修改。  
重载了()和[]操作符，使得可以通过[]以一维矩阵的方法访问数据，(x,y)以二维矩阵的方法访问数据。  
其它一些初始化方法和打印矩阵的方法  
![C](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/C.png)  
- 一个自由选择计算方法的矩阵乘法接口  
传入的参数可以是自定义的矩阵类或者正常的浮点指针，无论传入的是什么，方法会首先校验矩阵的合理性(是否可以相乘)，不合理直接返回，只有可以正常计算的情况下才会进行计算。  
接口包括正常的暴力（BruteForce）、多线程优化后的暴力（DevideBruteForce）、以及分块后的多线程（DevideBlock）。  
多线程会根据系统类型及配置分配合适的线程数目(兼容Linux)  
![CORE](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/core.png)  
- 一个封装的chrono类  
建立了一个TimePoint计时器类，调用计时，和打印时间只需要简单的“setTime”和“timeDffer”，使得源码更加易读  
- 程序优化  
对于暴力算法，我只测量了1000\*1000矩阵的平均耗时为4.6s，根据算法的复杂度可知在200m数据下暴力方法耗时约为 **4500s** :(10000/1000)^3\*4.6≈1小时30分钟  
![BF](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/BF.png)  
随后我简单地封装了暴力算法划分为多线程运算，并且通过多路循环优化,1000\*1000的矩阵计算用时达到了1s，提升了约四倍的效率。200m数据耗时约为 **1000s**  
![DBF](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/DBF.png)  
之后通过矩阵分块的方法，并进行release优化得到的结果为148ms,在10000\*10000的数据下运算，实际耗时为 **122s**  ，速度提升了30倍  
![DB](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/DB.png)  
- 与OpenBLAS的速度比较  
OpenBLAS启动后占满全部CPU，多线程稳定高效。仅用20s完成了计算  
单个计算结果OpenBLAS和我的代码没有差异，在向量点乘中我曾发现cblas_sodt存在明显的精度丢失。我通过kahan求和的方法提高了结果的精度。  
![blas](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/cblas.png)  
  
## ⚠硬件优化  
- 对于民用级大规模浮点运算，最佳的选项是显卡，通过编写调用cuda的cublas进行矩阵乘法，仅用 **1.5s** 即可完成200m的矩阵乘法运算  
优化倍数？**暴力算法的300倍，OpenBLAS的12倍**  
![blas](https://github.com/GhostFrankWu/SUSTech_CS205_C-Cpp_2020F/blob/master/Assignments/mid/sc/CUDA.png)  
