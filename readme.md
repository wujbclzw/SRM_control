# 开关磁阻电机控制程序 

电机描述：3相开关磁阻，定子/转子12/8极， 

## R10_（低速10rpm）调试记录

### 14:28 2014-10-30

可以定制出电流曲线，只是电流曲线方程还要改改，让电机更加平稳。
如果全相给电，开关器件的发热太严重了，现在改为四个区间加电，同时有两相上电。

### 11:23 2014-10-31

增加放大电路的放大比，最小电流应该可以控制到0.5A以下，但空载的时候还是有点不稳。带负载时比较稳。

### 2018-05-23 11:55:07 星期三

改用新的控制板，修正系统的IO口设置
早期写的代码 注释不详细 现在理解起来比较困难

### 2018-05-24 10:23:47 星期四

总的控制思路，控制每相一个周期内的电流曲线如下图，三相一次滞后120°

![电流曲线](/images/current_trig.png "三角形电流曲线")

