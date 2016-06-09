计算器
======
这是一个用 **C语言** 编写的命令行计算器，支持**数学常量**，**数学函数**和一些**命令操作**。

***

运算符
------
|运算符|运算操作|优先级|
|:----:|:------:|:----:|
|( )   |  ————  |1     |
|^     | 幂运算 |2     |
|%     |取模运算|3     |
|*     |乘法运算|3     |
|/     |除法运算|3     |
|+     |加法运算|4     |
|-     |减法运算|4     |

***

数学常量
--------
- pi 圆周率
- Ans 上一个表达式的结果 初始值为0

***

数学函数
--------
- ln(x) 自然对数值 **(x>0)**
- abs(x) 绝对值
- exp(x) 自然常数e为底的指数函数值
- ceil(x) 向上取整值
- floor(x) 向下取整值
- sin(x) 正弦函数
- cos(x) 余弦函数
- tan(x) 正切函数
- arcsin(x) 反正弦函数 **(-1<=x<=1)**
- arccos(x) 反余弦函数 **(-1<=x<=1)**
- arctan(x) 反正弦函数

***

命令操作
--------
- cls 清屏
- quit 退出
- color 更换控制台颜色

***

注意事项
--------
1. 表达式必须使用**全英文字符**，所有字母**不区分大小写**
2. 表达式的字符个数**不能超过 500 个字符**
3. 你可以使用方向键‘上’、‘下’来调用输入并计算计算过的式子

***

[Github-.King]() *150604118 金天鸷 A15计算机 浙江海洋大学*